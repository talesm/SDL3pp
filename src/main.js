const { writeFileSync } = require("fs");
const { parseApi } = require("./parse-api.js");
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
      case '': config.outputFile = ""; break;
      case '-o': config.outputFile = args[++i].replaceAll("\\", '/'); break;
      case '-d': config.baseDir = args[++i].replaceAll("\\", '/'); break;
      default:
        throw new Error(`Invalid option ${arg}`);
    }
  }
  if (!config.files.length) return;
  if (!config.baseDir && config.files[0].includes('/')) {
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
  const api = JSON.stringify(parseApi(config.baseDir, config.files), null, 2);
  if (config.outputFile) {
    writeFileSync(config.outputFile, api);
  } else {
    console.log(api);
  }
}

main(process.argv.slice(2));
