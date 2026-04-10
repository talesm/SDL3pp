import {
  addHints,
  ApiContext,
  combineHints,
  createBlockedNames,
  detectMethods,
  insertTransform,
  makeNaturalName,
  mirrorMethods,
  scanFreeFunction,
  transformDoc,
  transformEntry,
  transformMemberName,
  transformName,
  wrapLockFunctions,
} from "./transform";
import { getLockDefinition } from "./expandLock";
import {
  ApiEntry,
  ApiEntryTransform,
  ApiEntryTransformMap,
  ApiFileTransform,
  Dict,
  LockDefinition,
  ResourceDefinition,
} from "./types";
import { combineObject, system } from "./utils";

export function expandResource(
  sourceEntries: Dict<ApiEntry>,
  file: ApiFileTransform,
  context: ApiContext,
  sourceName: string,
  sourceEntry: ApiEntry,
  targetName: string,
  targetEntry: ApiEntryTransform,
) {
  const resourceEntry = getResourceDefinition(targetEntry) ?? {};

  const rawName = resourceEntry.rawName || `${targetName}Raw`;
  const constRawName = `${rawName}Const`;
  const refName = `${targetName}Ref`;
  const enableMemberAccess =
    resourceEntry.enableMemberAccess ?? sourceEntry.kind === "struct";
  const enableConstParam = resourceEntry.enableConstParam ?? enableMemberAccess;
  const constParamType = enableConstParam ? `${targetName}ConstRef` : refName;
  if (!targetEntry.kind) targetEntry.kind = "struct";
  const shared = resourceEntry.shared;
  const hasShared = !!shared;
  const hasScoped = resourceEntry.owning === false;
  const hasRef = resourceEntry.ref ?? !hasScoped;
  const scopedName = `${targetName}Scoped`;

  const type = targetEntry.type ?? sourceName;
  const isStruct =
    sourceEntry.kind === "struct" ||
    (sourceEntry.kind === "alias" && sourceEntry.type.startsWith("struct "));
  const pointerType = isStruct ? `${type} *` : type;
  const constPointerType = `const ${pointerType}`;
  const nullValue = isStruct ? "nullptr" : "0";
  const title = targetName[0].toLowerCase() + targetName.slice(1);

  if (!targetEntry.type) {
    targetEntry.type = enableConstParam
      ? `ResourceBase<${rawName}, ${constRawName}>`
      : `ResourceBase<${rawName}>`;
  }
  context.addName(sourceName, targetName);
  context.addCallbackType(isStruct ? `${sourceName} *` : sourceName, rawName);

  const referenceAliases: ApiEntryTransform[] = [];
  referenceAliases.push(
    { name: targetName, kind: "forward" },
    {
      name: rawName,
      kind: "alias",
      type: pointerType,
      doc: [`Alias to raw representation for ${targetName}.`],
    },
  );
  if (enableConstParam || enableMemberAccess) {
    referenceAliases.push({
      name: constRawName,
      kind: "alias",
      type: constPointerType,
      doc: [`Alias to const raw representation for ${targetName}.`],
    });
  }
  if (hasRef) referenceAliases.push({ name: refName, kind: "forward" });
  if (hasScoped) referenceAliases.push({ name: scopedName, kind: "forward" });
  const hasLock = getLockDefinition(targetEntry, targetName);
  let lockName = makeResourceLock(
    file,
    targetName,
    hasRef ? refName : targetName,
    targetEntry,
    hasLock,
  );

  if (hasScoped) {
    referenceAliases.push({
      kind: "alias",
      name: refName,
      type: targetName,
      doc: [`Alias to ${targetName} for non owning parameters.`],
    });
  } else if (!hasRef) {
    referenceAliases.push(createParam(refName, targetName, rawName));
  }

  if (enableConstParam) {
    referenceAliases.push(
      createConstParam(constParamType, targetName, rawName, constRawName),
    );
  }
  setupTypeTranslations(
    hasScoped,
    context,
    pointerType,
    targetName,
    constPointerType,
    refName,
    enableConstParam,
    constParamType,
    hasShared,
    hasRef,
    refName,
    rawName,
    constRawName,
  );

  const ctors = createBaselineCtors(
    hasScoped,
    targetName,
    shared,
    hasRef ? refName : undefined,
    rawName,
    isStruct,
  );
  const subEntries = targetEntry.entries || {};

  wrapCustomCtors(subEntries, targetName, ctors);
  createCustomFactories(
    resourceEntry,
    subEntries,
    file,
    targetName,
    context,
    sourceEntries,
    ctors,
  );
  wrapCtors(subEntries, file, targetName, ctors, sourceEntries);

  const destroyFunction = createOrDetectDestroyFunction(
    sourceEntries,
    resourceEntry,
    targetName,
    pointerType,
    context,
    subEntries,
    title,
    file,
    rawName,
  );
  if (resourceEntry.enableAutoMethods === false) {
    mirrorMethods(
      sourceEntries,
      file.transform,
      subEntries,
      refName,
      constParamType,
      targetName,
    );
  } else {
    const blockedNames = createBlockedNames(subEntries);
    blockedNames.add(targetName);
    Object.keys(ctors).forEach((k) => blockedNames.add(k));
    const detectedMethods = detectMethods(
      sourceEntries,
      file,
      context,
      sourceName,
      targetName,
      refName,
      constParamType,
      blockedNames,
    );
    mirrorMethods(
      sourceEntries,
      file.transform,
      subEntries,
      refName,
      constParamType,
      targetName,
    );
    combineObject(subEntries, detectedMethods);
  }

  if (sourceEntry.doc) {
    targetEntry.doc = [
      ...transformDoc(sourceEntry.doc, context),
      "@cat resource",
    ];
  } else {
    targetEntry.doc = [`Wraps ${title} resource.`, "@cat resource"];
  }
  populateTargetEntry(
    hasScoped,
    hasShared,
    targetEntry,
    rawName,
    nullValue,
    ctors,
    targetName,
    destroyFunction,
    subEntries,
    enableConstParam ? constParamType : undefined,
  );

  if (hasLock) wrapLockFunctions(targetEntry.entries, lockName, hasLock);

  const derivedEntries: ApiEntryTransform[] = [];

  if (hasRef) {
    derivedEntries.push(createRefEntry(refName, targetName, rawName));
  } else if (hasScoped) {
    derivedEntries.push(
      createScopedEntry(scopedName, targetName, destroyFunction),
    );
  }

  context.includeBefore(referenceAliases, "__begin");
  derivedEntries.forEach((e) => context.includeAfter(e, targetName));
  delete targetEntry.resource;
}

