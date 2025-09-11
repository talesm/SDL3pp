const { generateCallParameters } = require("./generate");
const { insertEntry } = require("./parse");
const { system, combineObject, looksLikeFreeFunction, deepClone } = require("./utils");

/**
 * @import { Api, ApiEntries, ApiEntry, ApiEntryKind, ApiEntryTransform, ApiEnumeration, ApiFile, ApiParameters, ApiResource, ApiSubEntryTransformLegacyMap, ApiTransform, Dict, ApiFileTransform, ReplacementRule, StringMap, ApiParameter, ApiType, VersionTag, ApiEntryBase, EntryHint, ApiEntryTransformMap, EnumerationDefinition } from "./types"
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
  const sourceIncludePrefix = transform.sourceIncludePrefix ?? '';

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
      if (!fileConfig.sourceIncludePrefix) fileConfig.sourceIncludePrefix = sourceIncludePrefix;
    } else {
      fileTransformMap[sourceName] = { transform: {}, definitionPrefix: context.definitionPrefix, sourceIncludePrefix };
    }
  }

  // Step 1: Expand types
  for (const [sourceName, sourceFile] of Object.entries(source.files)) {
    context.setFile(sourceName);
    const fileConfig = fileTransformMap[sourceName];
    expandTypes(sourceFile.entries, fileConfig, context);
  }

  // Step 2: Transform Files
  for (const [sourceName, sourceFile] of Object.entries(source.files)) {
    context.setFile(sourceName);
    const fileConfig = fileTransformMap[sourceName];
    context.enableException = fileConfig.enableException !== false;
    const targetName = fileConfig.name || transformIncludeName(sourceName, context);
    system.log(`Transforming api ${sourceName} => ${targetName}`);

    const includes = fileConfig.includes ?? [];
    const qualifiedSourceFile = fileConfig.sourceIncludePrefix + sourceName;
    includes.push(qualifiedSourceFile);

    files.push({
      name: targetName,
      doc: fileConfig.doc || transformFileDoc(sourceFile.doc, context) || "",
      entries: transformEntries(sourceFile.entries, fileConfig, context),
      includes,
      localIncludes: fileConfig.localIncludes,
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

    this.minVersions = transform.minVersions ?? {};

    /** @type {Dict<Dict<ApiEntryTransform[]>>} */
    this.includeBeforeMap = {};
    /** @type {Dict<Dict<ApiEntryTransform[]>>} */
    this.includeAfterMap = {};
    this.file = '';

    /** @type {Dict<ApiEntryTransform[]>} */
    this.currentIncludeBefore = null;
    /** @type {Dict<ApiEntryTransform[]>} */
    this.currentIncludeAfter = null;

    /** @type {StringMap} */
    this.paramTypeMap = {};
    Object.assign(this.paramTypeMap, transform.paramTypeMap ?? {});

    /** @type {StringMap} */
    this.returnTypeMap = {};
    Object.assign(this.returnTypeMap, transform.returnTypeMap ?? {});

    /** @type {StringMap} */
    this.nameMap = {};

    /** 
     * @type {Dict<ApiEntry>} 
     * @private
     */
    this.glossary = {};// Map of all entered apis

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

    this.enableException = false;
  }

  /**
   * 
   * @param {string} sourceName 
   * @param {ApiEntry} entry 
   */
  addGlossary(sourceName, entry) {
    this.glossary[sourceName] = deepClone(entry);
  }

  /**
   * 
   * @param {string} sourceName 
   * @returns {ApiEntry|undefined}
   */
  checkGlossary(sourceName) {
    const entry = this.glossary[sourceName];
    if (entry) return deepClone(entry);
  }

  /**
   * @param {string} originalType 
   * @param {string} targetType 
   */
  addName(originalType, targetType) {
    if (!this.nameMap[originalType]) this.nameMap[originalType] = targetType;
  }

  /**
   * @param {string} nameCandidate 
   * @returns 
   */
  getName(nameCandidate) {
    return this.nameMap[nameCandidate] || nameCandidate;
  }

  /**
   * @param {string} originalType 
   * @param {string} targetType 
   */
  addParamType(originalType, targetType) {
    if (!this.paramTypeMap[originalType]) this.paramTypeMap[originalType] = targetType;
  }

  /**
   * @param {string} originalType 
   * @param {string} targetType 
   */
  addReturnType(originalType, targetType) {
    if (!this.returnTypeMap[originalType]) this.returnTypeMap[originalType] = targetType;
  }

  /** @param {VersionTag} version  */
  isAfterMinVersion(version) {
    const tag = this.minVersions[version.tag];
    if (!tag) return false;
    if (version.major > tag.major) return true;
    if (version.major < tag.major) return false;
    if (version.minor > tag.minor) return true;
    if (version.minor < tag.minor) return false;
    if (version.patch > tag.patch) return true;
    /* if (version.patch <= tag.patch)*/ return false;
  }

  /**
   * 
   * @param {string} file 
   */
  setFile(file) {
    const includeBefore = this.includeBeforeMap[file];
    if (!includeBefore) {
      this.currentIncludeBefore = this.includeBeforeMap[file] = {};
    } else {
      this.currentIncludeBefore = includeBefore;
    }
    const includeAfter = this.includeAfterMap[file];
    if (!includeAfter) {
      this.currentIncludeAfter = this.includeAfterMap[file] = {};
    } else {
      this.currentIncludeAfter = includeAfter;
    }
  }

  /**
   * 
   * @param {string}  includeBeforeKey 
   */
  getOrCreateIncludeBefore(includeBeforeKey) {
    const includeBefore = this.currentIncludeBefore;

    const includeTarget = includeBefore[includeBeforeKey];
    if (Array.isArray(includeTarget)) return includeTarget;
    return includeBefore[includeBeforeKey] = includeTarget ? [includeTarget] : [];
  }

  /**
   * 
   * @param {string}  includeAfterKey 
   */
  getOrCreateIncludeAfter(includeAfterKey) {
    const includeAfter = this.currentIncludeAfter;

    const includeTarget = includeAfter[includeAfterKey];
    if (Array.isArray(includeTarget)) return includeTarget;
    return includeAfter[includeAfterKey] = includeTarget ? [includeTarget] : [];
  }

  /**
   * Add to includeBefore field
   * @param {string|ApiEntryTransform|ApiEntryTransform[]}  entryOrName 
   * @param {string}                                        includeBeforeKey 
   */
  includeBefore(entryOrName, includeBeforeKey) {
    const includeTarget = this.getOrCreateIncludeBefore(includeBeforeKey);
    if (Array.isArray(entryOrName)) {
      includeTarget.push(...entryOrName);
    } else {
      includeTarget.push((typeof entryOrName === "string") ? { name: entryOrName } : entryOrName);
    }
  }

  /**
   * Prepend to includeBefore field
   * @param {string|ApiEntryTransform|ApiEntryTransform[]}  entryOrName 
   * @param {string}                                        includeBeforeKey 
   */
  prependIncludeBefore(entryOrName, includeBeforeKey) {
    const includeTarget = this.getOrCreateIncludeBefore(includeBeforeKey);
    if (Array.isArray(entryOrName)) {
      includeTarget.unshift(...entryOrName);
    } else {
      includeTarget.unshift((typeof entryOrName === "string") ? { name: entryOrName } : entryOrName);
    }
  }

  /**
   * Add to includeAfter field
   * @param {string|ApiEntryTransform|ApiEntryTransform[]}  entryOrName 
   * @param {string}                                        includeAfterKey 
   */
  includeAfter(entryOrName, includeAfterKey) {
    const includeTarget = this.getOrCreateIncludeAfter(includeAfterKey);
    if (Array.isArray(entryOrName)) {
      includeTarget.push(...entryOrName);
    } else {
      includeTarget.push((typeof entryOrName === "string") ? { name: entryOrName } : entryOrName);
    }
  }

  /**
   * Prepend to includeAfter field
   * @param {string|ApiEntryTransform|ApiEntryTransform[]}  entryOrName 
   * @param {string}                                        includeAfterKey 
   */
  prependIncludeAfter(entryOrName, includeAfterKey) {
    const includeTarget = this.getOrCreateIncludeAfter(includeAfterKey);
    if (Array.isArray(entryOrName)) {
      includeTarget.unshift(...entryOrName);
    } else {
      includeTarget.unshift((typeof entryOrName === "string") ? { name: entryOrName } : entryOrName);
    }
  }
}

