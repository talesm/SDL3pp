const { readFileSync, writeFileSync } = require("node:fs");
const { parseApi } = require("./read-api.js")
const targetConfig = require('./target.json');

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
      replacement: [wrapDocString(file.doc)],
    }, {
      begin: entriesBegin,
      end: entriesEnd,
      replacement: ["// Content here"],
    }];
    changes.reverse();
    for (const change of changes) {
      updateRange(content, change.begin, change.end, ...(change.replacement ?? []))
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
 * 
 * @param {string} docStr 
 * @param {number=} spaces 
 */
function wrapDocString(docStr, spaces) {
  if (!docStr) return '';
  const spacesStr = spaces ? ' '.repeat(spaces) : '';
  const replacement = `\n${spacesStr} * `;
  return `${spacesStr}/**\n * ${docStr?.replaceAll('\n', replacement)}\n${spacesStr} **/`
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
