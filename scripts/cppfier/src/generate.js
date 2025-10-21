const { system, writeLinesSync } = require("./utils.js");
const { existsSync, mkdirSync } = require("fs");
/**
 * @import { Api, ApiEntries, ApiEntry, ApiFile, ApiParameter, ApiParameters, Dict } from "./types.js"
 */

/**
 * @typedef {object} GenerateApiConfig
 * @prop {Api}      api
 * @prop {string}   baseDir
 */

/**
 * Generate api headers from config
 * @param {GenerateApiConfig} config 
 */
function generateApi(config) {
  const { api, baseDir } = config;
  const files = Object.keys(api.files);
  if (!existsSync(baseDir)) {
    system.warn(`target dir does not exist, creating it: ${baseDir}`);
    mkdirSync(baseDir);
  }

  let totalChanges = 0;
  for (const name of files) {
    system.log(`Checking ${name}`);
    const targetFile = api.files[name];

    try {
      const targetFilename = baseDir + name;
      const generatedContent = generateFile(targetFile, config.api);
      writeLinesSync(targetFilename, generatedContent);

      totalChanges += 1;
    } catch (e) {
      system.warn(`Can not generate ${name}\n`, e);
    }
  }
  if (totalChanges) {
    system.log(`Total of ${totalChanges} file(s) generated`);
  } else {
    system.log("Nothing to generated");
  }
}

/**
 * @typedef {object} GenerateApiFileConfig
 * @prop {Dict<string>=}  paramReplacements
 * @prop {Dict<string>=}  delegatedReplacements
 */

/**
 * 
 * @param {ApiFile} targetFile 
 * @param {GenerateApiFileConfig} config 
 */
