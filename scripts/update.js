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
  const changes = checkChanges(sourceFile, targetFile, entriesBegin, entriesEnd);
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
 * @param {ApiFile} sourceFile 
 * @param {ApiFile} targetFile 
 * @param {number} begin 
 * @param {number} end 
 * @param {string=} prefix 
 */
function checkChanges(sourceFile, targetFile, begin, end, prefix) {
  /** @type {Change[]} */
  const changes = [];
  const targetEntries = targetFile.entries;
  const targetNames = Object.keys(targetEntries);
  if (!targetNames.length) {
    if (begin != end) changes.push({
      begin,
      end,
    });
    return changes;
  }
  const sourceEntries = sourceFile.entries;
  const sourceNames = Object.keys(sourceEntries);
  if (!sourceNames?.length) {
    changes.push({
      begin,
      end,
      replacement: "\n" + generateEntries(targetEntries, prefix)
    });
    return changes;
  }
  let sourceIndex = 0;
  for (const targetName of targetNames) {
    const targetEntry = targetEntries[targetName];
    let index = sourceNames.indexOf(targetName, sourceIndex);
    if (index == -1) {
      system.log(`${targetEntry.name} added ${begin}`);
      changes.push({
        begin,
        end: begin,
        replacement: '\n' + generateEntry(targetEntry, prefix),
      });
    } else if (sourceIndex < sourceNames.length) {
      begin = sourceEntries[sourceNames[sourceIndex]].begin ?? sourceEntries[sourceNames[sourceIndex]][0].begin;
      const sourceEntry = sourceEntries[targetName];
      const change = checkEntryChanged(sourceEntry, targetEntry);
      const sourceEntryEnd = Array.isArray(sourceEntry) ? sourceEntry[sourceEntry.length - 1].end : sourceEntry.end;
      if (change) {
        system.log(`${targetName} changed ${change} from ${begin} to ${sourceEntryEnd}`);
        changes.push({
          begin,
          end: sourceEntryEnd,
          replacement: generateEntry(targetEntry, prefix)
        });
        begin = sourceEntryEnd;
        // } else if (sourceEntry.entries || targetEntry.entries) {
        // TODO compound structs
      } else {
        if (index > sourceIndex) {
          changes.push({
            begin,
            end: Array.isArray(sourceEntry) ? sourceEntry[0].begin : sourceEntry.begin
          });
        }
        begin = sourceEntryEnd;
      }
      sourceIndex = index + 1;
    } else {
      system.log(`${targetEntry.name} added ${begin}`);
      changes.push({
        begin: end,
        end,
        replacement: generateEntry(targetEntry, prefix) + '\n',
      });

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
  if (sourceEntry.kind != targetEntry.kind) return 'kind';
  if (targetEntry.type && sourceEntry.type != targetEntry.type) return "type";
  if (targetEntry.parameters) {
    const sourceParameters = sourceEntry.parameters;
    const targetParameters = targetEntry.parameters;
    if (sourceEntry.parameters?.length != targetEntry.parameters.length) return "parameters";
    for (let i = 0; i < targetParameters.length; i++) {
      const targetParameter = targetParameters[i];
      const sourceParameter = sourceParameters[i];
      if (typeof targetParameter == "string") {
        if (targetParameter !== sourceParameter) return "parameters";
      } else if (targetParameter.name != sourceParameter?.name
        || targetParameter.type != sourceParameter?.type) {
        return "parameters";
      }
    }
  }
  return null;
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
 * @param {(string|ApiParameter)[]} parameter
 */
function generateParameter(parameter) {
  if (typeof parameter == "string") return parameter;
  return `${parameter.type} ${parameter.name}`;
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
  return `${prefix}/**\n${docStr}\n${prefix} **/`;
}

exports.updateApi = updateApi;
exports.updateContent = updateContent;
exports.updateChanges = updateChanges;