/**
 * 
 * @param {ApiEntryKind} kind 
 */
function isType(kind) {
  switch (kind) {
    case "alias":
    case "callback":
    case "enum":
    case "struct":
    case "union":
    case "ns":
      return true;
    default: return false;
  }
}

/**
 * 
 * @param {ApiEntries}        sourceEntries 
 * @param {ApiFileTransform}  file
 * @param {ApiContext}        context 
 */
function expandTypes(sourceEntries, file, context) {
  expandResources(sourceEntries, file, context);
  expandWrappers(sourceEntries, file, context);
  expandEnumerations(sourceEntries, file, context);
  expandNamespaces(sourceEntries, file, context);
  expandCallbacks(sourceEntries, file, context);

  const transformMap = file.transform ?? {};

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (context.blacklist.has(sourceName) || Array.isArray(sourceEntry)) continue;
    if (!isType(sourceEntry.kind)) continue;
    const targetDelta = transformMap[sourceName];
    const name = transformName(sourceName, context);
    if (targetDelta) {
      if (!targetDelta.name) targetDelta.name = name;
    } else {
      transformMap[sourceName] = {
        name,
        kind: "alias",
        type: sourceName
      };
    }
    const targetName = targetDelta?.name ?? name;
    if (targetName == sourceName) {
      context.blacklist.add(sourceName);
      continue;
    }
    context.addName(sourceName, targetName);
    context.addParamType(sourceName, targetName);
    context.addParamType(`${sourceName} *`, `${targetName} *`);
    context.addParamType(`const ${sourceName}`, `const ${targetName}`);
    context.addParamType(`const ${sourceName} *`, `const ${targetName} *`);
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

  let lastSourceName = "__begin";
  for (const [sourceName, transformEntry] of Object.entries(transformMap)) {
    if (sourceEntries[sourceName]) {
      lastSourceName = sourceName;
      if (transformEntry.after) {
        const targetName = transformEntry.name ?? transformName(sourceName, context);
        transformEntry.name = targetName;
        context.includeAfter(targetName, transformEntry.after);
      }
      continue;
    }
    if (!transformEntry.name) transformEntry.name = sourceName;
    context.includeAfter(transformEntry, lastSourceName);
  }
  const includeBefore = context.currentIncludeBefore;
  const includeAfter = context.currentIncludeAfter;

  insertEntryAndCheck(targetEntries, includeBefore.__begin ?? [], context, file);
  insertEntryAndCheck(targetEntries, includeAfter.__begin ?? [], context, file);

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (context.blacklist.has(sourceName)) continue;
    let targetName = transformName(sourceName, context);
    insertEntryAndCheck(targetEntries, includeBefore[sourceName] ?? [], context, file);
    if (Array.isArray(sourceEntry)) {
      const targetDelta = transformMap[sourceName];
      insertEntry(targetEntries, sourceEntry.map(e => {
        const targetEntry = transformEntry(e, context);
        if (targetDelta) {
          if (!targetDelta.name) targetDelta.name = targetName;
          combineObject(targetEntry, targetDelta);
        } else targetEntry.name = targetName;
        context.addName(sourceName, targetEntry.name?.replaceAll('::', '.'));
        return targetEntry;
      }));
    } else {
      const targetEntry = transformEntry(sourceEntry, context);
      context.addGlossary(sourceName, targetEntry);
      const targetDelta = transformMap[sourceName];
      if (targetDelta) {
        for (let link = targetDelta.link; link;) {
          const linkedEntry = deepClone(targetEntry);
          const nextLink = link.link;
          delete link.link;
          if (link.kind || sourceEntry.kind !== 'def') {
            combineObject(linkedEntry, link);
            insertEntryAndCheck(targetEntries, linkedEntry, context, file);
          }
          link = nextLink;
        }
        delete targetDelta.link;
        if (targetDelta.name) targetName = targetDelta.name;
        else targetDelta.name = targetName;
        combineObject(targetEntry, targetDelta);
      } else targetEntry.name = targetName;
      if (targetEntry.kind === "def") {
        if (!targetName.startsWith(defPrefix)) {
          targetName = defPrefix + targetName;
          targetEntry.name = targetName;
        }
        context.addName(sourceName, targetEntry.name);
      } else {
        context.addName(sourceName, targetEntry.name?.replaceAll('::', '.'));
      }
      insertEntryAndCheck(targetEntries, targetEntry, context, file, targetName);
    }
    insertEntryAndCheck(targetEntries, includeAfter[sourceName] ?? [], context, file);
  }
  insertEntryAndCheck(targetEntries, includeBefore.__end ?? [], context, file);
  insertEntryAndCheck(targetEntries, includeAfter.__end ?? [], context, file);

  return targetEntries;
}

/**
 * 
 * @param {ApiEntries}    sourceEntries 
 * @param {ApiFileTransform} file,
 * @param {ApiContext}    context 
 */
function expandNamespaces(sourceEntries, file, context) {
  const namespacesMap = file.namespacesMap ?? {};
  for (const [prefix, nsName] of Object.entries(namespacesMap)) {
    /** @type {ApiEntryTransformMap} */
    const nsEntries = {};
    const sourceEntriesListed = Object.entries(sourceEntries)
      .filter(([key]) => key.startsWith(prefix));
    for (const [key, entry] of sourceEntriesListed) {
      if (Array.isArray(entry)) continue;
      const transformEntry = file.transform[key] || {};
      const localName = entry.name.slice(prefix.length);
      file.transform[key] = transformEntry;
      transformEntry.name = nsName + '.' + localName;

      if (entry.kind === "def" && !transformEntry.kind) {
        transformEntry.kind = "var";
        transformEntry.type = "auto";
        transformEntry.constexpr = true;
      }
      context.addName(key, `${nsName}.${localName}`);
    }
    if (sourceEntriesListed.length) {
      context.includeBefore({ kind: "ns", name: nsName, entries: nsEntries }, sourceEntriesListed[0][0]);
    }
  }
}

