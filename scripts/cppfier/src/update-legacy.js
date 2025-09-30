const { generateDocString, generateEntries, generateEntry, combineHints } = require("./generate.js");
const { parseContent } = require("./parse.js");
const { readLinesSync, system, writeLinesSync, looksLikeFreeFunction } = require("./utils.js");
const { existsSync } = require("fs");

/**
 * @import { Api, ApiEntries, ApiEntry, ApiFile, ApiParameter, ApiParameters } from "./types.js"
 */

/**
 * @typedef {object} UpdateApiConfig
 * @prop {Api}      api
 * @prop {string}   baseDir
 * @prop {Api=}     currentApi
 * @prop {boolean=} resetDoc
 */

/**
 * 
 * @param {UpdateApiConfig} config
 */
function updateApi(config) {
  const { api, baseDir } = config;
  const files = Object.keys(api.files);
  let totalChanges = 0;
  for (const name of files) {
    const filename = baseDir + name;
    if (!existsSync(filename)) {
      system.warn(`File not found: ${name}`);
      continue;
    }
    system.log(`Checking ${name}`);
    const content = readLinesSync(filename);
    const targetFile = api.files[name];

    const fileChanges = updateContent(content, targetFile, config);
    if (fileChanges == 0) continue;
    totalChanges += 1;
    writeLinesSync(filename, content);
  }
  if (totalChanges) {
    system.log(`Total of ${totalChanges} file(s) updated`);
  } else {
    system.log("Nothing to update");
  }
}

/**
 * @typedef {object} UpdateContentConfig
 * @prop {Api=}     currentApi
 * @prop {boolean=} resetDoc
 */

/**
 * Check and applies changes.
 * @param {string[]}            content 
 * @param {ApiFile}             targetFile 
 * @param {UpdateContentConfig} config
 * @returns 0 if no changes happened, n > 0 if there are changes
 */
function updateContent(content, targetFile, config = {}) {
  const name = targetFile.name;
  const sourceFile = config?.currentApi?.files?.[name] ?? parseContent(name, content, { storeLineNumbers: true });
  if (config.resetDoc && sourceFile.entries) resetEntriesDoc(sourceFile.entries);

  const changes = checkChanges(sourceFile?.entries ?? {}, targetFile?.entries ?? {}, sourceFile.entriesBegin, sourceFile.entriesEnd, "", config)
    .reverse();
  if (!changes.length) {
    return 0;
  }
  if (targetFile.doc && !sourceFile.doc) {
    changes.push({
      begin: sourceFile.entriesBegin,
      end: sourceFile.entriesBegin,
      replacement: generateDocString("\n" + targetFile.doc + "\n\n@{") + "\n",
    });
    changes.unshift({
      begin: sourceFile.entriesEnd,
      end: sourceFile.entriesEnd,
      replacement: "/// @}"
    });
  } else if (config.resetDoc) {
    changes.push({
      begin: sourceFile.docBegin,
      end: sourceFile.docEnd,
      replacement: generateDocString("\n" + targetFile.doc + "\n\n@{") + "\n",
    });
  }
  updateChanges(content, changes);
  return changes.length;
}

/**
 * 
 * @param {string[]} content 
 * @param {Change[]} changes 
 */
