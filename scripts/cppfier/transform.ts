import { generateCallParameters } from "./generate";
import {
  Api,
  ApiEntries,
  ApiEntry,
  ApiEntryBase,
  ApiEntryKind,
  ApiEntryTransform,
  ApiEntryTransformMap,
  ApiFile,
  ApiFileTransform,
  ApiParameter,
  ApiParameters,
  ApiTransform,
  ApiType,
  CallbackDefinition,
  Dict,
  EntryHint,
  FunctorSupport,
  ListContent,
  LockDefinition,
  ParsedDoc,
  ParsedDocContent,
  QuickTransform,
  ReplacementRule,
  SignatureTransform,
  StringMap,
  TaggedContent,
  VersionTag,
} from "./types";
import {
  system,
  combineObject,
  looksLikeFreeFunction,
  deepClone,
} from "./utils";

export interface TransformConfig {
  sourceApi: Api;
  transform: ApiTransform;
}

/**
 * Transform the Api
 *
 * @param config the rules to apply into source
 */
export function transformApi(config: TransformConfig) {
  const source = config.sourceApi;
  const transform = config.transform ?? { namespace: "SDL" };
  const sourceIncludePrefix = transform.sourceIncludePrefix ?? "";

  const context = new ApiContext(transform, source);

  const files: ApiFile[] = [];
  const fileTransformMap = transform.files ?? {};

  // Ensure fileTransformMap is full
  for (const [sourceName, sourceFile] of Object.entries(source.files)) {
    const fileConfig = fileTransformMap[sourceName];
    if (fileConfig) {
      fileConfig.ignoreEntries?.forEach((s) => context.blacklist.add(s));
      if (!fileConfig.transform) fileConfig.transform = {};
      if (!fileConfig.definitionPrefix)
        fileConfig.definitionPrefix = context.definitionPrefix;
      if (!fileConfig.sourceIncludePrefix)
        fileConfig.sourceIncludePrefix = sourceIncludePrefix;
      sourceFile.name =
        fileConfig.name || transformIncludeName(sourceName, context);
    } else {
      sourceFile.name = transformIncludeName(sourceName, context);
      fileTransformMap[sourceName] = {
        transform: {},
        definitionPrefix: context.definitionPrefix,
        sourceIncludePrefix,
      };
    }
  }

  // Step 1: Expand types
  stepExpandTypes(source, context, fileTransformMap);

  // Step 2: Transform Files
  for (const [sourceName, sourceFile] of Object.entries(source.files)) {
    context.setFile(sourceName);
    const fileConfig = fileTransformMap[sourceName];
    context.enableException = fileConfig.enableException !== false;
    const targetName = sourceFile.name;
    system.log(`Transforming api ${sourceName} => ${targetName}`);

    const includes = fileConfig.includes ?? [];
    const qualifiedSourceFile = fileConfig.sourceIncludePrefix + sourceName;
    includes.push(qualifiedSourceFile);

    files.push({
      name: targetName,
      doc: transformFileDoc(sourceFile.doc, context),
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
  const api = {
    files: {},
    paramReplacements: context.paramReplacements,
    delegatedReplacements: context.delegatedReplacements,
  };
  files.forEach((file) => (api.files[file.name] = file));

  return api;
}

function stepExpandTypes(
  source: Api,
  context: ApiContext,
  fileTransformMap: Dict<ApiFileTransform>
) {
  const unorderedEntries = Object.entries(source.files);
  const targetNames = new Set(
    unorderedEntries.map(([sourceName, sourceFile]) => sourceFile.name)
  );
  const processed = new Set<string>();
  while (unorderedEntries.length) {
    const [sourceName, sourceFile] = unorderedEntries.shift();
    if (
      fileTransformMap[sourceName].localIncludes?.some(
        (dep) => targetNames.has(dep) && !processed.has(dep)
      )
    ) {
      unorderedEntries.push([sourceName, sourceFile]);
      continue;
    }
    processed.add(sourceFile.name);
    context.setFile(sourceName);
    const fileConfig = fileTransformMap[sourceName];
    expandTypes(sourceFile.entries, fileConfig, context);
  }
}

function isVersionAfter(version: VersionTag, tag: VersionTag) {
  if (version.major > tag.major) return true;
  if (version.major < tag.major) return false;
  if (version.minor > tag.minor) return true;
  if (version.minor < tag.minor) return false;
  if (version.patch > tag.patch) return true;
  return false;
}

class ApiContext {
  namespace: string;
  source: Dict<ApiEntry>;
  blacklist: Set<string>;
  definitionPrefix: string;
  enableException: boolean;
  paramReplacements: any;
  delegatedReplacements: any;
  minVersions: Dict<VersionTag>;
  baseVersions: Dict<VersionTag>;
  includeBeforeMap: Dict<Dict<ApiEntryTransform[]>>;
  includeAfterMap: Dict<Dict<ApiEntryTransform[]>>;
  file: string;
  currentIncludeBefore: Dict<ApiEntryTransform[]>;
  currentIncludeAfter: Dict<ApiEntryTransform[]>;
  paramTypeMap: StringMap;
  returnTypeMap: StringMap;
  callbackTypeMap: StringMap;
  nameMap: StringMap;
  glossary: Dict<ApiEntry>;
  prefixToRemove: RegExp;
  referenceCandidate: RegExp;
  renameRules: ReplacementRule[];
  docRules: ReplacementRule[];
  signatureRules: SignatureTransform[];
  types: Dict<ApiType>;

  constructor(transform: ApiTransform, source: Api) {
    this.namespace = transform.namespace;

    this.source = Object.fromEntries(
      Object.values(source.files).flatMap((f) =>
        Object.entries(/** @type {Dict<ApiEntry>} */ f.entries ?? {})
      )
    );

    this.blacklist = new Set();

    this.minVersions = transform.minVersions ?? {};
    this.baseVersions = transform.baseVersions ?? {};

    this.includeBeforeMap = {};
    this.includeAfterMap = {};
    this.file = "";

    this.currentIncludeBefore = null;
    this.currentIncludeAfter = null;

    this.paramTypeMap = {};
    Object.assign(this.paramTypeMap, transform.paramTypeMap ?? {});

    this.returnTypeMap = {};
    Object.assign(this.returnTypeMap, transform.returnTypeMap ?? {});

    this.nameMap = {};
    this.callbackTypeMap = {};

    /**
     * @type {Dict<ApiEntry>}
     * @private
     */
    this.glossary = {}; // Map of all entered apis

    if (transform.prefixes?.length) {
      this.prefixToRemove = Array.isArray(transform.prefixes)
        ? new RegExp(`^(${transform.prefixes.join("|")})`)
        : new RegExp("^" + transform.prefixes);
      this.referenceCandidate = Array.isArray(transform.prefixes)
        ? new RegExp(`\\b(?:${transform.prefixes.join("|")})\\w+`, "g")
        : new RegExp(`\\b${transform.prefixes}\\w+`, "g");
    }

    this.renameRules = transform.renameRules ?? [];
    this.renameRules.forEach(
      (rule) => (rule.pattern = new RegExp(rule.pattern))
    );

    this.docRules = transform.docRules ?? [];
    this.docRules.forEach(
      (rule) => (rule.pattern = new RegExp(rule.pattern, "g"))
    );

    this.definitionPrefix = transform.definitionPrefix ?? "";

    this.signatureRules = transform.signatureRules ?? [];

    this.types = {};

    this.enableException = false;

    this.paramReplacements = transform.paramReplacements ?? {};
    this.delegatedReplacements = transform.delegatedReplacements ?? {};
  }

  addGlossary(sourceName: string, entry: ApiEntry) {
    this.glossary[sourceName] = deepClone(entry);
  }

  checkGlossary(sourceName: string) {
    const entry = this.glossary[sourceName];
    if (entry) return deepClone(entry);
  }

  addName(originalType: string, targetType: string) {
    if (!this.nameMap[originalType]) this.nameMap[originalType] = targetType;
  }

  getName(nameCandidate: string) {
    return this.nameMap[nameCandidate] || nameCandidate;
  }

  addParamType(originalType: string, targetType: string) {
    if (!this.paramTypeMap[originalType]) {
      this.paramTypeMap[originalType] = targetType;
      return true;
    }
    return false;
  }

  addReturnType(originalType: string, targetType: string) {
    if (!this.returnTypeMap[originalType]) {
      this.returnTypeMap[originalType] = targetType;
      return true;
    }
    return false;
  }

  addCallbackType(originalType: string, targetType: string) {
    if (!this.callbackTypeMap[originalType]) {
      this.callbackTypeMap[originalType] = targetType;
      return true;
    }
    return false;
  }

  getCallbackType(type: string) {
    if (type.endsWith(" *"))
      return (
        this.callbackTypeMap[type] ??
        this.getCallbackType(type.slice(0, -2)) + " *"
      );
    return this.callbackTypeMap[type] ?? type;
  }

  isAfterMinVersion(version: VersionTag) {
    const tag = this.minVersions[version.tag];
    if (!tag) return false;
    return isVersionAfter(version, tag);
  }

  previewFutureVersion(version: VersionTag) {
    const tag = this.baseVersions[version.tag];
    if (
      !tag ||
      (tag.minor % 2 == 0 && tag.patch % 2 == 0) ||
      !isVersionAfter(version, tag)
    ) {
      return version;
    }
    return tag;
  }

  setFile(file: string) {
    const includeBefore = this.includeBeforeMap[file];
    if (includeBefore) {
      this.currentIncludeBefore = includeBefore;
    } else {
      this.currentIncludeBefore = this.includeBeforeMap[file] = {};
    }
    const includeAfter = this.includeAfterMap[file];
    if (includeAfter) {
      this.currentIncludeAfter = includeAfter;
    } else {
      this.currentIncludeAfter = this.includeAfterMap[file] = {};
    }
  }

  getOrCreateIncludeBefore(includeBeforeKey: string) {
    const includeBefore = this.currentIncludeBefore;

    const includeTarget = includeBefore[includeBeforeKey];
    if (Array.isArray(includeTarget)) return includeTarget;
    const r = (includeBefore[includeBeforeKey] = []);
    return r;
  }

  getOrCreateIncludeAfter(includeAfterKey: string) {
    const includeAfter = this.currentIncludeAfter;

    const includeTarget = includeAfter[includeAfterKey];
    if (Array.isArray(includeTarget)) return includeTarget;
    const r = (includeAfter[includeAfterKey] = []);
    return r;
  }

  /**
   * Add to includeBefore field
   */
  includeBefore(
    entryOrName: string | ApiEntryTransform | ApiEntryTransform[],
    includeBeforeKey: string
  ) {
    const includeTarget = this.getOrCreateIncludeBefore(includeBeforeKey);
    if (Array.isArray(entryOrName)) {
      includeTarget.push(...entryOrName);
    } else {
      includeTarget.push(
        typeof entryOrName === "string" ? { name: entryOrName } : entryOrName
      );
    }
  }

  /**
   * Prepend to includeBefore field
   */
  prependIncludeBefore(
    entryOrName: string | ApiEntryTransform | ApiEntryTransform[],
    includeBeforeKey: string
  ) {
    const includeTarget = this.getOrCreateIncludeBefore(includeBeforeKey);
    if (Array.isArray(entryOrName)) {
      includeTarget.unshift(...entryOrName);
    } else {
      includeTarget.unshift(
        typeof entryOrName === "string" ? { name: entryOrName } : entryOrName
      );
    }
  }

  /**
   * Add to includeAfter field
   */
  includeAfter(
    entryOrName: string | ApiEntryTransform,
    includeAfterKey: string
  ) {
    const includeTarget = this.getOrCreateIncludeAfter(includeAfterKey);
    includeTarget.push(
      typeof entryOrName === "string" ? { name: entryOrName } : entryOrName
    );
  }

  /**
   * Prepend to includeAfter field
   */
  prependIncludeAfter(
    entryOrName: string | ApiEntryTransform,
    includeAfterKey: string
  ) {
    const includeTarget = this.getOrCreateIncludeAfter(includeAfterKey);
    includeTarget.unshift(
      typeof entryOrName === "string" ? { name: entryOrName } : entryOrName
    );
  }
}

function isType(kind: ApiEntryKind) {
  switch (kind) {
    case "alias":
    case "callback":
    case "enum":
    case "struct":
    case "union":
    case "ns":
      return true;
    default:
      return false;
  }
}

function insertOrLink(
  entries: Dict<ApiEntryTransform>,
  entry: ApiEntryTransform,
  key: string
) {
  let currLink = entries[key];
  if (!currLink) {
    entries[key] = entry;
    return;
  }
  while (currLink.link) currLink = currLink.link;
  currLink.link = entry;
}

function getCallbackDef(
  callback: FunctorSupport | boolean | CallbackDefinition
): CallbackDefinition {
  switch (typeof callback) {
    case "boolean":
      return {};
    case "object":
      return callback;
    case "string":
      return { functorSupport: callback };
  }
}

function wrapLockFunctions(
  entries: ApiEntryTransformMap,
  lockName: string,
  lockDef: LockDefinition
) {
  const currLockTransform = entries[lockDef.lockFunc];
  const currLockDelta =
    typeof currLockTransform === "object"
      ? currLockTransform
      : (entries[lockDef.lockFunc] = {});
  if (currLockDelta) {
    currLockDelta.type = lockName;
    const paramName = lockDef.paramType ?? lockDef.controlType;
    addHints(currLockDelta, { body: `return {${paramName}(*this)};` });
  }
  const currUnlockTransform = entries[lockDef.unlockFunc];
  const currUnlockDelta =
    typeof currUnlockTransform === "object"
      ? currUnlockTransform
      : (entries[lockDef.unlockFunc] = {});
  if (currUnlockDelta) {
    currUnlockDelta.type = "void";
    currUnlockDelta.parameters = [{}, { type: `${lockName} &&`, name: "lock" }];
    addHints(currUnlockDelta, {
      body: `SDL_assert_paranoid(lock.get()==*this);lock.reset();`,
    });
  }
}

function expandTypes(
  sourceEntries: Dict<ApiEntry>,
  file: ApiFileTransform,
  context: ApiContext
) {
  expandNamespaces(sourceEntries, file, context);

  const transformMap = file.transform ?? {};

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (context.blacklist.has(sourceName)) continue;
    if (!isType(sourceEntry.kind)) continue;
    const targetDelta = getOrCreateDelta(sourceName);
    const targetName = targetDelta.name;
    tryDetectType(sourceEntry, targetDelta);
    if (targetDelta.callback)
      expandCallback(sourceName, sourceEntry, targetName, targetDelta);
    if (targetDelta.resource)
      expandResource(sourceName, sourceEntry, targetName, targetDelta);
    if (targetDelta.wrapper)
      expandWrapper(sourceName, sourceEntry, targetName, targetDelta);
    if (targetDelta.enum)
      expandEnumeration(sourceName, sourceEntry, targetName, targetDelta);

    if (!targetDelta.kind) {
      targetDelta.kind = "alias";
      if (!targetDelta.type) targetDelta.type = sourceName;
    }
    if (targetName === sourceName) {
      context.blacklist.add(sourceName);
      continue;
    }
    context.addCallbackType(sourceName, targetName);
    context.addName(sourceName, targetName);
    context.addParamType(sourceName, targetName);
    context.addParamType(`${sourceName} *`, `${targetName} *`);
    context.addParamType(`const ${sourceName}`, `const ${targetName}`);
    if (
      context.addParamType(`const ${sourceName} *`, `const ${targetName} &`)
    ) {
      context.paramReplacements[`const ${targetName} &`] = "&$";
    }
    context.addReturnType(sourceName, targetName);
    context.addReturnType(`${sourceName} *`, `${targetName} *`);
    context.addReturnType(`const ${sourceName}`, `const ${targetName}`);
    context.addReturnType(`const ${sourceName} *`, `const ${targetName} &`);
  }

  for (const [targetName, targetDelta] of Object.entries(file.transform)) {
    if (targetDelta.enum)
      expandEnumeration(
        targetName,
        { kind: "alias", name: targetName },
        targetName,
        targetDelta
      );
    if (targetDelta.lock) expandLock(targetName, targetDelta);
  }

  function tryDetectType(
    sourceEntry: ApiEntry,
    targetDelta: ApiEntryTransform
  ) {
    if (targetDelta.callback === undefined && sourceEntry.kind === "callback") {
      targetDelta.callback = true;
      return;
    }
    if (targetDelta.enum === undefined && sourceEntry.kind === "enum")
      targetDelta.enum = true;
    if (targetDelta.resource !== undefined || targetDelta.wrapper !== undefined)
      return;

    const sourceName = sourceEntry.name;
    let fCount = 0;
    let free = "";
    const ctors = [];

    for (const functionEntry of Object.values(sourceEntries)) {
      if (functionEntry.kind !== "function") continue;

      const param0 = functionEntry.parameters?.[0];
      if (typeof param0 !== "object") continue;
      if (
        param0.type.startsWith(sourceName) ||
        param0.type.startsWith(`const ${sourceName}`)
      ) {
        fCount++;
        if (functionEntry.name.match(/Close|Destroy|Free/))
          free = functionEntry.name;
      }

      const resultType = functionEntry.type;
      if (resultType.startsWith(sourceName)) {
        if (functionEntry.name.match(/Create|Open|Define/))
          ctors.push(functionEntry.name);
      }
    }

    if (ctors.length && !targetDelta.entries) {
      targetDelta.entries = {};
      ctors.forEach((n) => (targetDelta.entries[n] = "ctor"));
    }

    if (free) {
      targetDelta.resource = { free };
    } else if (fCount > 5 || targetDelta.entries) {
      targetDelta.wrapper = true;
    }
  }

  function getOrCreateDelta(sourceName): ApiEntryTransform {
    const targetDelta = transformMap[sourceName];
    const name = transformName(sourceName, context);
    if (!targetDelta) {
      const r = (transformMap[sourceName] = { name });
      return r;
    }
    if (!targetDelta.name) targetDelta.name = name;
    return targetDelta;
  }

  function expandCallback(
    sourceName: string,
    sourceEntry: ApiEntry,
    name: string,
    targetDelta: ApiEntryTransform
  ) {
    const parameters = transformCbParameters(sourceEntry.parameters, context);
    const callback = getCallbackDef(targetDelta.callback);
    targetDelta.kind = "alias";
    delete targetDelta.callback;
    const resultType = transformCbType(sourceEntry.type, context);
    targetDelta.type = `${resultType} (SDLCALL *)(${parameters
      .map((p) => p.type + " " + p.name)
      .join(", ")})`;
    if (!callback.functorSupport) return;
    const userdataIndex =
      callback.userdataIndex ??
      parameters.findIndex(
        (p) => p.type.endsWith("void *") && p.name === "userdata"
      );
    if (userdataIndex < 0) return;
    const callbackName =
      callback.wrapper ?? name.replace(/(Function|Callback)$/, "") + "CB";
    const doc = removeTagFromGroup(
      addToTagGroup(
        transformDoc(sourceEntry.doc ?? undefined, context),
        "@sa",
        name
      ),
      "@param userdata"
    );
    if (
      callback.functorSupport === "std" ||
      callback.functorSupport === "lightweight"
    ) {
      const typeParams = parameters.toSpliced(userdataIndex, 1);
      if (callback.functorSupport === "std") {
        makeCallbackAlias(
          "std::function",
          callback.type ?? resultType,
          callback.parameters ?? typeParams
        );
      } else if (
        callback.type === undefined &&
        callback.parameters === undefined
      ) {
        makeCallbackAlias(
          userdataIndex === 0 ? "MakeFrontCallback" : "MakeTrailingCallback",
          resultType,
          typeParams
        );
      } else {
        const templateType =
          userdataIndex === 0
            ? "LightweightCallbackT"
            : "LightweightTrailingCallbackT";
        const type = `${templateType}<${callbackName}, ${resultType}, ${typeParams
          .map((p) => p.type)
          .join(", ")}>`;
        const template = [
          {
            type: `std::invocable<${(callback.parameters ?? typeParams)
              .map((p) => p.type)
              .join(", ")}>`,
            name: "F",
          },
        ];
        const callbackEntry: ApiEntryTransform = {
          kind: "struct",
          name: callbackName,
          type,
          doc,
          entries: {
            [callbackName]: {
              kind: "function",
              doc: ["ctor"],
              type: "",
              template,
              parameters: [
                {
                  type: "const F &",
                  name: "func",
                },
              ],
              hints: {
                init: [`${type}(func)`],
              },
            },
            doCall: {
              kind: "function",
              doc: ["@private"],
              type: resultType,
              static: true,
              template,
              parameters: [
                {
                  type: "F &",
                  name: "func",
                },
                ...typeParams,
              ],
            },
          },
        };
        context.prependIncludeAfter(callbackEntry, name);
      }
    }

    function makeCallbackAlias(
      wrapper: string,
      resultType: string,
      typeParams: ApiParameters
    ) {
      const parameters = typeParams
        .map((p) => `${p.type} ${p.name}`)
        .join(", ");
      const callbackEntry: ApiEntryTransform = {
        kind: "alias",
        name: callbackName,
        type: `${wrapper}<${resultType}(${parameters})>`,
        doc,
        ...file.transform[callbackName],
        before: undefined,
        after: undefined,
      };
      context.prependIncludeAfter(callbackEntry, name);
    }
  }

  function expandWrapper(
    sourceType: string,
    sourceEntry: ApiEntry,
    targetType: string,
    transform: ApiEntryTransform
  ) {
    const wrapper = transform.wrapper === true ? {} : transform.wrapper;
    if (!wrapper) return;
    const isStruct = sourceEntry.kind === "struct" && !transform.type;
    if (!transform.kind) transform.kind = "struct";

    const type =
      isStruct || !sourceEntry.type?.startsWith("struct ")
        ? sourceType
        : sourceType + " *";
    const constexpr = transform.constexpr !== false;
    const paramName =
      wrapper.attribute ?? targetType[0].toLowerCase() + targetType.slice(1);
    const rawType = wrapper.rawName || `${targetType}Raw`;
    const paramType =
      wrapper.paramType ?? (isStruct ? `${rawType} *` : targetType);
    const constParamType =
      wrapper.paramType ?? (isStruct ? `const ${rawType} &` : rawType);
    const attribute = "m_" + paramName;
    context.includeBefore(
      {
        name: rawType,
        kind: "alias",
        type:
          sourceEntry.type && sourceEntry.type == `struct ${sourceType}`
            ? `${sourceType} *`
            : sourceType,
        doc: [`Alias to raw representation for ${targetType}.`],
      },
      "__begin"
    );

    context.includeBefore(
      {
        name: targetType,
        kind: "forward",
      },
      "__begin"
    );

    const fields: string[] = [];
    if (isStruct) {
      for (const e of Object.values(sourceEntry.entries)) fields.push(e.name);

      addHints(transform, {
        self: "this",
      });
    } else {
      addHints(transform, {
        self: attribute,
      });
    }

    const entries: Dict<ApiEntryTransform> = {};

    if (!isStruct) {
      insertTransform(entries, {
        kind: "var",
        name: attribute,
        type: rawType,
      });
      addHints(transform, {
        private: true,
      });
    }

    if (wrapper.genCtor !== false)
      insertTransform(entries, {
        kind: "function",
        name: targetType,
        type: "",
        constexpr,
        parameters: [
          {
            type: constParamType,
            name: paramName,
            default: wrapper.defaultValue ?? "{}",
          },
        ],
        doc: [
          `Wraps ${sourceType}.`,
          { tag: `@param ${paramName}`, content: "the value to be wrapped" },
        ],
        hints: {
          init: [`${isStruct ? rawType : attribute}(${paramName})`],
          changeAccess: isStruct ? undefined : "public",
          noexcept: true,
        },
      });
    if ((isStruct && wrapper.ordered) || wrapper.comparable) {
      const body =
        "return " +
        fields.map((f) => `lhs.${f} == rhs.${f}`).join(" && ") +
        ";";
      context.includeBefore(
        {
          before: sourceType,
          kind: "function",
          name: "operator==",
          type: "bool",
          constexpr,
          parameters: [
            { type: constParamType, name: "lhs" },
            { type: constParamType, name: "rhs" },
          ],
          doc: [`Comparison operator for ${targetType}.`],
          hints: { body, noexcept: true },
        },
        sourceType
      );
      if (wrapper.ordered) {
        const lastField = fields.length - 1;
        const body =
          fields
            .map((f, i) => {
              if (i == lastField) return `return lhs.${f} <=> rhs.${f}`;
              return `if (lhs.${f} != rhs.${f}) return lhs.${f} <=> rhs.${f}`;
            })
            .join(";\n") + ";";
        context.includeBefore(
          {
            before: sourceType,
            kind: "function",
            name: "operator<=>",
            type: "auto",
            constexpr,
            parameters: [
              { type: constParamType, name: "lhs" },
              { type: constParamType, name: "rhs" },
            ],
            doc: [`Spaceship operator for ${targetType}.`],
            hints: { body, noexcept: true },
          },
          sourceType
        );
      }
    }
    if (wrapper.nullable)
      insertTransform(entries, {
        kind: "function",
        name: "operator==",
        type: "bool",
        constexpr,
        immutable: true,
        parameters: [{ type: "std::nullptr_t", name: "_" }],
        doc: [
          "Compare with nullptr.",
          {
            tag: "@returns",
            content: `True if invalid state, false otherwise.`,
          },
        ],
        hints: { body: "return !bool(*this);", noexcept: true },
      });
    if (!isStruct)
      insertTransform(entries, {
        kind: "function",
        name: `operator ${rawType}`,
        type: "",
        constexpr,
        immutable: true,
        parameters: [],
        doc: [
          `Unwraps to the underlying ${sourceType}.`,
          { tag: "@returns", content: `the underlying ${rawType}.` },
        ],
        hints: { body: `return ${attribute};`, noexcept: true },
      });
    const body = isStruct
      ? `return *this != ${rawType}{};`
      : `return ${attribute} != 0;`;
    if (wrapper.invalidState !== false && isStruct)
      insertTransform(entries, {
        kind: "function",
        name: "operator bool",
        type: "",
        constexpr,
        explicit: true,
        immutable: true,
        parameters: [],
        doc: [
          "Check if valid.",
          { tag: "@returns", content: "True if valid state, false otherwise." },
        ],
        hints: { body, noexcept: true },
      });

    if (isStruct) {
      transform.type = rawType;
      transform.hints.super = rawType;

      if (wrapper.genMembers !== false) {
        const parameters: ApiParameter[] = [];
        for (const attrib of Object.values(sourceEntry.entries)) {
          if (attrib.kind !== "var") continue;
          const name = attrib.name;
          const type = attrib.type;
          const paramType = transformType(type, context.paramTypeMap);
          const returnType = transformType(type, context.returnTypeMap);
          parameters.push({ type: paramType, name });
          const capName = name[0].toUpperCase() + name.slice(1);
          insertTransform(entries, {
            kind: "function",
            name: `Get${capName}`,
            type: returnType,
            constexpr,
            immutable: true,
            parameters: [],
            doc: [
              `Get the ${name}.`,
              { tag: "@returns", content: `current ${name} value.` },
            ],
            hints: { body: `return ${name};`, noexcept: true },
          });
          insertTransform(entries, {
            kind: "function",
            name: `Set${capName}`,
            type: `${targetType} &`,
            constexpr,
            parameters: [
              {
                type: paramType,
                name: `new${capName}`,
              },
            ],
            doc: [
              `Set the ${name}.`,
              [
                {
                  tag: `@param new${capName}`,
                  content: `the new ${name} value.`,
                },
                { tag: "@returns", content: "Reference to self." },
              ],
            ],
            hints: {
              body: `${name} = new${capName};\nreturn *this;`,
              noexcept: true,
            },
          });
          context.addCallbackType(sourceType, rawType);
          context.addParamType(sourceType, rawType);
          context.addParamType(`${sourceType} *`, `${rawType} *`);
          context.addParamType(`const ${sourceType}`, `const ${rawType}`);
          context.addParamType(`const ${sourceType} *`, `const ${rawType} &`);
        }
        insertTransform(entries, {
          kind: "function",
          name: targetType,
          type: "",
          constexpr,
          parameters,
          doc: [
            "Constructs from its fields.",
            parameters.map((p) => ({
              tag: `@param ${p.name}`,
              content: `the value for ${p.name}.`,
            })),
          ],
          hints: {
            init: [`${rawType}{${parameters.map((p) => p.name).join(", ")}}`],
            noexcept: true,
          },
        });
      }
    } else {
      transform.type = "";
    }

    const currentCtor = transform.entries?.[targetType];
    if (currentCtor) {
      insertTransform(
        entries,
        typeof currentCtor === "string" ? {} : currentCtor,
        targetType
      );
      delete transform.entries[targetType];
    }
    const blockedNames = new Set(Object.keys(entries));
    blockedNames.add(targetType);
    if (transform.entries)
      createBlockedNames(transform.entries).forEach((k) => blockedNames.add(k));
    const detectedMethods = detectMethods(
      sourceType,
      targetType,
      rawType,
      `const ${rawType}`,
      blockedNames
    );
    mirrorMethods(
      sourceEntries,
      file.transform,
      transform.entries ?? {},
      paramType,
      constParamType,
      targetType
    );
    mirrorMethods(
      sourceEntries,
      file.transform,
      detectedMethods,
      paramType,
      constParamType,
      targetType
    );
    transform.entries = {
      ...entries,
      ...transform.entries,
      ...detectedMethods,
    };
    if (type !== sourceType) {
      context.addParamType(type, targetType);
      context.addReturnType(type, targetType);
    }

    delete transform.wrapper;
  }

  function createBlockedNames(entries?: ApiEntryTransformMap) {
    const result = new Set<string>();

    if (!entries) entries = {};
    for (const [k, v] of Object.entries(entries)) {
      if (v === "plc") continue;
      result.add(k);
    }
    return result;
  }

  function expandResource(
    sourceName: string,
    sourceEntry: ApiEntry,
    targetName: string,
    targetEntry: ApiEntryTransform
  ) {
    const resourceEntry = getResourceDefinition(targetEntry) ?? {};

    const rawName = resourceEntry.rawName || `${targetName}Raw`;
    const constRawName = `const ${rawName}`;
    const paramType = `${targetName}Param`;
    const enableMemberAccess =
      resourceEntry.enableMemberAccess ?? sourceEntry.kind === "struct";
    const enableConstParam =
      resourceEntry.enableConstParam ?? enableMemberAccess;
    const constParamType = enableConstParam
      ? `${targetName}ConstParam`
      : paramType;
    if (!targetEntry.kind) targetEntry.kind = "struct";
    const hasShared = !!resourceEntry.shared;
    const hasScoped = resourceEntry.owning === false;
    const hasRef = resourceEntry.ref ?? !hasScoped;
    const refName = `${targetName}Ref`;
    const scopedName = `${targetName}Scoped`;

    const type = targetEntry.type ?? sourceName;
    const isStruct =
      sourceEntry.kind === "struct" ||
      (sourceEntry.kind === "alias" && sourceEntry.type.startsWith("struct "));
    const pointerType = isStruct ? `${type} *` : type;
    const constPointerType = `const ${pointerType}`;
    const nullValue = isStruct ? "nullptr" : "0";
    const title = targetName[0].toLowerCase() + targetName.slice(1);
    if (sourceEntry.type && !targetEntry.type) targetEntry.type = "";
    context.addName(sourceName, targetName);
    context.addCallbackType(isStruct ? `${sourceName} *` : sourceName, rawName);

    const referenceAliases: ApiEntryTransform[] = [];
    referenceAliases.push({ name: targetName, kind: "forward" });
    referenceAliases.push({
      name: rawName,
      kind: "alias",
      type: pointerType,
      doc: [`Alias to raw representation for ${targetName}.`],
    });
    if (hasRef) referenceAliases.push({ name: refName, kind: "forward" });
    if (hasScoped) referenceAliases.push({ name: scopedName, kind: "forward" });
    const hasLock = getLockDefinition(targetEntry, targetName);
    let lockName = `${targetName}Lock`;
    if (hasLock) {
      delete targetEntry.lock;
      const currDef = file.transform[lockName];
      lockName = currDef?.name || lockName;
      const lockEntry: ApiEntryTransform = {
        lock: hasLock,
      };
      hasLock.controlType = hasRef ? refName : targetName;
      if (currDef) {
        combineObject(lockEntry, currDef ?? {});
      } else {
        lockEntry.after = hasRef ? refName : targetName;
      }
      file.transform[lockName] = lockEntry;
    }

    const memberAccess: ApiEntryTransformMap = {};
    if (enableMemberAccess) {
      memberAccess["operator->"] = {
        doc: [`member access to underlying ${rawName}.`],
        kind: "function",
        constexpr: true,
        type: "auto",
        parameters: [],
        hints: { body: "return value;" },
      };
    }

    referenceAliases.push({
      name: paramType,
      kind: "struct",
      doc: [`Safely wrap ${targetName} for non owning parameters`],
      entries: {
        value: {
          kind: "var",
          name: "value",
          doc: [`parameter's ${rawName}`],
          type: rawName,
        },
        [paramType]: {
          kind: "function",
          name: paramType,
          doc: [`Constructs from ${rawName}`],
          constexpr: true,
          type: "",
          parameters: [{ type: rawName, name: "value" }],
          hints: { init: ["value(value)"] },
        },
        [`${paramType}#2`]: {
          kind: "function",
          name: paramType,
          doc: [`Constructs null/invalid`],
          constexpr: true,
          type: "",
          parameters: [
            { type: "std::nullptr_t", name: "_", default: "nullptr" },
          ],
          hints: { init: [`value(${nullValue})`] },
        },
        "operator bool": {
          kind: "function",
          type: "",
          immutable: true,
          constexpr: true,
          explicit: true,
          parameters: [],
          hints: { body: "return !!value;" },
          doc: [`Converts to bool`],
        },
        "operator <=>": {
          kind: "function",
          type: "auto",
          immutable: true,
          constexpr: true,
          parameters: [{ type: `const ${paramType} &`, name: "other" }],
          hints: { default: true },
          doc: [`Comparison`],
        },
        [`operator ${rawName}`]: {
          kind: "function",
          name: `operator ${rawName}`,
          doc: [`Converts to underlying ${rawName}`],
          constexpr: true,
          immutable: true,
          type: "",
          parameters: [],
          hints: { body: "return value;" },
        },
        ...memberAccess,
      },
    });
    if (enableConstParam) {
      referenceAliases.push({
        name: constParamType,
        kind: "struct",
        doc: [`Safely wrap ${targetName} for non owning const parameters`],
        entries: {
          value: {
            kind: "var",
            name: "value",
            doc: [`parameter's ${constRawName}`],
            type: constRawName,
          },
          [constParamType]: {
            kind: "function",
            name: constParamType,
            doc: [`Constructs from ${constRawName}`],
            constexpr: true,
            type: "",
            parameters: [{ type: constRawName, name: "value" }],
            hints: { init: ["value(value)"] },
          },
          [`${constParamType}#2`]: {
            kind: "function",
            name: constParamType,
            doc: [`Constructs from ${paramType}`],
            constexpr: true,
            type: "",
            parameters: [{ type: paramType, name: "value" }],
            hints: { init: ["value(value.value)"] },
          },
          [`${constParamType}#3`]: {
            kind: "function",
            name: constParamType,
            doc: [`Constructs null/invalid`],
            constexpr: true,
            type: "",
            parameters: [
              { type: "std::nullptr_t", name: "_", default: "nullptr" },
            ],
            hints: { init: [`value(${nullValue})`] },
          },
          "operator bool": {
            kind: "function",
            type: "",
            immutable: true,
            constexpr: true,
            explicit: true,
            parameters: [],
            hints: { body: "return !!value;" },
            doc: [`Converts to bool`],
          },
          "operator <=>": {
            kind: "function",
            type: "auto",
            immutable: true,
            constexpr: true,
            parameters: [{ type: `const ${constParamType} &`, name: "other" }],
            hints: { default: true },
            doc: [`Comparison`],
          },
          [`operator ${constRawName}`]: {
            kind: "function",
            name: `operator ${constRawName}`,
            doc: [`Converts to underlying ${constRawName}`],
            constexpr: true,
            immutable: true,
            type: "",
            parameters: [],
            hints: { body: "return value;" },
          },
          ...memberAccess,
        },
      });
    }
    if (hasScoped) {
      context.addParamType(pointerType, targetName);
      context.addParamType(constPointerType, `const ${targetName} &`);
    } else {
      context.addParamType(pointerType, paramType);
      context.addParamType(
        constPointerType,
        enableConstParam ? constParamType : paramType
      );
    }

    if (hasShared || hasScoped) {
      context.addReturnType(pointerType, targetName);
    } else if (hasRef) {
      context.addReturnType(pointerType, refName);
    } else {
      context.addReturnType(pointerType, rawName);
    }
    context.addReturnType(constPointerType, constRawName);

    const ownershipDisclaimer = hasScoped
      ? []
      : [
          "This assumes the ownership, call release() if you need to take back.",
        ];

    const ctors: Dict<ApiEntryTransform> = {
      [targetName]: {
        kind: "function",
        type: "",
        constexpr: true,
        parameters: [{ name: "", type: "std::nullptr_t", default: "nullptr" }],
        hints: {
          init: ["m_resource(0)"],
          noexcept: true,
          changeAccess: "public",
        },
        doc: ["Default ctor"],
      },
      [`${targetName}#2`]: {
        kind: "function",
        type: "",
        constexpr: true,
        explicit: !hasScoped,
        parameters: [{ name: "resource", type: constRawName }],
        hints: {
          init: ["m_resource(resource)"],
          noexcept: true,
        },
        doc: [
          `Constructs from ${paramType}.`,
          { tag: "@param resource", content: `a ${rawName} to be wrapped.` },
          ...ownershipDisclaimer,
        ],
      },
      [`${targetName}#3`]: {
        kind: "function",
        type: "",
        constexpr: true,
        parameters: [{ name: "other", type: `const ${targetName} &` }],
        hints: { default: true, noexcept: true },
        doc: ["Copy constructor"],
      },
      [`${targetName}#4`]: {
        kind: "function",
        type: "",
        constexpr: true,
        parameters: [{ name: "other", type: `${targetName} &&` }],
        hints: {
          init: [`${targetName}(other.release())`],
          noexcept: true,
        },
        doc: ["Move constructor"],
      },
    };
    if (hasShared) {
      const copyCtorHints: EntryHint = {};
      ctors[`${targetName}#3`].hints = copyCtorHints;
      if (resourceEntry.shared !== true) {
        copyCtorHints.init = ["m_resource(other.m_resource)"];
        copyCtorHints.body = `++m_resource->${resourceEntry.shared};`;
        ctors["Borrow"] = {
          kind: "function",
          static: true,
          constexpr: true,
          type: targetName,
          parameters: [{ name: "resource", type: paramType }],
          hints: {
            body: `if (resource) {\n  ++resource.value->${resourceEntry.shared};\n  return ${targetName}(resource.value);}\nreturn {};`,
          },
          doc: [
            `Safely borrows the from ${paramType}.`,
            {
              tag: "@param resource",
              content: `a ${rawName} or ${targetName}.`,
            },
            "This does not takes ownership!",
          ],
        };
      }
    } else if (!hasScoped) {
      ctors[`${targetName}#3`].hints.changeAccess = "protected";
      ctors[`${targetName}#4`].hints.changeAccess = "public";
    }
    if (hasRef && !hasShared) {
      insertTransform(
        ctors,
        {
          kind: "function",
          doc: [],
          type: "",
          constexpr: true,
          parameters: [{ name: "other", type: `const ${refName} &` }],
          hints: { delete: true },
        },
        targetName
      );
      insertTransform(
        ctors,
        {
          kind: "function",
          doc: [],
          type: "",
          constexpr: true,
          parameters: [{ name: "other", type: `${refName} &&` }],
          hints: { delete: true },
        },
        targetName
      );
    }
    const subEntries = targetEntry.entries || {};

    Object.keys(subEntries)
      .filter((k) => k === targetName || k.startsWith(targetName + "#"))
      .map((k) => {
        const e = subEntries[k];
        delete subEntries[k];
        return e;
      })
      .filter((e) => typeof e === "object")
      .forEach((e) => insertTransform(ctors, e, targetName));
    for (const sourceName of resourceEntry.ctors ?? []) {
      const entry = subEntries[sourceName] ?? {};
      delete subEntries[sourceName];
      const ctorTransform = file.transform[sourceName];
      if (typeof entry === "string") {
        system.warn(
          `${sourceName} can not be a custom ctor, only objects containing name property can be accepted.`
        );
        continue;
      }
      entry.static = true;
      entry.type = targetName;
      if (!entry.name)
        entry.name = transformMemberName(sourceName, targetName, context);
      if (!entry.sourceName && sourceEntries[sourceName])
        entry.sourceName = sourceName;
      addHints(entry, { wrapSelf: true });
      ctors[sourceName] = entry;
      if (!ctorTransform) {
        file.transform[sourceName] = {
          type: targetName,
          hints: { wrapSelf: true },
        };
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
          const parameters = file.transform[sourceName]?.parameters;
          const ctor: ApiEntryTransform = {
            kind: "function",
            type: "",
            name: targetName,
            sourceName,
          };
          if (parameters) ctor.parameters = parameters;
          ctors[sourceName] = ctor;
          isCtor = true;
        }
      } else if (entry.name === "ctor") {
        entry.kind = "function";
        entry.type = "";
        entry.name = targetName;
        if (!entry.sourceName && sourceEntries[sourceName])
          entry.sourceName = sourceName;
        ctors[sourceName] = entry;
        isCtor = true;
      }
      if (isCtor) {
        delete subEntries[sourceName];
        if (!ctorTransform) {
          file.transform[sourceName] = {
            type: targetName,
            hints: { wrapSelf: true },
          };
        } else if (!ctorTransform.type) {
          ctorTransform.type = targetName;
          addHints(ctorTransform, { wrapSelf: true });
        }
      }
    }

    let freeFunction: ApiEntry =
      sourceEntries[resourceEntry.free ?? "reset"] ??
      scanFreeFunction(sourceEntries, targetName, pointerType);
    if (freeFunction) {
      const sourceName = freeFunction.name;
      freeFunction = transformEntry(freeFunction, context);
      const freeTransformEntry =
        (subEntries[sourceName] as ApiEntryTransform) ?? {};
      combineObject(freeFunction, freeTransformEntry);
      freeFunction.name =
        freeTransformEntry.name ??
        makeNaturalName(transformName(sourceName, context), targetName);
      freeFunction.doc = transformDoc(freeFunction.doc, context) ?? [
        `frees up ${title}.`,
      ];
      const fileLevelEntry: ApiEntryTransform = file.transform[sourceName] ?? {
        parameters: [{ type: rawName }, ...freeFunction.parameters.slice(1)],
      };
      file.transform[sourceName] = fileLevelEntry;
      const fileLevelName =
        fileLevelEntry.name ?? transformName(sourceName, context);
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
      subEntries["Destroy"] = freeFunction = {
        kind: "function",
        name: "Destroy",
        doc: [`frees up ${title}.`],
        type: "void",
        parameters: [],
      };
    }
    if (resourceEntry.enableAutoMethods !== false) {
      const blockedNames = createBlockedNames(subEntries);
      blockedNames.add(targetName);
      Object.keys(ctors).forEach((k) => blockedNames.add(k));
      const detectedMethods = detectMethods(
        sourceName,
        targetName,
        paramType,
        constParamType,
        blockedNames
      );
      mirrorMethods(
        sourceEntries,
        file.transform,
        subEntries,
        paramType,
        constParamType,
        targetName
      );
      combineObject(subEntries, detectedMethods);
    } else {
      mirrorMethods(
        sourceEntries,
        file.transform,
        subEntries,
        paramType,
        constParamType,
        targetName
      );
    }

    if (enableMemberAccess) {
      ctors["operator->"] = {
        doc: [`member access to underlying ${rawName}.`],
        kind: "function",
        constexpr: true,
        immutable: true,
        type: constRawName,
        parameters: [],
        hints: { body: "return m_resource;", noexcept: true },
      };
      ctors["operator->#2"] = {
        kind: "function",
        constexpr: true,
        type: rawName,
        parameters: [],
        hints: { body: "return m_resource;", noexcept: true },
      };
    }

    if (sourceEntry.doc) {
      targetEntry.doc = [
        ...transformDoc(sourceEntry.doc, context),
        "@cat resource",
      ];
    } else {
      targetEntry.doc = [`Wraps ${title} resource.`, "@cat resource"];
    }
    const isCopyable = hasScoped || hasShared;
    targetEntry.entries = {
      m_resource: {
        kind: "var",
        type: rawName,
        hints: { body: nullValue },
      },
      ...ctors,
      [`~${targetName}`]: {
        kind: "function",
        doc: ["Destructor"],
        type: "",
        parameters: [],
        hints: {
          body:
            freeFunction && !hasScoped
              ? `${freeFunction.sourceName ?? freeFunction.name}(m_resource);`
              : "",
        },
      },
      "operator=": {
        kind: "function",
        type: `${targetName} &`,
        parameters: [{ name: "other", type: `${targetName} &&` }],
        constexpr: true,
        hints: {
          body: "std::swap(m_resource, other.m_resource);\nreturn *this;",
          noexcept: true,
        },
        doc: ["Assignment operator."],
      },
      "operator=#2": {
        kind: "function",
        type: `${targetName} &`,
        parameters: [{ name: "other", type: `const ${targetName} &` }],
        constexpr: true,
        hints: {
          default: true,
          changeAccess: isCopyable ? undefined : "protected",
          noexcept: true,
        },
        doc: ["Assignment operator."],
      },
      get: {
        kind: "function",
        type: rawName,
        immutable: true,
        constexpr: true,
        parameters: [],
        hints: {
          body: "return m_resource;",
          changeAccess: isCopyable ? undefined : "public",
          noexcept: true,
        },
        doc: [`Retrieves underlying ${rawName}.`],
      },
      release: {
        kind: "function",
        type: rawName,
        constexpr: true,
        parameters: [],
        hints: {
          body: `auto r = m_resource;\nm_resource = ${nullValue};\nreturn r;`,
          noexcept: true,
        },
        doc: [`Retrieves underlying ${rawName} and clear this.`],
      },
      "operator <=>": {
        kind: "function",
        type: "auto",
        immutable: true,
        constexpr: true,
        parameters: [{ type: `const ${targetName} &`, name: "other" }],
        hints: { default: true, noexcept: true },
        doc: [`Comparison`],
      },
      "operator bool": {
        kind: "function",
        type: "",
        immutable: true,
        constexpr: true,
        explicit: true,
        parameters: [],
        hints: { body: "return !!m_resource;", noexcept: true },
        doc: [`Converts to bool`],
      },
      [`operator ${paramType}`]: {
        kind: "function",
        type: "",
        immutable: true,
        constexpr: true,
        parameters: [],
        hints: { body: "return {m_resource};", noexcept: true },
        doc: [`Converts to ${paramType}`],
      },
      [freeFunction.name]: "plc",
      ...subEntries,
    };
    addHints(targetEntry, {
      self: "m_resource",
      super: "m_resource",
      private: true,
    });

    if (hasLock) {
      wrapLockFunctions(targetEntry.entries, lockName, hasLock);
    }

    const derivedEntries: ApiEntryTransform[] = [];

    if (hasShared && hasRef) {
      derivedEntries.push({
        kind: "struct",
        name: refName,
        type: targetName,
        doc: [`Safe reference for ${targetName}.`],
        entries: {
          [`${targetName}::${targetName}`]: "alias",
          [refName]: {
            kind: "function",
            type: "",
            parameters: [
              {
                type: rawName,
                name: "resource",
              },
            ],
            hints: {
              init: [`${targetName}(Borrow(resource))`],
              noexcept: true,
            },
            doc: [
              `Constructs from ${rawName}.`,
              {
                tag: "@param resource",
                content: `a ${rawName}.`,
              },
              "This borrows the ownership, increments the refcount!",
            ],
          },
          [`${refName}#2`]: {
            kind: "function",
            type: "",
            parameters: [
              {
                type: targetName,
                name: "resource",
              },
            ],
            hints: {
              init: [`${targetName}(std::move(resource))`],
              noexcept: true,
            },
            doc: [`Constructs from ${targetName}.`],
          },
        },
      });
    } else if (hasRef) {
      derivedEntries.push({
        kind: "struct",
        name: refName,
        type: targetName,
        doc: [`Semi-safe reference for ${targetName}.`],
        entries: {
          [`${targetName}::${targetName}`]: "alias",
          [refName]: {
            kind: "function",
            type: "",
            parameters: [
              {
                type: paramType,
                name: "resource",
              },
            ],
            hints: { init: [`${targetName}(resource.value)`], noexcept: true },
            doc: [
              `Constructs from ${paramType}.`,
              {
                tag: "@param resource",
                content: `a ${rawName} or ${targetName}.`,
              },
              "This does not takes ownership!",
            ],
          },
          [`${refName}#2`]: {
            kind: "function",
            type: "",
            parameters: [
              {
                type: rawName,
                name: "resource",
              },
            ],
            hints: { init: [`${targetName}(resource)`], noexcept: true },
            doc: [
              `Constructs from ${paramType}.`,
              {
                tag: "@param resource",
                content: `a ${rawName} or ${targetName}.`,
              },
              "This does not takes ownership!",
            ],
          },
          [`${refName}#3`]: {
            kind: "function",
            type: "",
            constexpr: true,
            parameters: [
              {
                type: `const ${refName} &`,
                name: "other",
              },
            ],
            hints: { default: true, noexcept: true },
            doc: ["Copy constructor."],
          },
          [`~${refName}`]: {
            kind: "function",
            doc: ["Destructor"],
            type: "",
            parameters: [],
            hints: { body: "release();" },
          },
        },
      });
    }
    if (hasScoped)
      derivedEntries.push({
        kind: "struct",
        name: scopedName,
        type: targetName,
        doc: [`RAII owning version ${targetName}.`],
        entries: {
          [`${targetName}::${targetName}`]: "alias",
          [scopedName]: {
            kind: "function",
            type: "",
            constexpr: true,
            parameters: [{ name: "other", type: `const ${targetName} &` }],
            hints: { delete: true },
          },
          [`${scopedName}#2`]: {
            kind: "function",
            doc: ["Move constructor"],
            type: "",
            constexpr: true,
            parameters: [{ name: "other", type: `${targetName} &&` }],
            hints: {
              init: [`${targetName}(other.release())`],
              noexcept: true,
            },
          },
          [`~${scopedName}`]: {
            kind: "function",
            doc: ["Destructor"],
            type: "",
            parameters: [],
            hints: { body: `${freeFunction?.name ?? "Destroy"}();` },
          },
        },
      });

    context.includeBefore(referenceAliases, "__begin");
    derivedEntries.forEach((e) => context.includeAfter(e, targetName));
    delete targetEntry.resource;
  }

  function detectMethods(
    sourceType: string,
    targetType: string,
    paramType: string,
    constParamType: string,
    blockedNames: Set<string>
  ) {
    const foundEntries: Dict<ApiEntryTransform | QuickTransform> = {};
    const prefix = `${targetType}::`;
    let lastKey = "__begin";
    const placeAfter = new Map<string, string[]>();
    for (let [sourceName, entryDelta] of Object.entries(transformMap)) {
      const hasPrefix = sourceName.startsWith(prefix);
      if (hasPrefix) sourceName = sourceName.slice(prefix.length);
      if (blockedNames.has(sourceName)) continue;
      const sourceEntry = sourceEntries[sourceName];
      if (sourceEntry) lastKey = sourceName;
      if (
        entryDelta.kind !== "function" &&
        (entryDelta.kind || sourceEntry?.kind !== "function")
      )
        continue;
      const parameters = entryDelta.parameters;
      let param0 = parameters?.[0];
      if (!param0?.type) {
        if (!hasPrefix) continue;
        param0 = {
          type: entryDelta.immutable ? constParamType : paramType,
          name: param0?.name,
        };
      }
      const m = paramMatchesVariants(
        param0,
        [paramType, `${paramType} *`],
        [constParamType, `${constParamType} &`]
      );
      if (!m && !hasPrefix) {
        blockedNames.add(sourceName);
        continue;
      }
      if (sourceEntry) {
        if (hasPrefix) {
          delete transformMap[prefix + sourceName];
          blockedNames.add(sourceName);
        }
        foundEntries[sourceName] = {
          ...deepClone(entryDelta),
          immutable: m === "immutable" || entryDelta.immutable,
          name: entryDelta.hints?.methodName ?? undefined,
        };
        delete entryDelta.immutable;
      } else if (hasPrefix) {
        if (placeAfter.has(lastKey)) placeAfter.get(lastKey).push(sourceName);
        else placeAfter.set(lastKey, [sourceName]);
        blockedNames.add(sourceName);
      } else if (!sourceName.includes("::")) {
        const methodName = transformMemberName(
          entryDelta.hints?.methodName ?? entryDelta.name ?? sourceName,
          targetType,
          context
        );
        if (blockedNames.has(methodName)) continue;
        if (entryDelta.after) lastKey = entryDelta.after;
        if (placeAfter.has(lastKey)) placeAfter.get(lastKey).push(methodName);
        else placeAfter.set(lastKey, [methodName]);
        const name = `${targetType}::${methodName}`;
        insertOrLink(
          transformMap,
          {
            ...deepClone(entryDelta),
            after: sourceName,
            name,
            static: false,
            parameters: parameters.slice(1),
            hints: {
              delegate: `${context.namespace}::${transformName(
                sourceName,
                context
              )}`,
            },
          },
          name
        );
        delete entryDelta.immutable;
      }
    }
    for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
      if (blockedNames.has(sourceName) || foundEntries[sourceName]) continue;
      if (sourceEntry.kind !== "function") continue;
      const parameters = sourceEntry.parameters;
      if (!parameters?.length) continue;
      const param0 = parameters[0];
      if (typeof param0 === "string") continue;
      const m = paramMatchesVariants(
        param0,
        [sourceType, `${sourceType} *`],
        [`const ${sourceType}`, `const ${sourceType} *`]
      );
      if (!m) continue;
      const entryDelta = transformMap[sourceName];
      if (
        entryDelta?.name ||
        entryDelta?.parameters ||
        entryDelta?.type ||
        entryDelta?.hints?.methodName
      ) {
        const e: ApiEntryTransform = {
          ...deepClone(entryDelta),
          immutable: m === "immutable" || entryDelta.immutable,
        };
        e.name = entryDelta.hints?.methodName ?? undefined;
        foundEntries[sourceName] = e;
        if (
          entryDelta.immutable &&
          !entryDelta.parameters &&
          (!constParamType.startsWith("const ") || constParamType.endsWith("*"))
        ) {
          entryDelta.parameters = sourceEntry.parameters.map((_) => ({}));
          entryDelta.parameters[0].type = constParamType;
        }
        delete entryDelta.immutable;
      } else if (entryDelta?.immutable) {
        foundEntries[sourceName] = "immutable";
        if (
          entryDelta.immutable &&
          !entryDelta.parameters &&
          (!constParamType.startsWith("const ") || constParamType.endsWith("*"))
        ) {
          entryDelta.parameters = sourceEntry.parameters.map((_) => ({}));
          entryDelta.parameters[0].type = constParamType;
        }
        delete entryDelta.immutable;
      } else {
        foundEntries[sourceName] = m;
      }
      if (!entryDelta) transformMap[sourceName] = {};
    }
    const orderedEntries: Dict<ApiEntryTransform | QuickTransform> = {};
    placeAfter
      .get("__begin")
      ?.forEach((sKey) => (orderedEntries[sKey] = "plc"));
    Object.keys(sourceEntries).forEach((key) => {
      const value = foundEntries[key];
      if (value) orderedEntries[key] = value;
      placeAfter.get(key)?.forEach((sKey) => {
        if (!orderedEntries[sKey]) orderedEntries[sKey] = "plc";
      });
    });
    return orderedEntries;
  }

  function expandEnumeration(
    sourceName: string,
    sourceEntry: ApiEntry,
    targetName: string,
    transform: ApiEntryTransform
  ) {
    const definition = getEnumDefinition(transform) ?? {};
    const valueType = definition.valueType ?? targetName;
    if (!transform.kind && !transform.type) {
      if (sourceEntry.kind === "alias") {
        transform.type = sourceEntry.type;
      } else {
        transform.kind = "alias";
        transform.type = sourceName;
      }
    }

    let values = definition.values ?? Object.keys(sourceEntry.entries ?? {});
    const prefix = definition.prefix ?? sourceName.toUpperCase() + "_";
    const newNames: StringMap = {};
    if (!values?.length) {
      values = Object.values(sourceEntries)
        .filter(
          (e) =>
            e.kind === "def" &&
            !e.parameters &&
            e.name.startsWith(prefix) &&
            !transformMap[e.name]?.type
        )
        .map((e) => e.name);
      const newPrefix = definition.newPrefix;
      if (newPrefix) {
        const oldPrefixLen = prefix.length;
        values.forEach(
          (n) => (newNames[n] = newPrefix + n.slice(oldPrefixLen))
        );
      }
    }
    const after = transform.after;
    if (after) context.includeAfter(targetName, after);
    const since =
      transform.since ?? resolveVersionDoc(sourceEntry.doc, context);

    for (const value of values) {
      const valueSource = sourceEntries[value];
      const valueTransform = file.transform[value];
      delete file.transform[value];
      const valueTarget: ApiEntryTransform & ApiEntry = {
        kind: "var",
        name: newNames[value] ?? transformName(value, context),
        constexpr: true,
        type: valueType,
        after: targetName,
        since,
      };
      combineObject(valueTarget, valueTransform || {});
      if (!valueSource?.doc) {
        const sourceDoc = valueSource?.doc ?? sourceEntry.entries?.[value]?.doc;
        valueTarget.doc = sourceDoc || [
          value.startsWith(prefix)
            ? value.slice(prefix.length)
            : valueTarget.name,
        ];
      }
      context.addName(value, valueTarget.name);
      if (!valueSource) valueTarget.sourceName = value;
      file.transform[value] = valueTarget;
    }
    delete transform.enum;
  }

  function expandLock(targetName: string, targetEntry: ApiEntryTransform) {
    const lockDef = getLockDefinition(targetEntry, targetName);
    delete targetEntry.lock;
    context.includeAfter(
      {
        kind: "forward",
        name: targetName,
      },
      "__begin"
    );
    if (!transformMap[targetName]) transformMap[targetName] = targetEntry;

    addHints(targetEntry, { private: true });
    if (!targetEntry.doc) addHints(targetEntry, { copyDoc: lockDef.lockFunc });
    if (!targetEntry.kind) targetEntry.kind = "struct";
    const controlType = lockDef.controlType;
    const paramType = lockDef.paramType ?? controlType;

    const ctors: Dict<ApiEntryTransform> = {};
    if (lockDef.controlVar !== false) {
      ctors["m_lock"] = {
        kind: "var",
        type: controlType ?? "bool",
      };
    }
    ctors[targetName] = { kind: "plc" };
    ctors[`~${targetName}`] = { kind: "plc" };
    ctors["operator="] = { kind: "plc" };
    ctors["operator bool"] = { kind: "plc" };
    const lockTargetName =
      file.transform[lockDef.lockFunc]?.name ??
      transformName(lockDef.lockFunc, context);
    const unlockTargetName =
      file.transform[lockDef.unlockFunc]?.name ??
      transformName(lockDef.unlockFunc, context);
    context.includeAfter(
      {
        kind: "function",
        name: `${targetName}::${targetName}`,
        type: "",
        parameters: paramType ? [{ type: paramType, name: "resource" }] : [],
        hints: {
          copyDoc: lockDef.lockFunc,
          changeAccess: "public",
          init: [`m_lock(${controlType ? "std::move(resource)" : "true"})`],
          body: `${lockTargetName}(${controlType ? "m_lock" : ""});`,
        },
      },
      lockDef.lockFunc
    );
    context.includeAfter(
      {
        kind: "function",
        name: `${targetName}.${targetName}`,
        type: "",
        parameters: [{ type: `const ${targetName} &`, name: "other" }],
        doc: ["Copy constructor"],
        hints: {
          delete: true,
        },
      },
      lockDef.lockFunc
    );
    context.includeAfter(
      {
        kind: "function",
        name: `${targetName}.${targetName}`,
        type: "",
        doc: ["Move constructor"],
        parameters: [{ type: `${targetName} &&`, name: "other" }],
        constexpr: true,
        hints: {
          init: ["m_lock(other.m_lock)"],
          body: `other.m_lock = {};`,
          noexcept: true,
        },
      },
      lockDef.lockFunc
    );
    context.includeAfter(
      {
        kind: "function",
        name: `${targetName}.~${targetName}`,
        type: "",
        parameters: [],
        hints: {
          copyDoc: lockDef.unlockFunc,
          body: `reset();`,
        },
      },
      lockDef.lockFunc
    );
    context.includeAfter(
      {
        kind: "function",
        name: `${targetName}.operator=`,
        type: `${targetName} &`,
        parameters: [{ type: `const ${targetName} &`, name: "other" }],
        static: false,
        hints: {
          delete: true,
        },
      },
      lockDef.lockFunc
    );
    context.includeAfter(
      {
        kind: "function",
        name: `${targetName}.operator=`,
        type: `${targetName} &`,
        parameters: [{ type: `${targetName} &&`, name: "other" }],
        static: false,
        doc: ["Assignment operator"],
        hints: {
          noexcept: true,
          body: `std::swap(m_lock, other.m_lock);return *this;`,
        },
      },
      lockDef.lockFunc
    );
    context.includeAfter(
      {
        kind: "function",
        name: `${targetName}.operator bool`,
        type: "",
        constexpr: true,
        immutable: true,
        parameters: [],
        doc: ["True if not locked."],
        hints: {
          body: `return bool(m_lock);`,
        },
      },
      lockDef.lockFunc
    );
    context.includeAfter(
      {
        kind: "function",
        name: `${targetName}::reset`,
        type: "void",
        parameters: [],
        hints: {
          copyDoc: lockDef.unlockFunc,
          body: controlType
            ? `if (!m_lock) return;\n${unlockTargetName}(m_lock);\nm_lock = {};`
            : `if (!m_lock) return;\n${unlockTargetName}();\nm_lock = false;`,
        },
      },
      lockDef.unlockFunc
    );

    if (controlType) {
      context.includeAfter(
        {
          kind: "function",
          name: `${targetName}.get`,
          type: controlType,
          parameters: [],
          doc: [`Get the reference to locked resource.`],
          hints: {
            body: "return m_lock;",
          },
        },
        lockDef.unlockFunc
      );
    }
    context.includeAfter(
      {
        kind: "function",
        name: `${targetName}.release`,
        type: "void",
        parameters: [],
        doc: [`Releases the lock without unlocking.`],
        hints: {
          body: controlType ? "m_lock.release();" : `m_lock = false;`,
        },
      },
      lockDef.unlockFunc
    );

    const subEntries = targetEntry.entries || {};
    targetEntry.entries = { ...ctors, ...subEntries };
  }
}

