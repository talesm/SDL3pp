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
  'SDL_FORCE_INLINE',
  'SDL_DECLSPEC',
  'SDL_MALLOC',
  'SDL_ALLOC_SIZE',
  'SDLCALL',
  'SDL_(OUT|IN|INOUT)_(Z_)?(BYTE)?CAP',
].join("|")})(\\(\\w*\\))?`, "g");

/**
 * @typedef {object} ParseConfig
 * @prop {string}   baseDir
 * @prop {string[]} files
 * @prop {boolean=} storeLineNumbers
 */

/**
 * @param {ParseConfig} config
 * 
 */
function parseApi(config) {
  const { baseDir, files } = config;

  /** @type {Api} */
  const api = { files: {} };
  for (const name of files) {
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
      const token = this.match("doc");
      this.checkFileDoc(lastBegin, lastEnd, lastDoc);
      lastDoc = token.value;
      lastEnd = token.end;
      lastBegin = token.begin;
    }

    if (this.lookup()?.kind === "namespace") {
      const token = this.match("namespace");
      this.checkFileDoc(lastBegin, lastEnd, lastDoc);
      if (!this.entriesBegin) {
        this.entriesBegin = token.end;
      }
      return this.parseEntry();
    }

    if (this.lookup()?.kind === "template") {
      const token = this.match("template");
      if (lastEnd != token.begin || lastTemplate) {
        lastBegin = token.begin;
        lastDoc = null;
      }
      lastTemplate = token.parameters;
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
        entry.parameters = parseParams(token.parameters);
        break;
      case "enum":
        entry.parameters = token.parameters.map(p => p.replace(',', '').trim()).filter(p => !!p);
        break;
      case "function":
        entry.type = token.type;
        entry.parameters = parseParams(token.parameters);
        if (token.constexpr) entry.constexpr = token.constexpr;
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
      if (lastTemplate) entry.template = parseParams(lastTemplate);
      if (this.storeLineNumbers) {
        entry.begin = lastBegin;
        entry.decl = lastDecl || token.begin;
        entry.end = token.end;
      }
    } else if (this.storeLineNumbers) {
      entry.begin = token.begin;
      entry.decl = token.begin;
      entry.end = token.end;
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
  match(kind) {
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
  if (params?.length == 0 || params == 'void') {
    return [];
  }
  return params.split(',').map(param => {
    param = param.trim();
    const nameIndex = param.lastIndexOf(' ');
    if (nameIndex == -1) return param;
    let name = param.slice(nameIndex + 1).trim();
    let type = param.slice(0, nameIndex).trim();
    while (name.startsWith('*')) {
      name = name.slice(1);
      type += '*';
    }
    type = type.replaceAll(/(\w)\*/g, '$1 *').replaceAll(/\s+/g, ' ');
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
  for (let i = 0; i < lines.length; i++) {
    const lineUntrimmed = lines[i];
    const line = lineUntrimmed.trim();
    if (!line || hasIgnoredPrefix(line)) continue;

    let m = lineUntrimmed.match(/^\s+/);
    /** @type {FileToken} */
    const token = {
      begin: i + 1,
      spaces: m?.[0]?.length ?? 0,
    };

    if (m = line.match(/^\/\/\s*Forward decl/)) {
      token.kind = "doc";
    } else if (m = line.match(/^#pragma\s+region\s+impl/)) {
      break;
    } else if (m = line.match(/^\/\//)) {
      continue;
    } else if (m = line.match(/^\/\*(\*)?/)) {
      let doc = '';
      const isDoc = !!m[1];
      for (; i < lines.length; ++i) {
        const line = lines[i];
        if (line.match(/\*\//)) {
          break;
        }
        const m = line.match(/^\s*\*\s?/);
        doc += m ? line.slice(m[0].length) : line;
        doc += '\n';
      }
      if (isDoc) {
        token.kind = "doc";
        token.value = doc.replace('/**', '').trim();
      } else continue;
    } else if (m = line.match(/^#define\s+(\w+)(\(([\w\s,]+)\))?/)) {
      token.kind = "def";
      token.value = m[1];
      if (m[2]) token.parameters = m[3].split(',').map(p => p.trim());
      let ln = line;
      while (ln.endsWith('\\')) {
        if (i > lines.length) break;
        ln = lines[++i];
      }
      if (token.value.endsWith('_')) continue;
    } else if (m = line.match(/^typedef\s+(([\w*]+\s+)+\**)(\w+);/)) {
      token.kind = "alias";
      token.value = m[3];
      token.type = m[1].trimEnd();
    } else if (m = line.match(/^using\s+(\w+)\s*=\s*([^;]+);/)) {
      token.kind = "alias";
      token.value = m[1];
      token.type = m[2].trimEnd();
    } else if (m = line.match(/^typedef\s+((\w+\s+)+\*?)\((SDLCALL )?\*(\w+)\)\(([^)]*)\)/)) {
      token.value = m[4];
      token.kind = "callback";
      token.type = m[1].trimEnd();
      token.parameters = m[5]?.split(',')?.map(p => p.trim()) ?? [];
    } else if (m = line.match(/^typedef\s+(struct|enum|union)\s+(\w+)?$/)) {
      token.kind = m[1];
      token.value = m[2];
      if (!line.endsWith("{")) i++;
      if (token.kind != "struct") {
        i = ignoreBody(lines, i + 1, token.spaces);
      }
    } else if (m = line.match(/^struct\s+([\w<>]+);/)) {
      token.kind = "forward";
      token.value = m[1];
    } else if (m = line.match(/^struct\s+([\w<>]+)\s*(:\s*([\w<>,\s]+))?/)) {
      token.kind = "struct";
      token.value = m[1];
      if (m[3]) {
        token.type = m[3].trim();
      }
      if (!line.endsWith("{")) i++;
    } else if (m = line.match(/^template</)) {
      token.kind = "template";
      let parameters;
      if (line.endsWith(">")) {
        parameters = line.slice(m[0].length, line.length - 1);
      } else {
        parameters = line.slice(m[0].length) ?? "";
        for (++i; i < lines.length; ++i) {
          const line = lines[i];
          if (line.endsWith('>')) {
            parameters += '\n' + line.slice(0, line.length - 1);
            break;
          }
          parameters += '\n' + line;
        }
      }
      token.parameters = parameters.split(",").map(p => p.trim()) ?? [];
    } else if (m = line.match(/^namespace\s+([^{]*)\{/)) {
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
      const member = line.replaceAll(ignoreInSignature, "");
      m = /^(([\w*]+\s+)*)(\w+)(\s*\()?/.exec(member);
      if (!m) {
        system.warn(`Unknown token at line ${i + 1}: ${line}`);
        continue;
      }
      token.kind = m[4] ? "function" : "var";
      token.value = m[3];
      const typeWords = m[1]?.trim()?.split(/\s+/) ?? [];
      for (let i = 0; i < typeWords.length; i++) {
        const word = typeWords[i];
        if (!memberSpecifiers.has(word)) {
          typeWords.splice(0, i);
          break;
        }
        if (word == "constexpr") token.constexpr = true;
      }

      token.type = normalizeType(typeWords.join(' '));
      let inline = false;
      if (m[4]) {
        let parameters = line.slice(m[0].length);
        const endBracket = parameters.indexOf(")");
        if (endBracket < 0) {
          for (++i; i < lines.length; ++i) {
            const line = lines[i];
            if (line.endsWith(');')) {
              inline = false;
              parameters += '\n' + line.slice(0, line.length - 2);
              break;
            }
            if (line.endsWith(')')) {
              inline = true;
              parameters += '\n' + line.slice(0, line.length - 1);
              break;
            }
            parameters += '\n' + line;
          }
        } else {
          inline = parameters[endBracket + 1] != ";" && !parameters.endsWith("}");
          parameters = parameters.slice(0, endBracket);
        }
        token.parameters = parameters;
      } else {
        inline = member.indexOf(';') === -1;
      }
      if (inline && member.indexOf('}') === -1) {
        if (lines[++i].startsWith("{")) ++i;
        ignoreBody(lines, i, token.spaces);
      }
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
  for (let i = begin; i < lines.length; i++) {
    const indentation = lines[i].match(/^\s+/)?.[0] ?? "";
    if (indentation.length <= spaces) {
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
