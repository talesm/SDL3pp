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
  EntryHint,
  LockDefinition,
} from "./types";
import { combineObject, system } from "./utils";

export function expandResource(
  sourceEntries: Dict<ApiEntry>,
  file: ApiFileTransform,
  context: ApiContext,
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
  const enableConstParam = resourceEntry.enableConstParam ?? enableMemberAccess;
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
  referenceAliases.push(
    { name: targetName, kind: "forward" },
    {
      name: rawName,
      kind: "alias",
      type: pointerType,
      doc: [`Alias to raw representation for ${targetName}.`],
    }
  );
  if (hasRef) referenceAliases.push({ name: refName, kind: "forward" });
  if (hasScoped) referenceAliases.push({ name: scopedName, kind: "forward" });
  const hasLock = getLockDefinition(targetEntry, targetName);
  let lockName = makeResourceLock(hasLock);

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
        parameters: [{ type: "std::nullptr_t", name: "_", default: "nullptr" }],
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
    : ["This assumes the ownership, call release() if you need to take back."];

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
      sourceEntries,
      file,
      context,
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

  function makeResourceLock(lockDef: LockDefinition) {
    let lockName = `${targetName}Lock`;
    if (lockDef) {
      delete targetEntry.lock;
      const currDef = file.transform[lockName];
      lockName = currDef?.name || lockName;
      const lockEntry: ApiEntryTransform = {
        lock: lockDef,
      };
      lockDef.controlType = hasRef ? refName : targetName;
      if (currDef) {
        combineObject(lockEntry, currDef ?? {});
      } else {
        lockEntry.after = hasRef ? refName : targetName;
      }
      file.transform[lockName] = lockEntry;
    }
    return lockName;
  }
}