function updateChanges(content, changes) {
  for (const change of changes) {
    updateRange(content, change.begin, change.end, change.replacement ?? undefined);
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
 * @param {ApiEntries}  sourceEntries 
 * @param {ApiEntries}  targetEntries 
 * @param {number}      begin 
 * @param {number}      end 
 * @param {string}      prefix 
 * @param {UpdateContentConfig} config
 */
function checkChanges(sourceEntries, targetEntries, begin, end, prefix, config) {
  const targetNames = Object.keys(targetEntries);
  const sourceNames = Object.keys(sourceEntries);
  if (!targetNames.length) {
    if (!sourceNames.length || begin == end) return [];
    logEntryDeletion(Object.values(sourceEntries));
    return [{
      begin,
      end,
    }];
  }
  if (!sourceNames.length) {
    logEntryAddition(Object.values(targetEntries), begin);
    return [{
      begin,
      end,
      replacement: "\n" + generateEntries(targetEntries, prefix)
    }];
  }
  /** @type {Change[]} */
  const changes = [];
  const processed = new Set();
  let sourceIndex = 0;
  for (const targetName of targetNames) {
    processed.add(targetName);
    const targetEntry = targetEntries[targetName];
    let index = sourceNames.indexOf(targetName, sourceIndex);
    if (index == -1) {
      logEntryAddition(targetEntry, begin);
      changes.push({
        begin,
        end: begin,
        replacement: generateEntry(targetEntry, prefix) + "\n",
      });
    } else if (sourceIndex < sourceNames.length) {
      let deleteBegin = null;
      if (sourceIndex <= index) {
        let deleteEnd = null;
        while (sourceIndex < index) {
          const sourceName = sourceNames[sourceIndex];
          const sourceEntry = sourceEntries[sourceName];
          if (!processed.has(sourceName) && sourceEntries[sourceName]) {
            begin = getBegin(sourceEntry);
            if (deleteBegin != null) {
              changes.push({
                begin: deleteBegin,
                end: deleteEnd
              });
            }
            break;
          }
          logEntryDeletion(sourceEntry);
          const currBegin = getBegin(sourceEntry);
          if (deleteEnd != null && currBegin !== deleteEnd) {
            if (deleteBegin != null) {
              changes.push({
                begin: deleteBegin,
                end: deleteEnd
              });
            }
            deleteBegin = currBegin;
          } else if (deleteBegin == null) deleteBegin = currBegin;
          deleteEnd = getEnd(sourceEntry);
          sourceIndex += 1;
        }
      } else {
        begin = getBegin(sourceEntries[sourceNames[sourceIndex]]);
      }
      if (sourceIndex === index) {
        const sourceEntry = sourceEntries[targetName];
        const sourceEntryBegin = deleteBegin ?? getBegin(sourceEntry);
        const sourceEntryEnd = getEnd(sourceEntry);
        changes.push(...checkEntryChanges(targetName, sourceEntry, targetEntry, sourceEntryBegin, sourceEntryEnd, prefix, config));
        begin = sourceEntryEnd;
        sourceIndex++;
      } else {
        logEntryAddition(targetEntry, begin);
        changes.push({
          begin,
          end: begin,
          replacement: generateEntry(targetEntry, prefix) + '\n',
        });
      }
    } else {
      logEntryAddition(targetEntry, end);
      changes.push({
        begin: end,
        end,
        replacement: generateEntry(targetEntry, prefix) + '\n',
      });
    }
  }
  let deleteBegin = null;
  let deleteEnd = null;
  while (sourceIndex < sourceNames.length) {
    const sourceName = sourceNames[sourceIndex];
    const sourceEntry = sourceEntries[sourceName];
    if (!targetEntries[sourceName]) {
      begin = getBegin(sourceEntry);
      if (deleteBegin != null) {
        changes.push({
          begin: deleteBegin,
          end: deleteEnd
        });
      }
      deleteBegin = null;
    } else {
      logEntryDeletion(sourceEntry);
      const currBegin = getBegin(sourceEntry);
      if (deleteEnd != null && currBegin !== deleteEnd) {
        if (deleteBegin != null) {
          changes.push({
            begin: deleteBegin,
            end: deleteEnd
          });
        }
        deleteBegin = currBegin;
      } else if (deleteBegin == null) deleteBegin = currBegin;
      deleteEnd = getEnd(sourceEntry);
    }
    sourceIndex += 1;
  }
  if (deleteBegin) {
    changes.push({
      begin: deleteBegin,
      end: deleteEnd
    });
  }
  return changes;
}

/**
 * 
 * @param {string} name 
 * @param {ApiEntry|ApiEntry[]} sourceEntry 
 * @param {ApiEntry|ApiEntry[]} targetEntry 
 * @param {number} begin 
 * @param {number} end 
 * @param {string} prefix 
 * @param {UpdateContentConfig} config
 */
function checkEntryChanges(name, sourceEntry, targetEntry, begin, end, prefix, config) {
  /** @type {Change[]} */
  const changes = [];
  if (Array.isArray(targetEntry)) {
    if (!Array.isArray(sourceEntry)) sourceEntry = [sourceEntry];
    for (let i = 0; i < targetEntry.length; i++) {
      const sourceSubEntry = sourceEntry[i];
      if (!sourceSubEntry) {
        system.log(`${name} overload(s) added to line ${end} or after`);
        changes.push({ begin: end, end, replacement: generateEntry(targetEntry.slice(i), prefix) });
        break;
      }
      changes.push(...checkEntryChanges(name,
        sourceSubEntry,
        targetEntry[i],
        sourceSubEntry.begin,
        sourceSubEntry.end,
        prefix,
        config));
    }
    if (sourceEntry.length > targetEntry.length) {
      begin = sourceEntry[targetEntry.length].begin;
      system.log(`${name} overload(s) removed from line ${begin} to ${end}`);
      changes.push({ begin, end });

    }
    return changes;
  } else if (Array.isArray(sourceEntry)) {
    return checkEntryChanges(name, sourceEntry, [targetEntry], begin, end, prefix, config);
  }
  const change = checkEntryChanged(sourceEntry, targetEntry);
  const sourceEntriesCount = Object.keys(sourceEntry.entries ?? {})?.length ?? 0;
  const targetEntriesCount = Object.keys(targetEntry.entries ?? {})?.length ?? 0;
  if (change === "doc" && sourceEntry.decl && sourceEntry.kind !== 'var') {
    const end = sourceEntry.decl;
    system.log(`Documentation for ${name} changed "${change}" from ${begin} to ${end}`);
    changes.push({ begin, end, replacement: generateDocString(targetEntry.doc, prefix) });
  } else if (change || (sourceEntriesCount == 0 && targetEntriesCount > 0)) {
    system.log(`${name} changed "${change}" from ${begin} to ${end}`);
    if (change === "parameters") {
      system.log(`  parameters went from "${JSON.stringify(sourceEntry.parameters)}" to "${JSON.stringify(targetEntry.parameters)}"`);
    } else if (change !== 'since') {
      delete targetEntry.since;
    }
    changes.push({ begin, end, replacement: generateEntry(targetEntry, prefix) + "\n" });
  } else {
    if (sourceEntry.begin > begin) {
      changes.push({
        begin,
        end: Array.isArray(sourceEntry) ? sourceEntry[0].begin : sourceEntry.begin
      });
    }
    if (targetEntry.entries) {
      combineHints(targetEntry);
      const sourceBegin = getBegin(Object.values(sourceEntry.entries));
      const sourceEnd = getEnd(Object.values(sourceEntry.entries));
      changes.push(...checkChanges(sourceEntry.entries, targetEntry.entries, sourceBegin, sourceEnd, prefix + "  ", config));
    }
  }
  return changes;
}

/** @param {ApiEntry|ApiEntry[]|(ApiEntry|ApiEntry[])[]} entry  */
function logEntryDeletion(entry) {
  if (Array.isArray(entry)) {
    entry.forEach(logEntryDeletion);
  } else {
    system.log(`${entry.name} removed (from ${entry.begin} to ${entry.end})`);
  }
}

/** @param {ApiEntry|ApiEntry[]|(ApiEntry|ApiEntry[])[]} entry  */
function logEntryAddition(entry, begin) {
  if (Array.isArray(entry)) {
    entry.forEach(e => logEntryAddition(e, begin));
  } else {
    system.log(`${entry.name} added to line ${begin} or after`);
  }
}

/**
 * 
 * @param {ApiEntry|ApiEntry[]|(ApiEntry|ApiEntry[])[]} entry 
 */
function getBegin(entry) {
  if (!Array.isArray(entry)) return entry.begin;
  if (!entry.length) return null;
  return getBegin(entry[0]);
}

/**
 * 
 * @param {ApiEntry|ApiEntry[]|(ApiEntry|ApiEntry[])[]} entry 
 */
function getEnd(entry) {
  if (!Array.isArray(entry)) return entry.end;
  if (!entry.length) return null;
  return getEnd(entry[entry.length - 1]);
}

const filterOutAttributes = new Set([
  "doc",
  "entries",
  "sourceName",
  "value",
  "hints"
]);

/**
 * 
 * @param {ApiEntry} sourceEntry 
 * @param {ApiEntry} targetEntry 
 */
function checkEntryChanged(sourceEntry, targetEntry) {
  const keys = Object.keys(targetEntry).filter(k => !filterOutAttributes.has(k));
  for (const key of keys) {
    if (checkValueChanged(sourceEntry[key], targetEntry[key])) return key;
  }
  if (!sourceEntry.doc && targetEntry.doc) return "doc";
  return null;
}

function checkValueChanged(sourceValue, targetValue) {
  if (typeof targetValue === "undefined") return false;
  if (typeof sourceValue === "undefined") return !!targetValue;
  if (Array.isArray(targetValue)) {
    if (!Array.isArray(sourceValue) || targetValue.length !== sourceValue.length) return true;
    for (let i = 0; i < sourceValue.length; i++) {
      if (checkValueChanged(sourceValue[i], targetValue[i])) return true;
    }
  } else if (targetValue === true) {
    if (!sourceValue) return true;
  } else if (targetValue === null || typeof targetValue !== "object") {
    if (sourceValue !== targetValue) return true;
  } else if (sourceValue != null) {
    const keys = Object.keys(targetValue);
    for (const key of keys) {
      if (checkValueChanged(targetValue[key], sourceValue[key])) return true;
    }
  } else return true;
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
 * @param {ApiEntries} entries 
 */
function resetEntriesDoc(entries) {
  Object.values(entries).forEach(resetEntryDoc);
}

/**
 * Remove docs
 * @param {ApiEntry|ApiEntry[]} entry entry
 */
function resetEntryDoc(entry) {
  if (Array.isArray(entry)) {
    entry.forEach(e => e.doc = "");
    return;
  }
  entry.doc = "";
  if (entry.entries) resetEntriesDoc(entry.entries);
}

exports.updateApi = updateApi;
exports.updateContent = updateContent;
exports.updateChanges = updateChanges;
