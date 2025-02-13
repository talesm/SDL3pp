const { readFileSync, writeFileSync } = require("node:fs");

/**
 * Read a file as an array of lines
 * @param {import("node:fs").PathOrFileDescriptor} path 
 */
function readLinesSync(path) {
  return readFileSync(path, "utf8").split(/\r?\n/);
}

/**
 * Write an array of lines to a file
 * @param {import("node:fs").PathOrFileDescriptor} path 
 * @param {string[]} data 
 */
function writeLinesSync(path, data) {
  return writeFileSync(path, data.join('\n').trim() + "\n");
}

/**
 * Read a file as a JSON
 * @param {import("node:fs").PathOrFileDescriptor} path 
 */
function readJSONSync(path) {
  return JSON.parse(readFileSync(path, "utf8"));
}

/**
 * Write an array of lines to a file
 * @param {import("node:fs").PathOrFileDescriptor} path 
 * @param {any} data 
 */
function writeJSONSync(path, data) {
  return writeFileSync(path, JSON.stringify(data, null, 2) + "\n");
}

var system = {
  silent: true,
  stopOnWarn: true,

  log(...data) {
    if (!system.silent) console.log(...data);
  },
  warn(...data) {
    if (!system.silent) console.warn(...data);
    if (this.stopOnWarn) {
      let message = "Stopped on warning";
      if (this.silent && data[0]) message += ": " + data[0] ?? "";
      throw new Error(message);
    }
  },
};

exports.readLinesSync = readLinesSync;
exports.writeLinesSync = writeLinesSync;
exports.readJSONSync = readJSONSync;
exports.writeJSONSync = writeJSONSync;
exports.system = system;
