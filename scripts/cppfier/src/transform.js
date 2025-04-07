const { insertEntry } = require("./parse");
const { system, combineObject, looksLikeFreeFunction } = require("./utils");

/**
 * @import { Api, ApiEntries, ApiEntry, ApiEntryKind, ApiEntryTransform, ApiEnumeration, ApiFile, ApiParameters, ApiResource, ApiSubEntryTransformMap, ApiTransform, Dict, ApiFileTransform, ReplacementRule, StringMap, ApiParameter, ApiType } from "./types"
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
 */
function transformApi(config) {
  const source = config.sourceApi;
  const transform = config.transform ?? { files: {} };

  /** @type {ApiContext} */
  const context = new ApiContext(transform);

  /** @type {ApiFile[]} */
  const files = [];
  const fileTransformMap = transform.files ?? {};

  // Ensure fileTransformMap is full
  for (const sourceName of Object.keys(source.files)) {
    const fileConfig = fileTransformMap[sourceName];
    if (fileTransformMap[sourceName]) {
      fileConfig.ignoreEntries?.forEach(s => context.blacklist.add(s));
      if (!fileConfig.transform) fileConfig.transform = {};
      if (!fileConfig.definitionPrefix) fileConfig.definitionPrefix = context.definitionPrefix;
      if (!fileConfig.includeAfter) fileConfig.includeAfter = {};
    } else {
      fileTransformMap[sourceName] = { transform: {}, definitionPrefix: context.definitionPrefix, includeAfter: {} };
    }
  }

  // Step 1: Expand types
  for (const [sourceName, sourceFile] of Object.entries(source.files)) {
    const fileConfig = fileTransformMap[sourceName];
    expandTypes(sourceFile.entries, fileConfig, context);
  }

  // Step 2: Transform Files
  for (const [sourceName, sourceFile] of Object.entries(source.files)) {
    const fileConfig = fileTransformMap[sourceName];
    const targetName = fileConfig.name || transformIncludeName(sourceName, context);
    system.log(`Transforming api ${sourceName} => ${targetName}`);

    files.push({
      name: targetName,
      doc: fileConfig.doc || transformFileDoc(sourceFile.doc, context) || "",
      entries: transformEntries(sourceFile.entries, fileConfig, context)
    });
  }

  // Step 3: Fix hierarchy
  for (const file of files) {
    transformHierarchy(file.entries, context);
    validateEntries(file.entries);
  }

  // Step 4: Transform docs
  for (const file of files) {
    if (file.doc) file.doc = resolveDocRefs(file.doc, context);
    if (file.entries) transformEntriesDocRefs(file.entries, context);
  }

  /** @type {Api} */
  const api = { files: {} };
  files.forEach(file => api.files[file.name] = file);

  return api;
}

class ApiContext {
  /** @param {ApiTransform} transform  */
  constructor(transform) {
    /** @type {Set<string>} */
    this.blacklist = new Set();

    /** @type {StringMap} */
    this.paramTypeMap = {};
    Object.assign(this.paramTypeMap, transform.paramTypeMap ?? {});

    /** @type {StringMap} */
    this.returnTypeMap = {};
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

    this.definitionPrefix = transform.definitionPrefix ?? "";

    /** @type {Dict<ApiType>} */
    this.types = {};
  }

  addParamType(originalType, targetType) {
    if (!this.paramTypeMap[originalType]) this.paramTypeMap[originalType] = targetType;
  }

  addReturnType(originalType, targetType) {
    if (!this.returnTypeMap[originalType]) this.returnTypeMap[originalType] = targetType;
  }
}

/**
 * 
 * @param {ApiEntries}        sourceEntries 
 * @param {ApiFileTransform}  file
 * @param {ApiContext}        context 
 */
