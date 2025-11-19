import { readFileSync, writeSync, openSync, closeSync } from "node:fs";
import { chdir } from "node:process";

function main(args: string[]) {
  let targetName = "";
  let baseDir = "./include/SDL3pp";
  for (let i = 2; i < args.length; i++) {
    const arg = args[i];
    if (!arg.startsWith("-")) {
      targetName = arg;
    } else if (arg == "--base-dir") {
      baseDir = args[++i];
    } else {
      throw new Error("Invalid option " + arg);
    }
  }

  chdir(baseDir);
  const result = parseFileRecursive("SDL3pp.h");

  if (targetName) {
    if (!/^[~/]/.test(targetName)) {
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
 */
function writeHeader(fd: number, result: ParsedFileResult) {
  const files = sortHierarchy(result.files).filter((f) => f.content?.length);

  // Disclaimer
  writeSync(fd, "// Amalgamated SDL3pp\n");

  // TODO: Defines

  // Begin guard
  writeSync(fd, "#ifndef SDL3PP_H_\n#define SDL3PP_H_\n\n");

  // Includes
  writeSync(
    fd,
    result.includes.map((inc) => `#include <${inc}>`).join("\n") + "\n\n"
  );

  // Unconditional files
  writeSync(fd, "namespace SDL {\n\n");
  for (const file of files.filter((f) => !f.conditional)) {
    writeSync(fd, file.content?.join("\n").trim() + "\n\n");
  }
  writeSync(fd, "} // namespace SDL\n");

  // Conditional files
  for (const file of files.filter((f) => f.conditional)) {
    writeSync(fd, file.content?.join("\n").trim() + "\n\n");
  }

  // End guard
  writeSync(fd, "#endif // SDL3PP_H_\n");
}

function sortHierarchy(files: ParsedFile[]) {
  const result: ParsedFile[] = [];
  const foundSet = new Set();

  while (foundSet.size < files.length) {
    const independentFiles = files.filter((file) => {
      if (foundSet.has(file.name)) return false;
      for (const dep of file.deps) {
        if (!foundSet.has(dep)) return false;
      }
      return true;
    });
    independentFiles.sort((a, b) =>
      a.name < b.name ? -1 : a.name > b.name ? +1 : 0
    );
    for (const file of independentFiles) {
      const name = file.name;
      result.push(file);
      foundSet.add(name);
    }
  }

  return result;
}

interface ParsedFileResult {
  includes: string[];
  files: ParsedFile[];
}

function parseFileRecursive(filename: string) {
  const result: ParsedFileResult = {
    includes: [],
    files: [],
  };
  const queue = [filename];
  const alreadyIncluded = new Set<string>();
  const alreadySet = new Set<string>();
  while (queue.length) {
    const name = queue.shift();
    if (!name) break;
    if (alreadySet.has(name)) continue;

    const fileInfo = parseFile(name);
    alreadySet.add(name);
    queue.push(...fileInfo.deps);
    result.files.push(fileInfo);

    if (!fileInfo.conditional && fileInfo.includes) {
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

interface ParsedFile {
  name: string;
  deps: Set<string>;
  includes?: string[];
  conditional?: boolean;
  content?: string[];
}

function parseFile(filename: string) {
  const lines = readFileSync(filename, "utf8").split("\n");
  const result: ParsedFile = {
    name: filename,
    deps: new Set(),
    includes: [],
  };
  let state: "begin" | "includes" | "content" = "begin";
  let contentStart = 0;
  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    let m: RegExpExecArray | null = null;
    switch (state) {
      case "begin":
        if (line.startsWith("#ifndef") && lines[i + 1].startsWith("#define")) {
          state = "includes";
          i++;
        }
        break;
      case "includes":
        if ((m = /^#include (["<])([\w./*-]+)[">]/.exec(line))) {
          if (m[1] == '"') {
            result.deps.add(m[2]);
          } else if (!m[2].startsWith("SDL3/")) {
            result.includes?.push(m[2]);
          }
        } else if (line.startsWith("namespace SDL {")) {
          if (!result.conditional) contentStart = i + 1;
          state = "content";
        } else if (line.startsWith("#if")) {
          result.conditional = true;
          contentStart = i;
          state = "content";
        }
        break;
      case "content":
        if (line.startsWith("} // namespace SDL")) {
          result.content = lines.slice(
            contentStart,
            result.conditional ? lines.length - 2 : i
          );
          return result;
        }
        break;
      default:
        throw new Error(`Unknown state ${state}`);
    }
  }
  if (state === "content")
    throw new Error(`Expected "} // namespace SDL" at file ${filename}`);
  return result;
}

main(process.argv);