/**
 * 
 * @param {ApiEntries}        sourceEntries 
 * @param {ApiFileTransform}  file,
 * @param {ApiContext}        context 
 */
function expandCallbacks(sourceEntries, file, context) {
  const transformMap = file.transform;
  for (const sourceEntry of Object.values(sourceEntries)) {
    if (Array.isArray(sourceEntry) || sourceEntry.kind !== "callback") continue;
    const sourceName = sourceEntry.name;
    const targetDelta = transformMap[sourceName];
    const name = targetDelta?.name ?? transformName(sourceName, context);
    if (!targetDelta) {
      transformMap[sourceName] = {
        name,
        kind: "alias",
        type: sourceName
      };
    } else if (!targetDelta.name) {
      targetDelta.name = name;
    }
    const parameters = sourceEntry.parameters;
    delete sourceEntry.parameters;
    for (let i = 0; i < parameters.length; i++) {
      const parameter = parameters[i];
      if (typeof parameter !== "string" && parameter.type === "void *" && parameter.name === "userdata") {
        const typeParams = parameters.map(p => (typeof p === "string") ? p : p.type);
        const callbackName = name.replace(/(Function|Callback)$/, "") + "CB";
        typeParams.splice(i, 1);
        /** @type {ApiEntryTransform}  */
        const callbackEntry = {
          kind: "alias",
          name: callbackName,
          type: `std::function<${sourceEntry.type}(${typeParams.join(", ")})>`,
          doc: transformDoc(sourceEntry.doc ?? "", context) + `\n@sa ${name}`,
          ...(file.transform[callbackName] ?? {})
        };
        context.prependIncludeAfter(callbackEntry, sourceName);
        break;
      }
    }
  }
}

/**
 * 
 * @param {ApiEntryBase|ApiEntryBase[]} entry 
 * @param {EntryHint}                   hints 
 */
function addHints(entry, hints) {
  if (Array.isArray(entry)) return entry.forEach(e => addHints(e, { ...hints }));
  if (entry.hints) {
    combineObject(entry.hints, hints);
  } else {
    entry.hints = hints;
  }
}

/**
 * 
 * @param {ApiEntryBase|ApiEntryBase[]} entry 
 * @param {EntryHint}                   hints 
 */
function combineHints(entry, hints) {
  if (Array.isArray(entry)) return entry.forEach(e => combineHints(e, hints));
  if (entry.hints) {
    entry.hints = { ...hints, ...entry.hints };
  } else {
    entry.hints = { ...hints };
  }
}

/**
 * 
 * @param {ApiEntries}       sourceEntries 
 * @param {ApiFileTransform} file,
 * @param {ApiContext}       context 
 */
function expandWrappers(sourceEntries, file, context) {
  for (const [sourceType, transform] of Object.entries(file.transform)) {
    if (transform.kind && transform.kind !== 'struct') continue;

    const wrapper = transform.wrapper === true ? {} : transform.wrapper;
    if (!wrapper) continue;

    const sourceEntry = sourceEntries[sourceType];
    if (!sourceEntry || Array.isArray(sourceEntry)) continue;

    transform.kind = "struct";
    const targetType = transform.name ?? transformName(sourceType, context);
    transform.name = targetType;
    const isStruct = sourceEntry.kind === "struct" && !transform.type;

    const type = isStruct || !sourceEntry.type?.startsWith("struct ") ? sourceType : sourceType + " *";
    const constexpr = transform.constexpr !== false;
    const paramName = wrapper.attribute ?? (targetType[0].toLowerCase() + targetType.slice(1));
    const rawType = `${targetType}Raw`;
    const paramType = wrapper.paramType ?? (isStruct ? `const ${rawType} &` : type);
    const attribute = "m_" + paramName;
    context.includeAfter({ name: rawType, kind: 'alias', type: sourceType }, '__begin');

    /** @type {string[]} */
    const fields = [];
    if (isStruct) {
      for (const e of Object.values(sourceEntry.entries)) {
        if (!Array.isArray(e)) fields.push(e.name);
      }

      addHints(transform, {
        self: 'this',
      });
    } else {
      addHints(transform, {
        self: attribute,
      });
    }

    /** @type {ApiEntries} */
    const entries = {};

    if (!isStruct) {
      insertEntry(entries, {
        kind: "var",
        name: attribute,
        type: type,
      });
      addHints(transform, {
        private: true,
      });
    }

    if (wrapper.genCtor !== false) insertEntry(entries, {
      kind: "function",
      name: targetType,
      type: "",
      constexpr,
      parameters: [{
        type: paramType,
        name: paramName,
        default: wrapper.defaultValue ?? "{}"
      }],
      doc: `Wraps ${sourceType}.\n\n@param ${paramName} the value to be wrapped`,
      hints: {
        init: [`${isStruct ? sourceType : attribute}(${paramName})`],
        changeAccess: isStruct ? undefined : 'public',
      }
    });
    if (wrapper.ordered) {
      insertEntry(entries, [{
        kind: "function",
        name: "operator<=>",
        type: "auto",
        constexpr,
        immutable: true,
        parameters: [{
          type: `const ${targetType} &`,
          name: "other",
        }],
        doc: "Default comparison operator",
        hints: { default: true, }
      }, {
        kind: "function",
        name: "operator<=>",
        type: "auto",
        constexpr,
        immutable: true,
        parameters: [{
          type: paramType,
          name: paramName,
        }],
        doc: "Compares with the underlying type",
        hints: { body: `return operator<=>(${targetType}(${paramName}));`, }
      }]);
    } else if (!isStruct) {
      insertEntry(entries, [{
        kind: "function",
        name: "operator==",
        type: "bool",
        constexpr,
        immutable: true,
        parameters: [{
          type: `const ${targetType} &`,
          name: "other",
        }],
        doc: "Default comparison operator",
        hints: { default: true, }
      }, {
        kind: "function",
        name: "operator==",
        type: "bool",
        constexpr,
        immutable: true,
        parameters: [{
          type: paramType,
          name: paramName,
        }],
        doc: "Compares with the underlying type",
        hints: { body: `return operator==(${targetType}(${paramName}));`, }
      }]);
    } else if (wrapper.comparable) {
      const body = 'return ' + fields.map(f => `${f} == other.${f}`).join(' && ') + ';';
      insertEntry(entries, [{
        kind: "function",
        name: "operator==",
        type: "bool",
        constexpr,
        immutable: true,
        parameters: [{
          type: paramType,
          name: "other",
        }],
        doc: "Compares with the underlying type",
        hints: { body },
      }, {
        kind: "function",
        name: "operator==",
        type: "bool",
        constexpr,
        immutable: true,
        parameters: [{
          type: `const ${targetType} &`,
          name: "other",
        }],
        doc: "Compares with the underlying type",
        hints: { body: `return *this == (${paramType})(other);` },
      }]);
    }
    if (wrapper.nullable) insertEntry(entries, {
      kind: "function",
      name: "operator==",
      type: "bool",
      constexpr,
      immutable: true,
      parameters: [{ type: "std::nullptr_t", name: "_" }],
      doc: `Compare with nullptr.\n\n@returns True if invalid state, false otherwise.`,
      hints: { body: "return !bool(*this);" }
    });
    if (!isStruct) insertEntry(entries, {
      kind: "function",
      name: `operator ${type}`,
      type: "",
      constexpr,
      immutable: true,
      parameters: [],
      doc: `Unwraps to the underlying ${sourceType}.\n\n@returns the underlying ${type}.`,
      hints: { body: `return ${attribute};` }
    });
    if (wrapper.invalidState !== false) insertEntry(entries, {
      kind: "function",
      name: "operator bool",
      type: "",
      constexpr,
      explicit: true,
      immutable: true,
      parameters: [],
      doc: `Check if valid.\n\n@returns True if valid state, false otherwise.`,
      hints: { body: isStruct ? `return *this != ${sourceType}{};` : `return ${attribute} != 0;` }
    });

    if (isStruct) {
      transform.type = sourceType;
      transform.hints.super = sourceType;

      if (wrapper.genMembers !== false) {
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
            doc: `Get the ${name}.\n\n@returns current ${name} value.`,
            hints: { body: `return ${name};` },
          }, {
            kind: "function",
            name: `Set${capName}`,
            type: `${targetType} &`,
            constexpr,
            parameters: [{
              type,
              name: `new${capName}`
            }],
            doc: `Set the ${name}.\n\n@param new${capName} the new ${name} value.\n@returns Reference to self.`,
            hints: { body: `${name} = new${capName};\nreturn *this;` },
          }]);
          context.addParamType(sourceType, rawType);
          context.addParamType(`${sourceType} *`, `${rawType} *`);
          context.addParamType(`const ${sourceType}`, `const ${rawType}`);
          context.addParamType(`const ${sourceType} *`, `const ${rawType} &`);
        }
        insertEntry(entries, {
          kind: "function",
          name: targetType,
          type: "",
          constexpr,
          parameters,
          doc: `Constructs from its fields.\n\n` + parameters.map(p => `@param ${p.name} the value for ${p.name}.`).join("\n"),
          hints: { init: [`${sourceType}{${parameters.map(p => p.name).join(", ")}}`] },
        });
      }
    } else {
      transform.type = "";
    }

    const currentCtors = transform.entries?.[targetType];
    if (currentCtors) {
      insertEntry(entries, /** @type {ApiEntry} */(currentCtors), targetType);
      delete transform.entries[targetType];
    }
    const blockedNames = new Set(Object.keys(entries));
    blockedNames.add(targetType);
    if (transform.entries) Object.keys(transform.entries).forEach(k => blockedNames.add(k));
    const detectedMethods = detectMethods(sourceEntries, file.transform, sourceType, rawType, blockedNames);
    transform.entries = { ...entries, ...(transform.entries ?? {}), ...detectedMethods };
    if (type !== sourceType) {
      context.addParamType(type, targetType);
      context.addReturnType(type, targetType);
    }

    delete transform.wrapper;
  }
}



