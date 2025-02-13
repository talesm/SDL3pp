const { readFileSync } = require("node:fs");

const ignorePrefixes = [
  'void *alloca',
  'SDL_COMPILE_TIME_ASSERT(',
  'extern "C" {',
  'size_t strl',
  'size_t wcsl',
  'char *str',
  '}',
  'namespace',
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
    console.log(`Reading file ${name}`);
    const content = readFileSync(baseDir + name, 'utf-8').split(/\r?\n/);
    api.files[name] = parseContent(name, content, config);
  }
  return api;
}

exports.parseApi = parseApi;
exports.parseContent = parseContent;
exports.removeEntryLineNumbers = removeEntryLineNumbers;

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
  if (tokens?.length) {
    apiFile.doc = tokens.shift().value;
  }
  const parser = new ContentParser(tokens, config);
  const entries = apiFile.entries;
  for (const entry of parser.parseEntries()) {
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
  }
  return apiFile;
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
  }

  /** Parse all entries */
  parseEntries() {
    const entries = [];
    for (let entry = this.parseEntry(); !!entry; entry = this.parseEntry()) {
      entries.push(entry);
    }
    return entries;
  }

  /** Parses a single entry */
  parseEntry() {
    let lastEnd = 0;
    let lastDecl = 0;
    let lastBegin = 0;
    let lastDoc = "";
    /** @type {ApiParameters} */
    let lastTemplate = null;

    while (this.lookup()?.kind === "doc") {
      const token = this.match("doc");
      lastDoc = token.value;
      lastEnd = token.end;
      lastBegin = token.begin;
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
      begin: token.begin,
      decl: token.begin,
      end: token.end,
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
        entry.constexpr = token.constexpr;
        break;
      case "struct":
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
      entry.begin = lastBegin;
      if (lastDecl) entry.decl = lastDecl;
    }
    if (!this.storeLineNumbers) removeEntryLineNumbers(entry);
    return entry;
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
 * @typedef {"alias"|"callback"|"def"|"doc"|"enum"|"forward"|"function"|"struct"|"template"|"union"} FileTokenKind
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

    let m = lineUntrimmed.match(/^s+/);
    /** @type {FileToken} */
    const token = {
      begin: i + 1,
      spaces: m?.[0]?.length ?? 0,
    };

    if (m = line.match(/^\/\/\s*Forward decl/)) {
      token.kind = "doc";
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
    } else if (m = line.match(/^typedef\s+(struct|enum|union)\s+\w+\s*(\{\s*)?$/)) {
      token.kind = m[1];
      const parameters = [];
      i += m[2] ? 1 : 2;
      for (; i < lines.length; i++) {
        const line = lines[i].slice(spaces);
        if (line.startsWith('}')) {
          token.value = line.slice(1, line.length - 1).trim();
          break;
        }
        parameters.push(line);
      }
      token.parameters = parameters;
    } else if (m = line.match(/^struct\s+([\w<>]+);/)) {
      token.kind = "forward";
      token.value = m[1];
    } else if (m = line.match(/^struct\s+([\w<>]+)\s*(:\s*([\w<>,\s]+))?/)) {
      token.kind = "struct";
      token.value = m[1];
      if (m[3]) {
        token.type = m[3].trim();
      }
      const parameters = [];
      if (!line.endsWith("{};") && !lines[++i].endsWith("{};")) {
        if (lines[i].endsWith("{")) ++i;
        for (; i < lines.length; i++) {
          const line = lines[i].slice(spaces).trim();
          if (line.startsWith('}')) break;
          parameters.push(line);
        }
      }
      token.parameters = parameters;
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
    } else if (line.startsWith('#')) {
      let ln = line;
      while (ln.endsWith('\\')) {
        if (i > lines.length) break;
        ln = lines[i++];
      }
      continue;
    } else {
      let member = line.replaceAll(ignoreInSignature, "");
      m = member.match(/^(([\w*]+\s+)*)(\w+)\s*\(/);
      if (!m) {
        console.warn(`Unknown token at line ${i + 1}: ${line}`);
        continue;
      }
      token.kind = "function";
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
      if (inline && lines[i].indexOf('}') === -1) {
        for (i++; i < lines.length; i++) {
          const line = lines[i].slice(token.spaces);
          if (line.startsWith('}') || line.startsWith('{}')) break;
        }
      }
    }
    token.end = i + 2;
    if (token.end - token.begin > 15 && token.kind != "doc") {
      console.warn(`Warning: Token at ${token.begin} seems very large ${token.value}`);
    }
    result.push(token);
  }
  return result;
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
