const { system } = require("./utils");

/**
 * @typedef {{[name: string]: TransformEntryConfig}} TransformEntryConfigs
 */

/**
 * @typedef {object} TransformConfig
 * @prop {Api}                   source
 * @prop {TransformEntryConfigs} files
 * @prop {string[]=}             prefixes
 * @prop {ReplacementRule[]=}    renameRules
 * @prop {ReplacementRule[]=}    docRules
 * @prop {StringMap=}             typeMap
 * @prop {StringMap=}             paramTypeMap
 * @prop {StringMap=}             returnTypeMap
 */

/**
 * @typedef {object} ReplacementRule
 * @prop {RegExp} pattern
 * @prop {string} replacement
 */

/**
 * Transform the Api
 * 
 * @param {TransformConfig} config the rules to apply into source
 * @returns {Api} the transformed api
 */
function transformApi(config) {
  const source = config.source;
  const typeMap = {};
  const paramTypeMap = Object.create(typeMap);
  const returnTypeMap = Object.create(typeMap);
  // paramTypeMap["const char *"] = "StringParam";
  if (config.typeMap) Object.assign(typeMap, config.typeMap);
  if (config.paramTypeMap) Object.assign(paramTypeMap, config.paramTypeMap);
  if (config.returnTypeMap) Object.assign(returnTypeMap, config.returnTypeMap);
  /** @type {ApiContext} */
  const context = { typeMap, paramTypeMap, returnTypeMap };
  if (config.prefixes?.length) {
    context.prefixToRemove = `^(${config.prefixes.join("|")})`;
  }
  context.renameRules = config.renameRules ?? [];
  context.renameRules.forEach(rule => rule.pattern = RegExp(rule.pattern));
  context.docRules = config.docRules ?? [];
  context.docRules.forEach(rule => rule.pattern = RegExp(rule.pattern, 'g'));

  /** @type {{[file: string]: ApiFile}} */
  const files = {};
  const keys = Object.keys(source.files);
  for (const sourceName of keys) {
    const sourceFile = source.files[sourceName];
    const targetName = transformIncludeName(sourceName, context);
    system.log(`Transforming api ${sourceName} => ${targetName}`);

    files[targetName] = {
      name: targetName,
      doc: transformFileDoc(sourceFile.doc, context, targetName),
      entries: transformEntries(sourceFile.entries, context, config.files[targetName] ?? {})
    };
  }
  return { files };
}

/**
 * @typedef {object} ApiContext
 * @prop {{[source: string]: string}} typeMap 
 * @prop {{[source: string]: string}} paramTypeMap
 * @prop {{[source: string]: string}} returnTypeMap 
 * @prop {RegExp=}                    prefixToRemove
 * @prop {ReplacementRule[]}          renameRules
 * @prop {ReplacementRule[]}          docRules
 */

/**
 * @typedef {object} TransformEntryConfig
 * @prop {string[]=}   includeDefs
 * @prop {string[]=}   ignoreEntries
 * @prop {ApiEntries=} transform
 * @prop {ApiEntries=} includeAt
 * @prop {ApiEntries=} types this should be only structs
 */

/**
 * 
 * @param {ApiEntries}           sourceEntries 
 * @param {ApiContext}           context 
 * @param {TransformEntryConfig} config
 */
function transformEntries(sourceEntries, context, config) {
  /** @type {ApiEntries} */
  const targetEntries = {};
  const defWhitelist = new Set(config.includeDefs ?? []);
  const blacklist = new Set(config.ignoreEntries ?? []);
  const transformMap = config.transform ?? {};

  for (const entry of config.includeAt?.begin ?? []) {
    const targetName = entry.kind == "forward" ? `${entry.name}-forward` : entry.name;
    targetEntries[targetName] = entry;
  }

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (blacklist.has(sourceName)) continue;
    let targetName = transformName(sourceName, context);
    if (Array.isArray(sourceEntry)) {
      targetEntries[targetName] = sourceEntry.map(e => {
        const targetEntry = transformEntry(e, context, config);
        targetEntry.name = targetName;
        return targetEntry;
      });
    } else if (sourceEntry.kind != "def" || defWhitelist.has(sourceName)) {
      const targetEntry = transformEntry(sourceEntry, context, config);
      targetName = transformName(targetEntry.name, context);
      const targetDelta = transformMap[targetName];
      if (targetDelta) {
        if (targetDelta.name) targetName = targetDelta.name;
        else targetDelta.name = targetName;
        Object.assign(targetEntry, targetDelta);
      } else
        targetEntry.name = targetName;
      if (targetName === targetEntry.sourceName) targetEntry.sourceName = "::" + targetEntry.sourceName;
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
 * @param {ApiEntry}             sourceEntry 
 * @param {ApiContext}           context 
 * @param {TransformEntryConfig} config
 */
function transformEntry(sourceEntry, context, config) {
  /** @type {ApiEntry} */
  const targetEntry = { ...sourceEntry };
  if (sourceEntry.doc) {
    targetEntry.doc = transformDoc(targetEntry.doc, context);
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
        const targetName = type?.name ?? transformName(sourceName, context);
        targetEntry.name = targetName + "Base";
        context.paramTypeMap[sourceName] = targetName + "Ref";
        context.paramTypeMap[`${sourceName} *`] = targetName + "Ref";
        context.returnTypeMap[sourceName] = targetName;
        context.returnTypeMap[`${sourceName} *`] = targetName;
      } else {
        if (type) {
          system.warn(`Alias ${sourceEntry.name} can not be ${type}`);
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
 * @param {string}      docStr 
 * @param {ApiContext}  context 
 * @param {string}      filename 
 */
function transformFileDoc(docStr, context, filename) {
  if (!docStr) return "";
  docStr = docStr.replace(/^(([@\\]file [^\n]*\s*)?)/, `@file ${filename}\n\n`);
  return transformDoc(docStr, context);
}

/** 
 * @param {string}      docStr
 * @param {ApiContext}  context   
 **/
function transformDoc(docStr, context) {
  return transformString(docStr.replace(/\\(\w+)/g, '@$1'), context.docRules);
}

/**
 * 
 * @param {string} name 
 * @param {ApiContext} context 
 */
function transformIncludeName(name, context) {
  return transformString(transformName(name, context), context.renameRules);
}

/**
 * 
 * @param {string} str the string to apply to
 * @param {ReplacementRule[]} rules the replacements to apply
 */
function transformString(str, rules) {
  rules.forEach(rule => str = str.replace(rule.pattern, rule.replacement));
  return str;
}

/**
 * 
 * @param {string} name 
 * @param {ApiContext} context 
 */
function transformName(name, context) {
  return context?.prefixToRemove ? name.replace(context.prefixToRemove, '') : name;
}

exports.transformApi = transformApi;
exports.transformEntries = transformEntries;
exports.transformEntry = transformEntry;
