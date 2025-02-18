const { readLinesSync, system } = require("./utils");

const ignorePrefixes = [
  'void *alloca',
  'SDL_COMPILE_TIME_ASSERT(',
  'extern "C" {',
  'size_t strl',
  'size_t wcsl',
  'char *str',
  '}',
];

const ignoreInSignature = new RegExp(`(${[
  'extern',
  'SDL_FORCE_INLINE',
  'SDL_DECLSPEC',
  'SDL_MALLOC',
  'SDL_ALLOC_SIZE2\\(1,\\s*2\\)',
  'SDL_ALLOC_SIZE',
  'SDLCALL',
  'SDL_(OUT|IN|INOUT)_(Z_)?(BYTE)?CAP',
].join("|")})(\\(\\w*\\))?`, "g");

/**
 * @typedef {object} ParseConfig
 * @prop {string}   baseDir
 * @prop {string[]} sources
 * @prop {boolean=} storeLineNumbers
 */

/**
 * @param {ParseConfig} config
 * 
 */
function parseApi(config) {
  const { baseDir, sources } = config;

  /** @type {Api} */
  const api = { files: {} };
  for (const name of sources) {
    system.log(`Reading file ${name}`);
    const content = readLinesSync(baseDir + name);
    api.files[name] = parseContent(name, content, config);
  }
  return api;
}

/**
 * @typedef {object} ParseContentConfig
 * @prop {boolean=} storeLineNumbers
 */

/**
 * 
 * @param {string} name
 * @param {string[]} content 
 * @param {ParseContentConfig} config 
 */
function parseContent(name, content, config) {
  const tokens = tokenize(content);
  if (!config) config = {};

  /** @type {ApiFile} */
  const apiFile = {
    name,
    doc: '',
    entries: {},
  };
  const parser = new ContentParser(tokens, config);
  const entryArray = parser.parseEntries();
  insertEntry(apiFile.entries, entryArray);
  if (config.storeLineNumbers) {
    apiFile.docBegin = parser.docBegin || 1;
    apiFile.docEnd = parser.docEnd || apiFile.docBegin;
    apiFile.entriesBegin = parser.entriesBegin || entryArray[0]?.begin || apiFile.docEnd;
    apiFile.entriesEnd = parser.entriesEnd || entryArray[entryArray.length - 1]?.end || apiFile.entriesBegin;
  }
  apiFile.doc = parser.doc;
  return apiFile;
}

/**
 * Insert entry into entries
 * 
 * @param {ApiEntries} entries 
 * @param {ApiEntry|ApiEntry[]} entry 
 */
function insertEntry(entries, entry) {
  if (Array.isArray(entry)) {
    entry.forEach(e => insertEntry(entries, e));
    return entries;
  }
  fixEntry(entry);
  const name = entry.kind == "forward" ? entry.name + "-forward" : entry.name;
  if (entries[name]) {
    const currEntry = entries[name];
    if (Array.isArray(currEntry)) {
      currEntry.push(entry);
    } else if (currEntry.kind != 'function') {
      if (entry.doc || !currEntry.doc) entries[name] = entry;
    } else if (entry.kind == 'function') {
      entries[name] = [currEntry, entry];
    }
  } else {
    entries[name] = entry;
  }
  return entries;
}

/**
 * Add missing fields
 * @param {ApiEntry} entry 
 */
function fixEntry(entry) {
  if (typeof entry.doc != "string") entry.doc = "";
  if (entry.entries) {
    for (const subEntry of Object.values(entry.entries)) {
      if (Array.isArray(subEntry)) {
        subEntry.forEach(fixEntry);
      } else if (typeof subEntry === 'object') {
        fixEntry(subEntry);
      }
    }
  }
}

/**
 * Parses a list of tokens into Entry
 */
class ContentParser {
  /**
   * 
   * @param {FileToken[]}        tokens the tokens
   * @param {ParseContentConfig} config the configuration
   */
  constructor(tokens, config) {
    /** @private */
    this.next = () => tokens.shift();
    /** @private */
    this.lookup = () => tokens[0];
    this.storeLineNumbers = config.storeLineNumbers;
    this.docBegin = 0;
    this.doc = "";
    this.docEnd = 0;
    this.entriesBegin = 0;
    this.entriesEnd = 0;
  }