function expandTypes(sourceEntries, file, context) {
  if (file.resources) expandResources(file.resources, file, context);
  if (file.enumerations) expandEnumerations(sourceEntries, file, context);
  if (file.wrappers) expandWrappers(sourceEntries, file, context);
  if (file.namespacesMap) expandNamespaces(sourceEntries, file, context);

  const transformMap = file.transform;

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (context.blacklist.has(sourceName) || Array.isArray(sourceEntry)) continue;
    const sourceKind = sourceEntry.kind;
    if (sourceKind !== "alias" && sourceKind !== "callback" && sourceKind !== "enum" && sourceKind !== "struct" && sourceKind !== "union"
      && sourceKind !== "ns"
    ) continue;
    const targetDelta = transformMap[sourceName];
    const name = transformName(sourceName, context);
    if (!targetDelta) {
      transformMap[sourceName] = {
        name,
        kind: "alias",
        type: sourceName
      };
    } else if (!targetDelta.name) targetDelta.name = name;
    const targetName = targetDelta?.name ?? name;
    if (targetName == sourceName) {
      context.blacklist.add(sourceName);
      continue;
    }
    context.nameMap[sourceName] = targetName;
    context.addParamType(sourceName, targetName);
    context.addParamType(`${sourceName} *`, `${targetName} *`);
    context.addParamType(`const ${sourceName}`, `const ${targetName}`);
    context.addParamType(`const ${sourceName} *`, `const ${targetName} &`);
    context.addReturnType(sourceName, targetName);
    context.addReturnType(`${sourceName} *`, `${targetName} *`);
    context.addReturnType(`const ${sourceName}`, `const ${targetName}`);
    context.addReturnType(`const ${sourceName} *`, `const ${targetName} *`);
  }
}

/**
 * 
 * @param {ApiEntries}        sourceEntries 
 * @param {ApiFileTransform}  file
 * @param {ApiContext}        context 
 */
function transformEntries(sourceEntries, file, context) {
  /** @type {ApiEntries} */
  const targetEntries = {};
  const transformMap = file.transform;
  const defPrefix = file.definitionPrefix;
  const includeAfter = file.includeAfter;

  insertEntryAndCheck(targetEntries, includeAfter.__begin ?? [], context, file);

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (context.blacklist.has(sourceName)) continue;
    let targetName = transformName(sourceName, context);
    if (Array.isArray(sourceEntry)) {
      const targetDelta = transformMap[sourceName];
      insertEntry(targetEntries, sourceEntry.map(e => {
        const targetEntry = transformEntry(e, context);
        if (targetDelta) {
          if (!targetDelta.name) targetDelta.name = targetName;
          combineObject(targetEntry, targetDelta);
        } else targetEntry.name = targetName;
        context.nameMap[sourceName] = targetEntry.name;
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
      if (targetEntry.kind === "def") {
        if (!targetName.startsWith(defPrefix)) {
          targetName = defPrefix + targetName;
          targetEntry.name = targetName;
        }
        context.nameMap[sourceName] = targetEntry.name;
      } else if (!context.nameMap[sourceName]) {
        context.nameMap[sourceName] = targetEntry.name;
      }
      insertEntryAndCheck(targetEntries, targetEntry, context, file, targetName);
      if (sourceEntry.kind === "enum") {
        insertEntry(targetEntries, Object.values(sourceEntry.entries).map(e => {
          if (Array.isArray(e)) throw new Error("Unimplemented");
          const name = transformName(e.name, context);
          context.nameMap[e.name] = name;
          return {
            ...e,
            sourceName: e.name,
            name,
            constexpr: true,
            type: targetName,
          };
        }));
      }
    }
    insertEntryAndCheck(targetEntries, includeAfter[sourceName] ?? [], context, file);
  }
  insertEntryAndCheck(targetEntries, includeAfter.__end ?? [], context, file);

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
    /** @type {ApiEntry} */
    const ns = {
      kind: "ns",
      name: nsName,
      entries: {}
    };
    const sourceEntriesListed = Object.entries(sourceEntries)
      .filter(([key]) => key.startsWith(prefix));
    sourceEntriesListed.forEach(([key, entry]) => {
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
        context.nameMap[key] = `${nsName}.${entry.name}`;
      }
    });
    includeAfter(ns, transform, sourceEntriesListed[0][0]);
  }
}


/**
 * 
 * @param {ApiEntries}    sourceEntries 
 * @param {ApiFileTransform} transform,
 * @param {ApiContext}    context 
 */
