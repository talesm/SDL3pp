const { generateCallParameters } = require("./generate");
const { insertEntry } = require("./parse");
const { system, combineObject, looksLikeFreeFunction, deepClone } = require("./utils");

/**
 * @import { Api, ApiEntries, ApiEntry, ApiEntryKind, ApiEntryTransform, ApiEnumeration, ApiFile, ApiParameters, ApiResource, ApiSubEntryTransformLegacyMap, ApiTransform, Dict, ApiFileTransform, ReplacementRule, StringMap, ApiParameter, ApiType, VersionTag, ApiEntryBase, EntryHint, ApiEntryTransformMap, EnumerationDefinition, QuickTransform, WrapperDefinition } from "./types"
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
  const transform = config.transform ?? { namespace: "SDL" };
  const sourceIncludePrefix = transform.sourceIncludePrefix ?? '';

  /** @type {ApiContext} */
  const context = new ApiContext(transform, source);

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
      doc: fileConfig.doc ?? transformFileDoc(sourceFile.doc, context) ?? "",
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
  /** 
   * @param {ApiTransform}  transform  
   * @param {Api}           source 
   **/
  constructor(transform, source) {
    this.namespace = transform.namespace;

    /** @type {Dict<ApiEntry>} */
    this.source = Object.fromEntries(Object.values(source.files)
      .flatMap(f => Object.entries(/** @type {Dict<ApiEntry>} */(f.entries) ?? {})));

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

    this.signatureRules = transform.signatureRules ?? [];

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
  expandNamespaces(sourceEntries, file, context);

  const transformMap = file.transform ?? {};

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (context.blacklist.has(sourceName) || Array.isArray(sourceEntry)) continue;
    if (!isType(sourceEntry.kind)) continue;
    const targetDelta = getOrCreateDelta(sourceName);
    const targetName = targetDelta.name;
    tryDetectOoLike(sourceEntry, targetDelta);
    if (sourceEntry.kind === "callback") expandCallback(sourceName, sourceEntry, targetName);
    if (targetDelta.resource) expandResource(sourceName, sourceEntry, targetName, targetDelta);
    if (targetDelta.wrapper) expandWrapper(sourceName, sourceEntry, targetName, targetDelta);
    if (targetDelta.enum || sourceEntry.kind === "enum")
      expandEnumeration(sourceName, sourceEntry, targetName, targetDelta);

    if (!targetDelta.kind) {
      targetDelta.kind = "alias";
      if (!targetDelta.type) targetDelta.type = sourceName;
    }
    if (targetName === sourceName) {
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

  /**
   * 
   * @param {ApiEntry}          sourceEntry 
   * @param {ApiEntryTransform} targetDelta 
   */
  function tryDetectOoLike(sourceEntry, targetDelta) {
    const sourceName = sourceEntry.name;
    if (typeof targetDelta.resource !== "undefined" || typeof targetDelta.wrapper !== "undefined") return;

    let fCount = 0;
    let free = '';
    const ctors = [];

    for (const functionEntry of Object.values(sourceEntries)) {
      if (Array.isArray(functionEntry) || functionEntry.kind !== "function") continue;

      const param0 = functionEntry.parameters?.[0];
      if (typeof param0 !== "object") continue;
      if (param0.type.startsWith(sourceName) || param0.type.startsWith(`const ${sourceName}`)) {
        fCount++;
        if (functionEntry.name.match(/Close|Destroy|Free/)) free = functionEntry.name;
      }

      const resultType = functionEntry.type;
      if (resultType.startsWith(sourceName)) {
        if (functionEntry.name.match(/Create|Open|Define/)) ctors.push(functionEntry.name);
      }
    }

    if (ctors.length && !targetDelta.entries) {
      targetDelta.entries = {};
      ctors.forEach(n => targetDelta.entries[n] = "ctor");
    }

    if (free) {
      targetDelta.resource = { free };
    } else if (fCount > 5 || targetDelta.entries) {
      targetDelta.wrapper = true;
    }
  }

  function getOrCreateDelta(sourceName) {
    const targetDelta = transformMap[sourceName];
    const name = transformName(sourceName, context);
    if (!targetDelta) return transformMap[sourceName] = { name };
    if (!targetDelta.name) targetDelta.name = name;
    return targetDelta;
  }

  /**
   * 
   * @param {string}    sourceName 
   * @param {ApiEntry}  sourceEntry 
   * @param {string}    name 
   */
  function expandCallback(sourceName, sourceEntry, name) {
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

  /**
   * 
   * @param {string}            sourceType 
   * @param {ApiEntry}          sourceEntry 
   * @param {string}            targetType 
   * @param {ApiEntryTransform} transform
   */
  function expandWrapper(sourceType, sourceEntry, targetType, transform) {
    const wrapper = transform.wrapper === true ? {} : transform.wrapper;
    if (!wrapper) return;
    const isStruct = sourceEntry.kind === "struct" && !transform.type;
    if (!transform.kind) transform.kind = "struct";

    const type = isStruct || !sourceEntry.type?.startsWith("struct ") ? sourceType : sourceType + " *";
    const constexpr = transform.constexpr !== false;
    const paramName = wrapper.attribute ?? (targetType[0].toLowerCase() + targetType.slice(1));
    const rawType = wrapper.rawName || `${targetType}Raw`;
    const paramType = wrapper.paramType ?? (isStruct ? `${rawType} *` : targetType);
    const constParamType = wrapper.paramType ?? (isStruct ? `const ${rawType} &` : rawType);
    const attribute = "m_" + paramName;
    context.includeBefore({
      name: rawType,
      kind: 'alias',
      type: (sourceEntry.type && sourceEntry.type == `struct ${sourceType}`) ? `${sourceType} *` : sourceType,
      doc: `Alias to raw representation for ${targetType}.`,
    }, '__begin');

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
        type: rawType,
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
        type: constParamType,
        name: paramName,
        default: wrapper.defaultValue ?? "{}"
      }],
      doc: `Wraps ${sourceType}.\n\n@param ${paramName} the value to be wrapped`,
      hints: {
        init: [`${isStruct ? rawType : attribute}(${paramName})`],
        changeAccess: isStruct ? undefined : 'public',
      }
    });
    if (wrapper.ordered) {
      insertEntry(entries, [{
        kind: "function",
        name: "operator==",
        type: "bool",
        constexpr,
        immutable: true,
        parameters: [{
          type: constParamType,
          name: paramName,
        }],
        doc: "Default comparison operator",
        hints: { body: `return ${attribute} == ${paramName};` }
      }, {
        kind: "function",
        name: "operator<=>",
        type: "auto",
        constexpr,
        immutable: true,
        parameters: [{
          type: constParamType,
          name: paramName,
        }],
        doc: "Default comparison operator",
        hints: { body: `return ${attribute} <=> ${paramName};` }
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
          type: constParamType,
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
          type: constParamType,
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
        hints: { body: `return *this == (${constParamType})(other);` },
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
      name: `operator ${rawType}`,
      type: "",
      constexpr,
      immutable: true,
      parameters: [],
      doc: `Unwraps to the underlying ${sourceType}.\n\n@returns the underlying ${rawType}.`,
      hints: { body: `return ${attribute};` }
    });
    if (wrapper.invalidState !== false && isStruct) insertEntry(entries, {
      kind: "function",
      name: "operator bool",
      type: "",
      constexpr,
      explicit: true,
      immutable: true,
      parameters: [],
      doc: `Check if valid.\n\n@returns True if valid state, false otherwise.`,
      hints: { body: isStruct ? `return *this != ${rawType}{};` : `return ${attribute} != 0;` }
    });

    if (isStruct) {
      transform.type = rawType;
      transform.hints.super = rawType;

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
          hints: { init: [`${rawType}{${parameters.map(p => p.name).join(", ")}}`] },
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
    const detectedMethods = detectMethods(sourceType, targetType, rawType, `const ${rawType}`, blockedNames);
    mirrorMethods(sourceEntries, file.transform, transform.entries ?? {}, paramType, constParamType, targetType);
    transform.entries = { ...entries, ...(transform.entries ?? {}), ...detectedMethods };
    if (type !== sourceType) {
      context.addParamType(type, targetType);
      context.addReturnType(type, targetType);
    }

    delete transform.wrapper;
  }

  /**
   * 
   * @param {string}            sourceName 
   * @param {ApiEntry}          sourceEntry 
   * @param {string}            targetName 
   * @param {ApiEntryTransform} targetEntry
   */
  function expandResource(sourceName, sourceEntry, targetName, targetEntry) {
    const resourceEntry = getResourceDefinition(targetEntry) ?? {};

    const rawName = resourceEntry.rawName || `${targetName}Raw`;
    const constRawName = `const ${rawName}`;
    const paramType = `${targetName}Param`;
    const constParamType = resourceEntry.enableConstParam ? `${targetName}ConstParam` : paramType;
    if (!targetEntry.kind) targetEntry.kind = 'struct';
    const hasShared = !!resourceEntry.shared;
    const hasScoped = resourceEntry.owning === false;
    const hasRef = resourceEntry.ref ?? !(hasShared || hasScoped);
    const refName = `${targetName}Ref`;
    const scopedName = `${targetName}Scoped`;

    const type = targetEntry.type ?? sourceName;
    const isStruct = sourceEntry.kind === "struct" || (sourceEntry.kind === "alias" && sourceEntry.type.startsWith('struct '));
    const pointerType = (isStruct ? `${type} *` : type);
    const constPointerType = `const ${pointerType}`;
    const nullValue = isStruct ? 'nullptr' : '0';
    const title = targetName[0].toLowerCase() + targetName.slice(1);
    if (sourceEntry.type && !targetEntry.type) targetEntry.type = '';
    context.addName(sourceName, targetName);
    /** @type {ApiEntryTransform[]} */
    const referenceAliases = [];
    referenceAliases.push({ name: targetName, kind: "forward" });
    referenceAliases.push({
      name: rawName,
      kind: "alias",
      type: pointerType,
      doc: `Alias to raw representation for ${targetName}.`,
    });
    if (hasRef) referenceAliases.push({ name: refName, kind: "forward" });
    if (hasScoped) referenceAliases.push({ name: scopedName, kind: "forward" });

    referenceAliases.push({
      name: paramType,
      kind: 'struct',
      doc: `Safely wrap ${targetName} for non owning parameters`,
      entries: {
        'value': {
          kind: 'var',
          name: 'value',
          doc: `parameter's ${rawName}`,
          type: rawName
        },
        [paramType]: [{
          kind: 'function',
          name: paramType,
          doc: `Constructs from ${rawName}`,
          constexpr: true,
          type: '',
          parameters: [{ type: rawName, name: 'value' }],
          hints: { init: ['value(value)'] }
        }, {
          kind: 'function',
          name: paramType,
          doc: `Constructs null/invalid`,
          constexpr: true,
          type: '',
          parameters: [{ type: "std::nullptr_t", name: "_", default: "nullptr" }],
          hints: { init: [`value(${nullValue})`] }
        }],
        "operator bool": {
          kind: "function",
          type: '',
          immutable: true,
          constexpr: true,
          explicit: true,
          parameters: [],
          hints: { body: "return !!value;" },
          doc: `Converts to bool`,
        },
        "operator <=>": {
          kind: "function",
          type: 'auto',
          immutable: true,
          constexpr: true,
          parameters: [{ type: `const ${paramType} &`, name: "other" }],
          hints: { default: true },
          doc: `Comparison`,
        },
        [`operator ${rawName}`]: {
          kind: 'function',
          name: `operator ${rawName}`,
          doc: `Converts to underlying ${rawName}`,
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
        doc: `Safely wrap ${targetName} for non owning const parameters`,
        entries: {
          'value': {
            kind: 'var',
            name: 'value',
            doc: `parameter's ${constRawName}`,
            type: constRawName
          },
          [constParamType]: [{
            kind: 'function',
            name: constParamType,
            doc: `Constructs from ${constRawName}`,
            constexpr: true,
            type: '',
            parameters: [{ type: constRawName, name: 'value' }],
            hints: { init: ['value(value)'] }
          }, {
            kind: 'function',
            name: constParamType,
            doc: `Constructs from ${paramType}`,
            constexpr: true,
            type: '',
            parameters: [{ type: paramType, name: 'value' }],
            hints: { init: ['value(value.value)'] }
          }, {
            kind: 'function',
            name: constParamType,
            doc: `Constructs null/invalid`,
            constexpr: true,
            type: '',
            parameters: [{ type: "std::nullptr_t", name: "_", default: "nullptr" }],
            hints: { init: [`value(${nullValue})`] }
          }],
          "operator bool": {
            kind: "function",
            type: '',
            immutable: true,
            constexpr: true,
            explicit: true,
            parameters: [],
            hints: { body: "return !!value;" },
            doc: `Converts to bool`,
          },
          "operator <=>": {
            kind: "function",
            type: 'auto',
            immutable: true,
            constexpr: true,
            parameters: [{ type: `const ${constParamType} &`, name: "other" }],
            hints: { default: true },
            doc: `Comparison`,
          },
          [`operator ${constRawName}`]: {
            kind: 'function',
            name: `operator ${constRawName}`,
            doc: `Converts to underlying ${constRawName}`,
            constexpr: true,
            immutable: true,
            type: '',
            parameters: [],
            hints: { body: 'return value;' }
          }
        },
      });
    }
    if (hasScoped) {
      context.addParamType(pointerType, targetName);
      context.addParamType(constPointerType, `const ${targetName} &`);
    } else {
      context.addParamType(pointerType, paramType);
      context.addParamType(constPointerType, resourceEntry.enableConstParam ? constParamType : paramType);
    }

    if (hasRef) {
      context.addReturnType(pointerType, refName);
    } else if (hasShared || hasScoped) {
      context.addReturnType(pointerType, targetName);
    } else {
      context.addReturnType(pointerType, rawName);
    }
    context.addReturnType(constPointerType, constRawName);

    const ownershipDisclaimer = hasScoped ? "" : "\n\nThis assumes the ownership, call release() if you need to take back.";

    /** @type {ApiEntryTransformMap} */
    const ctors = {
      [targetName]: [{
        kind: "function",
        type: "",
        constexpr: true,
        parameters: [],
        hints: { default: true, changeAccess: "public" },
        doc: "Default ctor"
      }, {
        kind: "function",
        type: "",
        constexpr: true,
        explicit: !hasScoped,
        parameters: [{ name: "resource", type: constRawName }],
        hints: { init: ["m_resource(resource)"] },
        doc: `Constructs from ${paramType}.\n\n@param resource a ${rawName} to be wrapped.${ownershipDisclaimer}`,
      }, {
        kind: "function",
        type: "",
        constexpr: true,
        parameters: [{ name: "other", type: `const ${targetName} &` }],
        hints: { delete: true },
        doc: "Copy constructor"
      }, {
        kind: "function",
        type: "",
        constexpr: true,
        parameters: [{ name: "other", type: `${targetName} &&` }],
        hints: {
          init: [`${targetName}(other.release())`]
        },
        doc: "Move constructor"
      }]
    };
    if (hasShared) {
      /** @type {EntryHint} */
      const copyCtorHints = {};
      ctors[targetName][2].hints = copyCtorHints;
      if (resourceEntry.shared !== true) {
        copyCtorHints.init = ["m_resource(other.m_resource)"];
        copyCtorHints.body = `++m_resource->${resourceEntry.shared};`;
        ctors["Borrow"] = {
          kind: "function",
          static: true,
          constexpr: true,
          type: targetName,
          parameters: [{ name: "resource", type: paramType }],
          hints: { body: `if (resource) {\n  ++resource.value->${resourceEntry.shared};\n  return ${targetName}(resource.value);}\nreturn {};` },
          doc: `Safely borrows the from ${paramType}.\n\n@param resource a ${rawName} or ${targetName}.\n\nThis does not takes ownership!`
        };
      }
    } else if (hasScoped) {
      ctors[targetName][2].hints = { default: true };
    }
    if (hasRef) {
      // @ts-ignore
      insertEntry(ctors, [{
        kind: "function",
        doc: "",
        type: "",
        constexpr: true,
        parameters: [{ name: "other", type: `const ${refName} &` }],
        hints: { delete: true },
      }, {
        kind: "function",
        doc: "",
        type: "",
        constexpr: true,
        parameters: [{ name: "other", type: `${refName} &&` }],
        hints: { delete: true },
      }], targetName);
    }
    const subEntries = targetEntry.entries || {};

    let extraUniqueCtors = subEntries[targetName];
    if (extraUniqueCtors) {
      delete subEntries[targetName];
      if (typeof extraUniqueCtors === "string") {
        extraUniqueCtors = [{ name: targetName, kind: "function", parameters: [] }];
      } else if (!Array.isArray(extraUniqueCtors)) {
        extraUniqueCtors = [extraUniqueCtors];
      }
      for (const entry of extraUniqueCtors) {
        entry.kind = "function";
        entry.type = "";
        // @ts-ignore
        insertEntry(ctors, entry, targetName);
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
          e.type = targetName;
          if (!e.name) e.name = transformMemberName(sourceName, targetName, context);
          if (!e.sourceName && sourceEntries[sourceName]) e.sourceName = sourceName;
          addHints(e, { wrapSelf: true });
        });
      } else {
        entry.static = true;
        entry.type = targetName;
        if (!entry.name) entry.name = transformMemberName(sourceName, targetName, context);
        if (!entry.sourceName && sourceEntries[sourceName]) entry.sourceName = sourceName;
        addHints(entry, { wrapSelf: true });
      }
      ctors[sourceName] = entry;
      if (!ctorTransform) {
        file.transform[sourceName] = { type: targetName, hints: { wrapSelf: true } };
      } else if (!ctorTransform.type) {
        file.transform[sourceName].type = targetName;
        addHints(ctorTransform, { wrapSelf: true });
      }
    }

    for (const [sourceName, entry] of Object.entries(subEntries)) {
      const ctorTransform = file.transform[sourceName];
      let isCtor = false;
      if (typeof entry === "string") {
        if (entry === "ctor") {
          ctors[sourceName] = {
            kind: "function",
            type: "",
            name: targetName,
            sourceName,
          };
          isCtor = true;
        }
      } else if (!Array.isArray(entry) && entry.name === "ctor") {
        entry.kind = "function";
        entry.type = "";
        entry.name = targetName;
        if (!entry.sourceName && sourceEntries[sourceName]) entry.sourceName = sourceName;
        ctors[sourceName] = entry;
        isCtor = true;
      }
      if (isCtor) {
        delete subEntries[sourceName];
        if (!ctorTransform) {
          file.transform[sourceName] = { type: targetName, hints: { wrapSelf: true } };
        } else if (!ctorTransform.type) {
          ctorTransform.type = targetName;
          addHints(ctorTransform, { wrapSelf: true });
        }
      }
    }

    let freeFunction = /** @type {ApiEntry} */(sourceEntries[resourceEntry.free ?? "reset"]) ?? scanFreeFunction(sourceEntries, targetName, pointerType);
    if (freeFunction) {
      const sourceName = freeFunction.name;
      freeFunction = transformEntry(freeFunction, context);
      const freeTransformEntry = /** @type {ApiEntryTransform} */(subEntries[sourceName]) ?? {};
      combineObject(freeFunction, freeTransformEntry);
      freeFunction.name = freeTransformEntry.name ?? makeNaturalName(transformName(sourceName, context), targetName);
      freeFunction.doc = freeFunction.doc ? transformDoc(freeFunction.doc, context) : `frees up ${title}.`;
      const fileLevelEntry = file.transform[sourceName] ?? { parameters: [{ type: rawName }, ...freeFunction.parameters.slice(1)] };
      file.transform[sourceName] = fileLevelEntry;
      const fileLevelName = fileLevelEntry.name ?? transformName(sourceName, context);
      if (!freeFunction.hints?.body) {
        let body = `${fileLevelName}(release());`;
        if (freeFunction.hints?.mayFail && freeFunction.type === "bool") {
          freeFunction.type = "void";
        }
        if (freeFunction.type !== "void") body = `return ${body}`;
        combineHints(freeFunction, { body });
      }
      subEntries[sourceName] = freeFunction;
    } else {
      freeFunction = {
        kind: "function",
        name: "Destroy",
        type: "void",
        parameters: [],
      };
    }
    if (resourceEntry.enableAutoMethods !== false) {
      const blockedNames = new Set(Object.keys(subEntries));
      blockedNames.add(targetName);
      Object.keys(ctors).forEach(k => blockedNames.add(k));
      const detectedMethods = detectMethods(sourceName, targetName, paramType, constParamType, blockedNames);
      mirrorMethods(sourceEntries, file.transform, subEntries, paramType, constParamType, targetName);
      combineObject(subEntries, detectedMethods);
    } else {
      mirrorMethods(sourceEntries, file.transform, subEntries, paramType, constParamType, targetName);
    }

    for (const [key, subEntry] of Object.entries(subEntries)) {
      if (Array.isArray(subEntry)) subEntry.forEach(s => checkIfProtoNeeded(key, s));
      else checkIfProtoNeeded(key, subEntry);
    }

    targetEntry.doc = transformDoc(sourceEntry.doc ?? `Wraps ${title} resource.`, context) + `\n\n@cat resource`;
    targetEntry.entries = {
      "m_resource": {
        kind: "var",
        type: rawName,
        hints: { body: nullValue },
      },
      ...ctors,
      [`~${targetName}`]: {
        kind: "function",
        doc: "Destructor",
        type: "",
        parameters: [],
        hints: { body: (freeFunction && !hasScoped) ? `${freeFunction.sourceName ?? freeFunction.name}(m_resource);` : '' }
      },
      'operator=': {
        kind: "function",
        type: `${targetName} &`,
        parameters: [{ name: 'other', type: targetName }],
        hints: { body: "std::swap(m_resource, other.m_resource);\nreturn *this;" },
        doc: "Assignment operator.",
      },
      "get": {
        kind: "function",
        type: rawName,
        immutable: true,
        constexpr: true,
        parameters: [],
        hints: { body: "return m_resource;" },
        doc: `Retrieves underlying ${rawName}.`,
      },
      "release": {
        kind: "function",
        type: rawName,
        constexpr: true,
        parameters: [],
        hints: { body: `auto r = m_resource;\nm_resource = ${nullValue};\nreturn r;` },
        doc: `Retrieves underlying ${rawName} and clear this.`,
      },
      "operator <=>": {
        kind: "function",
        type: 'auto',
        immutable: true,
        constexpr: true,
        parameters: [{ type: `const ${targetName} &`, name: "other" }],
        hints: { default: true },
        doc: `Comparison`,
      },
      "operator ==": {
        kind: "function",
        type: 'bool',
        immutable: true,
        constexpr: true,
        parameters: [{ type: `std::nullptr_t`, name: "_" }],
        hints: { body: "return !m_resource;" },
        doc: `Comparison`,
      },
      "operator bool": {
        kind: "function",
        type: '',
        immutable: true,
        constexpr: true,
        explicit: true,
        parameters: [],
        hints: { body: "return !!m_resource;" },
        doc: `Converts to bool`,
      },
      [`operator ${paramType}`]: {
        kind: "function",
        type: '',
        immutable: true,
        constexpr: true,
        parameters: [],
        hints: { body: "return {m_resource};" },
        doc: `Converts to ${paramType}`,
      },
      [freeFunction.name]: "plc",
      ...subEntries,
    };
    addHints(targetEntry, {
      self: "m_resource",
      super: "m_resource",
      private: true,
    });

    /** @type {ApiEntryTransform[]} */
    const derivedEntries = [];

    if (hasRef) derivedEntries.push({
      kind: 'struct',
      name: refName,
      type: targetName,
      doc: `Semi-safe reference for ${targetName}.`,
      entries: {
        [refName]: [{
          kind: 'function',
          type: "",
          parameters: [{
            type: paramType,
            name: "resource"
          }],
          hints: { init: [`${targetName}(resource.value)`] },
          doc: `Constructs from ${paramType}.\n\n@param resource a ${rawName} or ${targetName}.\n\nThis does not takes ownership!`
        }, {
          kind: 'function',
          type: "",
          parameters: [{
            type: `const ${refName} &`,
            name: "other"
          }],
          hints: { init: [`${targetName}(other.get())`] },
          doc: "Copy constructor.",
        }],
        [`~${refName}`]: {
          kind: 'function',
          doc: "Destructor",
          type: "",
          parameters: [],
          hints: { body: "release();" }
        }
      }
    });
    if (hasScoped) derivedEntries.push({
      kind: 'struct',
      name: scopedName,
      type: targetName,
      doc: `RAII owning version ${targetName}.`,
      entries: {
        [`${targetName}::${targetName}`]: "alias",
        [scopedName]: [{
          kind: "function",
          type: "",
          constexpr: true,
          parameters: [{ name: "other", type: `const ${targetName} &` }],
          hints: { delete: true },
        }, {
          kind: "function",
          doc: "Move constructor",
          type: "",
          constexpr: true,
          parameters: [{ name: "other", type: `${targetName} &&` }],
          hints: {
            init: [`${targetName}(other.release())`]
          }
        }],
        [`~${scopedName}`]: {
          kind: 'function',
          doc: "Destructor",
          type: "",
          parameters: [],
          hints: { body: `${freeFunction?.name ?? "Destroy"}();` }
        }
      }
    });

    context.includeBefore(referenceAliases, '__begin');
    context.includeAfter(derivedEntries, targetName);
    delete targetEntry.resource;

    /**
     * 
     * @param {string} key 
     * @param {ApiEntryTransform|QuickTransform} subEntry 
     */
    function checkIfProtoNeeded(key, subEntry) {
      const source = /** @type {ApiEntry}*/(sourceEntries[key]);
      if (typeof subEntry !== 'string' && typeof subEntry.type !== "undefined") {
        if (subEntry.kind !== 'function' && source.kind !== 'function') return;
        if (protoNeeded(subEntry.type)) {
          subEntry.proto = true;
          return;
        }
      } else if (source) {
        if (source.kind !== 'function') return;
        const type = context.returnTypeMap[source.type];
        if (!protoNeeded(type)) return;
        if (subEntry === "function") {
          subEntries[key] = { proto: true };
        } else if (subEntry === "immutable") {
          subEntries[key] = { proto: true, immutable: true };
        } else if (subEntry === "ctor") {
          subEntries[key] = { proto: true, name: "ctor" };
        } else if (typeof subEntry === 'string') {
          return;
        } else {
          subEntry.proto = true;
        }
      }
      function protoNeeded(type) {
        return type === scopedName || type === refName;
      }
    }
  }


  /**
   * 
   * @param {string}                  sourceType 
   * @param {string}                  targetType 
   * @param {string}                  paramType 
   * @param {string}                  constParamType 
   * @param {Set<string>}             blockedNames 
   */
  function detectMethods(sourceType, targetType, paramType, constParamType, blockedNames) {
    /** @type {ApiSubEntryTransformLegacyMap} */
    const foundEntries = {};
    for (let [sourceName, transformEntry] of Object.entries(transformMap)) {
      if (blockedNames.has(sourceName) || foundEntries[sourceName]) continue;
      const sourceEntry = /** @type {ApiEntryTransform}*/(sourceEntries[sourceName]);
      /** @type {ApiEntryTransform[]} */
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
      const m = paramMatchesVariants(param0, [paramType, `${paramType} *`], [constParamType, `${constParamType} &`]);
      if (!m) {
        blockedNames.add(sourceName);
        continue;
      }
      if (sourceEntry) {
        foundEntries[sourceName] = {
          ...transformEntry,
          immutable: m === 'immutable'
        };
      } else {
        const name = transformMemberName(transformEntry.hints?.methodName ?? transformEntry.name ?? sourceName, targetType, context);
        if (blockedNames.has(name)) continue;
        const key = `${targetType}::${transformMemberName(sourceName, targetType, context)}`;
        transformMap[key] = {
          ...transformEntry,
          after: sourceName,
          name: `${targetType}::${name}`,
          static: false,
          parameters: parameters.slice(1),
          hints: { delegate: `${context.namespace}::${sourceName}` }
        };
      }
    }
    for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
      if (blockedNames.has(sourceName) || foundEntries[sourceName]) continue;
      if (Array.isArray(sourceEntry)) continue;
      if (sourceEntry.kind !== "function") continue;
      const parameters = sourceEntry.parameters;
      if (!parameters?.length) continue;
      const param0 = parameters[0];
      if (typeof param0 === 'string') continue;
      const m = paramMatchesVariants(param0, [sourceType, `${sourceType} *`], [`const ${sourceType}`, `const ${sourceType} *`]);
      if (!m) continue;
      const transformEntry = /** @type {ApiEntryTransform}*/(transformMap[sourceName]);
      if (transformEntry?.name || transformEntry?.parameters || transformEntry?.hints?.methodName) {
        /** @type {ApiEntryTransform} */
        const e = {
          ...deepClone(transformEntry),
          immutable: m === 'immutable' || undefined,
        };
        e.name = transformEntry.hints?.methodName ?? undefined;
        foundEntries[sourceName] = e;
      } else {
        foundEntries[sourceName] = m;
      }
      if (!transformEntry) transformMap[sourceName] = {};
    }
    return foundEntries;
  }

  /**
   * 
   * @param {string}                sourceName 
   * @param {ApiEntry}              sourceEntry 
   * @param {string}                targetName 
   * @param {ApiEntryTransform}     transform
   */
  function expandEnumeration(sourceName, sourceEntry, targetName, transform) {
    const definition = getEnumDefinition(transform) ?? {};
    const valueType = definition.valueType ?? targetName;
    if (!transform.kind && !transform.type) {
      if (sourceEntry.kind === 'alias') {
        transform.type = sourceEntry.type;
      } else {
        transform.kind = "alias";
        transform.type = sourceName;
      }
    }

    let values = definition.values ?? Object.keys(sourceEntry.entries ?? {});
    const prefix = definition.prefix ?? (sourceName.toUpperCase() + "_");
    /** @type {StringMap} */
    const newNames = {};
    if (!values?.length) {
      values = Object.values(sourceEntries)
        .filter(e => !Array.isArray(e)
          && e.kind === "def"
          && !e.parameters
          && e.name.startsWith(prefix)
          && !transformMap[e.name]?.type
        )
        .map(e => /** @type {ApiEntry}*/(e).name);
      const newPrefix = definition.newPrefix;
      if (newPrefix) {
        const oldPrefixLen = prefix.length;
        values.forEach(n => newNames[n] = newPrefix + n.slice(oldPrefixLen));
      }
    }
    const after = transform.after;
    if (after) context.includeAfter(targetName, after);

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
        after: targetName,
      };
      combineObject(valueTarget, valueTransform || {});
      if (typeof valueTarget.doc !== "string") {
        // @ts-ignore
        const sourceDoc = valueSource?.doc ?? sourceEntry.entries?.[value]?.doc;
        valueTarget.doc = sourceDoc || (value.startsWith(prefix) ? value.slice(prefix.length) : valueTarget.name);
      }
      context.addName(value, valueTarget.name);
      if (!valueSource) valueTarget.sourceName = value;
      file.transform[value] = valueTarget;
    }
    delete transform.enum;
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

  const sortedEntries = makeSortedEntryArray(sourceEntries, file, context);
  for (const targetEntry of sortedEntries) {
    const sourceName = targetEntry.sourceName;
    if (sourceName) context.addGlossary(sourceName, targetEntry);
    const targetDelta = transformMap[sourceName];
    if (targetDelta) {
      for (let link = targetDelta.link; link;) {
        const linkedEntry = deepClone(targetEntry);
        const nextLink = link.link;
        delete link.link;
        if (link.kind || targetEntry.kind !== 'def') {
          combineObject(linkedEntry, link);
          if (linkedEntry.parameters) {
            linkedEntry.parameters = linkedEntry.parameters.filter(p => typeof p === 'string' || (p.type && p.name));
          }
          insertEntryAndCheck(targetEntries, linkedEntry, context, file);
        }
        link = nextLink;
      }
      delete targetDelta.link;
      combineObject(targetEntry, targetDelta);
      if (targetEntry.parameters) {
        targetEntry.parameters = targetEntry.parameters.filter(p => typeof p === 'string' || (p.type && p.name));
      }
    }
    if (sourceName) context.addName(sourceName, targetEntry.name?.replaceAll('::', '.'));
    insertEntryAndCheck(targetEntries, targetEntry, context, file);
  }

  return targetEntries;
}

