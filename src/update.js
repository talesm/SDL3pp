const { parseContent } = require("./parse.js");
const { readLinesSync, system, writeLinesSync } = require("./utils.js");

/**
 * @typedef {object} UpdateApiConfig
 * @prop {Api}       api
 * @prop {string}    baseDir
 */

/**
 * 
 * @param {UpdateApiConfig} config
 */
function updateApi(config) {
  const { api, baseDir } = config;
  const files = Object.keys(api.files);
  for (const name of files) {
    system.log(`Updating ${name}`);
    const filename = baseDir + name;
    const content = readLinesSync(filename);
    const targetFile = api.files[name];

    if (updateContent(content, targetFile) == 0) continue;
    writeLinesSync(filename, content);
  }
}

/**
 * Check and applies changes.
 * @param {string[]} content 
 * @param {ApiFile} targetFile 
 * @returns 0 if no changes happened, n > 0 if there are changes
 */
function updateContent(content, targetFile) {
  const name = targetFile.name;
  const sourceFile = parseContent(name, content, { storeLineNumbers: true });
  const { docBegin, docEnd, entriesBegin, entriesEnd } = sourceFile;
  const changes = checkChanges(sourceFile?.entries ?? {}, targetFile?.entries ?? {}, entriesBegin, entriesEnd, "").reverse();
  if (!changes.length) {
    system.log(`No changes for ${name}`);
    return 0;
  }
  if (targetFile.doc) {
    changes.push({
      begin: docBegin,
      end: docEnd,
      replacement: generateDocString(targetFile.doc),
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
 * @param {ApiEntries} sourceEntries 
 * @param {ApiEntries} targetEntries 
 * @param {number} begin 
 * @param {number} end 
 * @param {string=} prefix 
 */
function checkChanges(sourceEntries, targetEntries, begin, end, prefix) {
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
  let sourceIndex = 0;
  for (const targetName of targetNames) {
    const targetEntry = targetEntries[targetName];
    let index = sourceNames.indexOf(targetName, sourceIndex);
    if (index == -1) {
      logEntryAddition(targetEntry, begin);
      changes.push({
        begin,
        end: begin,
        replacement: "\n" + generateEntry(targetEntry, prefix) + "\n",
      });
    } else if (sourceIndex < sourceNames.length) {
      if (sourceIndex < index) {
        logEntryDeletion(Object.values(sourceEntries).slice(sourceIndex, index));
      }
      begin = getBegin(sourceEntries[sourceNames[sourceIndex]]);
      const sourceEntry = sourceEntries[targetName];
      const sourceEntryEnd = getEnd(sourceEntry);
      changes.push(...checkEntryChanges(targetName, sourceEntry, targetEntry, begin, sourceEntryEnd, prefix));
      begin = sourceEntryEnd;
      sourceIndex = index + 1;
    } else {
      logEntryAddition(targetEntry, begin);
      changes.push({
        begin: end,
        end,
        replacement: generateEntry(targetEntry, prefix) + '\n',
      });

    }
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
 */
function checkEntryChanges(name, sourceEntry, targetEntry, begin, end, prefix) {
  /** @type {Change[]} */
  const changes = [];
  if (Array.isArray(targetEntry)) {
    if (!Array.isArray(sourceEntry)) sourceEntry = [sourceEntry];
    for (let i = 0; i < targetEntry.length; i++) {
      const sourceSubEntry = sourceEntry[i];
      if (!sourceSubEntry) {
        system.log(`${name} overload(s) added to line ${end} or after`);
        changes.push({ begin: end, end, replacement: "\n" + generateEntry(targetEntry.slice(i), prefix) });
        break;
      }
      changes.push(...checkEntryChanges(name,
        sourceSubEntry,
        targetEntry[i],
        sourceSubEntry.begin,
        sourceSubEntry.end,
        prefix));
    }
    if (sourceEntry.length > targetEntry.length) {
      begin = sourceEntry[targetEntry.length].begin;
      system.log(`${name} overload(s) removed from line ${begin} to ${end}`);
      changes.push({ begin, end });

    }
    return changes;
  } else if (Array.isArray(sourceEntry)) {
    return checkEntryChanges(name, sourceEntry, [targetEntry], begin, end, prefix);
  }
  const change = checkEntryChanged(sourceEntry, targetEntry);
  const sourceEntriesCount = Object.keys(sourceEntry.entries ?? {})?.length ?? 0;
  const targetEntriesCount = Object.keys(targetEntry.entries ?? {})?.length ?? 0;
  if (change || (sourceEntriesCount == 0 && targetEntriesCount > 0)) {
    system.log(`${name} changed "${change}" from ${begin} to ${end}`);
    changes.push({ begin, end, replacement: generateEntry(targetEntry, prefix) });
  } else {
    if (sourceEntry.begin > begin) {
      changes.push({
        begin,
        end: Array.isArray(sourceEntry) ? sourceEntry[0].begin : sourceEntry.begin
      });
    }
    if (targetEntry.entries) {
      const sourceBegin = getBegin(sourceEntry.entries);
      const sourceEnd = getEnd(sourceEntry.entries);
      changes.push(...checkChanges(sourceEntry.entries, targetEntry.entries, sourceBegin, sourceEnd, prefix + "  "));
    }
  }
  return changes;
}

/** @param {ApiEntry|ApiEntry[]} entry  */
function logEntryDeletion(entry) {
  if (Array.isArray(entry)) {
    entry.forEach(logEntryDeletion);
  } else {
    system.log(`${entry.name} removed (from ${entry.begin} to ${entry.end})`);
  }
}

/** @param {ApiEntry|ApiEntry[]} entry  */
function logEntryAddition(entry, begin) {
  if (Array.isArray(entry)) {
    entry.forEach(e => logEntryAddition(e, begin));
  } else {
    system.log(`${entry.name} added to line ${begin} or after`);
  }
}

/**
 * 
 * @param {ApiEntry|ApiEntry[]} entry 
 */
function getBegin(entry) {
  if (!Array.isArray(entry)) {
    if (entry.kind) return entry.begin;
    return getBegin(Object.values(entry));
  }
  if (!entry.length) return null;
  return getBegin(entry[0]);
}

/**
 * 
 * @param {ApiEntry|ApiEntry[]} entry 
 */
function getEnd(entry) {
  if (!Array.isArray(entry)) {
    if (entry.kind) return entry.end;
    return getEnd(Object.values(entry));
  }
  if (!entry.length) return null;
  return getEnd(entry[entry.length - 1]);
}

/**
 * 
 * @param {ApiEntry} sourceEntry 
 * @param {ApiEntry} targetEntry 
 */
function checkEntryChanged(sourceEntry, targetEntry) {
  const keys = Object.keys(targetEntry).filter(k => k !== "doc" && k !== "entries" && k !== "sourceName");
  for (const key of keys) {
    if (checkValueChanged(sourceEntry[key], targetEntry[key])) return key;
  }
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
 * @param {string=} prefix 
 */
function generateEntries(entries, prefix) {
  const result = [];
  for (const name of Object.keys(entries)) {
    const entry = entries[name];
    if (Array.isArray(entry)) {
      result.push(entry.map(e => generateEntry(e, prefix)).join("\n\n"));
    } else {
      result.push(generateEntry(entry, prefix));
    }
  }
  return result.join('\n\n') + '\n';
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
  prefix = prefix ?? '';
  const doc = entry.doc ? generateDocString(entry.doc, prefix) + '\n' : '';
  const placeholder = 'static_assert(false, "Not implemented");';
  switch (entry.kind) {
    case "alias":
      if (!entry.type) return `${doc}${prefix}using ${entry.name};`;
      return `${doc}${prefix}using ${entry.name} = ${entry.type};`;
    case "forward":
      return '// Forward decl\n' + generateStructSignature(entry, prefix) + ';';
    case "function":
      var staticStr = entry.static ? "static " : "";
      var specifier = entry.constexpr ? "constexpr" : "inline";
      var constStr = entry.immutable ? " const" : "";
      const parameters = generateParameters(entry.parameters);
      const body = !entry.sourceName ? placeholder
        : (entry.type == "void" ? "" : "return ") + generateCall(entry.sourceName, ...entry.parameters);
      return `${doc}${prefix}${staticStr}${specifier} ${entry.type} ${entry.name}(${parameters})${constStr}\n${prefix}{\n${prefix}  ${body}\n${prefix}}`;
    case "struct":
      return doc + generateStruct(entry, prefix);
    case "var":
      var staticStr = entry.static ? "static " : "";
      var specifier = entry.constexpr ? "constexpr " : "";
      return `${doc}${prefix}${staticStr}${specifier}${entry.type} ${entry.name};`;
    default:
      system.warn(`Unknown kind: ${entry.kind} for ${entry.name}`);
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
    .join(", ");
  return `${name}(${paramStr});`;
}

/**
 * 
 * @param {ApiEntry} entry 
 * @param {string} prefix 
 */
function generateStruct(entry, prefix) {
  const signature = generateStructSignature(entry, prefix);
  const parent = entry.type ? ` : ${entry.type}` : "";
  const subEntries = generateEntries(entry.entries ?? {}, prefix + "  ");
  return `${signature}${parent}\n${prefix}{\n${subEntries}\n${prefix}};`;
}

/**
 * 
 * @param {ApiEntry} entry 
 * @param {string} prefix 
 */
function generateStructSignature(entry, prefix) {
  const template = entry.template ? generateTemplateSignature(entry.template, prefix) : '';
  return `${template}${prefix}struct ${entry.name}`;
}

/**
 * 
 * @param {(string|ApiParameter)[]} template 
 * @param {string} prefix 
 */
function generateTemplateSignature(template, prefix) {
  return `${prefix}template<${generateParameters(template)}>\n`;
}

/**
 * 
 * @param {(string|ApiParameter)[]} parameters 
 */
function generateParameters(parameters) {
  return parameters.map(p => generateParameter(p)).join(', ');
}

/**
 * 
 * @param {string|ApiParameter} parameter
 */
function generateParameter(parameter) {
  if (typeof parameter == "string") return parameter;
  if (!parameter.default) return `${parameter.type} ${parameter.name}`;
  return `${parameter.type} ${parameter.name} = ${parameter.default}`;
}

/**
 * 
 * @param {string} docStr 
 * @param {string=} prefix 
 */
function generateDocString(docStr, prefix) {
  if (!docStr) return '';
  prefix = prefix ?? '';
  const replacement = `\n${prefix} *$1`;
  docStr = docStr.split('\n').map(l => l ? `${prefix} * ${l}` : `${prefix} *`).join('\n');
  return `${prefix}/**\n${docStr}\n${prefix} */`;
}

exports.updateApi = updateApi;
exports.updateContent = updateContent;
exports.updateChanges = updateChanges;
