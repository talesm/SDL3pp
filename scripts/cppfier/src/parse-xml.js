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
  system.log(`Reading ${name}`);
  const sections = xmlObj.doxygen?.compounddef?.[0]?.sectiondef || [];
  for (const section of sections) {
    for (const member of section.memberdef) {
      const kind = member.$.kind;
      const name = member.name[0];
      const location = member.location[0].$;
      /** @type {ApiEntry} */
      const entry = {
        kind: "def",
        name,
        decl: location.line,
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
          entry.type = member.type[0]; // TODO Remove SDL_DECLSPEC
          entry.static = member.$.static === 'yes';
          entry.parameters = member.argsstring[0] === "(void)" ? [] : member.param.map(p => ({ type: p.type.join(" "), name: p.declname.join(" ") }));
          break;
        default:
          system.warn(`Error at ${stringLocation(location)}: Unknown kind for ${name} (${kind})`);
          continue;
      }
      apiFile.entries[name] = entry;
    }
  }
  // TODO sort by file order
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

exports.parseXmlApi = parseXmlApi;
exports.parseXmlFile = parseXmlFile;