function getLockDefinition(
  targetEntry: ApiEntryTransform,
  sourceName: string
): LockDefinition {
  if (typeof targetEntry.lock === "object") {
    return {
      lockFunc: `SDL_Lock${sourceName}`,
      unlockFunc: `SDL_Unlock${sourceName}`,
      ...targetEntry.lock,
    };
  }
  if (!targetEntry.lock) return undefined;
  return {
    lockFunc: `SDL_Lock${sourceName}`,
    unlockFunc: `SDL_Unlock${sourceName}`,
  };
}

function transformEntries(
  sourceEntries: Dict<ApiEntry>,
  file: ApiFileTransform,
  context: ApiContext
) {
  const targetEntries: ApiEntries = {};
  const transformMap = file.transform;

  const sortedEntries = makeSortedEntryArray(sourceEntries, file, context);
  for (const targetEntry of sortedEntries) {
    const sourceName = targetEntry.sourceName;
    if (sourceName) context.addGlossary(sourceName, targetEntry);
    const targetDelta = transformMap[sourceName];
    insertLinks(targetEntry, targetDelta?.link ?? targetEntry.link);
    if (targetDelta) {
      combineObject(targetEntry, targetDelta);
      delete targetEntry.link;
      if (targetEntry.parameters) {
        targetEntry.parameters = targetEntry.parameters.filter(
          (p) => p.type || p.name
        );
      }
    }
    if (targetEntry.hints?.copyDoc) {
      const copyDoc = targetEntry.hints.copyDoc;
      delete targetEntry.hints.copyDoc;
      targetEntry.doc = transformDoc(sourceEntries[copyDoc]?.doc, context);
    }
    if (sourceName)
      context.addName(sourceName, targetEntry.name?.replaceAll("::", "."));
    insertEntryAndCheck(targetEntries, targetEntry, context, sourceEntries);
  }

  return targetEntries;

  function insertLinks(entry: ApiEntry, link: ApiEntryTransform) {
    if (!link) return;
    insertLinks(entry, link.link);
    delete link.link;

    const linkedEntry = deepClone(entry);
    if (link.kind || entry.kind !== "def") {
      combineObject(linkedEntry, link);
      link = linkedEntry.link;
      delete linkedEntry.link;
      if (linkedEntry.parameters) {
        linkedEntry.parameters = linkedEntry.parameters.filter(
          (p) => typeof p === "string" || (p.type && p.name)
        );
      }
      insertEntryAndCheck(targetEntries, linkedEntry, context, sourceEntries);
    }
  }
}

