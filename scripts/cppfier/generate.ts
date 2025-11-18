import {
  Api,
  ApiEntries,
  ApiEntry,
  ApiFile,
  ApiParameter,
  ApiParameters,
  Dict,
  ParsedDoc,
  ParsedDocContent,
} from "./types.js";
import { system, writeLinesSync } from "./utils";
import { existsSync, mkdirSync } from "node:fs";

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
  const guardName = targetName.toUpperCase().replace(".", "_") + "_";
  const namespace = "SDL";
  const generatedEntries = generateEntries(targetFile.entries, "");

  const includes = [
    ...(targetFile.includes ?? []).toSorted().map((s) => `#include <${s}>`),
    ...(targetFile.localIncludes ?? [])
      .toSorted()
      .map((s) => `#include "${s}"`),
  ];

  const doc = generateFileDocString(targetFile.doc);

  return [
    `#ifndef ${guardName}\n#define ${guardName}\n`,
    ...includes,
    `\nnamespace ${namespace} {\n`,
    `${doc}\n`,
    generatedEntries,
    `/// @}\n\n} // namespace ${namespace}\n\n#endif /* ${guardName} */`,
  ];

  function generateEntries(entries: ApiEntries, prefix: string) {
    if (!prefix) prefix = "";

    const result: string[] = [];
    for (const entry of Object.values(entries)) doGenerateEntries(entry);
    return result.join("\n\n") + "\n";

    function doGenerateEntries(entry: ApiEntry) {
      result.push(generateEntry(entry, prefix));
      if (entry.overload) doGenerateEntries(entry.overload);
    }
  }

  function generateFileDocString(doc: ParsedDoc) {
    if (!doc?.length) return undefined;
    doc.push("@{");
    return generateDocString(doc);
  }

  function generateDocString(doc: ParsedDoc, prefix = "") {
    if (!doc?.length) return undefined;
    if (doc.length === 1) {
      const docStr = doc[0];
      if (
        typeof docStr === "string" &&
        docStr.length < 80 - 4 - prefix.length
      ) {
        return `\n${prefix} /// ${docStr}`;
      }
    }
    const internalPrefix = prefix + " * ";
    const maxLength = 80 - internalPrefix.length;
    const docStr = doc
      .map(generateDocItem)
      .join(`\n${prefix} *\n${internalPrefix}`);
    return `\n${prefix}/**\n${internalPrefix}${docStr}\n${prefix} */`;

    function generateDocItem(item: ParsedDocContent): string {
      if (typeof item === "string")
        return generateDocStringItem(item, internalPrefix, maxLength).trimEnd();
      if (Array.isArray(item))
        return item.map(generateDocItem).join("\n" + internalPrefix);
      if (!item.tag)
        return item.content.replaceAll("\n", "\n" + internalPrefix);
      const tagLen = item.tag.length + 1;
      const content = generateDocStringItem(
        item.content,
        internalPrefix + " ".repeat(tagLen),
        maxLength - tagLen
      );
      return `${item.tag} ${content}`;
    }
  }
  function generateDocStringItem(
    docStr: string,
    prefix: string,
    maxLength: number
  ) {
    if (docStr.length <= maxLength) return docStr;
    const result: string[] = [];
    while (docStr.length > maxLength) {
      let cutPoint = docStr.lastIndexOf(" ", maxLength);
      if (cutPoint === -1) {
        cutPoint = docStr.indexOf(" ");
        if (cutPoint === -1) break;
      }
      result.push(docStr.slice(0, cutPoint));
      docStr = docStr.slice(cutPoint + 1);
    }
    docStr = docStr.trim();
    if (docStr) result.push(docStr);
    return result.join("\n" + prefix);
  }

  function generateEntry(entry: ApiEntry, prefix = "") {
    const doc = generateDocString(entry.doc, prefix) ?? "";
    const template = generateTemplateSignature(entry.template, prefix);
    const version = entry.since;
    const accessMod = entry.hints?.changeAccess
      ? `${prefix.slice(2)}${entry.hints?.changeAccess}:\n`
      : "";
    if (!version) return accessMod + doGenerate(entry);
    const versionStr = `${version.tag}_VERSION_ATLEAST(${version.major}, ${version.minor}, ${version.patch})`;
    return `${accessMod}#if ${versionStr}\n\n${doGenerate(
      entry
    )}\n\n#endif // ${versionStr}`;

    function doGenerate(entry: ApiEntry) {
      switch (entry.kind) {
        case "alias": {
          if (!entry.type) return `${doc}\n${prefix}using ${entry.name};`;
          const target =
            entry.name === entry.type ? `::${entry.type}` : entry.type;
          return `${doc}\n${template}${prefix}using ${entry.name} = ${target};`;
        }
        case "def":
          return `${doc}\n${generateDef(entry)}`;
        case "forward":
          return (
            "// Forward decl\n" +
            template +
            generateStructSignature(entry, prefix) +
            ";"
          );
        case "function":
          return `${doc}\n${template}${generateFunction(entry, prefix)}`;
        case "ns":
          return doc + generateNS(entry);
        case "struct":
          return `${doc}\n${template}${generateStruct(entry, prefix)}`;
        case "var": {
          const varStr = generateVar(entry, prefix);
          if (entry?.doc?.length === 1) {
            const firstLine = entry.doc[0];
            if (typeof firstLine === "string" && firstLine.length <= 50) {
              return template + varStr + " ///< " + firstLine;
            }
          }
          return `${doc}\n${template}${varStr}`;
        }
        default:
          system.warn(`Unknown kind: ${entry.kind} for ${entry.name}`);
          return `${doc}\n#${prefix}error "${entry.name} (${entry.kind})"`;
      }
    }
  }

  function generateVar(entry: ApiEntry, prefix: string) {
    let value = "";
    if (entry.hints?.body) value = ` = ${entry.hints?.body}`;
    else if (entry.sourceName) value = ` = ${entry.sourceName}`;
    return generateDeclPrefix(entry, prefix) + value + ";";
  }

  function generateDef(entry: ApiEntry) {
    const sourceName =
      entry.sourceName === entry.name ? undefined : entry.sourceName;
    if (!entry.parameters)
      return `#define ${entry.name} ${sourceName ?? entry.value ?? ""}`;

    const parameters = `(${entry.parameters.map((p) => p.name).join(", ")})`;
    const body = sourceName ? `${entry.sourceName}${parameters}` : entry.value;
    return `#define ${entry.name}${parameters} ${body ?? ""}`;
  }

  function generateBody(entry: ApiEntry, prefix: string) {
    const hint = entry.hints;
    if (hint?.delete) return " = delete;";
    if (hint?.default) return " = default;";
    if (!entry.proto) {
      if (hint?.body) {
        return `\n${prefix}{\n${prefix}  ${hint.body.replaceAll(
          "\n",
          `\n${prefix}  `
        )}\n${prefix}}`;
      }
      if (hint?.init?.length) {
        return `\n${prefix}  : ${hint.init.join(
          `\n${prefix}  , `
        )}\n${prefix}{}`;
      }
    }
    const maybeDelegatedTo = hint?.delegate ?? entry.sourceName;
    const delegatedTo =
      maybeDelegatedTo === entry.name
        ? "::" + maybeDelegatedTo
        : maybeDelegatedTo;
    if (!delegatedTo) {
      if (entry.proto) return ";";
      if (entry.type === "" && !entry.name.startsWith("operator")) return "{}";
      return `\n${prefix}{\n${prefix}  static_assert(false, "Not implemented");\n${prefix}}`;
    }
    if (entry.proto) return ";";
    const paramReplacements: Dict<string> = hint?.delegate
      ? config.delegatedReplacements
      : config.paramReplacements;
    const selfStr =
      entry.type && !entry.static && !entry.hints?.static && hint?.self;
    const selfStrPrefix =
      (selfStr || "") + (entry.parameters?.length && selfStr ? ", " : "");
    const paramStr =
      selfStrPrefix +
      generateCallParameters(entry.parameters, paramReplacements);
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
      if (typeof hint.mayFail === "string")
        return `CheckErrorIfNot(${internalCallStr}, ${hint.mayFail})`;
      return `CheckError(${internalCallStr})`;
    }
  }

  function generateFunction(entry: ApiEntry, prefix: string) {
    const reference = entry.reference ? "&".repeat(entry.reference) : "";
    const specifier = entry.immutable
      ? ` const${reference}`
      : reference
      ? " " + reference
      : "";
    const parameters = generateParameters(entry.parameters ?? []);
    const body = generateBody(entry, prefix);
    return `${generateDeclPrefix(
      entry,
      prefix
    )}(${parameters})${specifier}${body}`;
  }

  function generateDeclPrefix(entry: ApiEntry, prefix: string) {
    const staticStr = entry.static ? "static " : "";
    const explicitStr = entry.explicit ? "explicit " : "";
    const specifier = entry.constexpr ? "constexpr " : prefix ? "" : "inline ";
    return `${prefix}${staticStr}${specifier}${explicitStr}${
      entry.type ?? "auto"
    } ${entry.name}`;
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
}
function generateStructSignature(entry: ApiEntry, prefix: string) {
  if (entry.hints?.private) {
    return `${prefix}class ${entry.name}`;
  }
  return `${prefix}struct ${entry.name}`;
}

function generateTemplateSignature(template: ApiParameters, prefix: string) {
  return template ? `${prefix}template<${generateParameters(template)}>\n` : "";
}

export function generateCallParameters(
  parameters: ApiParameters,
  replacements: Dict<string>
) {
  return parameters?.map((p) => unwrap(p))?.join(", ") ?? "";

  /** @param {ApiParameter} p  */
  function unwrap(p) {
    const m = replacements[p.type];
    if (!m) return p.name;
    return m.replaceAll("$", p.name);
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
  return parameters.map((p) => generateParameter(p)).join(", ");
}

function generateParameter(parameter: ApiParameter) {
  if (!parameter.type) return parameter.name ?? "";
  if (!parameter.default) return `${parameter.type} ${parameter.name ?? ""}`;
  return `${parameter.type} ${parameter.name ?? ""} = ${parameter.default}`;
}
