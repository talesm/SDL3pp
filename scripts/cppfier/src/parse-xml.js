const { parseStringPromise } = require("xml2js");
const { system } = require("./utils");
const { existsSync } = require("fs");
const { readFile } = require("fs/promises");
const { readContent, parseParams } = require("./parse");

/**
 * @typedef {object} ParseXmlConfig
 * @prop {string[]} xmlDir
 * @prop {string[]} baseDir
 * @prop {string[]} sources
 */

/**
 * @param {ParseXmlConfig} config 
 */
async function parseXmlApi(config) {
  const { sources } = config;
  /** @type {Api} */
  const api = { files: {} };
  for (const name of sources) {
    system.log(`Reading file ${name}`);
    api.files[name] = await parseXmlFile(name, config);
  }
  return api;
}

/**
 * @typedef {object} ParseXmlFileConfig
 * @prop {string[]=} xmlDir
 * @prop {string[]=} baseDir
 */

/**
 * 
 * @param {string} name 
 * @param {ParseXmlFileConfig} config 
 */
async function parseXmlFile(name, config = {}) {
  const xmlDirs = config?.xmlDir ?? ["./"];
  const mangledName = name.replace(/_/g, '__').replace(/\./g, '_8') + ".xml";

  for (const dir of xmlDirs) {
    const xmlDir = dir.endsWith('/') ? dir : dir + "/";
    const candidate = xmlDir + mangledName;
    if (existsSync(candidate)) {
      const content = await readFile(candidate, "utf-8");
      return parseXmlContent(name, content, xmlDir, config);
    }
  }
  throw new Error(`Could not find ${name}`);
}


/**
 * @typedef {object} ParseXmlContentConfig
 * @prop {string[]=} baseDir
 */

/**
 * 
 * @param {string} name 
 * @param {string} xmlContent 
 * @param {string} xmlDir 
 * @param {ParseXmlContentConfig} config 
 * @returns 
 */
async function parseXmlContent(name, xmlContent, xmlDir, config) {
  system.log(`Reading ${name}`);
  const xmlObj = await parseStringPromise(xmlContent);
  const sourceContent = readContent(name, config.baseDir ?? []);
  /** @type {ApiFile} */
  const apiFile = {
    name,
    doc: readCategoryDoc(sourceContent),
    entries: {},
  };
  const sections = xmlObj.doxygen?.compounddef?.[0]?.sectiondef || [];
  /** @type {ApiEntry[]} */
  const entriesArray = [];
  for (const section of sections) {
    for (const member of section.memberdef) {
      const name = member.name[0];
      const kind = member.$.kind;
      const location = member.location[0].$;
      const doc = unwrapDoc(sourceContent, location);
      /** @type {ApiEntry} */
      const entry = {
        doc,
        name,
        kind: "def",
        decl: +location.line,
      };
      switch (kind) {
        case "define": break;
        case "enum": {
          entry.kind = "enum";
          entry.entries = {};
          for (const value of member.enumvalue ?? []) {
            const name = value.name[0].trim();
            entry.entries[name] = {
              doc: value.briefdescription?.[0]?.para?.join("\n\n") ?? "",
              name,
              kind: "var",
              type: ""
            };
          }
          break;
        }
        case "typedef": {
          entry.kind = "alias";
          const type = member.type?.[0];
          // if (name == "SDL_Event") console.log(member);
          if (type) {
            if (typeof type != "string") {
              const typeString = type._ ?? "";
              if (typeString.startsWith('struct')) {
                entry.kind = "struct";
                const structXml = await loadTypeXml(name, xmlDir, "struct");
                const location = structXml.location[0]?.$;
                entry.doc = unwrapDoc(sourceContent, location);
                entry.decl = +location.line;
                entry.entries = parseXmlStruct(structXml.sectiondef[0].memberdef, sourceContent);
              } else if (typeString.startsWith("union")) {
                entry.kind = "union";
                const unionXml = await loadTypeXml(name, xmlDir, "union");
                const location = unionXml.location[0]?.$;
                entry.doc = unwrapDoc(sourceContent, location);
                entry.decl = +location.line;
              } else {
                system.warn(`At ${stringLocation(location)}: could not parse type from ${name}`, type);
              }
            } else {
              const argsstring = member.argsstring?.[0];
              if (type.startsWith("enum")) {
                // TODO ensure enum is correctly named
                continue;
              } else if (argsstring) {
                entry.kind = "callback";
                entry.type = normalizeType(type.slice(0, type.indexOf('(')));
                const params = argsstring.slice(2, argsstring.length - 1);
                entry.parameters = parseParams(params);
              } else {
                entry.type = type;
              }
            }
          }
          break;
        }
        case "function": {
          entry.kind = "function";
          // if (member.type[0].startsWith("SDL_FORCE_INLINE")) entry.constexpr = true;
          entry.type = normalizeType(member.type[0]); // TODO Remove SDL_DECLSPEC
          entry.static = member.$.static === 'yes';
          const argsstring = member.argsstring[0];
          const params = argsstring.slice(1, argsstring.length - 1);
          entry.parameters = parseParams(params);
          break;
        }
        default:
          system.warn(`Error at ${stringLocation(location)}: Unknown kind for ${name} (${kind})`);
          continue;
      }
      entriesArray.push(entry);
    }
  }
  sortAndAddEntries(entriesArray, apiFile.entries);
  return apiFile;
}