function makeSortedEntryArray(
  sourceEntries: Dict<ApiEntry>,
  file: ApiFileTransform,
  context: ApiContext
) {
  const transformEntries = file.transform ?? {};

  let lastSourceName = "__begin";
  for (const [sourceName, entryDelta] of Object.entries(transformEntries)) {
    if (sourceEntries[sourceName]) {
      lastSourceName = sourceName;
      const kind = entryDelta.kind ?? sourceEntries[sourceName].kind;
      if (kind !== "def") {
        const targetName =
          entryDelta.name ?? transformName(sourceName, context);
        entryDelta.name = targetName;
        if (entryDelta.before) {
          context.includeBefore(targetName, entryDelta.before);
        } else if (entryDelta.after) {
          context.includeAfter(targetName, entryDelta.after);
        }
      }
    } else {
      if (!entryDelta.name) entryDelta.name = sourceName;
      if (entryDelta.before) {
        lastSourceName = entryDelta.before;
        context.includeBefore(entryDelta, lastSourceName);
      } else {
        lastSourceName = entryDelta.after ?? lastSourceName;
        context.includeAfter(entryDelta, lastSourceName);
      }
    }
    delete entryDelta.before;
    delete entryDelta.after;
  }

  const sortedEntries: Dict<ApiEntry> = {};
  const countInstance: Dict<number> = {};

  const defPrefix = file.definitionPrefix;
  const includeBefore = context.currentIncludeBefore;
  const includeAfter = context.currentIncludeAfter;
  const processedSourceNames = new Set<string>();

  if (includeBefore.__begin) addIncluded(includeBefore.__begin);
  if (includeAfter.__begin) addIncluded(includeAfter.__begin);

  for (const [key, sourceEntry] of Object.entries(sourceEntries)) {
    const sourceName = sourceEntry.name;
    if (processedSourceNames.has(sourceName)) continue;
    processedSourceNames.add(sourceName);

    const targetEntry = {
      ...transformEntry(sourceEntry, context),
      name: transformName(sourceName, context),
      entries: undefined,
    };
    const targetDelta = transformEntries[sourceName];
    if (targetDelta) {
      if (targetDelta.name) targetEntry.name = targetDelta.name;
      targetEntry.kind = targetDelta.kind ?? targetEntry.kind;
      if (targetEntry.kind !== sourceEntry.kind && sourceEntry.kind === "def") {
        let replacement = "constant";
        if (targetDelta.type === undefined) targetEntry.type = "auto";
        if (targetEntry.kind === "function") replacement = "function";
        if (!targetDelta.doc && targetEntry.doc) {
          const macro = getTagInGroup(targetEntry.doc, "@since");
          if (macro)
            macro.content = macro.content.replaceAll("macro", replacement);
          const tSafety = getTagInGroup(targetEntry.doc, "@threadsafety");
          if (tSafety)
            tSafety.content = tSafety.content.replaceAll("macro", replacement);
        }
      }
    } else if (context.blacklist.has(sourceName)) {
      continue;
    }
    const targetName = targetEntry.name;
    const firstAppearance = !processedSourceNames.has(targetName);
    processedSourceNames.add(targetName);

    if (key !== sourceName) addIncluded(includeBefore[key]);
    addIncluded(includeBefore[sourceName]);
    if (firstAppearance) addIncluded(includeBefore[targetName]);

    if (targetEntry.kind === "def") {
      if (!targetName.startsWith(defPrefix)) {
        targetEntry.name = defPrefix + targetName;
      }
    }
    if (targetDelta?.entries) {
      targetEntry.entries = targetDelta.entries;
      delete targetDelta.entries;
    }
    addTransform(targetEntry);

    if (firstAppearance) addIncluded(includeAfter[targetName]);
    if (key !== sourceName) addIncluded(includeAfter[key]);
    addIncluded(includeAfter[sourceName]);
  }

  if (includeBefore.__end) addIncluded(includeBefore.__end);
  if (includeAfter.__end) addIncluded(includeAfter.__end);

  return Object.values(sortedEntries);

  function addIncluded(transformEntries?: ApiEntryTransform[]) {
    if (!transformEntries) return;
    for (const entryDelta of transformEntries) {
      const name = entryDelta.name;
      let checkSubIncludes = true;
      if (processedSourceNames.has(name)) {
        checkSubIncludes = false;
        if (entryDelta.kind !== "function") continue;
      } else if (entryDelta.kind === "forward") {
        checkSubIncludes = false;
      } else processedSourceNames.add(name);

      if (checkSubIncludes) addIncluded(includeBefore[name]);
      addTransform(entryDelta);
      if (checkSubIncludes) addIncluded(includeAfter[name]);
    }
  }

  function addTransform(entryDelta: ApiEntryTransform) {
    const currEntry = sortedEntries[entryDelta.name];
    const currKind = currEntry?.kind;
    const nextName =
      entryDelta.kind === "forward"
        ? entryDelta.name + "#forward"
        : entryDelta.name;
    const targetEntry = {
      ...entryDelta,
      kind: entryDelta.kind ?? "plc",
      name: entryDelta.name ?? "",
      entries: undefined,
    } as ApiEntry;
    if (entryDelta.entries)
      targetEntry.entries = transformSubEntries(entryDelta);
    if (!currKind) {
      sortedEntries[nextName] = targetEntry;
      return;
    }
    const nextKind = entryDelta.kind;
    if (currKind === "function") {
      if (nextKind !== "function") return;
      const n = (countInstance[nextName] ?? 1) + 1;
      countInstance[nextName] = n;
      sortedEntries[`${nextName}#${n}`] = targetEntry;
      return;
    }
    combineObject(currEntry, targetEntry);
  }

  function transformSubEntries(targetEntry: ApiEntryTransform) {
    /** @type {ApiEntries} */
    const entries = {};
    const type = targetEntry.name;
    for (const [key, entry] of Object.entries(targetEntry.entries)) {
      transformSubEntry(key, entry);
    }
    return entries;

    function transformSubEntry(
      key: string,
      entry: ApiEntryTransform | QuickTransform
    ) {
      const sourceEntry = context.source[key];
      const nameCandidate = transformName(key, context);
      if (!sourceEntry) {
        if (typeof entry === "string") {
          const newEntry: ApiEntry = { kind: "function", name: nameCandidate };
          if (entry === "ctor") {
            newEntry.name = newEntry.type = type;
          } else if (entry === "immutable") {
            newEntry.immutable = true;
          } else {
            newEntry.kind = entry;
          }
          insertEntry(entries, newEntry);
        } else if (entry.hints?.delegate) {
          context.includeAfter(
            { ...entry, name: `${type}::${nameCandidate}` },
            entry.hints.delegate
          );
          if (!entries[nameCandidate]) {
            insertEntry(entries, { name: nameCandidate, kind: "plc" });
          }
        } else {
          insertEntry(entries, entry as ApiEntry, nameCandidate);
        }
        return;
      }
      const nameChange = makeRenameEntry(entry, nameCandidate, type);
      if (context.blacklist.has(key)) addTransform(nameChange);
      if (!entries[nameChange.name]) {
        insertEntry(entries, { name: nameChange.name, kind: "plc" });
      }
      const currLink = file.transform[key];
      const currLinkName = currLink?.name ?? transformName(key, context);
      let isCtor =
        entry === "ctor" ||
        (typeof entry !== "string" &&
          !entry.name?.startsWith("operator") &&
          entry.type === "");
      if (isCtor || (typeof entry !== "string" && entry.proto === false)) {
        nameChange.name = `${type}.${nameChange.name}`;
      } else {
        nameChange.name = `${type}::${nameChange.name}`;
        addHints(nameChange, {
          delegate: `${context.namespace}::${currLinkName}`,
        });
      }
      if (typeof entry !== "string") delete nameChange.proto;
      file.transform[key] = nameChange;
      if (currLink) {
        currLink.name = currLinkName;
        if (isCtor) {
          let l = currLink;
          while (l) {
            addHints(l, { delegate: type });
            l = l.link;
          }
        }
        file.transform[key].link = currLink;
      }
    }
  }
}