/**
 * 
 * @param {ApiEntries}            sourceEntries 
 * @param {ApiEntryTransformMap}  transformEntries 
 * @param {string}                sourceType 
 * @param {string}                paramType 
 * @param {Set<string>}           blockedNames 
 */
function detectMethods(sourceEntries, transformEntries, sourceType, paramType, blockedNames) {
  /** @type {ApiSubEntryTransformLegacyMap} */
  const foundEntries = {};
  for (let [sourceName, transformEntryArray] of Object.entries(transformEntries)) {
    if (blockedNames.has(sourceName) || foundEntries[sourceName]) continue;
    const sourceEntry = /** @type {ApiEntryTransform}*/(sourceEntries[sourceName]);
    const resultArray = [];
    if (!Array.isArray(transformEntryArray)) transformEntryArray = [transformEntryArray];
    /** @type {ApiEntryTransform[]} */
    for (const transformEntry of transformEntryArray) {
      if (transformEntry.kind !== 'function' && (transformEntry.kind || sourceEntry?.kind !== 'function')) continue;
      if (!transformEntry.parameters?.length) continue;
      const parameters = transformEntry.parameters;
      if (!parameters) continue;
      if (parameters.length === 0) {
        blockedNames.add(sourceName);
        continue;
      }
      const param0 = parameters[0];
      if (typeof param0 === 'string') {
        blockedNames.add(sourceName);
        continue;
      }
      if (!param0.type) continue;
      const m = paramMatchesVariants(param0, [paramType, `const ${paramType} &`, `${paramType} *`, `const ${paramType} *`]);
      if (!m) {
        blockedNames.add(sourceName);
        continue;
      }
      if (!sourceEntry) parameters.shift();
      resultArray.push({
        ...transformEntry,
        immutable: m === 'immutable'
      });
    }
    if (resultArray.length === 0) continue;
    foundEntries[sourceName] = resultArray.length === 1 ? resultArray[0] : resultArray;
  }
  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (blockedNames.has(sourceName) || foundEntries[sourceName]) continue;
    if (Array.isArray(sourceEntry)) continue;
    if (sourceEntry.kind !== "function") continue;
    const parameters = sourceEntry.parameters;
    if (!parameters?.length) continue;
    const param0 = parameters[0];
    if (typeof param0 === 'string') continue;
    const m = paramMatchesVariants(param0, [sourceType, `${sourceType} *`, `const ${sourceType}`, `const ${sourceType} *`]);
    if (!m) continue;
    const transformEntry = /** @type {ApiEntryTransform}*/(transformEntries[sourceName]);
    const targetName = transformEntry?.name;
    const targetParameters = transformEntry?.parameters;
    if (targetName || targetParameters) {
      foundEntries[sourceName] = {
        name: targetName,
        parameters: targetParameters,
        immutable: m === 'immutable' || undefined
      };
    } else {
      foundEntries[sourceName] = m;
    }
  }
  return foundEntries;
}

/**
 * 
 * @param {ApiParameter}  param0 
 * @param {string[]}      variants 
 */
function paramMatchesVariants(param0, variants) {
  for (const variant of variants) {
    if (param0.type === variant) return variant.startsWith('const ') ? 'immutable' : 'function';
  }
  return false;
}


/**
 * 
 * @param {ApiEntryTransform} entry 
 */
function getResourceDefinition(entry) {
  const resourceDef = entry.resource;
  switch (typeof resourceDef) {
    case "string": return { shared: resourceDef };
    case "boolean": return resourceDef ? {} : null;
    case "object": return resourceDef;
    default: return undefined;
  }
}