/**
 * 
 * @param {*}         members 
 * @param {string[]}  sourceContent 
 */
function parseXmlStruct(members, sourceContent) {
  /** @type {ApiEntry[]} */
  const entriesArray = members.map(member => {
    const name = member.name[0];
    const location = member.location[0].$;
    const doc = unwrapDoc(sourceContent, location);
    const type = member.type[0];
    const entry = {
      doc,
      name,
      kind: "var",
      type,
      decl: +location.line,
    };
    return entry;
  });
  return sortAndAddEntries(entriesArray);
}

/**
 * 
 * @param {ApiEntry[]} entriesArray 
 * @param {ApiEntries} entries 
 * @returns 
 */
function sortAndAddEntries(entriesArray, entries = {}) {
  entriesArray.sort((a, b) => a.decl - b.decl).forEach(e => {
    entries[e.name] = e;
    delete e.decl;
  });
  return entries;
}

/**
 * 
 * @param {string} name 
 * @param {string} dir 
 * @param {"struct"|"union"} kind 
 */
async function loadTypeXml(name, dir, kind) {
  const filename = `${dir}${kind}${name.replace(/_/g, "__")}.xml`;
  const content = await readFile(filename, "utf-8");
  const xml = await parseStringPromise(content);;
  return xml.doxygen.compounddef[0];
}

/**
 * @param {object} location  
 * @param {string} location.file
 * @param {string} location.line
 */
function stringLocation(location) {
  return `${location?.file ?? "Unknown"}:${location?.line ?? 0}`;
}

/**
 * Unwrap docstring
 * @param {string[]} content 
 * @param {{line: number, bodyend: number}} location 
 */
function unwrapDoc(content, location) {
  const startIndex = location.line - 1;
  if (location.bodyend == -1) {
    const m = /\/\*\*<(.*)\*\//.exec(content[startIndex]);
    if (m) return m[1].trim();
  }
  if (content[startIndex - 1].endsWith('*/')) {
    const doc = [];
    for (let i = startIndex - 2; i > 0; --i) {
      const line = content[i];
      let m = /^\s*\/\*\*(.*)$/.exec(line);
      if (m) {
        doc.unshift(m[1].trim());
        break;
      }
      doc.unshift(normalizeDocLine(line));
    }
    return doc.join('\n').trim();
  }
  return "";
}

/** @param {string[]} content  */
function readCategoryDoc(content) {
  let lineIndex = 0;
  const doc = [];
  for (let i = lineIndex; i < content.length; ++i) {
    const line = content[i];
    const m = /^\s*\/\*\*(.*)$/.exec(line);
    if (m) {
      doc.push(m[1].trim());
      lineIndex = i + 1;
      break;
    }
  }
  if (!doc.length) return "";
  for (let i = lineIndex; i < content.length; ++i) {
    const line = content[i];
    if (line.endsWith('*/')) break;
    doc.push(normalizeDocLine(line));
  }
  return doc.join('\n').trim();
}

/** @param {string} line  */
function normalizeDocLine(line) {
  const m = /^\s*\*\s?(.*)$/.exec(line);
  if (m) return m[1].trimEnd();
  return line.trimEnd();
}

/** @param {string} typeString  */
function normalizeType(typeString) {
  if (!typeString) return "";
  return typeString
    .replace(/SDL_DECLSPEC|SDLCALL|SDL_FORCE_INLINE/g, "")
    .replace(/(\w+)\s*([&*])/g, "$1 $2")
    .replace(/(\w+)\s+(\w+)/g, "$1 $2")
    .replace(/([*&])\s+(&*)/g, "$1$2").trim();
}

exports.parseXmlApi = parseXmlApi;
exports.parseXmlFile = parseXmlFile;