function insertTransform(
  entries: Dict<ApiEntryTransform>,
  entryDelta: ApiEntryTransform,
  defaultName: string = undefined
) {
  if (defaultName) entryDelta.name = entryDelta.name ?? defaultName;
  const currEntry = entries[entryDelta.name];
  const currKind = currEntry?.kind;
  const nextName =
    entryDelta.kind === "forward"
      ? entryDelta.name + "#forward"
      : entryDelta.name;
  if (!currKind) {
    entries[nextName] = entryDelta;
    return;
  }
  const nextKind = entryDelta.kind;
  if (currKind === "function") {
    if (nextKind !== "function") return;
    const n = makeSignatureSuffix(entryDelta.parameters);
    entries[`${nextName}#${n}`] = entryDelta;
    return;
  }
  combineObject(currEntry, entryDelta);
}

function makeSignatureSuffix(parameters: ApiParameter[]) {
  return parameters.map((p) => p.type ?? "_").join(",");
}

function expandNamespaces(
  sourceEntries: Dict<ApiEntry>,
  file: ApiFileTransform,
  context: ApiContext
) {
  const namespacesMap = file.namespacesMap ?? {};
  for (const [prefix, nsName] of Object.entries(namespacesMap)) {
    const nsEntries: Dict<ApiEntryTransform> = {};
    const sourceEntriesListed = Object.entries(sourceEntries).filter(([key]) =>
      key.startsWith(prefix)
    );
    for (const [key, entry] of sourceEntriesListed) {
      const entryDelta = file.transform[key] || {};
      const localName = entry.name.slice(prefix.length);
      file.transform[key] = entryDelta;
      entryDelta.name = nsName + "." + localName;

      if (entry.kind === "def" && !entryDelta.kind) {
        entryDelta.kind = "var";
        entryDelta.type = "auto";
        entryDelta.constexpr = true;
      }
      context.addName(key, `${nsName}.${localName}`);
    }
    if (sourceEntriesListed.length) {
      context.includeBefore(
        { kind: "ns", name: nsName, entries: nsEntries },
        sourceEntriesListed[0][0]
      );
    }
  }
}