function generateFile(targetFile, config) {
  const targetName = targetFile.name;
  const guardName = targetName.toUpperCase().replace('.', '_') + '_';
  const namespace = "SDL";
  const generatedEntries = generateEntries(targetFile.entries, '');

  const includes = [
    ...(targetFile.includes ?? []).sort().map(s => `#include <${s}>`),
    ...(targetFile.localIncludes ?? []).sort().map(s => `#include "${s}"`),
  ];


  return [
    `#ifndef ${guardName}\n#define ${guardName}\n`,
    ...includes,
    `\nnamespace ${namespace} {\n`,
    generateDocString(targetFile.doc + "\n\n@{") + "\n",
    generatedEntries,
    `/// @}\n\n} // namespace ${namespace}\n\n#endif /* ${guardName} */`
  ];
  /**
   * 
   * @param {ApiEntries}  entries 
   * @param {string}      prefix
   */
  function generateEntries(entries, prefix) {
    if (!prefix) prefix = '';

    /** @type {string[]} */
    const result = [];
    Object.values(entries).forEach(entry => doGenerateEntries(entry));
    return result.join('\n\n') + '\n';

    function doGenerateEntries(entry) {
      result.push(generateEntry(entry, prefix));
      if (entry.overload) doGenerateEntries(entry.overload);
    }
  }


  /**
   * 
   * @param {string}  docStr 
   * @param {string=} prefix 
   */
  function generateDocString(docStr, prefix) {
    if (!docStr) return '';
    prefix = prefix ?? '';
    const docLines = docStr.split('\n');
    if (docLines.length === 1 && docStr.length < 75) {
      return `\n${prefix}///${docStr}`;
    }
    docStr = docLines.map(l => l ? `${prefix} * ${l}` : `${prefix} *`).join('\n');
    return `\n${prefix}/**\n${docStr}\n${prefix} */`;
  }

  /**
   * @param {ApiEntry}  entry 
   * @param {string=}   prefix 
   * @returns {string}
   */
  function generateEntry(entry, prefix) {
    prefix = prefix ?? '';
    const doc = generateDocString(entry.doc, prefix) + "\n";
    const template = generateTemplateSignature(entry.template, prefix);
    const version = entry.since;
    const accessMod = entry.hints?.changeAccess ? `${prefix.slice(2)}${entry.hints?.changeAccess}:\n` : '';
    if (!version) return accessMod + doGenerate(entry);
    const versionStr = `${version.tag}_VERSION_ATLEAST(${version.major}, ${version.minor}, ${version.patch})`;
    return `${accessMod}#if ${versionStr}\n\n${doGenerate(entry)}\n\n#endif // ${versionStr}`;

    /** @param {ApiEntry} entry  */
    function doGenerate(entry) {
      switch (entry.kind) {
        case "alias":
          if (!entry.type) return `${doc}${prefix}using ${entry.name};`;
          return `${doc}${template}${prefix}using ${entry.name} = ${entry.type};`;
        case "def":
          return doc + generateDef(entry);
        case "forward":
          return '// Forward decl\n' + template + generateStructSignature(entry, prefix) + ';';
        case "function":
          return doc + template + generateFunction(entry, prefix);
        case "ns":
          return doc + generateNS(entry);
        case "struct":
          return doc + template + generateStruct(entry, prefix);
        case "var":
          const varStr = generateVar(entry, prefix);
          if (entry.doc && !entry.doc.includes("\n") && entry.doc.length <= 50) {
            return template + varStr + " ///< " + entry.doc;
          }
          return doc + template + varStr;
        default:
          system.warn(`Unknown kind: ${entry.kind} for ${entry.name}`);
          return `${doc}#${prefix}error "${entry.name} (${entry.kind})"`;
      }

    }
  }


  /**
   * @param {ApiEntry} entry 
   * @param {string}   prefix
   */
  function generateVar(entry, prefix) {
    let value = '';
    if (entry.hints?.body) value = ` = ${entry.hints?.body}`;
    else if (entry.sourceName) value = ` = ${entry.sourceName}`;
    return generateDeclPrefix(entry, prefix) + value + ';';
  }

  /**
   * 
   * @param {ApiEntry}  entry 
   */
  function generateDef(entry) {
    const sourceName = entry.sourceName != entry.name ? entry.sourceName : undefined;
    if (!entry.parameters) return `#define ${entry.name} ${sourceName ?? entry.value ?? ""}`;

    const parameters = `(${entry.parameters.map(p => p.name).join(", ")})`;
    const body = sourceName ? `${entry.sourceName}${parameters}` : entry.value;
    return `#define ${entry.name}${parameters} ${body ?? ""}`;
  }

  /**
   * @param {ApiEntry} entry 
   * @param {string}   prefix
   */
  function generateBody(entry, prefix) {
    const hint = entry.hints;
    if (hint?.delete) return " = delete;";
    if (hint?.default) return " = default;";
    if (!entry.proto) {
      if (hint?.body) {
        return `\n${prefix}{\n${prefix}  ${hint.body.replaceAll("\n", `\n${prefix}  `)}\n${prefix}}`;
      }
      if (hint?.init?.length) {
        return `\n${prefix}  : ${hint.init.join(`\n${prefix}  , `)}\n${prefix}{}`;
      }
    }
    const maybeDelegatedTo = hint?.delegate ?? entry.sourceName;
    const delegatedTo = maybeDelegatedTo === entry.name ? ("::" + maybeDelegatedTo) : maybeDelegatedTo;
    if (!delegatedTo) {
      if (entry.proto) return ";";
      if (entry.type === "" && !entry.name.startsWith("operator")) return "{}";
      return `\n${prefix}{\n${prefix}  static_assert(false, "Not implemented");\n${prefix}}`;
    }
    if (entry.proto) return ";";
    /** @type {Dict<string>} */
    const paramReplacements = hint?.delegate ? config.delegatedReplacements : config.paramReplacements;
    const selfStr = entry.type && !entry.static && !entry.hints?.static && hint?.self;
    const selfStrPrefix = (selfStr || "") + ((entry.parameters?.length && selfStr) ? ", " : "");
    const paramStr = selfStrPrefix + generateCallParameters(entry.parameters, paramReplacements);
    const internalCallStr = `${delegatedTo}(${paramStr})`;
    const callStr = wrapFailCheck(internalCallStr);
    if (!entry.type && !entry.name.includes("operator")) {
      const superStr = hint?.super ?? hint?.self ?? "T";
      return `\n${prefix}  : ${superStr}(${callStr})\n${prefix}{}`;
    }
    if (hint?.wrapSelf && entry.type && !hint?.delegate) {
      return `\n${prefix}{\n${prefix}  return ${entry.type}(${callStr});\n${prefix}}`;
    }
    const returnStr = entry.type === "void" ? "" : "return ";
    return `\n${prefix}{\n${prefix}  ${returnStr}${callStr};\n${prefix}}`;

    /** @param {string} source  */
    function wrapFailCheck(source) {
      if (!hint?.mayFail || hint?.delegate) return source;
      if (typeof hint.mayFail === 'string') return `CheckErrorIfNot(${internalCallStr}, ${hint.mayFail})`;
      return `CheckError(${internalCallStr})`;
    }
  }

  /**
   * @param {ApiEntry} entry 
   * @param {string} prefix 
   */
  function generateFunction(entry, prefix) {
    const reference = entry.reference ? "&".repeat(entry.reference) : "";
    const specifier = entry.immutable ? ` const${reference}` : (reference ? " " + reference : "");
    const parameters = generateParameters(entry.parameters ?? []);
    const body = generateBody(entry, prefix);
    return `${generateDeclPrefix(entry, prefix)}(${parameters})${specifier}${body}`;
  }

  /**
   * @param {ApiEntry} entry 
   * @param {string} prefix 
   */
  function generateDeclPrefix(entry, prefix) {
    const staticStr = entry.static ? "static " : "";
    const explicitStr = entry.explicit ? "explicit " : "";
    const specifier = entry.constexpr ? "constexpr " : (prefix ? "" : "inline ");
    return `${prefix}${staticStr}${specifier}${explicitStr}${entry.type ?? "auto"} ${entry.name}`;
  }

  /**
   * 
   * @param {ApiEntry} entry 
   */
  function generateNS(entry) {
    const name = entry.name;
    const subEntries = generateEntries(entry.entries ?? {}, "");
    return `namespace ${name} {\n\n${subEntries}\n\n} // namespace ${name}\n`;
  }

  /**
   * 
   * @param {ApiEntry} entry 
   * @param {string} prefix 
   */
  function generateStruct(entry, prefix) {
    const signature = generateStructSignature(entry, prefix);
    const parent = entry.type ? ` : ${entry.type}` : "";
    const subEntries = entry.entries ?? {};
    combineHints(entry);
    const subEntriesStr = generateEntries(subEntries, prefix + "  ");
    return `${signature}${parent}\n${prefix}{${subEntriesStr}\n${prefix}};`;
  }

  /**
   * 
   * @param {ApiEntry} entry 
   * @param {string} prefix 
   */
  function generateStructSignature(entry, prefix) {
    if (entry.hints?.private) {
      return `${prefix}class ${entry.name}`;
    }
    return `${prefix}struct ${entry.name}`;
  }

  /**
   * 
   * @param {ApiParameters} template 
   * @param {string}        prefix 
   */
  function generateTemplateSignature(template, prefix) {
    return !template ? "" : `${prefix}template<${generateParameters(template)}>\n`;
  }
}