function expandWrappers(sourceEntries, transform, context) {
  for (const [type, wrapper] of Object.entries(transform.wrappers)) {
    const sourceEntry = sourceEntries[type];
    if (Array.isArray(sourceEntry)) continue;

    wrapper.kind = "struct";
    combineObject(wrapper, transform.transform[type] ?? {});
    transform.transform[type] = wrapper;
    const targetType = wrapper.name ?? transformName(type, context);
    if (wrapper.includeAfter) {
      includeAfter(targetType, transform, wrapper.includeAfter);
    }
    const isStruct = sourceEntry.kind === "struct" && !wrapper.type;
    const constexpr = wrapper.constexpr !== false;
    const param = wrapper.attribute ?? (targetType[0].toLowerCase() + targetType.slice(1));
    const attribute = "m_" + param;

    /** @type {ApiEntries} */
    const entries = {};

    if (!isStruct) insertEntry(entries, {
      kind: "var",
      name: attribute,
      type: type,
    });

    insertEntry(entries, {
      kind: "function",
      name: targetType,
      type: "",
      constexpr,
      parameters: [{
        type: isStruct ? `const ${type} &` : type,
        name: param,
        default: wrapper.defaultValue ?? "{}"
      }],
      doc: `Wraps ${type}.\n\n@param ${param} the value to be wrapped`
    });
    if (wrapper.ordered) {
      insertEntry(entries, {
        kind: "function",
        name: "operator<=>",
        type: "auto",
        constexpr,
        immutable: true,
        parameters: [{
          type: `const ${targetType} &`,
          name: "other",
        }],
      });
    } else {
      insertEntry(entries, {
        kind: "function",
        name: "operator==",
        type: "bool",
        constexpr,
        immutable: true,
        parameters: [{
          type: `const ${targetType} &`,
          name: "other",
        }],
      });
    }
    if (wrapper.nullable) insertEntry(entries, {
      kind: "function",
      name: "operator==",
      type: "bool",
      constexpr,
      immutable: true,
      parameters: ["std::nullptr_t"],
      doc: `Compare with nullptr.\n\n@returns True if invalid state, false otherwise.`
    });
    if (!isStruct) insertEntry(entries, {
      kind: "function",
      name: `operator ${type}`,
      type: "",
      constexpr,
      immutable: true,
      parameters: [],
      doc: `Unwraps to the underlying ${type}.\n\n@returns the underlying ${type}.`
    });
    if (wrapper.invalidState !== false) insertEntry(entries, {
      kind: "function",
      name: "operator bool",
      type: "",
      constexpr,
      explicit: true,
      immutable: true,
      parameters: [],
      doc: `Check if valid.\n\n@returns True if valid state, false otherwise.`
    });

    if (isStruct) {
      wrapper.type = type;
      /** @type {ApiParameter[]} */
      const parameters = [];
      for (const attrib of Object.values(sourceEntry.entries)) {
        if (Array.isArray(attrib) || attrib.kind !== "var") continue;
        const name = attrib.name;
        const type = attrib.type;
        parameters.push({ type, name });
        const capName = name[0].toUpperCase() + name.slice(1);
        insertEntry(entries, [{
          kind: "function",
          name: `Get${capName}`,
          type,
          constexpr,
          immutable: true,
          parameters: [],
          doc: `Get the ${name}.\n\n@returns current ${name} value.`
        }, {
          kind: "function",
          name: `Set${capName}`,
          type: `${targetType} &`,
          constexpr,
          parameters: [{
            type,
            name: `new${capName}`
          }],
          doc: `Set the ${name}.\n\n@param new${capName} the new ${name} value.\n@returns Reference to self.`
        }]);
        context.addParamType(type, type);
        context.addParamType(`${type} *`, `${type} *`);
        context.addParamType(`const ${type}`, `const ${type}`);
        context.addParamType(`const ${type} *`, `const ${type} &`);
      }
      insertEntry(entries, {
        kind: "function",
        name: targetType,
        type: "",
        constexpr,
        parameters,
        doc: `Constructs from its fields.\n\n` + parameters.map(p => `@param ${p.name} the value for ${p.name}.`).join("\n")
      });
    } else {
      wrapper.type = "";
    }

    const currentCtors = wrapper.entries?.[targetType];
    if (currentCtors) {
      insertEntry(entries, /** @type {ApiEntry} */(currentCtors), targetType);
      delete wrapper.entries[targetType];
    }
    wrapper.entries = { ...entries, ...(wrapper.entries ?? {}) };

    delete wrapper.invalidState;
    delete wrapper.attribute;
    delete wrapper.includeAfter;
    delete wrapper.nullable;
    delete wrapper.ordered;
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
      prependIncludeAfter(freeEntry, transform, sourceName);
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
        context.addParamType(replaceTypeConst, uniqueName);
        context.paramTypeMap[replacePointerType] = uniqueName;
        context.addParamType(replacePointerTypeConst, uniqueName);
        break;
      default:
        context.paramTypeMap[replaceType] = refName;
        context.addParamType(replaceTypeConst, refName);
        context.paramTypeMap[replacePointerType] = refName;
        context.addParamType(replacePointerTypeConst, refName);
        break;
    }
    switch (resourceEntry.returnType) {
      case "none": break;
      case "unique":
        context.returnTypeMap[replaceType] = uniqueName;
        context.addReturnType(replaceTypeConst, uniqueName);
        context.returnTypeMap[replacePointerType] = uniqueName;
        context.addReturnType(replacePointerTypeConst, uniqueName);
        break;
      default:
        context.returnTypeMap[replaceType] = refName;
        context.addReturnType(replaceTypeConst, refName);
        context.returnTypeMap[replacePointerType] = refName;
        context.addReturnType(replacePointerTypeConst, refName);
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
  includeAfter(referenceAliases, transform, "__begin");
}

/**
 * 
 * @param {ApiEntries}            sourceEntries 
 * @param {ApiFileTransform}         transform,
 * @param {ApiContext}            context 
 */
function expandEnumerations(sourceEntries, transform, context) {
  for (const [type, enumTransform] of Object.entries(transform.enumerations)) {
    const sourceEntry = sourceEntries[type];
    if (Array.isArray(sourceEntry)) continue;

    combineObject(enumTransform, transform.transform[type] ?? {});
    transform.transform[type] = enumTransform;
    const targetType = enumTransform.name ?? transformName(type, context);
    if (enumTransform.includeAfter) {
      includeAfter(targetType, transform, enumTransform.includeAfter);
    }

    if (!enumTransform.kind && sourceEntry.kind !== "alias") {
      enumTransform.kind = "alias";
      enumTransform.type = type;
    }

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
      }, enumTransform.includeAfter, newNames[value]);
    }

    delete enumTransform.values;
    delete enumTransform.prefix;
    delete enumTransform.includeAfter;
    delete enumTransform.newPrefix;
  }

  /** 
   * @param {string}            name 
   * @param {ApiEntryTransform} entry  
   * @param {string=}           includeAfterKey 
   * @param {string=}           newName
   */
  function addTransform(name, entry, includeAfterKey, newName) {
    combineObject(entry, transform.transform[name] || {});
    if (newName) entry.name = newName;
    const targetName = entry.name ?? transformName(name, context);
    context.nameMap[name] = targetName;
    if (includeAfterKey) includeAfter(targetName, transform, includeAfterKey);
    transform.transform[name] = entry;
  }
}

