const { insertEntry } = require("./parse");
const { system, combineObject, looksLikeFreeFunction } = require("./utils");

/**
 * @import { Api, ApiEntries, ApiEntry, ApiEntryKind, ApiEntryTransform, ApiEnumeration, ApiFile, ApiParameters, ApiResource, ApiSubEntryTransformMap, ApiTransform, Dict, ApiFileTransform, ReplacementRule, StringMap } from "./types"
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
      doc: fileConfig.doc || transformFileDoc(sourceFile.doc, context) || "",
      entries: transformEntries(sourceFile.entries, context, fileConfig)
    };
  }
  for (const file of Object.values(files)) {
    if (file.doc) file.doc = resolveDocRefs(file.doc, context);
    if (file.entries) transformEntriesDocRefs(file.entries, context);
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

    /** @type {StringMap} */
    this.returnTypeMap = Object.create(this.typeMap);
    Object.assign(this.returnTypeMap, transform.returnTypeMap ?? {});

    /** @type {StringMap} */
    this.nameMap = {};

    if (transform.prefixes?.length) {
      this.prefixToRemove = Array.isArray(transform.prefixes)
        ? RegExp(`^(${transform.prefixes.join("|")})`)
        : RegExp("^" + transform.prefixes);
      this.referenceCandidate = Array.isArray(transform.prefixes)
        ? RegExp(`\\b(?:${transform.prefixes.join("|")})\\w+`, "g")
        : RegExp(`\\b${transform.prefixes}\\w+`, "g");
    }

    this.renameRules = transform.renameRules ?? [];
    this.renameRules.forEach(rule => rule.pattern = RegExp(rule.pattern));

    this.docRules = transform.docRules ?? [];
    this.docRules.forEach(rule => rule.pattern = RegExp(rule.pattern, 'g'));

    this.definitionPrefix = transform.definitionPrefix;
  }
}

/**
 * 
 * @param {ApiEntries}    sourceEntries 
 * @param {ApiContext}    context 
 * @param {ApiFileTransform} transform
 */
function transformEntries(sourceEntries, context, transform) {
  /** @type {ApiEntries} */
  const targetEntries = {};
  const blacklist = new Set(transform.ignoreEntries ?? []);

  const transformMap = transform.transform ?? {};
  if (!transform.transform) transform.transform = transformMap;

  const defPrefix = transform.definitionPrefix ?? context.definitionPrefix ?? "";
  if (!transform.definitionPrefix) transform.definitionPrefix = defPrefix;

  const includeAfter = transform.includeAfter ?? {};
  if (!transform.includeAfter) transform.includeAfter = includeAfter;

  if (transform.resources) expandResources(transform.resources, transform, context);
  if (transform.enumerations) expandEnumerations(sourceEntries, transform, context);
  if (transform.namespacesMap) expandNamespaces(sourceEntries, transform, context);

  insertEntryAndCheck(targetEntries, includeAfter.__begin ?? [], context, transform);

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
      } else if (targetEntry.kind === "def") {
        if (!targetName.startsWith(defPrefix)) {
          targetName = defPrefix + targetName;
          targetEntry.name = targetName;
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
    insertEntryAndCheck(targetEntries, includeAfter[sourceName] ?? [], context, transform);
  }
  insertEntryAndCheck(targetEntries, includeAfter.__end ?? [], context, transform);
  for (const [key, entry] of Object.entries(targetEntries)) {
    if (Array.isArray(entry)) {
      entry.forEach(e => {
        if (e.sourceName) context.nameMap[e.sourceName] = key;
      });
    } else if (entry.sourceName) {
      context.nameMap[entry.sourceName] = key;
    }
  }
  transformHierarchy(targetEntries);
  validateEntries(targetEntries);

  return targetEntries;
}

/**
 * 
 * @param {ApiEntries}    sourceEntries 
 * @param {ApiFileTransform} transform,
 * @param {ApiContext}    context 
 */
