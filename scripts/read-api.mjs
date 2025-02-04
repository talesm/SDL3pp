import { readFileSync, writeFileSync } from "node:fs";

const baseDir = '/usr/local/include/SDL3/'
const filename = 'SDL_stdinc.h'

const ignorePrefixes = [
  'void *alloca',
  'SDL_COMPILE_TIME_ASSERT(',
  'extern "C" {',
  'size_t strl',
  'size_t wcsl',
  'char *str',
  '}',
];

writeFileSync('scripts/source.json', JSON.stringify(parseApi([filename]), null, 2))


/**
 * 
 * @param {string} baseDir 
 * @param {string[]} names 
 * 
 * @returns {{files: {[file: string]: ApiFile}}}
 */
function parseApi(names) {
  const files = {};
  for (const name of names) {
    console.log(`Reading file ${name}`)
    files[name] = parseContent(name)
  }
  return { files };
}


/**
 * @typedef {object} ApiFile
 * @property {string} name
 * @property {string=} doc
 * @property {{[name: string]: ApiEntry|ApiEntry[]}} entries
 */

/**
 * 
 * @param {string} name 
 */
function parseContent(name) {
  const content = readFileSync(baseDir + name, 'utf-8')
  const tokens = tokenize(content.split('\n'))

  /** @type {ApiFile} */
  const apiFile = {
    name,
    doc: '',
    entries: {},
  }
  if (tokens?.length) {
    apiFile.doc = tokens.unshift().value;
  }
  const entries = apiFile.entries
  /** @type {FileToken} */
  let lastDoc = null;
  for (const token of tokens) {
    if (token.kind == 'doc') {
      lastDoc = token;
      continue;
    }
    const entry = parseToken(token);
    if (token.begin == lastDoc?.end) {
      entry.doc = lastDoc.value;
      entry.begin = lastDoc.begin;
    }
    lastDoc = null;
    const name = entry.name;
    if (!name) continue;
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
  return apiFile
}

/**
 * @typedef {object} ApiEntry
 * @property {string} name
 * @property {'alias'|'callback'|'def'|'enum'|'function'|'struct'|'union'} kind
 * @property {string=} type
 * @property {string[]} parameters
 * @property {string} doc
 * @property {string} decl
 * @property {number} begin
 * @property {number} declPos
 * @property {number} end
 */

/**
 * 
 * @param {FileToken} token 
 */
function parseToken(token) {
  /** @type {ApiEntry} */
  const entry = {
    doc: '',
    begin: token.begin,
    declPos: token.begin,
    end: token.end,
    name: token.value,
    kind: token.kind,
  }
  switch (token.kind) {
    case "alias":
      entry.type = token.type;
      break;
    case "callback":
      entry.type = token.type;
      entry.parameters = parseParams(token.parameters)
      break
    case "def":
      entry.parameters = token.parameters;
      break;
    case "enum":
      entry.parameters = token.parameters.map(p => p.replace(',', '').trim()).filter(p => !!p);
      break;
    case "function":
      entry.type = token.type;
      entry.parameters = parseParams(token.parameters)
      break;
    case "struct":
      entry.parameters = parseParams(token.parameters.map(p => p.replace(';', '')));
      break;
    default:
      console.error(`Unimplemented kind ${token.kind} (${token.value}) at ${token.begin}`);
      break;
  }
  return entry;
}

/**
 * 
 * @param {string[]} params 
 */
function parseParams(params) {
  if (params[0] == 'void') {
    return []
  }
  return params.map(param => {
    const nameIndex = param.lastIndexOf(' ');
    if (nameIndex == -1) return param;
    return {
      name: param.slice(nameIndex + 1).trim(),
      type: param.slice(0, nameIndex).trim(),
    }
  })

}

/**
 * @typedef {object} FileToken
 * @property {string} value
 * @property {"alias"|"callback"|"def"|"doc"|"enum"|"function"|"struct"|"union"} kind
 * @property {string[]=} parameters
 * @property {string=} type
 * @property {number} begin
 * @property {number} end
 * @property {number} spaces
 */


/**
 * 
 * @param {string[]} lines
 */
function tokenize(lines) {
  /** @type {FileToken[]} */
  const result = []
  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    if (line.match(/^\s*($|\/\/)/)) continue;
    /** @type {FileToken} */
    const token = {
      begin: i + 1,
    }
    let m;
    if (m = line.match(/^(\s*)(\/\*.*$)/)) {
      let doc = '';
      const isDoc = m[2] == '/**';
      const spaces = m[1]?.length ?? 0;
      for (; i < lines.length; ++i) {
        const line = lines[i];
        if (line.match(/\*\//)) {
          break
        }
        const m = line.match(/^\s*\*\s?/)
        doc += m ? line.slice(m[0].length) : line;
        doc += '\n';
      }
      if (isDoc) {
        token.kind = "doc";
        token.value = doc.replace('/**', '').trim();
        token.spaces = spaces;
      } else continue;
    } else if (m = line.match(/^#define\s+(\w+)(\(([\w\s,]+)\))?/)) {
      token.kind = "def";
      token.value = m[1]
      if (m[2]) token.parameters = m[3].split(',').map(p => p.trim());
      let ln = line;
      while (ln.endsWith('\\')) {
        if (i > lines.length) break;
        ln = lines[++i]
      }
    } else if (m = line.match(/^(\s*)typedef\s+(([\w*]+\s+)+\**)(\w+);/)) {
      token.kind = "alias";
      token.value = m[4];
      token.type = m[2].trimEnd();
      token.spaces = m[1]?.length ?? 0;
    } else if (m = line.match(/^(\s*)typedef\s+((\w+\s+)+\*?)\((SDLCALL )?\*(\w+)\)\(([^)]*)\)/)) {
      token.value = m[5];
      token.kind = "callback";
      token.type = m[2].trimEnd();
      token.parameters = m[6]?.split(',')?.map(p => p.trim()) ?? [];
    } else if (m = line.match(/^(\s*)typedef\s+(struct|enum|union)\s+\w+\s*(\{\s*)?$/)) {
      token.kind = m[2];
      const spaces = m[1]?.length ?? 0
      token.spaces = spaces;
      const parameters = []
      i += m[3] ? 1 : 2;
      for (; i < lines.length; i++) {
        const line = lines[i].slice(spaces);
        if (line.startsWith('}')) {
          token.value = line.slice(1, line.length - 1).trim();
          break;
        }
        parameters.push(line);
      }
      token.parameters = parameters;
    } else if (m = line.match(/^(\s*)(extern|inline|SDL_FORCE_INLINE) (SDL_DECLSPEC )?(SDL_MALLOC )?(SDL_ALLOC_SIZE\d?\([\d, ]*\) )?/)) {
      const signature = line.slice(m[0].length).replaceAll(/SDL_(OUT|IN|INOUT)_(Z_)?(BYTE)?CAP\(\w+\)/g, "")
      const spaces = m[1]?.length ?? 0;
      if (signature == '"C" {') continue;
      m = signature.match(/(SDLCALL )?(\w+)\(([^)]*)(\))?/);
      token.value = m[2];
      token.kind = "function";
      token.type = signature.slice(0, m.index).trim();
      let parameters = m[3] ?? ''
      let inline = !signature.endsWith(';');
      if (!m[4]) {
        for (++i; i < lines.length; ++i) {
          const line = lines[i];
          if (line.endsWith(');') || line.endsWith(')')) {
            inline = !line.endsWith(';')
            parameters += '\n' + line.slice(line.length - 2);
            break;
          }
          parameters += '\n' + line;
        }
      }
      token.parameters = parameters.split(',').map(p => p.trim()) ?? []
      if (inline && lines[i].indexOf('}') === -1) {
        for (i++; i < lines.length; i++) {
          const line = lines[i].slice(spaces);
          if (line.startsWith('}') || line.startsWith('{}')) break;
        }
      }
    } else if (line.startsWith('#')) {
      let ln = line;
      while (ln.endsWith('\\')) {
        if (i > lines.length) break;
        ln = lines[i++]
      }
      continue;
    } else {
      let found = false;
      for (const prefix of ignorePrefixes) {
        if (line.startsWith(prefix)) {
          found = true;
          break
        }
      }
      if (found) continue;
      console.warn(`Unknown token at line ${i + 1}: ${line}`)
      continue;
    }
    token.end = i + 2;
    if (token.end - token.begin > 15 && token.kind != "doc") {
      console.warn(`Warning: Token at ${token.begin} seems very large ${token.value}`)
    }
    result.push(token);
  }
  return result
}