  /**
   * Parse all entries
   * @param {number} identLevel the level of ident
   */
  parseEntries(identLevel = 0) {
    const entries = [];
    for (let entry = this.parseEntry(identLevel); !!entry; entry = this.parseEntry(identLevel)) {
      entries.push(entry);
    }
    return entries;
  }

  /**
   * Parses a single entry
   * @param {number} identLevel the level of ident
   * @returns {ApiEntry}
   */
  parseEntry(identLevel = 0) {
    let lastEnd = 0;
    let lastDecl = 0;
    let lastBegin = 0;
    let lastDoc = "";
    /** @type {ApiParameters} */
    let lastTemplate = null;

    if ((this.lookup()?.spaces ?? -1) < identLevel) return undefined;

    while (this.lookup()?.kind === "doc") {
      const token = this.expect("doc");
      this.checkFileDoc(lastBegin, lastEnd, lastDoc);
      lastDoc = token.value;
      lastEnd = token.end;
      lastBegin = token.begin;
    }

    if (this.lookup()?.kind === "namespace") {
      const token = this.expect("namespace");
      this.checkFileDoc(lastBegin, lastEnd, lastDoc);
      if (!this.entriesBegin) {
        this.entriesBegin = token.end;
      }
      return this.parseEntry();
    }

    if (this.lookup()?.kind === "template") {
      const token = this.expect("template");
      if (lastEnd != token.begin || lastTemplate) {
        this.checkFileDoc(lastBegin, lastEnd, lastDoc);
        lastBegin = token.begin;
        lastDoc = null;
      }
      lastTemplate = parseParams(token.parameters);
      if (!lastDecl) lastDecl = token.begin;
      lastEnd = token.end;
    }
    const token = this.next();
    if (!token) {
      if (lastTemplate) throw new Error(`Error at ${lastEnd}: Expected an entity after template signature`);
      return undefined;
    }

    /** @type {ApiEntry} */
    const entry = {
      doc: '',
      name: token.value,
      kind: token.kind,
    };
    switch (token.kind) {
      case "alias":
        entry.type = token.type;
        break;
      case "callback":
        entry.type = token.type;
        entry.parameters = parseParams(token.parameters);
        break;
      case "def":
        if (token.parameters) entry.parameters = parseParams(token.parameters);
        break;
      case "enum":
        break;
      case "function":
        entry.type = token.type;
        entry.parameters = parseParams(token.parameters);
        if (token.constexpr) entry.constexpr = token.constexpr;
        if (token.immutable) entry.immutable = token.immutable;
        if (token.static) entry.static = token.static;
        break;
      case "struct":
        entry.type = token.type;
        entry.entries = insertEntry({}, this.parseEntries(token.spaces + 1));
        break;
      case "var":
        entry.type = token.type;
        break;
      case "forward":
        break;
      default:
        throw new Error(`Error at ${token.begin}: Unexpected ${token.kind}`);
    }
    if (token.begin == lastEnd) {
      if (lastDoc) entry.doc = lastDoc;
      if (lastTemplate) entry.template = lastTemplate;
      if (this.storeLineNumbers) {
        entry.begin = lastBegin;
        entry.decl = lastDecl || token.begin;
        entry.end = token.end;
      }
    } else {
      if (lastDoc) this.checkFileDoc(lastBegin, lastEnd, lastDoc);
      if (this.storeLineNumbers) {
        entry.begin = token.begin;
        entry.decl = token.begin;
        entry.end = token.end;
      }
    }
    return entry;
  }

  /**
   * Check if this is the file doc
   * @private
   * @param {number} begin 
   * @param {number} end 
   * @param {string} doc 
   */
  checkFileDoc(begin, end, doc) {
    if (!this.docBegin && begin) {
      this.docBegin = begin;
      this.docEnd = end;
      this.doc = doc;
    }
  }

  /**
   * Returns token only if matches the specific kind
   * @param {FileTokenKind} kind 
   */
  expect(kind) {
    const token = this.next();
    if (!token) throw new Error(`Error at ${token.begin}: expected ${kind}`);
    if (token.kind !== kind) throw new Error(`Error at ${token.begin}: expected ${kind} got ${token.kind}`);
    return token;
  }
}

/**
 * Remove line numbers from entry
 * @param {ApiEntry} entry 
 */
function removeEntryLineNumbers(entry) {
  delete entry.decl;
  delete entry.begin;
  delete entry.end;
}