/**
 * 
 * @param {ApiEntries}        sourceEntries
 * @param {ApiFileTransform}  file 
 * @param {ApiContext}        context 
 */
function expandResources(sourceEntries, file, context) {
  // TODO try to auto detect sourceEntries

  /** @type {ApiEntry[]} */
  const referenceAliases = [];
  for (const [sourceName, targetEntry] of Object.entries(file.transform ?? {})) {
    const resourceEntry = getResourceDefinition(targetEntry);
    if (!resourceEntry) continue;

    const uniqueName = targetEntry.name || transformName(sourceName, context);
    const rawName = resourceEntry.rawName || `${uniqueName}Raw`;
    const constRawName = `const ${rawName}`;
    const paramType = `${uniqueName}Param`;
    const constParamType = `${uniqueName}ConstParam`;
    if (!targetEntry.kind) targetEntry.kind = 'struct';

    const type = targetEntry.type ?? sourceName;
    const sourceEntry =  /** @type {ApiEntry} */(sourceEntries[sourceName]);
    const isStruct = sourceEntry.kind === "struct" || (sourceEntry.kind === "alias" && sourceEntry.type.startsWith('struct '));
    const pointerType = (isStruct ? `${type} *` : type);
    const constPointerType = `const ${pointerType}`;
    const nullValue = isStruct ? 'nullptr' : '0';
    const title = uniqueName[0].toLowerCase() + uniqueName.slice(1);
    if (sourceEntry.type && !targetEntry.type) targetEntry.type = '';
    context.addName(sourceName, uniqueName);
    referenceAliases.push({ name: uniqueName, kind: "forward" });
    referenceAliases.push({ name: rawName, kind: "alias", type: pointerType });

    referenceAliases.push({
      name: paramType,
      kind: 'struct',
      doc: `Safely wrap ${uniqueName} for non owning parameters`,
      entries: {
        'value': {
          kind: 'var',
          name: 'value',
          type: rawName
        },
        [paramType]: [{
          kind: 'function',
          name: paramType,
          constexpr: true,
          type: '',
          parameters: [{ type: rawName, name: 'value' }],
          hints: { init: ['value(value)'] }
        }, {
          kind: 'function',
          name: paramType,
          constexpr: true,
          type: '',
          parameters: [{ type: "std::nullptr_t", name: "_" }],
          hints: { init: [`value(${nullValue})`] }
        }],
        [`operator ${rawName}`]: {
          kind: 'function',
          name: `operator ${rawName}`,
          constexpr: true,
          immutable: true,
          type: '',
          parameters: [],
          hints: { body: 'return value;' }
        }
      },
    });
    if (resourceEntry.enableConstParam) {
      referenceAliases.push({
        name: constParamType,
        kind: 'struct',
        doc: `Safely wrap ${uniqueName} for non owning const parameters`,
        entries: {
          'value': {
            kind: 'var',
            name: 'value',
            type: constRawName
          },
          [constParamType]: [{
            kind: 'function',
            name: constParamType,
            constexpr: true,
            type: '',
            parameters: [{ type: constRawName, name: 'value' }],
            hints: { init: ['value(value)'] }
          }, {
            kind: 'function',
            name: constParamType,
            constexpr: true,
            type: '',
            parameters: [{ type: paramType, name: 'value' }],
            hints: { init: ['value(value.value)'] }
          }, {
            kind: 'function',
            name: constParamType,
            constexpr: true,
            type: '',
            parameters: [{ type: "std::nullptr_t", name: "_" }],
            hints: { init: [`value(${nullValue})`] }
          }],
          [`operator ${constRawName}`]: {
            kind: 'function',
            name: `operator ${constRawName}`,
            constexpr: true,
            immutable: true,
            type: '',
            parameters: [],
            hints: { body: 'return value;' }
          }
        },
      });
    }
    context.addParamType(pointerType, paramType);
    context.addParamType(constPointerType, resourceEntry.enableConstParam ? constParamType : paramType);

    context.addReturnType(pointerType, rawName);
    context.addReturnType(constPointerType, constRawName);

    /** @type {EntryHint} */
    const copyCtorHints = {};
    if (!resourceEntry.shared) {
      copyCtorHints.delete = true;
    } else if (resourceEntry.shared !== true) {
      copyCtorHints.init = ["m_resource(other.m_resource)"];
      copyCtorHints.body = `++m_resource->${resourceEntry.shared};`;
    }

    /** @type {Dict<ApiEntryTransform | ApiEntryBase[]>} */
    const ctors = {
      [uniqueName]: [{
        kind: "function",
        type: "",
        constexpr: true,
        parameters: [],
        hints: { default: true, changeAccess: "public" },
      }, {
        kind: "function",
        type: "",
        constexpr: true,
        explicit: true,
        parameters: [{ name: "resource", type: constRawName }],
        hints: { init: ["m_resource(resource)"] },
      }, {
        kind: "function",
        type: "",
        parameters: [{ name: "other", type: `const ${uniqueName} &` }],
        hints: copyCtorHints,
      }, {
        kind: "function",
        type: "",
        constexpr: true,
        parameters: [{ name: "other", type: `${uniqueName} &&` }],
        hints: {
          init: ["m_resource(other.m_resource)"],
          body: `other.m_resource = ${nullValue};`
        },
      }]
    };
    const subEntries = targetEntry.entries || {};

    let extraUniqueCtors = subEntries[uniqueName];
    if (extraUniqueCtors) {
      delete subEntries[uniqueName];
      if (typeof extraUniqueCtors === "string") {
        extraUniqueCtors = [{ name: uniqueName, kind: "function", parameters: [] }];
      } else if (!Array.isArray(extraUniqueCtors)) {
        extraUniqueCtors = [extraUniqueCtors];
      }
      for (const entry of extraUniqueCtors) {
        entry.kind = "function";
        entry.type = "";
        // @ts-ignore
        insertEntry(ctors, entry, uniqueName);
      }
    }
    for (const sourceName of resourceEntry.ctors ?? []) {
      const entry = subEntries[sourceName] ?? {};
      delete subEntries[sourceName];
      const ctorTransform = file.transform[sourceName];
      if (typeof entry === "string") {
        system.warn(`${sourceName} can not be a custom ctor, only objects containing name property can be accepted.`);
        continue;
      }
      if (Array.isArray(entry)) {
        entry.forEach(e => {
          e.static = true;
          e.type = uniqueName;
          if (!e.name) e.name = transformMemberName(sourceName, uniqueName, context);
          if (!e.sourceName && sourceEntries[sourceName]) e.sourceName = sourceName;
          addHints(e, { wrapSelf: true });
        });
      } else {
        entry.static = true;
        entry.type = uniqueName;
        if (!entry.name) entry.name = transformMemberName(sourceName, uniqueName, context);
        if (!entry.sourceName && sourceEntries[sourceName]) entry.sourceName = sourceName;
        addHints(entry, { wrapSelf: true });
      }
      ctors[sourceName] = entry;
      if (!ctorTransform) {
        file.transform[sourceName] = { type: uniqueName, hints: { wrapSelf: true } };
      } else if (!ctorTransform.type) {
        file.transform[sourceName].type = uniqueName;
        addHints(ctorTransform, { wrapSelf: true });
      }
    }

    // const staticCreateFunctions = !targetEntry.noStaticCtors;
    for (const [sourceName, entry] of Object.entries(subEntries)) {
      const ctorTransform = file.transform[sourceName];
      let isCtor = false;
      if (typeof entry === "string") {
        if (entry === "ctor") {
          ctors[sourceName] = {
            kind: "function",
            type: "",
            name: uniqueName,
            sourceName,
          };
        }
        isCtor = true;
      } else if (!Array.isArray(entry) && entry.name === "ctor") {
        entry.kind = "function";
        entry.type = "";
        entry.name = uniqueName;
        if (!entry.sourceName && sourceEntries[sourceName]) entry.sourceName = sourceName;
        ctors[sourceName] = entry;
        isCtor = true;
      }
      if (isCtor) {
        delete subEntries[sourceName];
        if (!ctorTransform) {
          file.transform[sourceName] = { type: uniqueName, hints: { wrapSelf: true } };
        } else if (!ctorTransform.type) {
          ctorTransform.type = uniqueName;
          addHints(ctorTransform, { wrapSelf: true });
        }
      }
    }
    if (resourceEntry.enableAutoMethods !== false) {
      const blockedNames = new Set(Object.keys(subEntries));
      blockedNames.add(uniqueName);
      Object.keys(ctors).forEach(k => blockedNames.add(k));
      const detectedMethods = detectMethods(sourceEntries, file.transform, sourceName, rawName, blockedNames);
      combineObject(subEntries, detectedMethods);
    }

    let freeFunction = /** @type {ApiEntry} */(sourceEntries[resourceEntry.free ?? "reset"]) ?? scanFreeFunction(sourceEntries, uniqueName, pointerType);
    if (freeFunction) {
      const sourceName = freeFunction.name;
      freeFunction = transformEntry(freeFunction, context);
      const freeTransformEntry = /** @type {ApiEntryTransform} */(subEntries[sourceName]) ?? {};
      combineObject(freeFunction, freeTransformEntry);
      freeFunction.name = freeTransformEntry.name ?? makeNaturalName(transformName(sourceName, context), uniqueName);
      freeFunction.doc = freeFunction.doc ? transformDoc(freeFunction.doc, context) : `frees up ${title}.`;
      if (!freeFunction.hints?.body) {
        let body = `${sourceName}(m_resource)`;
        if (freeFunction.hints?.mayFail) {
          body = `CheckError(${body})`;
          if (freeFunction.type === "bool") freeFunction.type = "void";
        }
        body += `;\nm_resource = ${nullValue};`;
        if (freeFunction.type !== "void") body = `auto r = ${body}\nreturn r;`;
        combineHints(freeFunction, { body });
      }
      subEntries[sourceName] = freeFunction;
      if (!file.transform[sourceName]) {
        file.transform[sourceName] = { parameters: [{ type: rawName }] };
      }
    }

    targetEntry.doc = transformDoc(sourceEntry.doc ?? `Wraps ${title} resource.`, context) + `\n\n@cat resource`;
    targetEntry.entries = {
      "m_resource": {
        kind: "var",
        type: rawName,
        hints: { body: nullValue },
      },
      ...ctors,
      [`~${uniqueName}`]: {
        kind: "function",
        type: "",
        parameters: [],
        hints: { body: freeFunction ? `${freeFunction.sourceName ?? freeFunction.name}(m_resource);` : '' }
      },
      'operator=': {
        kind: "function",
        type: `${uniqueName} &`,
        parameters: [{ name: 'other', type: uniqueName }],
        hints: { body: "std::swap(m_resource, other.m_resource);\nreturn *this;" },
      },
      "get": {
        kind: "function",
        type: rawName,
        immutable: true,
        constexpr: true,
        parameters: [],
        hints: { body: "return m_resource;" },
      },
      "release": {
        kind: "function",
        type: rawName,
        constexpr: true,
        parameters: [],
        hints: { body: `auto r = m_resource;\nm_resource = ${nullValue};\nreturn r;` },
      },
      [`operator ${paramType}`]: {
        kind: "function",
        type: '',
        immutable: true,
        constexpr: true,
        parameters: [],
        hints: { body: "return {m_resource};" },
      },
      ...subEntries,
    };
    addHints(targetEntry, {
      self: "m_resource",
      super: "m_resource",
      private: true,

    });

    // const extraParametersStr = resourceEntry.extraParameters?.length
    //   ? (", " + resourceEntry.extraParameters.join(", ")) : "";

    /** @type {ApiEntryTransform[]} */
    const derivedEntries = [
      // {
      //   name: uniqueName,
      //   kind: "struct",
      //   type: `ResourceUnique<${refName}${extraParametersStr}>`,
      //   doc: `Handle to an owned ${title}.\n\n@cat resource\n\n@sa ${refName}`,
      //   entries: {
      //     "ResourceUnique::ResourceUnique": "alias",
      //     ...uniqueSubEntries,
      //   },
      //   hints: { "self": uniqueName }
      // }, {
      //   name: `${uniqueName}::share`,
      //   kind: "function",
      //   type: sharedName,
      //   doc: `Move this ${title} into a ${sharedName}.`,
      //   parameters: [],
      //   hints: { body: `return ${sharedName}(std::move(*this));` }
      // }, {
      //   name: unsafeName,
      //   kind: "struct",
      //   type: `ResourceUnsafe<${refName}${extraParametersStr}>`,
      //   doc: `Unsafe Handle to ${title}.\n\nMust call manually reset() to free.\n\n@cat resource\n\n@sa ${refName}`,
      //   entries: {
      //     "ResourceUnsafe::ResourceUnsafe": "alias",
      //     [unsafeName]: {
      //       kind: "function",
      //       type: "",
      //       explicit: true,
      //       constexpr: true,
      //       parameters: [{
      //         type: `${uniqueName} &&`,
      //         name: "other"
      //       }],
      //       doc: `Constructs ${unsafeName} from ${uniqueName}.`,
      //       hints: { init: [`${unsafeName}(other.release())`] },
      //     },
      //   },
      // },
    ];
    // /** @type {Set<string>} */
    // const derivedNames = new Set();
    // if (resourceEntry.lock) {
    //   const lockEntry = resourceEntry.lock !== true ? resourceEntry.lock : {};
    //   lockEntry.kind = "struct";
    //   if (!lockEntry.name) lockEntry.name = uniqueName + "Lock";
    //   if (!lockEntry.type) lockEntry.type = `LockBase<${refName}>`;
    //   if (!lockEntry.doc) lockEntry.doc = `Locks a ${uniqueName}.`;
    //   combineHints(lockEntry, { super: lockEntry.type });
    //   derivedNames.add(lockEntry.name);
    //   referenceAliases.push({ kind: "forward", name: lockEntry.name });
    //   derivedEntries.push(lockEntry);
    //   const lockFunctionName = resourceEntry.lockFunction;
    //   const unlockFunctionName = resourceEntry.unlockFunction;
    //   const lockFunction = targetEntry.entries[lockFunctionName];
    //   if (typeof lockFunction === "string") {
    //     targetEntry.entries[lockFunctionName] = {
    //       type: lockEntry.name,
    //     };
    //   }

    //   lockEntry.entries = {
    //     [lockEntry.name]: [{
    //       kind: "function",
    //       type: "",
    //       constexpr: true,
    //       proto: true,
    //       parameters: [],
    //       doc: "Creates an empty lock",
    //       hints: { default: true }
    //     }, {
    //       kind: "function",
    //       type: "",
    //       constexpr: true,
    //       parameters: [{ type: `${lockEntry.name} &&`, name: "other" }],
    //       doc: "Move constructor",
    //       hints: { init: ["LockBase(other.release())"] }
    //     }],
    //     [lockFunctionName]: "ctor",
    //     [`~${lockEntry.name}`]: {
    //       kind: "function",
    //       type: "",
    //       parameters: [],
    //       doc: "Destructor\n\n@sa Unlock()",
    //       hints: { body: "Unlock();" }
    //     },
    //     [unlockFunctionName]: {
    //       name: "Unlock",
    //       static: false,
    //       hints: {
    //         body: `CheckError(${unlockFunctionName}(release()));`,
    //         removeParamThis: true,
    //       },
    //     },
    //     "reset": {
    //       kind: "function",
    //       type: "void",
    //       parameters: [],
    //       doc: "Same as Unlock(), just for uniformity.",
    //       hints: { body: "Unlock();" },
    //     },
    //     ...(lockEntry.entries ?? {})
    //   };
    // }

    const includeAfterKey = targetEntry.after ?? sourceName;
    // let pointToIncludeFunc = includeAfterKey;
    context.includeAfter(derivedEntries, includeAfterKey);
    // for (const [subName, subEntry] of Object.entries(refSubEntries)) {
    //   if (Array.isArray(subEntry)) {
    //     subEntry.forEach(e => {
    //       if (derivedNames.has(e.type)) {
    //         context.includeAfter(e, file.name, pointToIncludeFunc);
    //       }
    //     });
    //   } else if (typeof subEntry === "string") {
    //     if (!context.prefixToRemove.test(subName)) continue;
    //     pointToIncludeFunc = subName;
    //     if (subEntry === "ctor") continue;
    //     const sourceEntry = /** @type {ApiEntry}*/(sourceEntries[subName]);
    //     if (sourceEntry.kind === "function" && derivedNames.has(context.returnTypeMap[sourceEntry.type])) {
    //       if (subEntry === "immutable") entry.entries[subName] = { proto: true, immutable: true };
    //       else entry.entries[subName] = { proto: true };
    //     }
    //   } else {
    //     if (context.prefixToRemove.test(subName)) pointToIncludeFunc = subName;
    //     const sourceEntry = /** @type {ApiEntry}*/(sourceEntries[subName]);
    //     if (subEntry.kind !== "function" && sourceEntry.kind !== "function") continue;
    //     const returnType = subEntry.type ?? context.returnTypeMap[sourceEntry.type];
    //     if (derivedNames.has(returnType)) subEntry.proto = true;
    //   }
    // }
    delete targetEntry.resource;
  }
  context.includeAfter(referenceAliases, '__begin');
}

