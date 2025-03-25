const { parseStringPromise } = require("xml2js");
const { system } = require("./utils");
const { existsSync } = require("fs");
const { readFile } = require("fs/promises");

/**
 * @typedef {object} ParseXmlConfig
 * @prop {string[]} baseDir
 * @prop {string[]} sources
 * @prop {boolean=} storeLineNumbers
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
 * @prop {string[]=} baseDir
 * @prop {boolean=} storeLineNumbers
 */

/**
 * 
 * @param {string} name 
 * @param {ParseXmlFileConfig} config 
 */
async function parseXmlFile(name, config = {}) {
  const baseDirs = config?.baseDir ?? ["./"];
  const mangledName = name.replace(/_/g, '__').replace(/\./g, '_8') + ".xml";

  for (const dir of baseDirs) {
    const baseDir = dir.endsWith('/') ? dir : dir + "/";
    const candidate = baseDir + mangledName;
    if (existsSync(candidate)) {
      const content = await readFile(candidate, "utf-8");
      return parseXmlContent(name, content, baseDir, config);
    }
  }
  throw new Error(`Could not find ${name}`);
}


/**
 * @typedef {object} ParseXmlContentConfig
 * @prop {boolean=} storeLineNumbers
 */

/**
 * 
 * @param {string} name 
 * @param {string} content 
 * @param {string} baseDir 
 * @param {ParseXmlContentConfig} config 
 * @returns 
 */
async function parseXmlContent(name, content, baseDir, config) {
  /** @type {ApiFile} */
  const apiFile = {
    name,
    doc: '',
    entries: {},
  };
  const xmlObj = await parseStringPromise(content, {});
  const contentLines = content.split('\n');
  system.log(`Reading ${name}`);
  const sections = xmlObj.doxygen?.compounddef?.[0]?.sectiondef || [];
  /** @type {ApiEntry[]} */
  const entriesArray = [];
  for (const section of sections) {
    for (const member of section.memberdef) {
      const name = member.name[0];
      const kind = member.$.kind;
      const location = member.location[0].$;
      const doc = unwrapDoc(contentLines, location);
      /** @type {ApiEntry} */
      const entry = {
        doc,
        name,
        kind: "def",
        decl: +location.line,
      };
      switch (kind) {
        case "define": break;
        case "enum":
          entry.kind = "enum";
          break;
        case "typedef":
          entry.kind = "alias";
          break;
        case "function":
          entry.kind = "function";
          entry.type = normalizeType(member.type[0]); // TODO Remove SDL_DECLSPEC
          entry.static = member.$.static === 'yes';
          entry.parameters = member.argsstring[0] === "(void)" ? [] : member.param.map(p => ({ type: normalizeType(p.type.join(" ")), name: p.declname.join(" ") }));
          break;
        default:
          system.warn(`Error at ${stringLocation(location)}: Unknown kind for ${name} (${kind})`);
          continue;
      }
      entriesArray.push(entry);
    }
  }
  entriesArray.sort((a, b) => a.decl - b.decl).forEach(e => {
    apiFile.entries[e.name] = e;
    delete e.decl;
  });

  return apiFile;
}

/**
 * @param {object} location  
 * @param {string} location.file
 * @param {string} location.line
 */
function stringLocation(location) {
  return `${location?.file ?? "Unknown"}:${location?.line ?? 0}`;
}


/** @param {string} typeString  */
function normalizeType(typeString) {
  if (!typeString) return "";
  return typeString
    .replace(/SDL_DECLSPEC|SDLCALL/g, "")
    .replace(/(\w+)\s*([&*])/g, "$1 $2")
    .replace(/(\w+)\s+(\w+)/g, "$1 $2")
    .replace(/([*&])\s+(&*)/g, "$1$2").trim();
}

exports.parseXmlApi = parseXmlApi;
exports.parseXmlFile = parseXmlFile;
