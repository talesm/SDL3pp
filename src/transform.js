const { insertEntry } = require("./parse");
const { system } = require("./utils");

/**
 * @typedef {object} TransformConfig
 * @prop {Api}          sourceApi
 * @prop {ApiTransform} transform
 */

/**
 * Transform the Api
 * 
 * @param {TransformConfig} config the rules to apply into source
 * @returns {Api} the transformed api
 */
function transformApi(config) {
  const source = config.sourceApi;
  const transform = config.transform;
  /** @type {ApiContext} */
  const context = new ApiContext(transform);

  /** @type {ApiFileMap} */
  const files = {};
  const fileTransformMap = transform?.files ?? {};
  for (const [sourceName, sourceFile] of Object.entries(source.files)) {
    const fileConfig = fileTransformMap[sourceName] ?? {};
    const targetName = fileConfig.name || transformIncludeName(sourceName, context);
    system.log(`Transforming api ${sourceName} => ${targetName}`);

    files[targetName] = {
      name: targetName,
      doc: fileConfig.doc || transformFileDoc(sourceFile.doc, context, targetName),
      entries: transformEntries(sourceFile.entries, context, fileConfig)
    };
  }
  return { files };
}

class ApiContext {
  /** @param {ApiTransform} transform  */
  constructor(transform) {
    /** @type {StringMap} */
    this.typeMap = {};
    Object.assign(this.typeMap, transform.typeMap ?? {});

    /** @type {StringMap} */
    this.paramTypeMap = Object.create(this.typeMap);
    Object.assign(this.paramTypeMap, transform.paramTypeMap ?? {});
    // this.paramTypeMap["const char *"] = "StringParam";

    /** @type {StringMap} */
    this.returnTypeMap = Object.create(this.typeMap);
    Object.assign(this.returnTypeMap, transform.returnTypeMap ?? {});

    if (transform.prefixes?.length) {
      this.prefixToRemove = Array.isArray(transform.prefixes)
        ? RegExp(`^(${transform.prefixes.join("|")})`)
        : RegExp("^" + transform.prefixes);
    }

    this.renameRules = transform.renameRules ?? [];
    this.renameRules.forEach(rule => rule.pattern = RegExp(rule.pattern));

    this.docRules = transform.docRules ?? [];
    this.docRules.forEach(rule => rule.pattern = RegExp(rule.pattern, 'g'));
  }
}

/**
 * 
 * @param {ApiEntries}    sourceEntries 
 * @param {ApiContext}    context 
 * @param {FileTransform} transform
 */
function transformEntries(sourceEntries, context, transform) {
  /** @type {ApiEntries} */
  const targetEntries = {};
  const defWhitelist = new Set(transform.includeDefs ?? []);
  const blacklist = new Set(transform.ignoreEntries ?? []);
  const transformMap = transform.transform ?? {};
  if (!transform.transform) transform.transform = transformMap;

  insertEntry(targetEntries, transform.includeAfter?.__begin ?? []);

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (blacklist.has(sourceName)) continue;
    let targetName = transformName(sourceName, context);
    if (Array.isArray(sourceEntry)) {
      const targetDelta = transformMap[sourceName];
      insertEntry(targetEntries, sourceEntry.map(e => {
        const targetEntry = transformEntry(e, context);
        if (targetDelta) {
          if (!targetDelta.name) targetDelta.name = targetName;
          Object.assign(targetEntry, targetDelta);
        } else targetEntry.name = targetName;
        return targetEntry;
      }));
    } else if (sourceEntry.kind != "def" || defWhitelist.has(sourceName)) {
      const targetEntry = transformEntry(sourceEntry, context);
      const targetDelta = transformMap[sourceName];
      if (targetDelta) {
        if (targetDelta.name) targetName = targetDelta.name;
        else targetDelta.name = targetName;
        Object.assign(targetEntry, targetDelta);
      } else targetEntry.name = targetName;
      if (targetName === targetEntry.sourceName) targetEntry.sourceName = "::" + targetEntry.sourceName;
      if (targetEntry.kind == 'alias' || targetEntry.kind == 'struct') {
        if (targetName == targetEntry.type) {
          continue;
        }
        const type = targetEntry.type;
        context.typeMap[type] = targetName;
        context.typeMap[`${type} *`] = `${targetName} *`;
        context.typeMap[`const ${type}`] = `const ${targetName}`;
        context.typeMap[`const ${type} *`] = `const ${targetName} *`;
      }
      if (targetEntry.entries) {
        targetEntry.entries = transformSubEntries(targetEntry, context, transform, targetEntries);
      }
      insertEntry(targetEntries, targetEntry);
    }
    insertEntry(targetEntries, transform.includeAfter?.[sourceName] ?? []);
  }
  insertEntry(targetEntries, transform.includeAfter?.__end ?? []);
  transformHierarchy(targetEntries);

  validateEntries(targetEntries);

  return targetEntries;
}