/**
 * 
 * @param {ApiEntryTransform} entry 
 */
function getEnumDefinition(entry) {
  const enumDef = entry.enum;
  switch (typeof enumDef) {
    case "string": return { prefix: enumDef };
    case "boolean": return {};
    case "object": return enumDef;
    default: return undefined;
  }
}

/**
 * 
 * @param {ApiEntries}            sourceEntries 
 * @param {ApiFileTransform}      file,
 * @param {ApiContext}            context 
 */
function expandEnumerations(sourceEntries, file, context) {
  for (const sourceType of Object.values(sourceEntries)) {
    if (Array.isArray(sourceType) || sourceType.kind !== "enum") continue;
    const transform = file.transform[sourceType.name];
    if (!transform) {
      file.transform[sourceType.name] = { enum: true };
      continue;
    }
    if (!transform.kind && !transform.enum) transform.enum = true;
  }
  for (const [type, transform] of Object.entries(file.transform)) {
    const sourceEntry = sourceEntries[type];
    if (Array.isArray(sourceEntry)) continue;

    const definition = getEnumDefinition(transform);
    if (!definition) continue;

    const targetType = transform.name ?? transformName(type, context);

    const valueType = definition.valueType ?? (transform.kind === "struct" ? type : targetType);
    if (!transform.kind && !transform.type && sourceEntry?.kind !== "alias") {
      transform.kind = "alias";
      transform.type = type;
    }

    let values = definition.values ?? Object.keys(sourceEntry.entries ?? {});
    const prefix = definition.prefix ?? (type.toUpperCase() + "_");
    /** @type {StringMap} */
    const newNames = {};
    if (!values?.length) {
      values = Object.values(sourceEntries)
        .filter(e => !Array.isArray(e)
          && e.kind === "def"
          && !e.parameters
          && e.name.startsWith(prefix))
        .map(e => /** @type {ApiEntry}*/(e).name);
      const newPrefix = definition.newPrefix;
      if (newPrefix) {
        const oldPrefixLen = prefix.length;
        values.forEach(n => newNames[n] = newPrefix + n.slice(oldPrefixLen));
      }
    }
    const after = transform.after;
    if (after) context.includeAfter(targetType, after);

    for (const value of values) {
      const valueSource = sourceEntries[value];
      const valueTransform = file.transform[value];
      delete file.transform[value];
      /** @type {ApiEntryTransform & ApiEntry} */
      const valueTarget = {
        kind: "var",
        name: newNames[value] ?? transformName(value, context),
        constexpr: true,
        type: valueType,
      };
      combineObject(valueTarget, valueTransform || {});
      if (!valueTarget.doc) {
        // @ts-ignore
        const sourceDoc = valueSource?.doc ?? sourceEntry.entries?.[value]?.doc;
        valueTarget.doc = sourceDoc || (value.startsWith(prefix) ? value.slice(prefix.length) : valueTarget.name);
      }
      context.addName(value, valueTarget.name);
      if (!valueSource) {
        valueTarget.sourceName = value;
        context.includeAfter(valueTarget, after ?? type);
      } else if (after) {
        context.includeAfter(valueTarget, after);
      } else {
        file.transform[value] = valueTarget;
      }
    }
    delete transform.enum;
  }
}

