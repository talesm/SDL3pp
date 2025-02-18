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
 * @param {ApiEntries}     sourceEntries 
 * @param {ApiContext}     context 
 * @param {FileTransform} transform
 */
function transformEntries(sourceEntries, context, transform) {
  /** @type {ApiEntries} */
  const targetEntries = {};
  const defWhitelist = new Set(transform.includeDefs ?? []);
  const blacklist = new Set(transform.ignoreEntries ?? []);
  const transformMap = transform.transform ?? {};

  if (transform.includeAfter?.__begin) {
    insertEntry(targetEntries, transform.includeAfter.__begin);
  }

  for (const [sourceName, sourceEntry] of Object.entries(sourceEntries)) {
    if (blacklist.has(sourceName)) continue;
    let targetName = transformName(sourceName, context);
    if (Array.isArray(sourceEntry)) {
      targetEntries[targetName] = sourceEntry.map(e => {
        const targetEntry = transformEntry(e, context, transform);
        targetEntry.name = targetName;
        return targetEntry;
      });
    } else if (sourceEntry.kind != "def" || defWhitelist.has(sourceName)) {
      const targetEntry = transformEntry(sourceEntry, context, transform);
      targetName = transformName(targetEntry.name, context);
      const targetDelta = transformMap[sourceName];
      if (targetDelta) {
        if (targetDelta.name) targetName = targetDelta.name;
        else targetDelta.name = targetName;
        Object.assign(targetEntry, targetDelta);
      } else
        targetEntry.name = targetName;
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
        if (targetEntry.entries) {
          // TODO make this an initial step
          for (const [key, entry] of Object.entries(targetEntry.entries)) {
            const nameChange = checkNameChange(entry, transformName(key, context), targetEntry.name);
            if (nameChange) {
              transformMap[key] = nameChange;
            }
          }
        }
      } else if (targetEntry.kind == "forward") {
        targetName += "-forward";
      }
      targetEntries[targetName] = targetEntry;
    }
    const includeAfter = transform.includeAfter?.[targetName];
    if (includeAfter) {
      insertEntry(targetEntries, includeAfter);
    }
  }
  if (transform.includeAfter?.__end) {
    insertEntry(targetEntries, transform.includeAfter.__end);
  }
  // Change hierarchy
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

  return targetEntries;
}

/**
 * Check and if true, marshal name of member functions
 * @param {ApiEntry|ApiEntry[]} entry 
 * @param {string} name 
 * @param {string} typeName 
 * @returns {ApiEntry|ApiEntry[]|null}
 */
function checkNameChange(entry, name, typeName) {
  if (Array.isArray(entry)) {
    return entry
      .map(e => checkNameChange(entry, name, typeName))
      .filter(a => a != null);
  }
  // const naturalName = makeNaturalName()
  if (entry === "function") {
    return { name: `${typeName}.${makeNaturalName(name, typeName)}` };
  }
  if (entry.kind && entry.kind !== "function") return null;
  if (entry.parameters) return null;
  return { ...entry, name: `${typeName}.${entry.name || makeNaturalName(name, typeName)}` };
}

/**
 * 
 * @param {string} name 
 * @param {string} typeName 
 */
function makeNaturalName(name, typeName) {
  if (typeName.endsWith("Base")) typeName = typeName.slice(0, typeName.length - 4);
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
  entry.name = name;
  const parameters = entry.parameters;
  if (!parameters?.length) return;
  const type = parameters[0]?.type ?? "";
  if (type.includes(typeName)) {
    parameters.shift();
    if (type.startsWith("const ")) entry.immutable = true;
  } else {
    entry.static = true;
  }
}

/**
 * 
 * @param {ApiEntry}       sourceEntry 
 * @param {ApiContext}     context 
 * @param {FileTransform} transform
 */
function transformEntry(sourceEntry, context, transform) {
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
      const type = transform?.types?.[sourceName];
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
