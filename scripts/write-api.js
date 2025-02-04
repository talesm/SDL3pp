const { readFileSync, writeFileSync } = require("node:fs");
const source = require('./source.json');
const config = require('./transform.json');

const baseDir = 'include/SDL3pp/'
const filename = 'SDL_stdinc.h'

writeFileSync('scripts/target.json', JSON.stringify(transformApi([filename]), null, 2))

/**
 * 
 * @param {string[]} names the names to transform
 * 
 */
function transformApi(names) {
  /** @type {{[file: string]: ApiFile}} */
  const files = {}
  const keys = Object.keys(source.files).filter(names?.length ? (name => names.includes(name)) : (() => true));
  for (const sourceName of keys) {
    const sourceFile = source.files[sourceName];
    const targetName = transformIncludeName(sourceName);
    files[targetName] = {
      name: targetName,
      doc: sourceFile.doc,
      entries: transformEntries(sourceFile.entries)
    };
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
 * @typedef {object} ApiContext
 * @property {{[source: string]: string}} typeMap 
 * @property {{[source: string]: string}} paramTypeMap
 * @property {{[source: string]: string}} returnTypeMap 
 */

/**
 * 
 * @param {{[name: string]: ApiEntry|ApiEntry[]}} sourceEntries 
 */
function transformEntries(sourceEntries) {
  /** @type {{[name: string]: ApiEntry|ApiEntry[]}} */
  const targetEntries = {};
  const defWhitelist = new Set(config.includeDefs);
  const blacklist = new Set(config.ignoreEntries);
  const transformMap = config.transform

  const typeMap = {};
  const paramTypeMap = Object.create(typeMap);
  paramTypeMap["const char *"] = "StringParam";
  const returnTypeMap = Object.create(typeMap);

  /** @type {ApiContext} */
  const context = { typeMap, paramTypeMap, returnTypeMap };


  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (blacklist.has(sourceName)) continue;
    let targetName = transformName(sourceName);
    if (Array.isArray(sourceEntry)) {
      targetEntries[targetName] = sourceEntry.map(e => {
        const targetEntry = transformEntry(e, context);
        targetEntry.name = targetName;
        return targetEntry;
      });
    } else if (sourceEntry.kind != "def" || defWhitelist.has(sourceName)) {
      const targetEntry = transformEntry(sourceEntry, context);
      const targetDelta = transformMap[targetName]
      if (targetDelta) {
        if (targetDelta.name) targetName = targetDelta.name;
        else targetDelta.name = targetName;
        Object.assign(targetEntry, targetDelta);
      } else
        targetEntry.name = targetName;
      if (targetEntry.kind == 'alias' && targetName != targetEntry.type) {
        const type = targetEntry.type
        typeMap[type] = targetName;
        typeMap[`${type} *`] = `${targetName} *`;
        typeMap[`const ${type}`] = `const ${targetName}`;
        typeMap[`const ${type} *`] = `const ${targetName} *`;
      }
      targetEntries[targetName] = targetEntry;
    }
  }

  return targetEntries
}

/**
 * 
 * @param {ApiEntry} sourceEntry 
 * @param {ApiContext} context 
 */
function transformEntry(sourceEntry, context) {
  const targetEntry = { ...sourceEntry };
  switch (sourceEntry.kind) {
    case 'function':
      targetEntry.parameters = transformParameters(sourceEntry.parameters, context);
      targetEntry.type = transformType(sourceEntry.type, context.returnTypeMap);
      break
    case 'alias':
    case 'callback':
    case 'enum':
    case 'union':
    case 'struct':
      targetEntry.kind = 'alias';
      targetEntry.type = sourceEntry.name;
      delete targetEntry.parameters;
      break
    default:
      break;
  }
  return targetEntry
}

/**
 * 
 * @param {(string|{name: string, type: string})[]} parameters
 * @param {ApiContext} context 
 */
function transformParameters(parameters, context) {
  return parameters.map(parameter => {
    if (typeof parameter == "string") return parameter;
    let { name, type } = parameter;
    type = transformType(type, context.paramTypeMap);
    return { name, type };
  });
}

/**
 * 
 * @param {string} type 
 * @param {{[sourceType: string]: string}} typeMap 
 */
function transformType(type, typeMap) {
  return typeMap[type] ?? type;
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
 * @param {string} name 
 */
function transformIncludeName(name) {
  return transformName(name).replace(/\.h$/, ".hpp");
}

/**
 * 
 * @param {string} name 
 */
function transformName(name) {
  return name.replace(/^(SDL|IMG)_/, '');
}
