const { parseContent } = require("./parse.js");
const { readLinesSync, system, writeLinesSync, looksLikeFreeFunction } = require("./utils.js");
const { existsSync } = require("fs");

/**
 * @import { Api, ApiEntries, ApiEntry, ApiFile, ApiParameter, ApiParameters } from "./types"
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

  const changes = checkChanges(sourceFile?.entries ?? {}, targetFile?.entries ?? {}, sourceFile.entriesBegin, sourceFile.entriesEnd)
    .reverse();
  if (!changes.length) {
    return 0;
  }
  if (targetFile.doc && !sourceFile.doc) {
    changes.push({
      begin: sourceFile.entriesBegin,
      end: sourceFile.entriesBegin,
      replacement: generateDocString("\n" + targetFile.doc + "\n\n@{"),
    });
    changes.unshift({
      begin: sourceFile.entriesEnd,
      end: sourceFile.entriesEnd,
      replacement: "/// @}"
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
 */
function checkChanges(sourceEntries, targetEntries, begin, end, prefix = "") {
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
        changes.push(...checkEntryChanges(targetName, sourceEntry, targetEntry, sourceEntryBegin, sourceEntryEnd, prefix));
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
        changes.push({ begin: end, end, replacement: generateEntry(targetEntry.slice(i), prefix) });
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
      changes.push(...checkChanges(sourceEntry.entries, targetEntry.entries, sourceBegin, sourceEnd, prefix + "  "));
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
 * @param {ApiEntry|ApiEntry[]} entry 
 * @param {string=} prefix 
 * @returns {string}
 */
function generateEntry(entry, prefix) {
  if (Array.isArray(entry)) {
    return entry.map(e => generateEntry(e, prefix)).join('\n\n');
  }
  prefix = prefix ?? '';
  const doc = generateDocString(entry.doc, prefix);
  const template = generateTemplateSignature(entry.template, prefix);
  const version = entry.since;
  if (!version) return doGenerate(entry);
  const versionStr = `${version.tag}_VERSION_ATLEAST(${version.major}, ${version.minor}, ${version.patch})`;
  return `#if ${versionStr}\n\n${doGenerate(entry)}\n\n#endif // ${versionStr}`;

  /** @param {ApiEntry} entry  */
  function doGenerate(entry) {
    switch (entry.kind) {
      case "alias":
        if (!entry.type) return `${doc}${prefix}using ${entry.name};`;
        return `${doc}${template}${prefix}using ${entry.name} = ${entry.type};`;
      case "def":
        return doc + generateDef(entry);
      case "forward":
        return '// Forward decl\n' + template + generateStructSignature(entry, prefix) + ';';
      case "function":
        return doc + template + generateFunction(entry, prefix);
      case "ns":
        return doc + generateNS(entry);
      case "struct":
        return doc + template + generateStruct(entry, prefix);
      case "var":
        const varStr = generateVar(entry, prefix);
        if (entry.doc && !entry.doc.includes("\n") && entry.doc.length <= 50) {
          return template + varStr + " ///< " + entry.doc;
        }
        return doc + template + varStr;
      default:
        system.warn(`Unknown kind: ${entry.kind} for ${entry.name}`);
        return `${doc}#${prefix}error "${entry.name} (${entry.kind})"`;
    }

  }
}

/**
 * @param {ApiEntry} entry 
 * @param {string}   prefix
 */
function generateVar(entry, prefix) {
  return generateDeclPrefix(entry, prefix) + (entry.sourceName ? ` = ${entry.sourceName}` : "") + ';';
}

/**
 * 
 * @param {ApiEntry}  entry 
 */
function generateDef(entry) {
  const sourceName = entry.sourceName != entry.name ? entry.sourceName : undefined;
  if (!entry.parameters) return `#define ${entry.name} ${sourceName ?? entry.value ?? ""}`;

  const parameters = `(${entry.parameters.join(", ")})`;
  const body = sourceName ? `${entry.sourceName}${parameters}` : entry.value;
  return `#define ${entry.name}${parameters} ${body ?? ""}`;
}

/**
 * @param {ApiEntry} entry 
 * @param {string}   prefix
 */
function generateBody(entry, prefix) {
  const hint = entry.hints;
  if (hint?.delete) return " = delete;";
  if (hint?.default) return " = default;";
  if (!entry.proto) {
    if (hint?.body) {
      return `\n${prefix}{\n${prefix}  ${hint.body.replaceAll("\n", `\n${prefix}  `)}\n${prefix}}`;
    }
    if (hint?.init?.length) {
      return `\n${prefix}  : ${hint.init.join(`\n${prefix}  , `)}\n${prefix}{}`;
    }
  }
  const sourceName = entry.sourceName === entry.name ? ("::" + entry.sourceName) : entry.sourceName;
  if (!sourceName) {
    if (/operator(==|<=>)/.test(entry.name)) return " = default;";
    if (entry.proto) return ";";
    if (entry.type === "" && !entry.name.startsWith("operator")) return "{}";
    return `\n${prefix}{\n${prefix}  static_assert(false, "Not implemented");\n${prefix}}`;
  }
  if (entry.proto) return ";";
  const selfStr = hint?.self ?? "";
  const hasSelf = selfStr && entry.type && !entry.static && !entry.hints?.static;
  const paramStr = (hasSelf ? (selfStr + (entry.parameters?.length ? ", " : "")) : "") + entry.parameters
    .map(p => typeof p == "string" ? p : p.name)
    .join(", ");
  const internalCallStr = `${sourceName}(${paramStr})`;
  const callStr = hint?.mayFail ? `CheckError(${internalCallStr})` : internalCallStr;
  if (!entry.type) {
    const superStr = hint?.super ?? "T";
    return `\n${prefix}  : ${superStr}(${callStr})\n${prefix}{}`;
  }
  const returnStr = entry.type === "void" ? "" : "return ";
  return `\n${prefix}{\n${prefix}  ${returnStr}${callStr};\n${prefix}}`;
}

/**
 * @param {ApiEntry} entry 
 * @param {string} prefix 
 */
function generateFunction(entry, prefix) {
  const reference = entry.reference ? "&".repeat(entry.reference) : "";
  const specifier = entry.immutable ? ` const${reference}` : (reference ? " " + reference : "");
  const parameters = generateParameters(entry.parameters);
  const body = generateBody(entry, prefix);
  return `${generateDeclPrefix(entry, prefix)}(${parameters})${specifier}${body}`;
}

/**
 * @param {ApiEntry} entry 
 * @param {string} prefix 
 */
function generateDeclPrefix(entry, prefix) {
  const staticStr = entry.static ? "static " : "";
  const explicitStr = entry.explicit ? "explicit " : "";
  const specifier = entry.constexpr ? "constexpr " : (prefix ? "" : "inline ");
  return `${prefix}${staticStr}${specifier}${explicitStr}${entry.type} ${entry.name}`;
}

/**
 * 
 * @param {ApiEntry} entry 
 */
function generateNS(entry) {
  const name = entry.name;
  const subEntries = generateEntries(entry.entries ?? {}, "");
  return `namespace ${name} {\n\n${subEntries}\n\n} // namespace ${name}\n`;
}

/**
 * 
 * @param {ApiEntry} entry 
 */
function combineHints(entry) {
  const hints = entry.hints;
  const subEntries = entry.entries;
  if (!hints || !subEntries) return;
  for (const entry of Object.values(subEntries)) {
    if (Array.isArray(entry)) {
      entry.forEach(e => {
        if (e.hints) e.hints = { ...hints, ...e.hints };
        else e.hints = hints;
      });
    } else {
      if (entry.hints) entry.hints = { ...hints, ...entry.hints };
      else entry.hints = hints;
    }
  }
}

/**
 * 
 * @param {ApiEntry} entry 
 * @param {string} prefix 
 */
function generateStruct(entry, prefix) {
  const signature = generateStructSignature(entry, prefix);
  const parent = entry.type ? ` : ${entry.type}` : "";
  const subEntries = entry.entries ?? {};
  combineHints(entry);
  const subEntriesStr = generateEntries(subEntries, prefix + "  ");
  return `${signature}${parent}\n${prefix}{\n${subEntriesStr}\n${prefix}};`;
}

/**
 * 
 * @param {ApiEntry} entry 
 * @param {string} prefix 
 */
function generateStructSignature(entry, prefix) {
  if (entry.hints?.private) {
    return `${prefix}class ${entry.name}`;
  }
  return `${prefix}struct ${entry.name}`;
}

/**
 * 
 * @param {ApiParameters} template 
 * @param {string}        prefix 
 */
function generateTemplateSignature(template, prefix) {
  return !template ? "" : `${prefix}template<${generateParameters(template)}>\n`;
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
  docStr = docStr.split('\n').map(l => l ? `${prefix} * ${l}` : `${prefix} *`).join('\n');
  return `${prefix}/**\n${docStr}\n${prefix} */\n`;
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
