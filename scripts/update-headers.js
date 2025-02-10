const { readFileSync, writeFileSync } = require("node:fs");
const { parseApi, parseContent } = require("./read-api.js")
const target = require('./target.json');

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
  names = Object.keys(target.files)
    .filter(names?.length ? (name => names.includes(name)) : (() => true));
  for (const name of names) {
    console.log(`Updating ${name}`);
    const filename = baseDir + name;
    const content = readFileSync(filename, "utf8").split('\n');
    const [docBegin, docEnd] = getDocRange(content);
    const [entriesBegin, entriesEnd] = getEntriesRange(content, docEnd);

    const targetFile = target.files[name];
    const sourceFile = parseContent(name, content);
    const changes = checkChanges(sourceFile, targetFile, entriesBegin, entriesEnd);
    if (!changes.length) {
      console.log(`No changes for ${name}`)
      continue;
    }
    changes.push({
      begin: docBegin,
      end: docEnd,
      replacement: wrapDocString(targetFile.doc),
    });
    for (const change of changes) {
      updateRange(content, change.begin, change.end, change.replacement ?? undefined)
    }
    writeFileSync(filename, content.join('\n').trim() + '\n');
  }
}

/**
 * @typedef {object} Change
 * @property {number} begin
 * @property {number} end
 * @property {string=} replacement
 */

/**
 * 
 * @param {ApiFile} sourceFile 
 * @param {ApiFile} targetFile 
 * @param {number} begin 
 * @param {number} end 
 * @param {string=} prefix 
 */
function checkChanges(sourceFile, targetFile, begin, end, prefix) {
  /** @type {Change[]} */
  const changes = [];
  const targetEntries = targetFile.entries
  const targetNames = Object.keys(targetEntries)
  if (!targetNames.length) {
    changes.push({
      begin,
      end,
    })
    return changes;
  }
  const sourceEntries = sourceFile.entries
  const sourceNames = Object.keys(sourceEntries)
  if (!sourceNames?.length) {
    changes.push({
      begin,
      end,
      replacement: generateEntries(targetEntries, prefix)
    })
    return changes;
  }
  let sourceIndex = 0;
  for (const targetName of targetNames) {
    const targetEntry = targetEntries[targetName];
    let index = sourceNames.indexOf(targetName, sourceIndex);
    if (index == -1) {
      console.log(`${targetEntry.name} added ${begin}`)
      changes.push({
        begin,
        end: begin,
        replacement: '\n' + generateEntry(targetEntry, prefix),
      })
    } else {
      begin = sourceEntries[sourceNames[sourceIndex]].begin;
      const sourceEntry = sourceEntries[sourceNames[index]];
      if (checkEntryChanged(sourceEntry, targetEntry)) {
        console.log(`${targetEntry.name} changed ${begin} - ${sourceEntry.end}`)
        changes.push({
          begin,
          end: sourceEntry.end,
          replacement: generateEntry(targetEntry, prefix)
        })
        begin = sourceEntry.begin + 1;
        // } else if (sourceEntry.entries || targetEntry.entries) {
        // TODO compound structs
      } else {
        if (index > sourceIndex) {
          changes.push({
            begin,
            end: sourceEntry.begin
          })
        }
        begin = sourceEntry.end;
      }
      sourceIndex = index + 1;
    }
  }

  return changes.reverse();
}

/**
 * 
 * @param {ApiEntry} sourceEntry 
 * @param {ApiEntry} targetEntry 
 */
function checkEntryChanged(sourceEntry, targetEntry) {
  if (sourceEntry.kind != targetEntry.kind) return true;
  if (targetEntry.type && sourceEntry.type != targetEntry.type) return true;
  if (targetEntry.parameters) {
    const sourceParameters = sourceEntry.parameters;
    const targetParameters = targetEntry.parameters;
    if (sourceEntry.parameters?.length != targetEntry.parameters.length) return true;
    for (let i = 0; i < targetParameters.length; i++) {
      const targetParameter = targetParameters[i];
      const sourceParameter = sourceParameters[i];
      if (typeof targetParameter == "string") {
        if (targetParameter !== sourceParameter) return true;
      } else if (targetParameter.name != sourceParameter?.name
        || targetParameter.type != sourceParameter?.type) {
        return true;
      }
    }
  }
  return false;
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
 * @param {ApiEntry|ApiEntry[]} entry 
 * @param {string=} prefix 
 * @returns {string}
 */
function generateEntry(entry, prefix) {
  if (Array.isArray(entry)) {
    return entry.map(e => generateEntry(e, prefix)).join('\n\n');
  }
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
