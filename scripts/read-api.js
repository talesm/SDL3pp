
const { readFileSync, writeFileSync } = require("node:fs");

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
  'namespace SDL {',
];

if (require.main == module) {
  writeFileSync('scripts/source.json', JSON.stringify(parseApi(baseDir, [filename]), null, 2))
}

/**
 * 
 * @param {string} baseDir 
 * @param {string[]} names 
 * 
 * @returns {Api}
 */
function parseApi(baseDir, names) {
  const files = {};
  for (const name of names) {
    console.log(`Reading file ${name}`)
    const content = readFileSync(baseDir + name, 'utf-8').split('\n');
    files[name] = parseContent(name, content)
  }
  return { files };
}

exports.parseApi = parseApi;

/**
 * 
 * @param {string} name
 * @param {string[]} content 
 */
function parseContent(name, content) {
  const tokens = tokenize(content);

  /** @type {ApiFile} */
  const apiFile = {
    name,
    doc: '',
    entries: {},
  }
  if (tokens?.length) {
    apiFile.doc = tokens.shift().value;
  }
  const entries = apiFile.entries
  /** @type {FileToken} */
  let lastDoc = null;
  for (const token of tokens) {
    if (token.kind == 'doc') {
      lastDoc = token;
      continue;
    }
    const entry = parseEntry(token);
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
exports.parseContent = parseContent;

/**
 * 
 * @param {FileToken} token 
 */
function parseEntry(token) {
  /** @type {ApiEntry} */
  const entry = {
    doc: '',
    begin: token.begin,
    decl: token.begin,
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
  if (params.length == 1 && (params[0] == 'void' || params[0] == '')) {
    return []
  }
  return params.map(param => {
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
  })

}

/**
 * @typedef {object} FileToken
 * @property {string} value
 * @property {"alias"|"callback"|"def"|"doc"|"enum"|"function"|"struct"|"union"} kind
 * @property {string[]=} parameters
 * @property {string=} type
 * @property {boolean=} constexpr
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
      if (token.value.endsWith('_')) continue;
    } else if (m = line.match(/^(\s*)typedef\s+(([\w*]+\s+)+\**)(\w+);/)) {
      token.kind = "alias";
      token.value = m[4];
      token.type = m[2].trimEnd();
      token.spaces = m[1]?.length ?? 0;
    } else if (m = line.match(/^(\s*)using\s+(\w+)\s*=\s*([^;]+);/)) {
      token.kind = "alias";
      token.value = m[2];
      token.type = m[3].trimEnd();
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
    } else if (m = line.match(/^(\s*)(extern|inline|constexpr|SDL_FORCE_INLINE) (SDL_DECLSPEC )?(SDL_MALLOC )?(SDL_ALLOC_SIZE\d?\([\d, ]*\) )?/)) {
      token.constexpr = m[2] == "constexpr";
      const signature = line.slice(m[0].length).replaceAll(/SDL_(OUT|IN|INOUT)_(Z_)?(BYTE)?CAP\(\w+\)/g, "")
      const spaces = m[1]?.length ?? 0;
      if (signature == '"C" {') continue;
      m = signature.match(/(SDLCALL )?(\w+)\(([^)]*)(\))?/);
      token.value = m[2];
      token.kind = "function";
      token.type = normalizeType(signature.slice(0, m.index).trim());
      let parameters = m[3] ?? ''
      let inline = !signature.endsWith(';');
      if (!m[4]) {
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

/** @param {string} typeString  */
function normalizeType(typeString) {
  return typeString.replaceAll(/(\w+)\s*(\*)/g, "$1 $2");
}
