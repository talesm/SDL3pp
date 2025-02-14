const { parseApi } = require("./parse.js");
const { updateApi } = require("./update.js");
const { readJSONSync, system, writeJSONSync, writeLinesSync } = require("./utils.js");

/**
 * Process the main
 * @param {string[]} args the arguments
 */
function main(args) {
  system.silent = false;
  system.stopOnWarn = false;

  if (!args.length) {
    printErrorWithGuide("Missing verb");
    process.exit(1);
  }

  const command = args.shift();
  switch (command) {
    case "parse":
      parse(args);
      break;
    case "update":
      update(args);
      break;
    case "--help":
    case "help":
      help(args);
      break;
    default:
      printErrorWithGuide(`Invalid verb ${command}`);
      process.exit(1);
  }
}

const guideDoc = [
  wrapUsageText(`usage: node ${process.argv[1]} ` + "<verb> [OPTION]... [--] <file>..."),
  "\nValid verbs are:",
  "    parse      parse headers",
  "    transform  transform source C API into C++ API",
  "    update     update target headers",
  "    help       Show help",
];

/**
 * Prints error with a guide
 * @param {string} message 
 * @param {string[]} parameters 
 */
function printErrorWithGuide(message, ...parameters) {
  return printError(message, ...parameters, "", ...guideDoc);
}

/**
 * Prints error
 * @param {string} message 
 * @param {string[]} parameters 
 */
function printError(message, ...parameters) {
  return printLog(`Error: ${message}`, ...parameters);
}

/**
 * Scan files
 * @param {string[]} args the arguments
 */
function parse(args) {
  if (args?.length == 0) {
    return help(["parse"]);
  }
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
        mergeInto(config, readJSONSync(arg.replaceAll("\\", '/')));
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
      case '-c': mergeInto(config, readJSONSync(args[++i].replaceAll("\\", '/'))); break;
      case '--print-config': printConfig = true; break;
      case '--store-line-numbers':
      case '-l': config.storeLineNumbers = true; break;
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
    writeJSONSync(config.outputFile ? 1 : 2, config);
  }
  const api = parseApi(config);
  writeJSONSync(config.outputFile || 1, api);
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
  if (args?.length == 0) {
    return help("update");
  }
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
      case '-t': config.target = readJSONSync(args[++i].replaceAll("\\", '/')); break;
      case '-d': config.baseDir = args[++i].replaceAll("\\", '/'); break;
      case '-c': mergeInto(config, readJSONSync(args[++i].replaceAll("\\", '/'), "utf8")); break;
      default:
        throw new Error(`Invalid option ${arg}`);
    }
  }
  if (!config.target) {
    if (!config.files?.length) throw new Error("Missing target");
    config.target = readJSONSync(config.files.shift());
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

function help(args = []) {
  switch (args[0]) {
    case "parse":
      printLog(
        "Parse API from header files",
        "",
        wrapUsageText(
          `usage: node ${process.argv[1]} `,
          "parse [-c] [ <config-file>] [-o <output-file>] [-d <base-dir>] [--] <input>..."),
        "",
        wrapText("If no base-dir is defined, we try to deduce the closest common ancestor from the inputs. If no output file is given or if it is a single dash (\" - \") it just outputs on stdout. If the first filename ends with \".json\" it interprets it as a config file, making the \" - c\" optional. Multiple configurations can be added and their content is merged."),
      );
      break;
    default:
      printLog("Transform OO-like C APIs into actual C++ OO API\n", ...guideDoc);
      break;
  }
}

/**
 * 
 * @param {string} prefix 
 * @param {string} parameters 
 */
function wrapUsageText(prefix, parameters = "") {
  const wordRegex = /([^\[<\s]|\[[^\]]+\]|<[^>]+>)+/;
  let margin = prefix.length;
  if (margin > 40) {
    margin = 8;
  }
  return wrapText(prefix + parameters, { margin, wordRegex });
}

/**
 * Format text int 80 columns
 * @param {string} text the text to wrap
 * @param {object} config
 * @param {number=}     config.margin     the left margin, defaults to 0
 * @param {wordRegex=}  config.wordRegex  the word definition, defaults to /[^\s]+/
 * @param {number=}     config.columns    the number of columns, default to 80
 */
function wrapText(text, config = {}) {
  const margin = config.margin ?? 0;
  const wordRegex = config.wordRegex ?? /[^\s]+/;
  const columns = config.columns ?? 80;

  const result = [];
  let currentLine = "";
  const marginText = " ".repeat(margin);
  while (text.length) {
    const m = text.match(wordRegex);
    if (!m) break;
    const word = m[0];
    const wordStart = m.index;
    const wordEnd = wordStart + word.length;
    if (currentLine.length + wordEnd < columns || !currentLine) {
      currentLine += " ".repeat(wordStart) + word;
    } else {
      result.push(currentLine);
      currentLine = marginText + word;
    }
    text = text.slice(wordEnd);
  }
  if (currentLine) result.push(currentLine);

  return result.join('\n');
}

/**
 * Print into error/log out
 * 
 * @param {string} message the message line
 * @param  {...string} parameters optionally more message lines
 */
function printLog(message, ...parameters) {
  writeLinesSync(2, [message, ...parameters]);
}

main(process.argv.slice(2));