function addHints(entry: ApiEntryBase, hints: EntryHint) {
  if (entry.hints) {
    combineObject(entry.hints, hints);
  } else {
    entry.hints = hints;
  }
}

/**
 *
 * @param {ApiEntryBase}  entry
 * @param {EntryHint}     hints
 */
function combineHints(entry: ApiEntryBase, hints: EntryHint) {
  if (entry.hints) {
    entry.hints = { ...hints, ...entry.hints };
  } else {
    entry.hints = { ...hints };
  }
}

function mirrorMethods(
  sourceEntries: Dict<ApiEntry>,
  transformEntries: Dict<ApiEntryTransform>,
  transformSubEntries: ApiEntryTransformMap,
  paramType: string,
  constParamType: string,
  resultType: string
) {
  for (const [sourceName, subEntry] of Object.entries(transformSubEntries)) {
    const sourceEntry = sourceEntries[sourceName];
    if (!sourceEntry) continue;
    if (
      sourceEntry.kind !== "function" &&
      typeof subEntry !== "string" &&
      subEntry.kind !== "function"
    )
      continue;

    const entryDelta = transformEntries[sourceName];
    if (entryDelta?.type || entryDelta?.parameters) continue;
    let targetEntry: ApiEntryTransform;
    if (typeof subEntry === "string") {
      targetEntry = {};
      if (subEntry === "immutable") targetEntry.immutable = true;
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
    if (entryDelta) combineObject(targetEntry, entryDelta);
    transformEntries[sourceName] = targetEntry;
  }

  function mirrorParameters(
    sourceEntry: ApiEntry,
    targetEntry: ApiEntryTransform
  ) {
    if (targetEntry.static) return;

    const targetParameters: ApiParameters =
      targetEntry.parameters ?? sourceEntry?.parameters?.map(() => ({}));
    if (!targetParameters) return;

    const needSaveParameters = !targetEntry.parameters;
    let parametersChanged = false;
    const targetParam0 = targetParameters[0];

    const sourceParam0 = sourceEntry.parameters?.[0];
    if (
      !targetParam0 ||
      typeof targetParam0 === "string" ||
      targetParam0.type
    ) {
      switch (typeof sourceParam0) {
        case "object": {
          const selfParam: ApiParameter = { name: sourceParam0.name };
          if (!sourceParam0.type) {
            selfParam.type = targetEntry.immutable ? constParamType : paramType;
          } else if (sourceParam0.type.includes(resultType)) {
            if (
              !sourceParam0.type.startsWith("const ") &&
              targetEntry.immutable
            )
              selfParam.type = constParamType;
          }
          targetParameters.unshift(selfParam);
          parametersChanged = true;
          break;
        }
        case "string":
          if (
            typeof targetParam0 !== "object" ||
            sourceParam0 !== targetParam0.name
          ) {
            targetParameters.unshift({
              name: sourceParam0,
              type: constParamType,
            });
            parametersChanged = true;
          }
          break;
      }
    } else if (
      typeof sourceParam0 !== "object" ||
      typeof targetParam0 !== "object"
    ) {
      return;
    } else if (!targetParam0.name || targetParam0.name == sourceParam0.name) {
      if (sourceParam0.type?.includes(resultType) || sourceParam0.type === "") {
        targetParam0.type =
          targetEntry.immutable || sourceParam0.type.startsWith("const ")
            ? constParamType
            : paramType;
        parametersChanged = true;
      }
    }
    if (needSaveParameters && parametersChanged)
      targetEntry.parameters = targetParameters;
  }
}

function paramMatchesVariants(
  param0: ApiParameter,
  variants: string[],
  constVariants: string[]
) {
  for (const variant of variants) {
    if (param0.type === variant) return "function";
  }
  for (const variant of constVariants) {
    if (param0.type === variant) return "immutable";
  }
  return false;
}

function getResourceDefinition(entry: ApiEntryTransform) {
  const resourceDef = entry.resource;
  switch (typeof resourceDef) {
    case "string":
      return { shared: resourceDef };
    case "boolean":
      return resourceDef ? {} : null;
    case "object":
      return resourceDef;
    default:
      return undefined;
  }
}

function getEnumDefinition(entry: ApiEntryTransform) {
  const enumDef = entry.enum;
  switch (typeof enumDef) {
    case "string":
      return { prefix: enumDef };
    case "boolean":
      return {};
    case "object":
      return enumDef;
    default:
      return undefined;
  }
}

function scanFreeFunction(
  entries: Dict<ApiEntry>,
  uniqueType: string,
  pointerType: string
) {
  const candidates: ApiEntry[] = [];
  for (const sourceName of Object.keys(entries)) {
    if (looksLikeFreeFunction(sourceName)) {
      const sourceEntry = entries[sourceName];
      if (sourceEntry?.parameters?.length !== 1) continue;
      const firstParameter = sourceEntry.parameters[0];
      if (
        sourceEntry.name.endsWith(uniqueType) ||
        (typeof firstParameter !== "string" &&
          firstParameter.type === pointerType)
      ) {
        candidates.push(sourceEntry);
      }
    }
  }
  if (candidates.length === 1) return candidates[0];
}

/**
 * Insert entry into entries
 */
function insertEntry(
  entries: ApiEntries,
  entry: ApiEntry | ApiEntry[],
  defaultName: string = ""
) {
  if (Array.isArray(entry)) entry.forEach(doInsertEntry);
  else doInsertEntry(entry);
  return entries;

  /**
   * Insert entry into entries
   */
  function doInsertEntry(entry: ApiEntry) {
    if (!entry.name) entry.name = defaultName;
    entry = /** @type {ApiEntry} */ entry;
    fixEntry(entry);
    const name = entry.kind == "forward" ? entry.name + "-forward" : entry.name;
    const key = name.startsWith("ObjectRef")
      ? name
      : name.replace(/<[^>]*>::/, "::");
    if (entries[key]) {
      const currEntry = entries[key];
      if (currEntry.kind !== "function") {
        if (entry.doc || !currEntry.doc) {
          if (entry.kind === "def") {
            currEntry.doc = [...entry.doc];
          } else {
            entries[key] = entry;
          }
        }
      } else if (entry.kind === "function") {
        let e = currEntry;
        while (e.overload) e = e.overload;
        e.overload = entry;
        if (!entry.doc && currEntry.doc) entry.doc = [...currEntry.doc];
      }
    } else {
      entries[key] = entry;
    }
    return entries;
  }

  /**
   * Add missing fields
   */
  function fixEntry(entry: ApiEntry) {
    if (entry.entries) {
      for (const subEntry of Object.values(entry.entries)) {
        if (Array.isArray(subEntry)) {
          subEntry.forEach(fixEntry);
        } else if (typeof subEntry === "object") {
          fixEntry(subEntry);
        }
      }
    } else if (entry.kind == "struct") {
      entry.entries = {};
    }
    if (!entry.kind) entry.kind = "plc";
  }
}

/**
 * Insert entry into entries
 */
function insertEntryAndCheck(
  entries: ApiEntries,
  entry: ApiEntry,
  context: ApiContext,
  sourceEntries: ApiEntries,
  defaultName?: string
) {
  insertEntry(entries, entry, defaultName);
  if (entry.kind === "ns" || entry.kind === "struct") {
    const currType = context.types[entry.name];
    if (currType) {
      if (currType.kind !== "ns" && entry.kind !== "ns")
        system.warn(`Duplicate type definition ${entry.name}`);
    } else {
      context.types[entry.name] = entry as ApiType;
    }
  }

  checkCopyDoc(entry, sourceEntries, context);
}

function checkCopyDoc(
  entry: ApiEntry,
  sourceEntries: ApiEntries,
  context: ApiContext
) {
  if (!entry.doc && entry.hints?.copyDoc) {
    const copyDoc = entry.hints.copyDoc;
    delete entry.hints.copyDoc;
    entry.doc = transformDoc(sourceEntries[copyDoc]?.doc, context);
  }
  if (entry.entries) {
    Object.values(entry.entries).forEach((subEntry) =>
      checkCopyDoc(subEntry, sourceEntries, context)
    );
  }
  if (entry.overload) {
    checkCopyDoc(entry.overload, sourceEntries, context);
  }
}

/**
 * Fix hierarchy, executing any pending movements
 */
function transformHierarchy(targetEntries: ApiEntries, context: ApiContext) {
  for (const key of Object.keys(targetEntries)) {
    if (!key.includes(".") && !key.includes("::")) continue;
    const isMove = key.includes(".");
    const path = key.split(/\.|::/);
    const obj =
      getTypeFromPath(path, context) ??
      (isMove ? getTypeFromPath(key.split("."), context) : null);
    if (!obj) continue;
    let entry = targetEntries[key];
    const typeName = obj.name;
    const targetName = path.at(-1);
    if (isMove) {
      delete targetEntries[key];
      while (entry) {
        const nextEntry = entry.overload;
        prepareForTypeInsert(entry, targetName, typeName);
        insertEntry(obj.entries, { ...entry, overload: undefined });
        entry = nextEntry;
      }
    } else {
      while (entry) {
        const nextEntry = entry.overload;
        prepareForTypeInsert(entry, targetName, typeName);
        insertCopyEntry(entry);
        delete entry.hints?.changeAccess;
        entry = nextEntry;
      }
    }

    function insertCopyEntry(entry: ApiEntry) {
      insertEntry(obj.entries, {
        ...entry,
        proto: true,
        overload: undefined,
      });
      entry.name = makeMemberName(key, obj.template);
      if (obj.template) entry.template = obj.template;
      if (entry.parameters)
        entry.parameters = entry.parameters.map((p) => {
          p = deepClone(p);
          delete p.default;
          return p;
        });
      if (obj.hints) combineHints(entry, obj.hints);
      if (entry.static) addHints(entry, { static: true });
      delete entry.static;
      delete entry.explicit;
      if (!entry.since) entry.since = resolveVersionDoc(entry.doc, context);
      delete entry.doc;
    }
  }
}
function makeMemberName(key: string, template: ApiParameters) {
  if (!template) return key;
  const lastSeparator = key.lastIndexOf("::");
  const args = generateCallParameters(template, {});
  return `${key.slice(0, lastSeparator)}<${args}>${key.slice(lastSeparator)}`;
}

/**
 * Get element from path
 */
function getTypeFromPath(path: string[], context: ApiContext) {
  let obj = context.types[path[0]];
  if (!obj?.entries) return null;
  let i = 1;
  for (; i < path.length - 1; i++) {
    const el = obj.entries[path[i]];
    if (!el?.entries) {
      return null;
    }
    obj = el as ApiType;
  }
  return obj;
}

/**
 * Validate and report entries
 */
function validateEntries(targetEntries: ApiEntries) {
  // Do nothing for now
}

/**
 * Marshal name of member functions
 */
function makeRenameEntry(
  entry: ApiEntryTransform | string,
  name: string,
  typeName: string
): ApiEntryTransform {
  let newEntry: ApiEntryTransform = {};
  if (entry === "placeholder") {
    newEntry = {};
  } else if (entry === "ctor") {
    newEntry = { kind: "function", name: typeName, type: "" };
  } else if (entry === "immutable") {
    newEntry = { kind: "function", immutable: true };
  } else if (typeof entry !== "object") {
    newEntry = { kind: entry as ApiEntryKind };
  } else if (entry.name === "ctor") {
    newEntry = {
      ...entry,
      kind: "function",
      name: typeName,
      type: "",
      proto: false,
    };
  } else {
    newEntry = entry;
  }
  return {
    ...newEntry,
    name: newEntry.name || makeNaturalName(name, typeName),
  };
}

function transformMemberName(
  name: string,
  typeName: string,
  context: ApiContext
) {
  return makeNaturalName(transformName(name, context), typeName);
}

function makeNaturalName(name: string, typeName: string) {
  typeName = normalizeTypeName(typeName);
  if (!typeName.startsWith("F")) return name.replace(new RegExp(typeName), "");
  const replaceRegexp = new RegExp("F?" + typeName.slice(1));
  name = name.replace(replaceRegexp, "");
  if (name.endsWith("Float")) name = name.slice(0, -5);
  return name;
}

/**
 * Prepare function to become class or instance function
 */
function prepareForTypeInsert(entry: ApiEntry, name: string, typeName: string) {
  entry.name = name;
  if (name === typeName) {
    if (entry.doc) {
      const r = getTagInGroup(entry.doc, "@returns");
      if (r) r.tag = "@post";
    }
    entry.type = "";
    return;
  }
  typeName = normalizeTypeName(typeName);
  const parameters = entry.parameters;
  if (!parameters?.length) return;
  const parameter = parameters[0];
  const type = parameter.type ?? "";
  if (
    (type.includes(typeName) || entry.hints?.removeParamThis) &&
    !entry.static &&
    entry.sourceName
  ) {
    parameters.shift();
    if (entry.doc) removeTagFromGroup(entry.doc, "@param");
    if (type.startsWith("const ")) entry.immutable = true;
  } else if (entry.static !== false) {
    entry.static = !entry.immutable;
  }
}

function normalizeTypeName(typeName: string) {
  if (typeName.endsWith("Ref")) return typeName.slice(0, -3);
  return typeName;
}

/**
 *
 * @param {ApiEntry}    sourceEntry
 * @param {ApiContext}  context
 */
function transformEntry(sourceEntry: ApiEntry, context: ApiContext) {
  const targetEntry: ApiEntry = { ...sourceEntry };
  if (sourceEntry.doc) {
    targetEntry.doc = transformDoc(targetEntry.doc, context);
  }
  const sourceName = sourceEntry.name;
  targetEntry.sourceName = sourceName;
  switch (sourceEntry.kind) {
    case "function":
      targetEntry.parameters = transformParameters(
        sourceEntry.parameters,
        context
      );
      targetEntry.type = transformType(sourceEntry.type, context.returnTypeMap);
      checkSignatureRules(targetEntry, context);
      if (context.enableException) {
        const r = getTagInGroup(targetEntry.doc, "@returns");
        if (r) {
          const m = r.content.match(
            /(.*) on success|(an? valid [^,]+), or (?:\w+) on failure/
          );
          if (m) {
            targetEntry.hints = { mayFail: true };
            if (sourceEntry.type === "bool") {
              targetEntry.type = "void";
              r.content = "Error on failure.";
              r.tag = "@throws";
            } else {
              addToTagGroup(targetEntry.doc, "@returns", "Error on failure.");
              r.content = `${m[1] || m[2]} on success.`;
              getTagInGroup(targetEntry.doc, "@returns", 1).tag = "@throws";
            }
          }
        }
      }
      break;
    case "def":
      targetEntry.parameters = sourceEntry.parameters;
      break;
    default:
      delete targetEntry.parameters;
      break;
  }
  delete targetEntry.entries;
  return targetEntry;
}

function transformParameters(
  parameters: ApiParameters,
  context: ApiContext
): ApiParameters {
  return parameters.map((parameter) => {
    let { name, type, default: defaultValue } = parameter;
    type = transformType(type, context.paramTypeMap);
    return { name, type, default: defaultValue };
  });
}

function transformCbParameters(
  parameters: ApiParameters,
  context: ApiContext
): ApiParameters {
  return parameters.map((parameter) => {
    let { name, type, default: defaultValue } = parameter;
    if (type) {
      type = transformCbType(type, context);
    }
    return { name, type, default: defaultValue };
  });
}
function transformCbType(type: string, context: ApiContext): string {
  return type.replace(
    /^(const\s+)?(\w+(\s+\*)?)/,
    (_, ct: string, type: string) => (ct ?? "") + context.getCallbackType(type)
  );
}

function checkSignatureRules(targetEntry: ApiEntry, context: ApiContext) {
  const parameters = /** @type {ApiParameter[]}*/ targetEntry.parameters;
  if (parameters.some((p) => typeof p === "string")) return;
  for (const signatureRule of context.signatureRules) {
    if (parameters.length < signatureRule.pattern.length) continue;
    for (
      let i = 0;
      i <= parameters.length - signatureRule.pattern.length;
      i++
    ) {
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
        const replaceParams = signatureRule.replaceParams.map((p) => ({
          ...p,
        }));
        parameters.splice(i, signatureRule.pattern.length, ...replaceParams);
        break;
      }
    }
  }
}

