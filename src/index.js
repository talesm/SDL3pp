const { writeFileSync, readFileSync } = require("fs");
const { parseApi } = require("./parse.js");
const { updateApi } = require("./update.js");
/**
 * Process the main
 * @param {string[]} args the arguments
 */
function main(args) {

  if (!args.length)
    throw new Error("Missing command");

  const command = args.shift();
  switch (command) {
    case "parse":
      parse(args);
      break;
    case "update":
      update(args);
      break;
    default:
      throw new Error(`Invalid command ${command}`);
  }
}

/**
 * Scan files
 * @param {string[]} args the arguments
 */
function parse(args) {
  const config = {
    /** @type {string[]} */
    files: [],
    outputFile: "",
    baseDir: "",
  };
  let printConfig = false;
  for (let i = 0; i < args.length; i++) {
    const arg = args[i];
    if (arg == "--") {
      config.files.push(...args.slice(i + 1).map(arg => arg.replaceAll("\\", '/')));
      break;
    }
    if (!arg.startsWith('-')) {
      if (arg.endsWith(".json")) {
        mergeInto(config, loadJson(arg.replaceAll("\\", '/')));
      } else
        config.files.push(arg.replaceAll("\\", '/'));
      continue;
    }
    switch (arg) {
      case '--outputFile':
      case '-o': config.outputFile = args[++i].replaceAll("\\", '/'); break;
      case '--baseDir':
      case '-d': config.baseDir = args[++i].replaceAll("\\", '/'); break;
      case '--config':
      case '-c': mergeInto(config, loadJson(args[++i].replaceAll("\\", '/'))); break;
      case '--print-config': printConfig = true; break;
      default:
        throw new Error(`Invalid option ${arg}`);
    }
  }
  if (!config.baseDir && config.files.length && config.files[0].includes('/')) {
    config.baseDir = config.files[0].slice(0, config.files[0].lastIndexOf("/") + 1);
    for (let i = 1; i < config.files.length; i++) {
      const file = config.files[i];
      while (!file.startsWith(config.baseDir)) {
        const pos = config.baseDir.lastIndexOf('/');
        config.baseDir = config.baseDir.slice(0, pos + 1);
      }
    }
  }
  if (config.baseDir) {
    const baseDirLen = config.baseDir.length;
    config.files = config.files.map(file => file.startsWith(config.baseDir) ? file.slice(baseDirLen) : file);
  }
  if (printConfig) {
    console.log(JSON.stringify(config, null, 2));
    return;
  }
  const api = JSON.stringify(parseApi(config), null, 2);
  if (config.outputFile) {
    writeFileSync(config.outputFile, api);
  } else {
    console.log(api);
  }
}

/**
 * 
 * @param {{[key: string]: any}} destiny 
 * @param  {{[key: string]: any}} sources 
 */
function mergeInto(destiny, source) {
  for (const [key, value] of Object.entries(source)) {
    const prevValue = destiny[key];
    if (Array.isArray(prevValue)) {
      if (Array.isArray(value)) {
        prevValue.push(...value);
      } else
        prevValue.push(value);
    } else if (typeof prevValue != "object") {
      if (value == null) {
        delete destiny[key];
      } else
        destiny[key] = value;
    } else if (!prevValue) {
      destiny[key] = value;
    } else {
      mergeInto(prevValue, value);
    }
  }
}

/**
 * 
 * @param {string[]} args 
 */
function update(args) {
  const config = {
    /** @type {string[]} */
    files: [],
    /** @type {Api} */
    target: null,
    baseDir: "",
  };
  for (let i = 0; i < args.length; i++) {
    const arg = args[i];
    if (arg == "--") {
      config.files.push(...args.slice(i + 1).map(arg => arg.replaceAll("\\", '/')));
      break;
    }
    if (!arg.startsWith('-')) {
      config.files.push(arg.replaceAll("\\", '/'));
      continue;
    }
    switch (arg) {
      case '-t': config.target = loadJson(args[++i].replaceAll("\\", '/')); break;
      case '-d': config.baseDir = args[++i].replaceAll("\\", '/'); break;
      case '-c': mergeInto(config, loadJson(args[++i].replaceAll("\\", '/'), "utf8")); break;
      default:
        throw new Error(`Invalid option ${arg}`);
    }
  }
  if (!config.target) {
    if (!config.files?.length) throw new Error("Missing target");
    config.target = loadJson(config.files.shift());
  }

  if (!config.baseDir && config.files[0].includes('/')) {
    config.baseDir = config.files[0].slice(0, config.files[0].lastIndexOf("/") + 1);
    for (let i = 1; i < config.files.length; i++) {
      const file = config.files[i];
      while (!file.startsWith(config.baseDir)) {
        const pos = config.baseDir.lastIndexOf('/');
        config.baseDir = config.baseDir.slice(0, pos + 1);
      }
    }
    if (!config.baseDir) {
      throw new Error("Could not deduce baseDir");
    }
    const baseDirLen = config.baseDir.length;
    config.files = config.files.map(file => file.startsWith(config.baseDir) ? file.slice(baseDirLen) : file);
  }
  updateApi(config);
}

/**
 * 
 * @param {string} filename 
 */
function loadJson(filename) {
  return JSON.parse(readFileSync(filename, "utf8"));
}

main(process.argv.slice(2));