/**
 * 
 * @param {ApiEntries}    sourceEntries 
 * @param {ApiFileTransform} file,
 * @param {ApiContext}    context 
 */
function makeSortedEntryArray(sourceEntries, file, context) {
  const transformEntries = file.transform ?? {};

  let lastSourceName = "__begin";
  for (const [sourceName, transformEntry] of Object.entries(transformEntries)) {
    if (sourceEntries[sourceName]) {
      lastSourceName = sourceName;
      const targetName = transformEntry.name ?? transformName(sourceName, context);
      transformEntry.name = targetName;
      if (transformEntry.before) {
        context.includeBefore(targetName, transformEntry.before);
      } else if (transformEntry.after) {
        context.includeAfter(targetName, transformEntry.after);
      }
    } else {
      if (!transformEntry.name) transformEntry.name = sourceName;
      if (transformEntry.before) {
        lastSourceName = transformEntry.before;
        context.includeBefore(transformEntry, lastSourceName);
      } else {
        lastSourceName = transformEntry.after ?? lastSourceName;
        context.includeAfter(transformEntry, lastSourceName);
      }
    }
    delete transformEntry.before;
    delete transformEntry.after;
  }

  /** @type {Dict<ApiEntry>} */
  const sortedEntries = {};
  /** @type {Dict<number>} */
  const countInstance = {};

  const defPrefix = file.definitionPrefix;
  const includeBefore = context.currentIncludeBefore;
  const includeAfter = context.currentIncludeAfter;
  const processedSourceNames = new Set();

  if (includeBefore.__begin) addIncluded(includeBefore.__begin);
  if (includeAfter.__begin) addIncluded(includeAfter.__begin);

  for (const sourceEntry of Object.values(sourceEntries)) {
    if (Array.isArray(sourceEntry)) continue;
    const sourceName = sourceEntry.name;
    if (processedSourceNames.has(sourceName)) continue;
    processedSourceNames.add(sourceName);

    const targetEntry = {
      ...transformEntry(sourceEntry, context),
      name: transformName(sourceName, context)
    };
    const targetDelta = transformEntries[sourceName];
    if (targetDelta) {
      targetEntry.name = targetDelta.name ?? targetEntry.name;
      targetEntry.kind = targetDelta.kind ?? targetEntry.kind;
    } else if (context.blacklist.has(sourceName)) {
      continue;
    }
    const targetName = targetEntry.name;
    const firstAppearance = !processedSourceNames.has(targetName);
    processedSourceNames.add(targetName);

    addIncluded(includeBefore[sourceName]);
    if (firstAppearance) addIncluded(includeBefore[targetName]);

    if (targetEntry.kind === "def") {
      const targetName = targetEntry.name;
      if (!targetName.startsWith(defPrefix)) {
        targetEntry.name = defPrefix + targetName;
      }
    }
    addTransform(targetEntry);
    if (targetDelta?.entries) targetDelta.entries = transformSubEntries(targetDelta);

    if (firstAppearance) addIncluded(includeAfter[targetName]);
    addIncluded(includeAfter[sourceName]);
  }

  if (includeBefore.__end) addIncluded(includeBefore.__end);
  if (includeAfter.__end) addIncluded(includeAfter.__end);

  return Object.values(sortedEntries);

  /**
   * 
   * @param {ApiEntryTransform[]=} transformEntries 
   */
  function addIncluded(transformEntries) {
    if (!transformEntries) return;
    for (let i = 0; i < transformEntries.length; i++) {
      const transformEntry = transformEntries[i];
      const name = transformEntry.name;
      let checkSubIncludes = true;
      if (processedSourceNames.has(name)) {
        checkSubIncludes = false;
        if (transformEntry.kind !== 'function') continue;
      } else if (transformEntry.kind === 'forward') {
        checkSubIncludes = false;
      } else processedSourceNames.add(name);


      if (checkSubIncludes) addIncluded(includeBefore[name]);
      addTransform(/** @type {ApiEntry}*/(transformEntry));
      if (checkSubIncludes) addIncluded(includeAfter[name]);
    }
  }

  /**
   * 
   * @param {ApiEntry} transformEntry 
   */
  function addTransform(transformEntry) {
    const currEntry = sortedEntries[transformEntry.name];
    const currKind = currEntry?.kind;
    const nextName = transformEntry.kind === 'forward' ? transformEntry.name + '#forward' : transformEntry.name;
    if (transformEntry.entries) transformEntry.entries = transformSubEntries(transformEntry);
    if (!currKind) {
      sortedEntries[nextName] = transformEntry;
      return;
    }
    const nextKind = transformEntry.kind;
    if (currKind === 'function') {
      if (nextKind !== 'function') return;
      const n = (countInstance[nextName] ?? 1) + 1;
      countInstance[nextName] = n;
      sortedEntries[`${nextName}#${n}`] = transformEntry;
      return;
    }
    combineObject(currEntry, transformEntry);
  }

  /**
   * @param {ApiEntryTransform} targetEntry the entry we are inserting from
   */
  function transformSubEntries(targetEntry) {
    /** @type {ApiEntries} */
    const entries = {};
    const type = targetEntry.name;
    for (const [key, entry] of Object.entries(targetEntry.entries)) {
      if (Array.isArray(entry)) {
        insertEntry(entries, /** @type {ApiEntry[]}*/(entry), key);
        continue;
      }
      const sourceEntry = context.source[key];
      const nameCandidate = transformName(key, context);
      if (!sourceEntry) {
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
      if (context.blacklist.has(key)) addTransform(nameChange);
      if (!entries[nameChange.name]) {
        insertEntry(entries, { name: nameChange.name, kind: "plc" });
      }
      const currLink = file.transform[key];
      const currLinkName = currLink?.name ?? transformName(key, context);
      let isCtor = entry === "ctor" || (typeof entry !== "string" && !entry.name?.startsWith("operator") && entry.type === "");
      if (isCtor || (typeof entry !== "string" && entry.proto === false)) {
        nameChange.name = `${type}.${nameChange.name}`;
      } else {
        nameChange.name = `${type}::${nameChange.name}`;
        addHints(nameChange, { delegate: `${context.namespace}::${currLinkName}` });
      }
      if (typeof entry !== "string") delete nameChange.proto;
      file.transform[key] = nameChange;
      if (currLink) {
        currLink.name = currLinkName;
        if (isCtor) addHints(currLink, { delegate: type });
        file.transform[key].link = currLink;
      }
    }
    return entries;
  }
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
 * @param {ApiEntries}            sourceEntries 
 * @param {Dict<ApiEntryTransform>}  transformEntries 
 * @param {Dict<ApiEntryTransform | ApiEntryBase[] | QuickTransform>} transformSubEntries 
 * @param {string} paramType 
 * @param {string} constParamType 
 * @param {string} resultType 
 */
function mirrorMethods(sourceEntries, transformEntries, transformSubEntries, paramType, constParamType, resultType) {
  for (const [sourceName, subEntry] of Object.entries(transformSubEntries)) {
    if (Array.isArray(subEntry)) continue;

    const sourceEntry = sourceEntries[sourceName];
    if (!sourceEntry || Array.isArray(sourceEntry)) continue;
    if (sourceEntry.kind !== 'function' && typeof subEntry !== 'string' && subEntry.kind !== 'function') continue;

    const transformEntry = transformEntries[sourceName];
    if (transformEntry?.type || transformEntry?.parameters) continue;
    /** @type {ApiEntryTransform} */
    let targetEntry;
    if (typeof subEntry === 'string') {
      targetEntry = {};
      if (subEntry === 'immutable') targetEntry.immutable = true;
    } else {
      targetEntry = deepClone(subEntry);
      if (targetEntry.type === "") targetEntry.type = resultType;
      if (subEntry.name === "ctor") targetEntry.static = true;
    }
    mirrorParameters(sourceEntry, targetEntry);
    delete targetEntry.name;
    delete targetEntry.static;
    delete targetEntry.immutable;
    delete targetEntry.proto;
    delete targetEntry.hints?.delegate;
    if (transformEntry) combineObject(targetEntry, transformEntry);
    transformEntries[sourceName] = targetEntry;
  }

  /**
   * 
   * @param {ApiEntry}          sourceEntry
   * @param {ApiEntryTransform} targetEntry
   */
  function mirrorParameters(sourceEntry, targetEntry) {
    if (targetEntry.static) return;

    /**@type {ApiParameters} */
    const targetParameters = targetEntry.parameters ?? sourceEntry?.parameters?.map(p => (typeof p === 'string' ? p : {}));
    if (!targetParameters) return;

    const needSaveParameters = !targetEntry.parameters;
    let parametersChanged = false;
    const targetParam0 = targetParameters[0];

    const sourceParam0 = sourceEntry.parameters?.[0];
    if (!targetParam0 || typeof targetParam0 === "string" || targetParam0.type) {
      switch (typeof sourceParam0) {
        case 'object':
          const selfParam = { name: sourceParam0.name };
          if (sourceParam0.type?.includes(resultType)) {
            if (!sourceParam0.type.startsWith("const ") && targetEntry.immutable) selfParam.type = constParamType;
          }
          targetParameters.unshift(selfParam);
          parametersChanged = true;
          break;
        case 'string':
          if (typeof targetParam0 !== "object" || sourceParam0 !== targetParam0.name) {
            targetParameters.unshift({ name: sourceParam0, type: constParamType });
            parametersChanged = true;
          }
          break;
      }
    } else if (typeof sourceParam0 !== 'object' || typeof targetParam0 !== "object") {
      return;
    } else if (!targetParam0.name || targetParam0.name == sourceParam0.name) {
      if (sourceParam0.type?.includes(resultType)) {
        targetParam0.type = (targetEntry.immutable || sourceParam0.type.startsWith('const ')) ? constParamType : paramType;
        parametersChanged = true;
      }
    }
    if (needSaveParameters && parametersChanged) targetEntry.parameters = targetParameters;
  }
}


/**
 * 
 * @param {ApiParameter}  param0 
 * @param {string[]}      variants 
 * @param {string[]}      constVariants 
 */
function paramMatchesVariants(param0, variants, constVariants) {
  for (const variant of variants) {
    if (param0.type === variant) return 'function';
  }
  for (const variant of constVariants) {
    if (param0.type === variant) return 'immutable';
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
 * @param {ApiFileTransform}              file
 * @param {string=}                       defaultName
 */
function insertEntryAndCheck(entries, entry, context, file, defaultName) {
  if (Array.isArray(entry)) {
    entry.forEach(e => insertEntryAndCheck(entries, e, context, file, defaultName));
    return;
  }
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
    const args = generateCallParameters(template, {});
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
  if (!typeName.startsWith("F")) return name.replace(new RegExp(typeName), "");
  const replaceRegexp = new RegExp("F?" + typeName.slice(1));
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
      checkSignatureRules(targetEntry, context);
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
 * @returns {ApiParameters}
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
 * @param {ApiEntry}    targetEntry 
 * @param {ApiContext}  context 
 */
function checkSignatureRules(targetEntry, context) {
  const parameters = /** @type {ApiParameter[]}*/(targetEntry.parameters);
  if (parameters.some(p => typeof p === "string")) return;
  for (const signatureRule of context.signatureRules) {
    if (parameters.length < signatureRule.pattern.length) continue;
    for (let i = 0; i <= parameters.length - signatureRule.pattern.length; i++) {
      let matchParams = true;
      for (let j = 0; j < signatureRule.pattern.length; j++) {
        const param = parameters[i + j];
        const pattern = signatureRule.pattern[j];
        if (param.name !== pattern.name || param.type !== pattern.type) {
          matchParams = false;
          break;
        }
      }
      if (matchParams) {
        const replaceParams = signatureRule.replaceParams.map(p => ({ ...p }));
        parameters.splice(i, signatureRule.pattern.length, ...replaceParams);
        break;
      }
    }
  }
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

