const { readLinesSync, system, writeLinesSync, looksLikeFreeFunction } = require("./utils.js");
const { existsSync, mkdirSync } = require("fs");
/**
 * @import { Api, ApiEntries, ApiEntry, ApiFile, ApiParameter, ApiParameters } from "./types.js"
 */

/**
 * @typedef {object} GenerateApiConfig
 * @prop {Api}      api
 * @prop {string}   baseDir
 * @prop {string}   templateDir
 */

/**
 * Generate api headers from config
 * @param {GenerateApiConfig} config 
 */
function generateApi(config) {
  const { api, baseDir, templateDir } = config;
  const files = Object.keys(api.files);
  if (!existsSync(baseDir)) {
    system.warn(`target dir does not exist, creating it: ${baseDir}`);
    mkdirSync(baseDir);
  }

  let totalChanges = 0;
  for (const name of files) {
    const templateFilename = templateDir + name;
    if (!existsSync(templateFilename)) {
      system.warn(`File template not found: ${name}`);
      continue;
    }
    system.log(`Checking ${name}`);
    const content = readLinesSync(templateFilename);
    const targetFile = api.files[name];

    try {
      generateFile(content, targetFile, config);
    } catch (e) {
      system.warn(`Can not generate ${name}\n`, e);
      continue;
    }
    const targetFilename = baseDir + name;
    writeLinesSync(targetFilename, content);

    totalChanges += 1;
  }
  if (totalChanges) {
    system.log(`Total of ${totalChanges} file(s) generated`);
  } else {
    system.log("Nothing to generated");
  }
}

/**
 * 
 * @param {string[]}          content 
 * @param {ApiFile}           targetFile 
 * @param {GenerateApiConfig} config 
 */
function generateFile(content, targetFile, config) {
  const placeholderIndex = findPlaceholderIndex(content);
  if (placeholderIndex === null) {
    throw new Error("Can not find template's placeholder");
  }
  content.splice(placeholderIndex, 1);
}

/** @param {string[]} content  */
function findPlaceholderIndex(content) {
  for (let i = 0; i < content.length; i++) {
    if (content[i].startsWith("//! content")) {
      return i;
    }
  }
  return null;
}

exports.generateApi = generateApi;