/**
 * 
 * @param {ApiEntries}  entries 
 * @param {string}      uniqueType 
 * @param {string}      pointerType
 */
function scanFreeFunction(entries, uniqueType, pointerType) {
  /** @type {ApiEntry[]} */
  const candidates = [];
  for (const sourceName of Object.keys(entries)) {
    if (looksLikeFreeFunction(sourceName)) {
      const sourceEntry = entries[sourceName];
      if (Array.isArray(sourceEntry) || sourceEntry?.parameters?.length !== 1) {
        continue;
      }
      const firstParameter = sourceEntry.parameters[0];
      if (sourceEntry.name.endsWith(uniqueType) || (typeof firstParameter !== 'string' && firstParameter.type === pointerType)) {
        candidates.push(sourceEntry);
      }
    }
  }
  if (candidates.length === 1) return candidates[0];
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
  if (entry.entries) entry.entries = transformSubEntries(entry, context, transform);
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
 * @param {ApiEntryTransform} targetEntry the entry we are inserting from
 * @param {ApiContext}        context 
 * @param {ApiFileTransform}  file 
 */
function transformSubEntries(targetEntry, context, file) {
  /** @type {ApiEntries} */
  const entries = {};
  const type = targetEntry.name;
  const defPrefix = file.definitionPrefix;
  for (const [key, entry] of Object.entries(targetEntry.entries)) {
    if (Array.isArray(entry)) {
      insertEntry(entries, /** @type {ApiEntry[]}*/(entry), key);
      continue;
    }
    const nameCandidate = transformName(key, context);
    if (key == nameCandidate) {
      if (typeof entry === "string") {
        insertEntry(entries,/** @type {ApiEntry} */({
          kind: entry,
          name: key,
        }));
        continue;
      }
      insertEntry(entries, /** @type {ApiEntry}*/(entry), key);
      continue;
    }
    const nameChange = makeRenameEntry(entry, nameCandidate, type);
    if (!entries[nameChange.name]) {
      insertEntry(entries, { name: nameChange.name, kind: "def" });
    }
    if (typeof entry !== "string" && entry.proto && nameChange.type !== "") {
      nameChange.name = `${type}::${nameChange.name}`;
      delete nameChange.proto;
    } else {
      nameChange.name = `${type}.${nameChange.name}`;
    }
    if (file.transform[key]) {
      const currLink = file.transform[key];
      file.transform[key] = nameChange;
      file.transform[key].link = currLink;
      if (!currLink.name) currLink.name = transformName(key, context);
    } else {
      file.transform[key] = nameChange;
      const currEntry = file.transform[key];
      if (currEntry && currEntry.kind === 'def') {
        combineObject(currEntry, nameChange);
        continue;
      }
      if (!context.blacklist.has(key)) {
        file.transform[key].link = { name: transformName(key, context) };
      }
    }
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
    const isSameFile = !!targetEntries[path[0]];
    if (Array.isArray(entry)) {
      entry.forEach(e => prepareForTypeInsert(e, targetName, typeName));
    } else prepareForTypeInsert(entry, targetName, typeName);
    if (isMove) {
      delete targetEntries[key];
      insertEntry(obj.entries, entry);
    } else if (Array.isArray(entry)) {
      entry.forEach(e => insertCopyEntry(e));
    } else {
      insertCopyEntry(entry);
    }

    /**
     * @param {ApiEntry} entry 
     */
    function insertCopyEntry(entry) {
      insertEntry(obj.entries, {
        ...entry,
        doc: (isSameFile && entry.doc) || "",
        proto: true,
      });
      entry.name = makeMemberName(key, obj.template);
      entry.template = obj.template;
      if (obj.hints) combineHints(entry, obj.hints);
      if (entry.static) addHints(entry, { static: true });
      delete entry.static;
      delete entry.explicit;
      if (isSameFile) delete entry.doc;
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
    const args = generateCallParameters(template);
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
  } else if (entry.name === "ctor") {
    newEntry = { ...entry, kind: "function", name: typeName, type: "", proto: false };
  } else {
    newEntry = entry;
  }
  return /** @type {ApiEntry} */ ({ ...newEntry, name: newEntry.name || makeNaturalName(name, typeName) });
}

/**
 * 
 * @param {string}     name 
 * @param {string}     typeName 
 * @param {ApiContext} context
 */
function transformMemberName(name, typeName, context) {
  return makeNaturalName(transformName(name, context), typeName);
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
  if ((type.includes(typeName) || entry.hints?.removeParamThis) && !entry.static) {
    parameters.shift();
    if (entry.doc) entry.doc = entry.doc.replace(/@param \w+.*\n/, "");
    if (type.startsWith("const ")) entry.immutable = true;
  } else if (entry.static !== false) {
    entry.static = !entry.immutable;
  }
}

/** @param {string} typeName  */
function normalizeTypeName(typeName) {
  if (typeName.endsWith("Ref")) return typeName.slice(0, typeName.length - 3);
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
      const m = /@returns (?:(.*) on success|(an? valid [^,]+), or (?:\w+) on failure)/.exec(targetEntry.doc ?? "");
      if (context.enableException && m) {
        targetEntry.hints = { mayFail: true };
        const returnIndexBegin = m.index;
        const returnIndexEnd = targetEntry.doc.indexOf("\n\n", returnIndexBegin);
        const throwString = "@throws Error on failure.";
        if (sourceEntry.type === "bool") {
          targetEntry.type = "void";
          targetEntry.doc = `${targetEntry.doc.slice(0, returnIndexBegin)}${throwString}${targetEntry.doc.slice(returnIndexEnd)}`;
        } else {
          targetEntry.doc = `${targetEntry.doc.slice(0, returnIndexBegin)}@returns ${m[1] || m[2]} on success.\n${throwString}${targetEntry.doc.slice(returnIndexEnd)}`;
        }
      }
      break;
    case "def":
      targetEntry.parameters = sourceEntry.parameters;
      break;
    default:
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
function transformMemberDoc(docStr, context) {
  return transformDoc(docStr, context).replace(/@param \w+.*\n/, "");
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
        if (e.doc) {
          if (!e.since) e.since = resolveVersionDoc(e.doc, context);
          e.doc = resolveDocRefs(e.doc, context);
        }
      });
    } else {
      if (entry.doc) {
        if (!entry.since) entry.since = resolveVersionDoc(entry.doc, context);
        entry.doc = resolveDocRefs(entry.doc, context);
      }
      if (entry.entries) transformEntriesDocRefs(entry.entries, context);
    }
  }
}

/** 
 * @param {string} doc  
 * @param {ApiContext} context 
 */
function resolveVersionDoc(doc, context) {
  const m = /^@since\s*.*\b(\w+)\s*(\d+)\.(\d+)\.(\d+)\.$/m.exec(doc);
  if (!m) return undefined;
  /** @type {VersionTag} */
  const version = {
    tag: m[1].toUpperCase(),
    major: +m[2],
    minor: +m[3],
    patch: +m[4],
  };
  if (context.isAfterMinVersion(version)) return version;
}

/**
 * 
 * @param {string}     doc 
 * @param {ApiContext} context 
 */
function resolveDocRefs(doc, context) {
  if (!doc) return "";
  return doc.replaceAll(context.referenceCandidate, ref => context.getName(ref));
}

exports.transformApi = transformApi;

