import { readFileSync, writeSync, openSync, closeSync } from "fs";
import { chdir } from "process";

/** @param {string[]} args  */
function main(args) {
  let targetName = '';
  let baseDir = "./include/SDL3pp";
  for (let i = 2; i < args.length; i++) {
    const arg = args[i];
    if (!arg.startsWith('-')) {
      targetName = arg;
    } else if (arg == '--base-dir') {
      baseDir = args[++i];
    } else {
      throw new Error("Invalid option " + arg);
    }
  }

  chdir(baseDir);
  const result = parseFileRecursive("SDL3pp.h");

  if (targetName) {
    if (!(/^[~/]/.test(targetName))) {
      targetName = "../../" + targetName;
    }
    const fd = openSync(targetName, "w+");
    writeHeader(fd, result);
    closeSync(fd);
  } else {
    writeHeader(1, result);
  }
}

/**
 * Writes everything
 * 
 * @param {number} fd 
 * @param {ParsedFileResult} result 
 */
function writeHeader(fd, result) {
  const files = sortHierarchy(result.files).filter(f => f.content?.length);

  // Disclaimer
  writeSync(fd, "// Amalgamated SDL3pp\n");

  // TODO: Defines

  // Begin guard
  writeSync(fd, "#ifndef SDL3PP_H_\n#define SDL3PP_H_\n\n");

  // Includes
  writeSync(fd, result.includes.map(inc => `#include <${inc}>`).join('\n') + "\n\n");

  // Unconditional files
  writeSync(fd, "namespace SDL {\n\n");
  for (const file of files.filter(f => !f.conditional)) {
    writeSync(fd, file.content.join("\n").trim() + "\n\n");
  }
  writeSync(fd, "} // namespace SDL\n");

  // Conditional files
  for (const file of files.filter(f => f.conditional)) {
    writeSync(fd, file.content.join("\n").trim() + "\n\n");
  }

  // End guard
  writeSync(fd, "#endif // SDL3PP_H_\n");
}

/**
 * 
 * @param {ParsedFile[]} files 
 */
function sortHierarchy(files) {
  /** @type {ParsedFile[]} */
  const result = [];
  const foundSet = new Set();

  while (foundSet.size < files.length) {
    const independentFiles = files.filter(file => {
      if (foundSet.has(file.name)) return false;
      for (const dep of file.deps) {
        if (!foundSet.has(dep)) return false;
      }
      return true;
    });
    independentFiles.sort((a, b) => (a.name < b.name ? -1 : (a.name > b.name ? +1 : 0)));
    for (const file of independentFiles) {
      const name = file.name;
      result.push(file);
      foundSet.add(name);
    }
  }

  return result;
}

/**
 * @typedef {object} ParsedFileResult
 * @prop {string[]} includes
 * @prop {ParsedFile[]} files
 */

/**
 * @param {string} filename 
 */
function parseFileRecursive(filename) {
  /** @type {ParsedFileResult} */
  const result = {
    includes: [],
    files: [],
  };
  const queue = [filename];
  const alreadyIncluded = new Set();
  const alreadySet = new Set();
  while (queue.length) {
    const name = queue.shift();
    if (!name) break;
    if (alreadySet.has(name)) continue;

    const fileInfo = parseFile(name);
    alreadySet.add(name);
    queue.push(...fileInfo.deps);
    result.files.push(fileInfo);

    if (!fileInfo.conditional) {
      for (const include of fileInfo.includes) {
        if (!alreadyIncluded.has(include)) {
          alreadyIncluded.add(include);
          result.includes.push(include);
        }
      }
      delete fileInfo.includes;
    }
  }
  // Sort includes
  result.includes.sort().push("SDL3/SDL.h");
  return result;
}

/**
 * @typedef {object} ParsedFile
 * @prop {string}       name
 * @prop {Set<string>}  deps
 * @prop {string[]}     includes
 * @prop {boolean=}     conditional
 * @prop {string[]=}    content
 */

/**
 * 
 * @param {string} filename 
 */
function parseFile(filename) {
  const lines = readFileSync(filename, "utf8").split('\n');
  /** @type {ParsedFile} */
  const result = {
    name: filename,
    deps: new Set(),
    includes: [],
  };
  /** @type {"begin"|"includes"|"content"} */
  let state = "begin";
  let contentStart = 0;
  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    /** @type {RegExpExecArray|null} */
    let m = null;
    switch (state) {
      case "begin":
        if (line.startsWith("#ifndef") && lines[i + 1].startsWith("#define")) {
          state = "includes";
          i++;
        }
        break;
      case "includes":
        if (m = /^#include (["<])([\w./*-]+)[">]/.exec(line)) {
          if (m[1] == '"') {
            result.deps.add(m[2]);
          } else if (!(m[2].startsWith("SDL3/"))) {
            result.includes.push(m[2]);
          }
        } else if (line.startsWith("namespace SDL {")) {
          if (!result.conditional) contentStart = i + 1;
          state = "content";
        } else if (line.startsWith("#if")) {
          result.conditional = true;
          contentStart = i;
          state = "content";
        };
        break;
      case "content":
        if (line.startsWith("} // namespace SDL")) {
          result.content = lines.slice(contentStart, result.conditional ? lines.length - 2 : i);
          return result;
        }
        break;
      default:
        throw new Error(`Unknown state ${state}`);
    }
  }
  if (state === "content") throw new Error(`Expected "} // namespace SDL" at file ${filename}`);
  return result;
}

main(process.argv);