/**
 * Add to includeAfter field
 * @param {string|ApiEntry|ApiEntry[]}  entryOrName 
 * @param {ApiFileTransform}            transform 
 * @param {string}                      includeAfterKey 
 */
function includeAfter(entryOrName, transform, includeAfterKey) {
  const includeTarget = getOrCreateIncludeAfter(transform, includeAfterKey);
  if (Array.isArray(entryOrName)) {
    includeTarget.push(...entryOrName);
  } else {
    includeTarget.push((typeof entryOrName === "string") ? { name: entryOrName } : entryOrName);
  }
}

/**
 * Prepend to includeAfter field
 * @param {string|ApiEntry|ApiEntry[]}  entryOrName 
 * @param {ApiFileTransform}            transform 
 * @param {string}                      includeAfterKey 
 */
function prependIncludeAfter(entryOrName, transform, includeAfterKey) {
  const includeTarget = getOrCreateIncludeAfter(transform, includeAfterKey);
  if (Array.isArray(entryOrName)) {
    includeTarget.unshift(...entryOrName);
  } else {
    includeTarget.unshift((typeof entryOrName === "string") ? { name: entryOrName } : entryOrName);
  }
}

/**
 * 
 * @param {ApiFileTransform}  transform 
 * @param {string}            includeAfterKey 
 */