/**
 * 
 * @param {ApiEntry}      targetEntry 
 * @param {ApiContext}    context 
 * @param {FileTransform} transform 
 * @param {ApiEntries}    targetEntries
 */
function transformSubEntries(targetEntry, context, transform, targetEntries) {
  /** @type {ApiEntries} */
  const entries = {};
  const type = targetEntry.name;
  for (const [key, entry] of Object.entries(targetEntry.entries)) {
    const nameCandidate = transformName(key, context);
    const nameChange = checkNameChange(entry, nameCandidate, type);
    if (nameChange) {
      const currName = transform.transform[key]?.name ?? nameCandidate;
      const currEntry = targetEntries[currName];
      if (!currEntry) {
        insertEntry(entries, { name: nameChange.name, kind: "placeholder" });
      } else {
        if (Array.isArray(currEntry)) {
          currEntry.forEach(e => Object.assign(e, nameChange));
        } else {
          Object.assign(currEntry, nameChange);
        }
        insertEntry(entries, currEntry);
        delete targetEntries[currName];
      }
      nameChange.name = `${type}.${nameChange.name}`;
      transform.transform[key] = nameChange;
    } else {
      insertEntry(entries, entry, key);
    }
  }
  return entries;
}

/**
 * Fix hierarchy, executing any pending movements
 * @param {ApiEntries} targetEntries the entries to fix hierarchy
 */
function transformHierarchy(targetEntries) {
  for (const key of Object.keys(targetEntries)) {
    if (!key.includes(".")) continue;
    const path = key.split(".");
    let obj = targetEntries[path[0]];
    if (!obj || Array.isArray(obj) || !obj.entries) continue;
    let name = path[path.length - 1];
    let i = 1;
    for (; i < path[path.length - 1]; i++) {
      /** @type {ApiEntry} */
      const el = obj.entries[path[i]];
      if (!el || Array.isArray(el) || !el.entries) {
        name = path.slice(i).join('.');
        break;
      }
      obj = el;
    }
    const entry = targetEntries[key];
    const typeName = obj.name;
    if (Array.isArray(entry)) {
      entry.forEach(e => prepareForTypeInsert(e, name, typeName));
    } else prepareForTypeInsert(entry, name, typeName);
    delete targetEntries[key];
    insertEntry(obj.entries, entry);
  }
}

/**
 * Validate and report entries
 * @param {ApiEntries} targetEntries
 */
function validateEntries(targetEntries) {
  // Do nothing for now
}

/**
 * Check and if true, marshal name of member functions
 * @param {ApiEntry|ApiEntry[]} entry 
 * @param {string} name 
 * @param {string} typeName 
 * @returns {ApiEntry}
 */
function checkNameChange(entry, name, typeName) {
  if (Array.isArray(entry)) {
    return null;
  }
  if (entry === "function") return { name: makeNaturalName(name, typeName) };
  if (entry === "ctor") return { name: typeName, type: "", static: false };
  if (entry.kind && entry.kind !== "function") return null;
  if (entry.parameters && (!entry.name || entry.name == name)) return null;
  return { ...entry, name: entry.name || makeNaturalName(name, typeName) };
}

/**
 * 
 * @param {string} name 
 * @param {string} typeName 
 */
function makeNaturalName(name, typeName) {
  typeName = normalizeTypeName(typeName);
  const m = /^([GS]et|Has)/.exec(name);
  let prefix = "";
  if (m) {
    prefix = m[1];
    name = name.slice(3);
  }
  if (!name.startsWith(typeName)) return prefix + name;
  return prefix + name.slice(typeName.length);
}

/**
 * Prepare function to become class or instance function
 * @param {ApiEntry} entry 
 * @param {string} name 
 * @param {string} typeName 
 */
function prepareForTypeInsert(entry, name, typeName) {
  typeName = normalizeTypeName(typeName);
  entry.name = name;
  const parameters = entry.parameters;
  if (!parameters?.length) return;
  const type = parameters[0]?.type ?? "";
  if (type.includes(typeName)) {
    parameters.shift();
    if (type.startsWith("const ")) entry.immutable = true;
  } else if (entry.static !== false) {
    entry.static = true;
  }
}

/** @param {string} typeName  */
function normalizeTypeName(typeName) {
  if (typeName.endsWith("Base")) return typeName.slice(0, typeName.length - 4);
  return typeName;
}

/**
 * 
 * @param {ApiEntry}    sourceEntry 
 * @param {ApiContext}  context 
 */
function transformEntry(sourceEntry, context) {
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
      targetEntry.type = sourceEntry.name;
      break;
    case 'callback':
    case 'enum':
    case 'union':
    case 'struct':
      targetEntry.kind = 'alias';
      targetEntry.type = sourceEntry.name;
      delete targetEntry.parameters;
      delete targetEntry.entries;
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