function transformType(type: string, typeMap: StringMap) {
  return typeMap[type] ?? type;
}

function transformFileDoc(doc: ParsedDoc, context: ApiContext) {
  if (!doc?.length) return;
  const title = doc[0];
  if (typeof title === "object" && !Array.isArray(title) && title.tag === "#") {
    title.tag = `@defgroup ${title.content}`;
    title.content = title.content.replace(/(Category)(\w+)/, "$1 $2");
  }
  return transformDoc(doc, context);
}

function transformDoc(doc: ParsedDoc, context: ApiContext): ParsedDoc {
  return doc?.map(transformDocItem);

  function transformDocItem(docStr: ParsedDocContent): ParsedDocContent {
    if (typeof docStr === "string") return transformDocStr(docStr);
    if (Array.isArray(docStr))
      return docStr.map(transformDocItem) as ParsedDocContent;
    const r = { ...docStr };
    r.content = transformDocStr(r.content);
    if (r.tag) r.tag = r.tag.replace("\\", "@");
    return r;
  }
  function transformDocStr(docStr: string) {
    return transformString(docStr, context.docRules)
      .replaceAll(/\\(\w+)/g, "@$1")
      .replaceAll("NULL", "nullptr");
  }
}

function transformIncludeName(name: string, context: ApiContext) {
  return transformString(name, context.renameRules);
}