function createParam(
  paramType: string,
  targetName: string,
  rawName: string,
): ApiEntryTransform {
  return {
    name: paramType,
    kind: "alias",
    doc: [`Safely wrap ${targetName} for non owning parameters`],
    type: `ResourceRef<${rawName}>`,
  };
}

function createConstParam(
  constParamType: string,
  targetName: string,
  rawName: string,
  constRawName: string,
): ApiEntryTransform {
  return {
    name: constParamType,
    kind: "alias",
    doc: [`Safely wrap ${targetName} for non owning const parameters`],
    type: `ResourceConstRef<${rawName}, ${constRawName}>`,
  };
}

function setupTypeTranslations(
  hasScoped: boolean,
  context: ApiContext,
  pointerType: string,
  targetName: string,
  constPointerType: string,
  paramType: string,
  enableConstParam: boolean,
  constParamType: string,
  hasShared: boolean,
  hasRef: boolean,
  refName: string,
  rawName: string,
  constRawName: string,
) {
  if (hasScoped) {
    context.addParamType(pointerType, targetName);
    context.addParamType(constPointerType, `const ${targetName} &`);
  } else {
    context.addParamType(pointerType, paramType);
    context.addParamType(
      constPointerType,
      enableConstParam ? constParamType : paramType,
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
}

function createBaselineCtors(
  hasScoped: boolean,
  targetName: string,
  shared: boolean | string,
  refName: string | undefined,
  rawName: string,
  isStruct: boolean,
) {
  if (hasScoped) return {};
  const ctors: Dict<ApiEntryTransform> = {
    [`${targetName}#2`]: {
      kind: "function",
      type: "",
      constexpr: true,
      explicit: true,
      parameters: [{ name: "resource", type: rawName }],
      hints: {
        init: [`ResourceBase(resource)`],
        noexcept: true,
      },
      doc: [
        `Constructs from raw ${targetName}.`,
        { tag: "@param resource", content: `a ${rawName} to be wrapped.` },
        "This assumes the ownership, call release() if you need to take back.",
      ],
    },
    [`${targetName}#3`]: {
      kind: "function",
      type: "",
      constexpr: true,
      parameters: [{ name: "other", type: `const ${targetName} &` }],
      hints: {
        delete: shared ? undefined : true,
        init: shared ? [`${targetName}(other.get())`] : undefined,
        body: shared ? `if (auto res = get()) ++res->${shared};` : undefined,
      },
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
  if (shared) {
    addBorrowFunction(ctors, targetName, shared, rawName);
  } else if (refName) {
    deleteCtorsFromRef(ctors, refName, targetName);
  }
  return ctors;
}

function addBorrowFunction(
  ctors: Dict<ApiEntryTransform>,
  targetName: string,
  shared: true | string,
  rawName: string,
) {
  if (shared === true) {
    ctors[`${targetName}#3`].hints = {};
  } else {
    ctors["Borrow"] = {
      kind: "function",
      static: true,
      type: targetName,
      parameters: [{ name: "resource", type: rawName }],
      hints: {
        body: `if (resource) {\n  ++resource->${shared};\n  return ${targetName}(resource);}\nreturn {};`,
      },
      doc: [
        `Safely borrows the from ${rawName}.`,
        {
          tag: "@param resource",
          content: `a ${rawName}.`,
        },
        "This does not takes ownership!",
      ],
    };
  }
}

function deleteCtorsFromRef(
  ctors: Dict<ApiEntryTransform>,
  refName: string,
  targetName: string,
) {
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
    targetName,
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
    targetName,
  );
}

function wrapCustomCtors(
  subEntries: ApiEntryTransformMap,
  targetName: string,
  ctors: Dict<ApiEntryTransform>,
) {
  Object.keys(subEntries)
    .filter((k) => k === targetName || k.startsWith(targetName + "#"))
    .map((k) => {
      const e = subEntries[k];
      delete subEntries[k];
      return e;
    })
    .filter((e) => typeof e === "object")
    .forEach((e: ApiEntryTransform) => insertTransform(ctors, e, targetName));
}

function createCustomFactories(
  resourceEntry: ResourceDefinition,
  subEntries: ApiEntryTransformMap,
  file: ApiFileTransform,
  targetName: string,
  context: ApiContext,
  sourceEntries: Dict<ApiEntry>,
  ctors: Dict<ApiEntryTransform>,
) {
  for (const sourceName of resourceEntry.ctors ?? []) {
    const entry = subEntries[sourceName] ?? {};
    delete subEntries[sourceName];
    const ctorTransform = file.transform[sourceName];
    if (typeof entry === "string") {
      system.warn(
        `${sourceName} can not be a custom ctor, only objects containing name property can be accepted.`,
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
}

function wrapCtors(
  subEntries: ApiEntryTransformMap,
  file: ApiFileTransform,
  targetName: string,
  ctors: Dict<ApiEntryTransform>,
  sourceEntries: Dict<ApiEntry>,
) {
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
}

function createOrDetectDestroyFunction(
  sourceEntries: Dict<ApiEntry>,
  resourceEntry: ResourceDefinition,
  targetName: string,
  pointerType: string,
  context: ApiContext,
  subEntries: ApiEntryTransformMap,
  title: string,
  file: ApiFileTransform,
  rawName: string,
) {
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
  return freeFunction;
}

function populateTargetEntry(
  hasScoped: boolean,
  hasShared: boolean,
  targetEntry: ApiEntryTransform,
  rawName: string,
  nullValue: string,
  ctors: Dict<ApiEntryTransform>,
  targetName: string,
  freeFunction: ApiEntry,
  subEntries: ApiEntryTransformMap,
  constParamType: string,
) {
  if (constParamType) {
    ctors[`operator ${constParamType}`] = {
      kind: "function",
      type: "",
      immutable: true,
      constexpr: true,
      parameters: [],
      hints: { body: "return get();", noexcept: true },
      doc: [`Converts to ${constParamType}`],
    };
  } else if (hasScoped) {
    ctors[`operator ${rawName}`] = {
      kind: "function",
      type: "",
      immutable: true,
      constexpr: true,
      parameters: [],
      hints: { body: "return get();", noexcept: true },
      doc: [`Converts to underlying ${rawName}.`],
    };
  }
  const entries: ApiEntryTransformMap = {
    "ResourceBase::ResourceBase": "alias",
    ...ctors,
    [`~${targetName}`]: {
      kind: "function",
      doc: ["Destructor"],
      type: "",
      parameters: [],
      hints: {
        body: freeFunction
          ? `${freeFunction.sourceName ?? freeFunction.name}(get());`
          : "",
      },
    },
    "operator=": {
      kind: "function",
      type: `${targetName} &`,
      parameters: [{ name: "other", type: `${targetName} &&` }],
      constexpr: true,
      hints: {
        body: "swap(*this, other);\nreturn *this;",
        noexcept: true,
      },
      doc: ["Assignment operator."],
    },
    "operator=#2": {
      kind: "function",
      type: `${targetName} &`,
      parameters: [{ name: "other", type: `const ${targetName} &` }],
      hints: {
        delete: !hasShared,
        body: hasShared
          ? `if (get() != other.get()) {\n  ${targetName} tmp(other);\n  swap(*this, tmp);\n}\nreturn *this;`
          : undefined,
      },
      doc: ["Assignment operator."],
    },
    [freeFunction.name]: "plc",
    ...subEntries,
  };
  if (hasScoped) {
    delete entries[`~${targetName}`];
    delete entries["operator="];
    delete entries["operator=#2"];
  }
  addHints(targetEntry, {
    self: "get()",
    super: targetName,
  });
  targetEntry.entries = entries;
}

function createRefEntry(
  refName: string,
  targetName: string,
  rawName: string,
): ApiEntryTransform {
  const entries = {
    [`${targetName}::${targetName}`]: "alias",
    [refName]: {
      kind: "function",
      type: "",
      constexpr: true,
      parameters: [
        {
          type: rawName,
          name: "resource",
        },
      ],
      hints: { init: [`${targetName}(resource)`], noexcept: true },
      doc: [
        `Constructs from raw ${targetName}.`,
        {
          tag: "@param resource",
          content: `a ${rawName}.`,
        },
        "This does not takes ownership!",
      ],
    },
    [`${refName}#2`]: {
      kind: "function",
      type: "",
      constexpr: true,
      parameters: [
        {
          type: `const ${targetName} &`,
          name: "resource",
        },
      ],
      hints: { init: [`${targetName}(resource.get())`], noexcept: true },
      doc: [
        `Constructs from ${targetName}.`,
        {
          tag: "@param resource",
          content: `a ${targetName}.`,
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
          type: `${targetName} &&`,
          name: "resource",
        },
      ],
      hints: {
        init: [`${targetName}(std::move(resource).release())`],
        noexcept: true,
      },
      doc: [
        `Constructs from ${targetName}.`,
        {
          tag: "@param resource",
          content: `a ${targetName}.`,
        },
        "This will release the ownership from resource!",
      ],
    },
    [`${refName}#4`]: {
      kind: "function",
      type: "",
      constexpr: true,
      parameters: [
        {
          type: `const ${refName} &`,
          name: "other",
        },
      ],
      hints: { init: [`${targetName}(other.get())`], noexcept: true },
      doc: ["Copy constructor."],
    },
    [`${refName}#5`]: {
      kind: "function",
      type: "",
      constexpr: true,
      parameters: [
        {
          type: `${refName} &&`,
          name: "other",
        },
      ],
      hints: { init: [`${targetName}(other.get())`], noexcept: true },
      doc: ["Move constructor."],
    },
    [`~${refName}`]: {
      kind: "function",
      doc: ["Destructor"],
      type: "",
      parameters: [],
      hints: { body: "release();" },
    },
    [`operator=`]: {
      kind: "function",
      type: `${refName} &`,
      parameters: [{ type: `const ${refName} &`, name: "other" }],
      hints: {
        body: `release();${targetName}::operator=(${targetName}(other.get()));\nreturn *this;`,
        noexcept: true,
      },
      doc: [`Assignment operator.`],
    },
    [`operator ${rawName}`]: {
      kind: "function",
      type: "",
      immutable: true,
      constexpr: true,
      parameters: [],
      hints: { body: "return get();", noexcept: true },
      doc: [`Converts to ${rawName}`],
    },
  } as Dict<ApiEntryTransform>;
  return {
    kind: "struct",
    name: refName,
    type: targetName,
    doc: [`Reference for ${targetName}.`, "This does not take ownership!"],
    entries,
  };
}

function createScopedEntry(
  scopedName: string,
  targetName: string,
  freeFunction: ApiEntry,
): ApiEntryTransform {
  return {
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
        parameters: [{ name: "other", type: `const ${scopedName} &` }],
        hints: { delete: true },
      },
      [`${scopedName}#2`]: {
        kind: "function",
        doc: ["Move constructor"],
        type: "",
        constexpr: true,
        parameters: [{ name: "other", type: `${scopedName} &&` }],
        hints: {
          init: [`${targetName}(other.release())`],
          noexcept: true,
        },
      },
      [`${scopedName}#3`]: {
        kind: "function",
        doc: ["Move constructor"],
        type: "",
        constexpr: true,
        parameters: [{ name: "other", type: `${targetName} &&` }],
        hints: {
          init: [`${targetName}(std::move(other).release())`],
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
  };
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

function makeResourceLock(
  file: ApiFileTransform,
  targetName: string,
  controlType: string,
  targetEntry: ApiEntryTransform,
  lockDef: LockDefinition,
) {
  let lockName = `${targetName}Lock`;
  if (lockDef) {
    delete targetEntry.lock;
    const currDef = file.transform[lockName];
    lockName = currDef?.name || lockName;
    const lockEntry: ApiEntryTransform = {
      lock: lockDef,
    };
    lockDef.controlType = controlType;
    if (currDef) {
      combineObject(lockEntry, currDef ?? {});
    } else {
      lockEntry.after = controlType;
    }
    file.transform[lockName] = lockEntry;
  }
  return lockName;
}
