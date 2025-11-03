import { Api, ApiEntries, ApiEntry, ApiFile, ApiParameter, ApiParameters, Dict } from "./types.js";
import { system, writeLinesSync } from "./utils.js";
import { existsSync, mkdirSync } from "fs";

export interface GenerateApiConfig {
  api?: Api;
  baseDir?: string;
}

/**
 * Generate api headers from config
 * @param {GenerateApiConfig} config 
 */
export function generateApi(config) {
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

export interface GenerateApiFileConfig {
  paramReplacements?: Dict<string>;
  delegatedReplacements?: Dict<string>;
}

/**
 * 
 * @param {ApiFile} targetFile 
 * @param {GenerateApiFileConfig} config 
 */
function generateFile(targetFile: ApiFile, config: GenerateApiFileConfig) {
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

  function generateEntries(entries: ApiEntries, prefix: string) {
    if (!prefix) prefix = '';

    const result: string[] = [];
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
  function generateDocString(docStr: string, prefix?: string) {
    if (!docStr) return '';
    prefix = prefix ?? '';
    const docLines = docStr.split('\n');
    if (docLines.length === 1 && docStr.length < 75) {
      return `\n${prefix}///${docStr}`;
    }
    docStr = docLines.map(l => l ? `${prefix} * ${l}` : `${prefix} *`).join('\n');
    return `\n${prefix}/**\n${docStr}\n${prefix} */`;
  }

  function generateEntry(entry: ApiEntry, prefix?: string) {
    prefix = prefix ?? '';
    const doc = generateDocString(entry.doc, prefix) + "\n";
    const template = generateTemplateSignature(entry.template, prefix);
    const version = entry.since;
    const accessMod = entry.hints?.changeAccess ? `${prefix.slice(2)}${entry.hints?.changeAccess}:\n` : '';
    if (!version) return accessMod + doGenerate(entry);
    const versionStr = `${version.tag}_VERSION_ATLEAST(${version.major}, ${version.minor}, ${version.patch})`;
    return `${accessMod}#if ${versionStr}\n\n${doGenerate(entry)}\n\n#endif // ${versionStr}`;

    function doGenerate(entry: ApiEntry) {
      switch (entry.kind) {
        case "alias":
          if (!entry.type) return `${doc}${prefix}using ${entry.name};`;
          const target = entry.name === entry.type ? `::${entry.type}` : entry.type;
          return `${doc}${template}${prefix}using ${entry.name} = ${target};`;
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

  function generateVar(entry: ApiEntry, prefix: string) {
    let value = '';
    if (entry.hints?.body) value = ` = ${entry.hints?.body}`;
    else if (entry.sourceName) value = ` = ${entry.sourceName}`;
    return generateDeclPrefix(entry, prefix) + value + ';';
  }

  function generateDef(entry: ApiEntry) {
    const sourceName = entry.sourceName != entry.name ? entry.sourceName : undefined;
    if (!entry.parameters) return `#define ${entry.name} ${sourceName ?? entry.value ?? ""}`;

    const parameters = `(${entry.parameters.map(p => p.name).join(", ")})`;
    const body = sourceName ? `${entry.sourceName}${parameters}` : entry.value;
    return `#define ${entry.name}${parameters} ${body ?? ""}`;
  }

  function generateBody(entry: ApiEntry, prefix: string) {
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
    const paramReplacements: Dict<string> = hint?.delegate ? config.delegatedReplacements : config.paramReplacements;
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

    function wrapFailCheck(source: string) {
      if (!hint?.mayFail || hint?.delegate) return source;
      if (typeof hint.mayFail === 'string') return `CheckErrorIfNot(${internalCallStr}, ${hint.mayFail})`;
      return `CheckError(${internalCallStr})`;
    }
  }

  function generateFunction(entry: ApiEntry, prefix: string) {
    const reference = entry.reference ? "&".repeat(entry.reference) : "";
    const specifier = entry.immutable ? ` const${reference}` : (reference ? " " + reference : "");
    const parameters = generateParameters(entry.parameters ?? []);
    const body = generateBody(entry, prefix);
    return `${generateDeclPrefix(entry, prefix)}(${parameters})${specifier}${body}`;
  }

  function generateDeclPrefix(entry: ApiEntry, prefix: string) {
    const staticStr = entry.static ? "static " : "";
    const explicitStr = entry.explicit ? "explicit " : "";
    const specifier = entry.constexpr ? "constexpr " : (prefix ? "" : "inline ");
    return `${prefix}${staticStr}${specifier}${explicitStr}${entry.type ?? "auto"} ${entry.name}`;
  }

  function generateNS(entry: ApiEntry) {
    const name = entry.name;
    const subEntries = generateEntries(entry.entries ?? {}, "");
    return `namespace ${name} {\n\n${subEntries}\n\n} // namespace ${name}\n`;
  }

  function generateStruct(entry: ApiEntry, prefix: string) {
    const signature = generateStructSignature(entry, prefix);
    const parent = entry.type ? ` : ${entry.type}` : "";
    const subEntries = entry.entries ?? {};
    combineHints(entry);
    const subEntriesStr = generateEntries(subEntries, prefix + "  ");
    return `${signature}${parent}\n${prefix}{${subEntriesStr}\n${prefix}};`;
  }

  function generateStructSignature(entry: ApiEntry, prefix: string) {
    if (entry.hints?.private) {
      return `${prefix}class ${entry.name}`;
    }
    return `${prefix}struct ${entry.name}`;
  }

  function generateTemplateSignature(template: ApiParameters, prefix: string) {
    return !template ? "" : `${prefix}template<${generateParameters(template)}>\n`;
  }
}


export function generateCallParameters(parameters: ApiParameters, replacements: Dict<string>) {
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

export function combineHints(entry: ApiEntry) {
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

function generateParameters(parameters: ApiParameter[]) {
  return parameters.map(p => generateParameter(p)).join(', ');
}

function generateParameter(parameter: ApiParameter) {
  if (!parameter.type) return parameter.name ?? '';
  if (!parameter.default) return `${parameter.type} ${parameter.name ?? ""}`;
  return `${parameter.type} ${parameter.name ?? ""} = ${parameter.default}`;
}
