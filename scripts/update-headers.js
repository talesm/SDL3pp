const { readFileSync, writeFileSync } = require("node:fs");
const { parseApi } = require("./read-api.js")
const targetConfig = require('./target.json');
const { generateKey } = require("node:crypto");

const baseDir = 'include/SDL3pp/'
const filename = 'stdinc.hpp'


if (require.main == module) {
  updateHeaders([filename])
}

/**
 * 
 * @param {string[]} names 
 */
function updateHeaders(names) {
  names = Object.keys(targetConfig.files)
    .filter(names?.length ? (name => names.includes(name)) : (() => true));
  for (const name of names) {
    console.log(`Updating ${name}`);
    const filename = baseDir + name;
    const content = readFileSync(filename, "utf8").split('\n');
    const [docBegin, docEnd] = getDocRange(content);
    const [entriesBegin, entriesEnd] = getEntriesRange(content, docEnd);

    const file = targetConfig.files[name];
    const changes = [{
      begin: docBegin,
      end: docEnd,
      replacement: wrapDocString(file.doc),
    }, {
      begin: entriesBegin,
      end: entriesEnd,
      replacement: generateEntries(file.entries),
    }];
    changes.reverse();
    for (const change of changes) {
      updateRange(content, change.begin, change.end, change.replacement ?? undefined)
    }
    writeFileSync(filename, content.join('\n').trim() + '\n');
  }
}

/**
 * 
 * @param {string[]} content 
 * @param {number} begin 
 * @param {number} end 
 * @param {...string} replacement 
 */
function updateRange(content, begin, end, ...replacement) {
  if (end < begin) end = begin;
  if (end == begin && !replacement?.length) return;
  content.splice(begin - 1, end - begin, ...replacement);
}

/**
 * @typedef {object} ApiEntry
 * @property {string} name
 * @property {'alias'|'callback'|'def'|'enum'|'function'|'struct'|'union'} kind
 * @property {string=} type
 * @property {string[]} parameters
 * @property {string} doc
 * @property {number} begin
 * @property {number} decl
 * @property {number} end
 */

/**
 * 
 * @param {{[name: string]: ApiEntry|ApiEntry[]}} entries 
 * @param {string=} prefix 
 */
function generateEntries(entries, prefix) {
  const result = [];
  for (const name of Object.keys(entries)) {
    const entry = entries[name];
    if (Array.isArray(entry)) {

      result.push(entry.map(e => generateEntry(e, prefix)));
    } else {
      result.push(generateEntry(entry, prefix));
    }
  }
  return '\n' + result.join('\n\n') + '\n';
}

/**
 * 
 * @param {ApiEntry} entry 
 * @param {string=} prefix 
 */
function generateEntry(entry, prefix) {
  prefix = prefix ?? ''
  const doc = entry.doc ? wrapDocString(entry.doc, prefix) + '\n' : '';
  const placeholder = 'static_assert(false, "Not implemented");';
  switch (entry.kind) {
    case "alias":
      return `${doc}${prefix}using ${entry.name} = ${entry.type};`
    case "function":
      const specifier = entry.constexpr ? "constexpr" : "inline";
      const parameters = generateParameters(entry.parameters);
      const body = !entry.sourceName ? placeholder
        : (entry.type == "void" ? "" : "return ") + generateCall(entry.sourceName, ...entry.parameters);
      return `${doc}${prefix}${specifier} ${entry.type} ${entry.name}(${parameters})\n${prefix}{\n${prefix}  ${body}\n${prefix}}`
    default:
      console.warn(`Unknown kind: ${entry.kind} for ${entry.name}`)
      return `${doc}#${prefix}error "${entry.name} (${entry.kind})"`;
  }
}

/**
 * 
 * @param {string} name 
 * @param {string|{type: string, name: string}} parameters 
 */
function generateCall(name, ...parameters) {
  const paramStr = parameters
    .map(p => typeof p == "string" ? p : p.name)
    .join(", ")
  return `${name}(${paramStr});`
}

/**
 * 
 * @param {(string|{type: string, name: string})[]} parameters 
 */
function generateParameters(parameters) {
  return parameters
    .map(p => typeof p == "string" ? p : `${p.type} ${p.name}`)
    .join(', ');
}

/**
 * 
 * @param {string} docStr 
 * @param {string=} prefix 
 */
function wrapDocString(docStr, prefix) {
  if (!docStr) return '';
  prefix = prefix ?? '';
  const replacement = `\n${prefix} *$1`;
  docStr = docStr.split('\n').map(l => l ? `${prefix} * ${l}` : `${prefix} *`).join('\n')
  return `${prefix}/**\n${docStr}\n${prefix} **/`
}

/**
 * 
 * @param {string[]} content 
 * @param {number=} current 
 * @returns {[number, number]}
 */
function getDocRange(content, current) {
  current = current ? current - 1 : 0;
  if (!content[current].startsWith('/**')) return [current + 1, current + 1];
  for (var i = current + 1; i < content.length; i++) {
    if (!content[i].startsWith(' *')) break;
  }
  return [current + 1, i + 1];
}

/**
* 
* @param {string[]} content 
* @param {number=} current 
* @returns {[number, number]}
*/
function getEntriesRange(content, current) {
  current = current ? current - 1 : 0;
  const begin = content.indexOf('namespace SDL {', current) + 1;
  const end = content.indexOf('// #pragma region implementation', begin)
  return [Math.max(begin, current) + 1, (end != -1 ? end : content.lastIndexOf('} // namespace SDL')) + 1];
}
