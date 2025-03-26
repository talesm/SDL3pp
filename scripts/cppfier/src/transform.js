const { insertEntry } = require("./parse");
const { system, combineObject, looksLikeFreeFunction } = require("./utils");

/**
 * @import { Api, ApiEntries, ApiEntry, ApiEntryKind, ApiEntryTransform, ApiFile, ApiParameters, ApiResource, ApiSubEntryTransformMap, ApiTransform, Dict, FileTransform, ReplacementRule, StringMap } from "./types"
 */

/**
 * @typedef {object} TransformConfig
 * @prop {Api}            sourceApi
 * @prop {ApiTransform=}  transform
 */

/**
 * Transform the Api
 * 
 * @param {TransformConfig} config the rules to apply into source
 * @returns {Api} the transformed api
 */
function transformApi(config) {
  const source = config.sourceApi;
  const transform = config.transform ?? { files: {} };
  /** @type {ApiContext} */
  const context = new ApiContext(transform);

  /** @type {Dict<ApiFile>} */
  const files = {};
  const fileTransformMap = transform.files ?? {};
  for (const [sourceName, sourceFile] of Object.entries(source.files)) {
    const fileConfig = fileTransformMap[sourceName] ?? {};
    const targetName = fileConfig.name || transformIncludeName(sourceName, context);
    system.log(`Transforming api ${sourceName} => ${targetName}`);

    files[targetName] = {
      name: targetName,
      doc: fileConfig.doc || transformFileDoc(sourceFile.doc, context, targetName) || "",
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
  if (!transform.includeAfter) transform.includeAfter = {};

  if (transform.resources) expandResources(transform.resources, transform, context);

  insertEntryAndCheck(targetEntries, transform.includeAfter?.__begin ?? [], context, transform);

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (blacklist.has(sourceName)) continue;
    let targetName = transformName(sourceName, context);
    if (Array.isArray(sourceEntry)) {
      const targetDelta = transformMap[sourceName];
      insertEntry(targetEntries, sourceEntry.map(e => {
        const targetEntry = transformEntry(e, context);
        if (targetDelta) {
          if (!targetDelta.name) targetDelta.name = targetName;
          combineObject(targetEntry, targetDelta);
        } else targetEntry.name = targetName;
        return targetEntry;
      }));
    } else {
      const targetEntry = transformEntry(sourceEntry, context);
      const targetDelta = transformMap[sourceName];
      if (targetDelta) {
        if (targetDelta.name) targetName = targetDelta.name;
        else targetDelta.name = targetName;
        combineObject(targetEntry, targetDelta);
      } else targetEntry.name = targetName;
      if (targetName === targetEntry.sourceName) targetEntry.sourceName = "::" + targetEntry.sourceName;
      if (targetEntry.kind == 'alias' || targetEntry.kind == 'struct') {
        if (targetName == targetEntry.type) {
          continue;
        }
        const sourceType = targetEntry.sourceName;
        const targetType = targetEntry.name.replace(/[^.]*\./g, "");
        if (!context.typeMap[sourceType]) {
          context.typeMap[sourceType] = targetType;
          context.typeMap[`${sourceType} *`] = `${targetType} *`;
          context.typeMap[`const ${sourceType}`] = `const ${targetType}`;
          context.typeMap[`const ${sourceType} *`] = `const ${targetType} *`;
        }
      }
      insertEntryAndCheck(targetEntries, targetEntry, context, transform, targetName);
      if (sourceEntry.kind === "enum") {
        insertEntry(targetEntries, Object.values(sourceEntry.entries).map(e => {
          if (Array.isArray(e)) throw new Error("Unimplemented");
          return {
            ...e,
            sourceName: e.name,
            name: transformName(e.name, context),
            constexpr: true,
            type: targetName,
          };
        }));
      }
    }
    insertEntryAndCheck(targetEntries, transform.includeAfter?.[sourceName] ?? [], context, transform);
  }
  insertEntryAndCheck(targetEntries, transform.includeAfter?.__end ?? [], context, transform);
  transformHierarchy(targetEntries);
  for (const obj of Object.values(targetEntries)) {
    if (!Array.isArray(obj) && obj.kind == "def" && !!obj.sourceName && !defWhitelist.has(obj.sourceName)) {
      delete targetEntries[obj.name];
    }
  }

  validateEntries(targetEntries);

  return targetEntries;
}

/**
 * 
 * @param {Dict<ApiResource>} resources 
 * @param {FileTransform} transform 
 * @param {ApiContext} context 
 */
function expandResources(resources, transform, context) {
  /** @type {ApiEntry[]} */
  const referenceAliases = [];
  for (const [sourceName, resourceEntry] of Object.entries(resources)) {
    const uniqueName = resourceEntry.uniqueName || transformName(sourceName, context);
    const name = resourceEntry.name || (uniqueName + "Base");
    const refName = resourceEntry.refName || (uniqueName + "Ref");
    const type = resourceEntry.type ?? sourceName;
    const uniqueType = resourceEntry.uniqueType ?? `${name}<ObjectUnique<${type}>>`;
    const title = uniqueName[0].toLowerCase() + uniqueName.slice(1);
    const refType = resourceEntry.uniqueType ?? `${name}<ObjectRef<${type}>>`;
    const pointerType = type + " *";
    const template = resourceEntry.template ?? [{ type: `ObjectBox<${pointerType}>`, name: "T" }];
    const replaceType = sourceName;
    const replaceTypeConst = "const " + replaceType;
    const replacePointerType = pointerType;
    const replacePointerTypeConst = "const " + replacePointerType;
    const freeFunction = resourceEntry.free ?? scanFreeFunction(resourceEntry.entries ?? {});

    if (freeFunction) {
      /** @type {ApiEntry} */
      const freeEntry = {
        name: `ObjectRef<${type}>::doFree`,
        type: "void",
        kind: "function",
        doc: `Callback for ${title} resource cleanup\n\n@private`,
        template: [],
        parameters: [
          {
            "type": pointerType,
            "name": "resource"
          }
        ],
        sourceName: freeFunction
      };

      if (Array.isArray(transform.includeAfter[sourceName])) {
        transform.includeAfter[sourceName].unshift(freeEntry);
      } else if (transform.includeAfter[sourceName]) {
        transform.includeAfter[sourceName] = [freeEntry, transform.includeAfter[sourceName]];
      } else transform.includeAfter[sourceName] = freeEntry;
    }
    if (resourceEntry.prependAliases !== false) {
      referenceAliases.push({ name, kind: "forward", template, },
        {
          name: refName, kind: "alias", type: refType,
          doc: `Handle to a non owned ${title}\n\n@cat resource\n\n@sa ${name}\n@sa ${uniqueName}`
        },
        {
          name: uniqueName, kind: "alias", type: uniqueType,
          doc: `Handle to an owned ${title}\n\n@cat resource\n\n@sa ${name}\n@sa ${refName}`
        },
      );
    }
    switch (resourceEntry.paramType) {
      case "none": break;
      case "unique":
        context.paramTypeMap[replaceType] = uniqueName;
        if (!context.paramTypeMap[replaceTypeConst]) context.paramTypeMap[replaceTypeConst] = uniqueName;
        if (!context.paramTypeMap[replacePointerType]) context.paramTypeMap[replacePointerType] = uniqueName;
        if (!context.paramTypeMap[replacePointerTypeConst]) context.paramTypeMap[replacePointerTypeConst] = uniqueName;
        break;
      default:
        context.paramTypeMap[replaceType] = refName;
        if (!context.paramTypeMap[replaceTypeConst]) context.paramTypeMap[replaceTypeConst] = refName;
        if (!context.paramTypeMap[replacePointerType]) context.paramTypeMap[replacePointerType] = refName;
        if (!context.paramTypeMap[replacePointerTypeConst]) context.paramTypeMap[replacePointerTypeConst] = refName;
        break;
    }
    switch (resourceEntry.returnType) {
      case "none": break;
      case "unique":
        context.returnTypeMap[replaceType] = uniqueName;
        if (!context.returnTypeMap[replaceTypeConst]) context.returnTypeMap[replaceTypeConst] = uniqueName;
        if (!context.returnTypeMap[replacePointerType]) context.returnTypeMap[replacePointerType] = uniqueName;
        if (!context.returnTypeMap[replacePointerTypeConst]) context.returnTypeMap[replacePointerTypeConst] = uniqueName;
        break;
      default:
        context.returnTypeMap[replaceType] = refName;
        if (!context.returnTypeMap[replaceTypeConst]) context.returnTypeMap[replaceTypeConst] = refName;
        if (!context.returnTypeMap[replacePointerType]) context.returnTypeMap[replacePointerType] = refName;
        if (!context.returnTypeMap[replacePointerTypeConst]) context.returnTypeMap[replacePointerTypeConst] = refName;
        break;
    }
    const subEntries = resourceEntry.entries || {};
    /** @type {ApiEntryTransform} */
    const entry = {
      name,
      kind: "struct",
      type: "T",
      template,
      entries: {
        "T::T": "alias",
        ...subEntries
      }
    };
    // TODO if (resourceEntry.insertAfter) {...}
    transform.transform[sourceName] = entry;
  }
  if (!transform.includeAfter.__begin) {
    transform.includeAfter.__begin = referenceAliases;
  } else if (Array.isArray(transform.includeAfter.__begin)) {
    transform.includeAfter.__begin.push(...referenceAliases);
  } else {
    transform.includeAfter.__begin = [transform.includeAfter.__begin, ...referenceAliases];
  }
}

/**
 * 
 * @param {ApiSubEntryTransformMap} entries 
 */
function scanFreeFunction(entries) {
  for (const sourceName of Object.keys(entries)) {
    if (looksLikeFreeFunction(sourceName)) return sourceName;
  }
}

/**
 * Insert entry into entries
 * 
 * @param {ApiEntries}                    entries 
 * @param {ApiEntryTransform|ApiEntryTransform[]}  entry 
 * @param {ApiContext}                    context 
 * @param {FileTransform}                 transform
 * @param {string=}                       defaultName
 */
function insertEntryAndCheck(entries, entry, context, transform, defaultName) {
  if (Array.isArray(entry)) {
    entry.forEach(e => insertEntryAndCheck(entries, e, context, transform, defaultName));
    return;
  }
  if (entry.entries) entry.entries = transformSubEntries(entry, context, transform, entries);
  insertEntry(entries, /** @type {ApiEntry}*/(entry), defaultName);
}

/**
 * 
 * @param {ApiEntryTransform} targetEntry 
 * @param {ApiContext}        context 
 * @param {FileTransform}     transform 
 * @param {ApiEntries}        targetEntries
 */
function transformSubEntries(targetEntry, context, transform, targetEntries) {
  /** @type {ApiEntries} */
  const entries = {};
  const type = targetEntry.name;
  for (const [key, entry] of Object.entries(targetEntry.entries)) {
    const nameCandidate = transformName(key, context);
    if (Array.isArray(entry) || nameCandidate === key) {
      insertEntry(entries, /** @type {ApiEntry}*/(entry), key);
      continue;
    }
    const nameChange = makeRenameEntry(entry, nameCandidate, type);
    const currName = transform.transform[key]?.name ?? nameCandidate;
    const currEntry = targetEntries[currName];
    if (currEntry) {
      if (Array.isArray(currEntry)) {
        currEntry.forEach(e => combineObject(e, nameChange));
      } else {
        combineObject(currEntry, nameChange);
      }
      insertEntry(entries, currEntry);
      delete targetEntries[currName];
    } else if (!entries[nameChange.name]) {
      insertEntry(entries, { name: nameChange.name, kind: "def" });
    }
    nameChange.name = `${type}.${nameChange.name}`;
    transform.transform[key] = nameChange;
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
    for (; i < path.length - 1; i++) {
      const el = obj.entries[path[i]];
      if (!el || Array.isArray(el) || !el.entries) {
        name = path.slice(i).join('.');
        break;
      }
      obj = /**@type {ApiEntry} */(el);
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
 * Marshal name of member functions
 * @param {ApiEntryTransform|string} entry 
 * @param {string}          name 
 * @param {string}          typeName 
 */
function makeRenameEntry(entry, name, typeName) {
  let newEntry = {};
  if (entry === "placeholder") {
    newEntry = {};
  } else if (entry === "ctor") {
    newEntry = { kind: "function", name: typeName, type: "", static: false };
  } else if (entry === "immutable") {
    newEntry = { kind: "function", immutable: true };
  } else if (typeof entry !== "object") {
    newEntry = { kind: /** @type {ApiEntryKind} */(entry) };
  } else {
    newEntry = entry;
  }
  return /** @type {ApiEntry} */ ({ ...newEntry, name: newEntry.name || makeNaturalName(name, typeName) });
}

/**
 * 
 * @param {string} name 
 * @param {string} typeName 
 */
function makeNaturalName(name, typeName) {
  typeName = normalizeTypeName(typeName);
  const replaceRegexp = new RegExp(typeName.startsWith("F") ? "F?" + typeName.slice(1) : typeName);
  name = name.replace(replaceRegexp, "");
  if (/Float$/.test(name)) name = name.slice(0, name.length - 5);
  return name;
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
  const parameter = parameters[0];
  const type = typeof parameter !== "string" ? parameter.type : "";
  if (type.includes(typeName)) {
    parameters.shift();
    if (type.startsWith("const ")) entry.immutable = true;
  } else if (entry.static !== false) {
    entry.static = !entry.immutable;
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
 * @param {ApiParameters} parameters
 * @param {ApiContext} context 
 */
function transformParameters(parameters, context) {
  return parameters.map(parameter => {
    if (typeof parameter == "string") return parameter;
    let { name, type, default: defaultValue } = parameter;
    type = transformType(type, context.paramTypeMap);
    return { name, type, default: defaultValue };
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
  docStr = docStr.replace(/^# Category(\w+)/, `@defgroup Category$1 Category $1`);
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
  return transformString(name, context.renameRules);
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