/**
 *
 * @param str the string to apply to
 * @param rules the replacements to apply
 */
function transformString(str: string, rules: ReplacementRule[]) {
  rules.forEach((rule) => (str = str.replace(rule.pattern, rule.replacement)));
  return str;
}

function transformName(name: string, context: ApiContext) {
  name = name.replace(/#.*$/, "");
  return context?.prefixToRemove
    ? name.replace(context.prefixToRemove, "")
    : name;
}

function transformEntriesDocRefs(entries: ApiEntries, context: ApiContext) {
  for (const entry of Object.values(entries)) {
    transformEntryDoc(entry);
  }

  function transformEntryDoc(entry: ApiEntry) {
    if (entry.doc) {
      entry.doc = resolveDocRefs(entry.doc, context);
      if (!entry.since) entry.since = resolveVersionDoc(entry.doc, context);
    }
    if (entry.entries) transformEntriesDocRefs(entry.entries, context);
    if (entry.overload) transformEntryDoc(entry.overload);
  }
}

function resolveVersionDoc(doc: ParsedDoc, context: ApiContext) {
  const sinceTag = getTagInGroup(doc, "@since");
  if (!sinceTag) return;
  const m = /\b(\w+)\s*(\d+)\.(\d+)\.(\d+)\.$/m.exec(sinceTag.content);
  if (!m) return undefined;
  const version: VersionTag = {
    tag: m[1].toUpperCase(),
    major: +m[2],
    minor: +m[3],
    patch: +m[4],
  };
  if (context.isAfterMinVersion(version))
    return context.previewFutureVersion(version);
}

function resolveDocRefs(doc: ParsedDoc, context: ApiContext): ParsedDoc {
  return doc?.map(resolveDocRefItem);

  function resolveDocRefItem(item: ParsedDocContent): ParsedDocContent {
    if (typeof item === "string") return resolveDocRefStr(item);
    if (Array.isArray(item))
      return item.map(resolveDocRefItem) as ParsedDocContent;
    return { ...item, content: resolveDocRefStr(item.content) };
  }

  function resolveDocRefStr(docStr: string) {
    return docStr.replaceAll(context.referenceCandidate, (ref) =>
      context.getName(ref)
    );
  }
}

function addToTagGroup(doc: ParsedDoc, tag: string, content: string) {
  if (!doc) return;
  for (let i = doc.length - 1; i >= 0; i--) {
    const item = doc[i];
    if (typeof item !== "object") continue;
    if (Array.isArray(item)) {
      for (let j = item.length - 1; j >= 0; j--) {
        const subItem = item[j];
        if (matchTag(subItem.tag, tag)) {
          item.splice(j, 1, subItem, { tag, content });
          return doc;
        }
      }
    } else if (matchTag(item.tag, tag)) {
      doc.splice(i, 1, item, { tag, content });
      return doc;
    }
  }
  doc.push({ tag, content });
  return doc;
}

function removeTagFromGroup(doc: ParsedDoc, tag: string, count: number = 0) {
  if (!doc) return;
  return removeItemFromGroup(doc, findTagInGroup(doc, tag, count));
}

function removeItemFromGroup(doc: ParsedDoc, r: number | [number, number]) {
  if (!doc) return;
  if (Array.isArray(r)) {
    const l = doc[r[0]] as ListContent;
    l.splice(r[1], 1);
  } else if (r !== -1) {
    doc.splice(r, 1);
  }
  return doc;
}

function findTagInGroup(
  doc: ParsedDoc,
  tagOrFunction: string | ((tag: string) => boolean),
  count: number = 0
): number | [number, number] {
  if (!doc) return -1;
  const test =
    typeof tagOrFunction === "function"
      ? tagOrFunction
      : (item) => matchTag(item, tagOrFunction);
  for (let i = 0; i < doc.length; i++) {
    const item = doc[i];
    if (typeof item !== "object") continue;
    if (Array.isArray(item)) {
      for (let j = 0; j < item.length; j++) {
        const subItem = item[j];
        if (test(subItem.tag) && count-- === 0) return [i, j];
      }
    } else if (test(item.tag) && count-- === 0) return i;
  }
  return -1;
}

function getItemInGroup(
  doc: ParsedDoc,
  index: number | [number, number]
): ParsedDocContent {
  if (Array.isArray(index)) {
    return doc?.[index[0]]?.[index[1]];
  } else {
    return doc?.[index];
  }
}

function getTagInGroup(
  doc: ParsedDoc,
  tagOrFunction: string | ((tag: string) => boolean),
  count: number = 0
) {
  const r = findTagInGroup(doc, tagOrFunction, count);
  return getItemInGroup(doc, r) as TaggedContent;
}

function matchTag(item: string, tag: string) {
  return tag === item || item?.startsWith(tag + " ");
}
