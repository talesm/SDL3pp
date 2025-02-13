const { writeFileSync } = require("node:fs");
const source = require('./source.json');
const config = require('./transform.json');

const filename = 'SDL_stdinc.h';

if (require.main == module) {
  writeFileSync('scripts/target.json', JSON.stringify(transformApi([filename]), null, 2));
}

/**
 * 
 * @param {string[]} names the names to transform
 * 
 */
function transformApi(names) {
  const typeMap = {};
  const paramTypeMap = Object.create(typeMap);
  paramTypeMap["const char *"] = "StringParam";
  const returnTypeMap = Object.create(typeMap);
  /** @type {ApiContext} */
  const context = { typeMap, paramTypeMap, returnTypeMap };

  /** @type {{[file: string]: ApiFile}} */
  const files = {};
  const keys = Object.keys(source.files).filter(names?.length ? (name => names.includes(name)) : (() => true));
  for (const sourceName of keys) {
    const sourceFile = source.files[sourceName];
    const targetName = transformIncludeName(sourceName);
    console.log(`Transforming api ${sourceName} => ${targetName}`);

    files[targetName] = {
      name: targetName,
      doc: transformFileDoc(sourceFile.doc, targetName),
      entries: transformEntries(sourceFile.entries, context, config.files[targetName] ?? {})
    };
  }
  return { files };
}

/**
 * @typedef {object} ApiContext
 * @property {{[source: string]: string}} typeMap 
 * @property {{[source: string]: string}} paramTypeMap
 * @property {{[source: string]: string}} returnTypeMap 
 */

/**
 * 
 * @param {{[name: string]: ApiEntry|ApiEntry[]}} sourceEntries 
 * @param {ApiContext} context 
 * @param {TransformConfig} config
 */
function transformEntries(sourceEntries, context, config) {
  /** @type {{[name: string]: ApiEntry|ApiEntry[]}} */
  const targetEntries = {};
  const defWhitelist = new Set(config.includeDefs);
  const blacklist = new Set(config.ignoreEntries);
  const transformMap = config.transform;

  for (const entry of config.includeAt?.begin ?? []) {
    const targetName = entry.kind == "forward" ? `${entry.name}-forward` : entry.name;
    targetEntries[targetName] = entry;
  }

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (blacklist.has(sourceName)) continue;
    let targetName = transformName(sourceName);
    if (Array.isArray(sourceEntry)) {
      targetEntries[targetName] = sourceEntry.map(e => {
        const targetEntry = transformEntry(e, context, config);
        targetEntry.name = targetName;
        return targetEntry;
      });
    } else if (sourceEntry.kind != "def" || defWhitelist.has(sourceName)) {
      const targetEntry = transformEntry(sourceEntry, context, config);
      targetName = transformName(targetEntry.name);
      const targetDelta = transformMap[targetName];
      if (targetDelta) {
        if (targetDelta.name) targetName = targetDelta.name;
        else targetDelta.name = targetName;
        Object.assign(targetEntry, targetDelta);
      } else
        targetEntry.name = targetName;
      if (targetEntry.kind == 'alias') {
        if (targetName == targetEntry.type) {
          continue;
        }
        const type = targetEntry.type;
        context.typeMap[type] = targetName;
        context.typeMap[`${type} *`] = `${targetName} *`;
        context.typeMap[`const ${type}`] = `const ${targetName}`;
        context.typeMap[`const ${type} *`] = `const ${targetName} *`;
      } else if (targetEntry.kind == "forward") {
        targetName += "-forward";
      }
      targetEntries[targetName] = targetEntry;
    }
  }

  return targetEntries;
}

/**
 * 
 * @param {ApiEntry} sourceEntry 
 * @param {ApiContext} context 
 * @param {TransformConfig} config
 */
function transformEntry(sourceEntry, context, config) {
  const targetEntry = { ...sourceEntry, begin: undefined, decl: undefined, end: undefined };
  if (sourceEntry.doc) {
    targetEntry.doc = transformDoc(targetEntry.doc);
  }
  const sourceName = sourceEntry.name;
  targetEntry.sourceName = sourceName;
  switch (sourceEntry.kind) {
    case 'function':
      targetEntry.parameters = transformParameters(sourceEntry.parameters, context);
      targetEntry.type = transformType(sourceEntry.type, context.returnTypeMap);
      break;
    case 'alias':
      const type = config?.types[sourceName];
      if (type === "resource" || type?.kind == "resource") {
        targetEntry.kind = "struct";
        targetEntry.template = [{ type: "class", name: "T" }];
        targetEntry.type = "T";
        targetEntry.parameters = ["using T::T;"];
        const targetName = type?.name ?? transformName(sourceName);
        targetEntry.name = targetName + "Base";
        context.paramTypeMap[sourceName] = targetName + "Ref";
        context.paramTypeMap[`${sourceName} *`] = targetName + "Ref";
        context.returnTypeMap[sourceName] = targetName;
        context.returnTypeMap[`${sourceName} *`] = targetName;
      } else {
        if (type) {
          console.warn(`Alias ${sourceEntry.name} can not be ${type}`);
        }
        targetEntry.type = sourceEntry.name;
      }
      break;
    case 'callback':
    case 'enum':
    case 'union':
    case 'struct':
      targetEntry.kind = 'alias';
      targetEntry.type = sourceEntry.name;
      delete targetEntry.parameters;
      break;
    default:
      break;
  }
  return targetEntry;
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
 * 
 * @param {string} docStr 
 * @param {string} name 
 */
function transformFileDoc(docStr, name) {
  return transformDoc(`@file ${name}\n\n${docStr.replace(/^# Category\w*\n\n/, '')}`);
}

/** @param {string} docStr  */
function transformDoc(docStr) {
  return docStr.replaceAll(/\\(\w+)/g, '@$1');
}

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