/**
 * 
 * @param {string} params 
 * @returns {ApiParameters}
 */
function parseParams(params) {
  if (!params?.length || params == 'void') {
    return [];
  }
  return params.split(',').map(param => {
    param = param.trim();
    const nameIndex = param.lastIndexOf(' ');
    if (nameIndex == -1) return param;
    let name = param.slice(nameIndex + 1).trim();
    let type = param.slice(0, nameIndex).trim();
    while (name.startsWith('*') || name.startsWith('&')) {
      type += name[0];
      name = name.slice(1);
    }
    type = normalizeType(type);
    return { name, type };
  });

}

/**
 * @typedef {ApiEntryKind|"doc"|"namespace"|"template"} FileTokenKind
 */

/**
 * @typedef {object} FileToken
 * @property {string}         value
 * @property {FileTokenKind}  kind
 * @property {string=}        parameters
 * @property {string=}        type
 * @property {boolean=}       constexpr
 * @property {boolean=}       immutable
 * @property {boolean=}       static
 * @property {number}         begin
 * @property {number}         end
 * @property {number}         spaces
 */

const memberSpecifiers = new Set(["inline", "static", "constexpr"]);

/**
 * 
 * @param {string[]} lines
 */
function tokenize(lines) {
  /** @type {FileToken[]} */
  const result = [];
  const spaceRegex = /^\s+/;
  const endCommentRegex = /\*\//;
  const continueCommentRegex = /^\s*\*\s?/;
  for (let i = 0; i < lines.length; i++) {
    const lineUntrimmed = lines[i];
    const line = lineUntrimmed.trim();
    if (!line || hasIgnoredPrefix(line)) continue;

    let m = spaceRegex.exec(lineUntrimmed);
    /** @type {FileToken} */
    const token = {
      begin: i + 1,
      spaces: m?.[0]?.length ?? 0,
    };

    if (m = /^\/\/\s*Forward decl/.test(line)) {
      token.kind = "doc";
    } else if (m = /^#pragma\s+region\s+impl/.test(line)) {
      break;
    } else if (m = /^\/\//.test(line)) {
      continue;
    } else if (m = /^\/\*(\*)?/.exec(line)) {
      let doc = '';
      const isDoc = !!m[1];
      for (; i < lines.length; ++i) {
        const line = lines[i];
        if (endCommentRegex.test(line)) {
          break;
        }
        const m = continueCommentRegex.exec(line);
        doc += m ? line.slice(m[0].length) : line;
        doc += '\n';
      }
      if (isDoc) {
        token.kind = "doc";
        token.value = doc.replace('/**', '').trim();
      } else continue;
    } else if (m = /^#define\s+(\w+)(\(([\w\s,]+)\))?/.exec(line)) {
      token.kind = "def";
      token.value = m[1];
      if (m[2]) token.parameters = m[3]?.trim();
      let ln = line;
      while (ln.endsWith('\\')) {
        if (i > lines.length) break;
        ln = lines[++i];
      }
      if (token.value.endsWith('_')) continue;
    } else if (m = /^typedef\s+(([\w*]+\s+)+\**)(\w+);/.exec(line)) {
      token.kind = "alias";
      token.value = m[3];
      token.type = m[1].trimEnd();
    } else if (m = /^using\s+(\w+)\s*=\s*([^;]+);/.exec(line)) {
      token.kind = "alias";
      token.value = m[1];
      token.type = m[2].trimEnd();
    } else if (m = /^using\s+([\w:]+)\s*;/.exec(line)) {
      token.kind = "alias";
      token.value = m[1];
    } else if (m = /^typedef\s+((\w+\s+)+\*?)\((SDLCALL )?\*(\w+)\)\(([^)]*)\)/.exec(line)) {
      token.value = m[4];
      token.kind = "callback";
      token.type = m[1].trimEnd();
      token.parameters = m[5]?.trim();
    } else if (m = /^typedef\s+(struct|enum|union)\s+(\w+)?$/.exec(line)) {
      token.kind = m[1];
      token.value = m[2];
      if (!line.endsWith("{")) i++;
      if (token.kind != "struct") {
        i = ignoreBody(lines, i + 1, token.spaces);
      }
    } else if (m = /^struct\s+([\w<>]+);/.exec(line)) {
      token.kind = "forward";
      token.value = m[1];
    } else if (m = /^struct\s+([\w<>]+)\s*(:\s*([\w<>,\s]+))?/.exec(line)) {
      token.kind = "struct";
      token.value = m[1];
      if (m[3]) {
        token.type = m[3].trim();
      }
      if (!line.endsWith("{")) i++;
    } else if (m = /^template</.exec(line)) {
      token.kind = "template";
      let parameters;
      if (line.endsWith(">")) {
        token.parameters = line.slice(m[0].length, line.length - 1).trim();
      } else {
        let parameters = line.slice(m[0].length) ?? "";
        for (++i; i < lines.length; ++i) {
          const line = lines[i];
          if (line.endsWith('>')) {
            parameters += '\n' + line.slice(0, line.length - 1);
            break;
          }
          parameters += '\n' + line;
        }
        token.parameters = parameters;
      }
    } else if (m = /^namespace\s+([^{]*)\{/.exec(line)) {
      token.kind = "namespace";
      token.value = m[1]?.trim();
    } else if (line.startsWith('#')) {
      let ln = line;
      while (ln.endsWith('\\')) {
        if (i > lines.length) break;
        ln = lines[i++];
      }
      continue;
    } else {
      const member = line.replaceAll(ignoreInSignature, "").trimStart();
      m = /^(([\w*]+\s+)*)(\w+)(\s*\()?/.exec(member);
      if (!m) {
        system.warn(`Unknown token at line ${i + 1}: ${member}`);
        continue;
      }
      token.value = m[3];
      const typeWords = m[1]?.trim()?.split(/\s+/) ?? [];
      for (let i = 0; i < typeWords.length; i++) {
        const word = typeWords[i];
        if (!memberSpecifiers.has(word)) {
          typeWords.splice(0, i);
          break;
        }
        switch (word) {
          case "constexpr": token.constexpr = true;
          case "static": token.static = true;
        }
      }

      token.type = normalizeType(typeWords.join(' '));
      let inline = false;
      if (m[4]) {
        token.kind = "function";
        let parameters = member.slice(m[0].length);
        const endBracket = parameters.indexOf(")");
        if (endBracket < 0) {
          for (++i; i < lines.length; ++i) {
            const line = lines[i];
            if (line.endsWith(');')) {
              inline = false;
              parameters += '\n' + line.slice(0, line.length - 2);
              break;
            }
            if (line.endsWith(')') || line.endsWith(') const')) {
              inline = true;
              parameters += '\n' + line.slice(0, line.length - 1);
              break;
            }
            parameters += '\n' + line;
          }
        } else {
          const details = parameters.slice(endBracket + 1);
          if (details.startsWith(" const")) token.immutable = true;
          inline = details.endsWith("{") || (!details.endsWith(";") && !details.endsWith("}"));
          parameters = parameters.slice(0, endBracket);
        }
        token.parameters = parameters;
      } else {
        token.kind = "var";
        inline = member.indexOf(';') === -1;
      }
      if (lines[i + 1].endsWith("{")) ++i;
      i = ignoreBody(lines, i + 1, token.spaces);
      if (!lines[i].endsWith("}")) i--;
    }
    token.end = i + 2;
    if (token.end - token.begin > 15 && token.kind != "doc") {
      system.warn(`Warning: Token at ${token.begin} seems very large ${token.value}`);
    }
    result.push(token);
  }
  return result;
}

/**
 * 
 * @param {string[]} lines 
 * @param {number} begin 
 * @param {number} spaces 
 */
function ignoreBody(lines, begin, spaces) {
  const spaceRegex = /^\s+/;
  for (let i = begin; i < lines.length; i++) {
    const indentation = spaceRegex.exec(lines[i])?.[0]?.length ?? 0;
    if (indentation <= spaces) {
      return i;
    }
  }
  return lines.length;
}

/** @param {string} line  */
function hasIgnoredPrefix(line) {
  for (const prefix of ignorePrefixes) {
    if (line.startsWith(prefix)) {
      return true;
    }
  }
  return false;
}

/** @param {string} typeString  */
function normalizeType(typeString) {
  return typeString.replaceAll(/(\w+)\s*([&*])/g, "$1 $2").replaceAll(/([*&])\s+(&*)/g, "$1$2");
}

exports.parseApi = parseApi;
exports.parseContent = parseContent;
exports.insertEntry = insertEntry;
exports.removeEntryLineNumbers = removeEntryLineNumbers;