function expandNamespaces(sourceEntries, transform, context) {
  const namespacesMap = transform.namespacesMap;
  for (const [prefix, nsName] of Object.entries(namespacesMap)) {
    /** @type {ApiEntryTransform} */
    const ns = {
      kind: "ns",
      name: nsName,
      entries: {}
    };
    const sourceEntriesListed = Object.entries(sourceEntries)
      .filter(([key]) => key.startsWith(prefix));
    sourceEntriesListed
      .forEach(([key, entry]) => {
        ns.entries[key] = entry;
        if (Array.isArray(entry)) {
          entry.forEach(e => e.name = e.name.slice(prefix.length));
        } else {
          entry.name = entry.name.slice(prefix.length);
          if (entry.kind === "def") {
            entry.kind = "var";
            entry.type = "auto";
            entry.constexpr = true;
            entry.sourceName = key;
          }
        }
      });
    transform.includeAfter[sourceEntriesListed[0][0]] = ns;
  }
}

/**
 * 
 * @param {Dict<ApiResource>} resources 
 * @param {ApiFileTransform} transform 
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
    const customPointerType = resourceEntry.pointerType;
    const pointerType = customPointerType ?? `${type} *`;

    const refTypeParams = customPointerType ? `${type}, ${customPointerType}` : type;
    const refType = `${name}<ObjectRef<${refTypeParams}>>`;
    const uniqueTypeParams = customPointerType ? `${type}, ObjectDeleter<ObjectRef<${refTypeParams}>>` : type;
    const uniqueType = `${name}<ObjectUnique<${uniqueTypeParams}>>`;

    const title = uniqueName[0].toLowerCase() + uniqueName.slice(1);
    const template = resourceEntry.template ?? [{ type: `ObjectBox<${pointerType}>`, name: "T" }];
    const replaceType = sourceName;
    const replaceTypeConst = "const " + replaceType;
    const replacePointerType = pointerType;
    const replacePointerTypeConst = "const " + replacePointerType;
    const freeFunction = resourceEntry.free ?? scanFreeFunction(resourceEntry.entries ?? {});

    if (freeFunction) {
      /** @type {ApiEntry} */
      const freeEntry = {
        name: `ObjectRef<${refTypeParams}>::doFree`,
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
 * @param {ApiEntries}            sourceEntries 
 * @param {ApiFileTransform}         transform,
 * @param {ApiContext}            context 
 */
function expandEnumerations(sourceEntries, transform, context) {
  const enumerations = transform.enumerations;
  for (const [type, enumTransform] of Object.entries(enumerations)) {
    const includeAfter = enumTransform.includeAfter;
    const sourceEntry = sourceEntries[type];
    if (Array.isArray(sourceEntry)) continue;
    const targetType = addTransform(type, enumTransform, includeAfter);
    let values = enumTransform.values;
    /** @type {StringMap} */
    const newNames = {};
    if (!values?.length) {
      if (sourceEntry.kind === "enum") {
        values = Object.keys(sourceEntry.entries);
      } else {
        const prefix = enumTransform.prefix ?? type.toUpperCase();
        values = Object.values(sourceEntries)
          .filter(e => !Array.isArray(e)
            && e.kind === "def"
            && !e.parameters
            && e.name.startsWith(prefix))
          .map(e => /** @type {ApiEntry}*/(e).name);
        const newPrefix = enumTransform.newPrefix;
        if (newPrefix) {
          const oldPrefixLen = prefix.length;
          values.forEach(n => newNames[n] = newPrefix + n.slice(oldPrefixLen));
        }
      }
    }
    for (const value of values) {
      addTransform(value, {
        kind: "var",
        constexpr: true,
        type: targetType,
      }, includeAfter, newNames[value]);
    }
  }

  /** 
   * @param {string}            name 
   * @param {ApiEntryTransform} entry  
   * @param {string=}           includeAfter 
   * @param {string=}           newName
   */
  function addTransform(name, entry, includeAfter, newName) {
    const currEntry = transform.transform[name] || {};
    entry = { ...currEntry, ...entry };
    if (newName) entry.name = newName;
    const targetName = entry.name ?? transformName(name, context);

    // @ts-ignore
    delete entry.values;
    // @ts-ignore
    delete entry.prefix;
    // @ts-ignore
    delete entry.includeAfter;
    // @ts-ignore
    delete entry.newPrefix;

    if (includeAfter) {
      const placeholder = {
        name: targetName,
        type: entry.type,
      };
      const includeTarget = transform.includeAfter[includeAfter];
      if (!includeTarget) {
        transform.includeAfter[includeAfter] = placeholder;
      } else if (Array.isArray(includeTarget)) {
        includeTarget.push(placeholder);
      } else {
        transform.includeAfter[includeAfter] = [includeTarget, placeholder];
      }
    }
    transform.transform[name] = entry;
    return targetName;
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
 * @param {ApiFileTransform}                 transform
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
 * @param {ApiFileTransform}     transform 
 * @param {ApiEntries}        targetEntries
 */
function transformSubEntries(targetEntry, context, transform, targetEntries) {
  /** @type {ApiEntries} */
  const entries = {};
  const type = targetEntry.name;
  const defPrefix = transform.definitionPrefix;
  for (const [key, entry] of Object.entries(targetEntry.entries)) {
    const nameCandidate = transformName(key, context);
    if (Array.isArray(entry) || nameCandidate === key) {
      insertEntry(entries, /** @type {ApiEntry}*/(entry), key);
      continue;
    }
    const nameChange = makeRenameEntry(entry, nameCandidate, type);
    const currName = transform.transform[key]?.name ?? nameCandidate;
    const currDefName = defPrefix + currName;
    const currEntry = targetEntries[currName] ?? targetEntries[currDefName];
    if (currEntry) {
      if (Array.isArray(currEntry)) {
        currEntry.forEach(e => combineObject(e, nameChange));
      } else {
        combineObject(currEntry, nameChange);
      }
      insertEntry(entries, currEntry);
      delete targetEntries[currName];
      delete targetEntries[currDefName];
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
    newEntry = { kind: "function", name: typeName, type: "" };
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
  entry.name = name;
  if (name === typeName) {
    if (entry.doc) entry.doc = entry.doc.replace(/@returns?/, "@post");
    return;
  }
  typeName = normalizeTypeName(typeName);
  const parameters = entry.parameters;
  if (!parameters?.length) return;
  const parameter = parameters[0];
  const type = typeof parameter !== "string" ? parameter.type : "";
  if (type.includes(typeName)) {
    parameters.shift();
    if (entry.doc) entry.doc = entry.doc.replace(/@param \w+.*\n/, "");
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
 */
function transformFileDoc(docStr, context) {
  if (!docStr) return "";
  docStr = docStr.replace(/^# Category(\w+)/, `@defgroup Category$1 Category $1`);
  return transformDoc(docStr, context);
}

/** 
 * @param {string}      docStr
 * @param {ApiContext}  context   
 **/
function transformDoc(docStr, context) {
  return transformString(docStr, context.docRules)
    .replace(/\\(\w+)/g, '@$1')
    .replaceAll("NULL", "nullptr");
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

/**
 * @param {ApiEntries} entries 
 * @param {ApiContext} context 
 */
function transformEntriesDocRefs(entries, context) {
  for (const entry of Object.values(entries)) {
    if (Array.isArray(entry)) {
      entry.forEach(e => {
        if (e.doc) e.doc = resolveDocRefs(e.doc, context);
      });
    } else {
      if (entry.doc) entry.doc = resolveDocRefs(entry.doc, context);
      if (entry.entries) transformEntriesDocRefs(entry.entries, context);
    }
  }
}

/**
 * 
 * @param {string}     doc 
 * @param {ApiContext} context 
 */
function resolveDocRefs(doc, context) {
  if (!doc) return "";
  return doc.replaceAll(context.referenceCandidate, ref => context.nameMap[ref] || ref);
}


exports.transformApi = transformApi;
exports.transformEntries = transformEntries;
exports.transformEntry = transformEntry;