/**
 * @param {ApiParameters} parameters 
 * @param {Dict<string>}  replacements 
 */
function generateCallParameters(parameters, replacements) {
  return parameters
    ?.map(p => unwrap(p))
    ?.join(", ") ?? "";

  /** @param {ApiParameter} p  */
  function unwrap(p) {
    const m = replacements[p.type];
    if (!m) return p.name;
    return m.replaceAll('$', p.name);
  }
}

/**
 * 
 * @param {ApiEntry} entry 
 */
function combineHints(entry) {
  const hints = entry.hints;
  const subEntries = entry.entries;
  if (!hints || !subEntries) return;
  for (const entry of Object.values(subEntries)) {
    let e = entry;
    while (e) {
      if (e.hints) e.hints = { ...hints, ...e.hints };
      else e.hints = hints;
      e = e.overload;
    }
  }
}

/**
 * 
 * @param {ApiParameter[]} parameters 
 */
function generateParameters(parameters) {
  return parameters.map(p => generateParameter(p)).join(', ');
}

/**
 * 
 * @param {ApiParameter} parameter
 */
function generateParameter(parameter) {
  if (!parameter.type) return parameter.name ?? '';
  if (!parameter.default) return `${parameter.type} ${parameter.name ?? ""}`;
  return `${parameter.type} ${parameter.name ?? ""} = ${parameter.default}`;
}

exports.generateApi = generateApi;
exports.generateCallParameters = generateCallParameters;
exports.combineHints = combineHints;