function getOrCreateIncludeAfter(transform, includeAfterKey) {
  const includeTarget = transform.includeAfter[includeAfterKey];
  if (!includeTarget) {
    return transform.includeAfter[includeAfterKey] = [];
  } else if (Array.isArray(includeTarget)) {
    return includeTarget;
  } else {
    return transform.includeAfter[includeAfterKey] = [includeTarget];
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
 * @param {ApiFileTransform}              transform
 * @param {string=}                       defaultName
 */
function insertEntryAndCheck(entries, entry, context, transform, defaultName) {
  if (Array.isArray(entry)) {
    entry.forEach(e => insertEntryAndCheck(entries, e, context, transform, defaultName));
    return;
  }
  if (entry.entries) entry.entries = transformSubEntries(entry, context, transform, entries);
  insertEntry(entries, /** @type {ApiEntry}*/(entry), defaultName);
  if (entry.kind === 'ns' || entry.kind === "struct") {
    const currType = context.types[entry.name];
    if (currType) {
      if (currType.kind !== "ns" && entry.kind !== "ns") system.warn(`Duplicate type definition ${entry.name}`);
    } else {
      context.types[entry.name] = /**@type {ApiType}*/(entry);
    }
  }
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
 * @param {ApiContext} context the context
 */
function transformHierarchy(targetEntries, context) {
  for (const key of Object.keys(targetEntries)) {
    if (!key.includes(".") && !key.includes("::")) continue;
    const isMove = key.includes(".");
    const path = key.split(/\.|::/);
    const obj = getTypeFromPath(path, context) ?? (isMove ? getTypeFromPath(key.split('.'), context) : null);
    if (!obj) continue;
    const entry = targetEntries[key];
    const typeName = obj.name;
    const targetName = path[path.length - 1];
    if (Array.isArray(entry)) {
      entry.forEach(e => prepareForTypeInsert(e, targetName, typeName));
    } else prepareForTypeInsert(entry, targetName, typeName);
    if (isMove) {
      delete targetEntries[key];
      insertEntry(obj.entries, entry);
    } else if (Array.isArray(entry)) {
      entry.forEach(e => {
        insertEntry(obj.entries, {
          ...e,
          doc: "",
          proto: true,
        });
        e.name = makeMemberName(key, obj.template);;
        e.template = obj.template;
        delete e.static;
      });
    } else {
      insertEntry(obj.entries, {
        ...entry,
        doc: "",
        proto: true,
      });
      entry.name = makeMemberName(key, obj.template);
      entry.template = obj.template;
      delete entry.static;
    }
  }

  /**
   * 
   * @param {string}        key 
   * @param {ApiParameters} template 
   */
  function makeMemberName(key, template) {
    if (!template) return key;
    const lastSeparator = key.lastIndexOf('::');
    const args = template.map(n => (typeof n === "string" ? n : n.name)).join(", ");
    return `${key.slice(0, lastSeparator)}<${args}>${key.slice(lastSeparator)}`;
  }
}

/**
 * Get element from path
 * @param {string[]}    path 
 * @param {ApiContext}  context 
 */
function getTypeFromPath(path, context) {
  let obj = context.types[path[0]];
  if (!obj || Array.isArray(obj) || !obj.entries) return null;
  let i = 1;
  for (; i < path.length - 1; i++) {
    const el = obj.entries[path[i]];
    if (!el || Array.isArray(el) || !el.entries) {
      return null;
    }
    obj = /**@type {ApiType} */(el);
  }
  return obj;
}

/**
 * Get element type from path
 * @param {string[]}    path 
 * @param {ApiContext}  context 
 */
function getFromPath(path, context) {
  const obj = getTypeFromPath(path, context);
  return obj.entries[path[path.length - 1]];
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
    entry.type = "";
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
    case "def":
      targetEntry.parameters = sourceEntry.parameters;
      break;
    default:
      delete targetEntry.parameters;
      delete targetEntry.entries;
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
