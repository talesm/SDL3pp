const { system } = require("./utils");
/**
 * @import {FileToken} from "./types"
 */

const ignorePrefixes = [
  'void *alloca',
  'SDL_COMPILE_TIME_ASSERT(',
  'extern "C" {',
  'size_t strl',
  'size_t wcsl',
  'char *str',
  'public:',
  'private:',
  'protected:',
];

const ignoreInSignature = new RegExp(`(${[
  'extern',
  'SDL_FORCE_INLINE',
  'SDL_DECLSPEC',
  'SDL_MALLOC',
  'SDL_ALLOC_SIZE2\\(1,\\s*2\\)',
  'SDL_ALLOC_SIZE',
  'SDLCALL',
  'SDL_(OUT|IN|INOUT)_(Z_)?(BYTE)?CAP',
].join("|")})(\\(\\w*\\))?`, "g");

const memberSpecifiers = new Set(["inline", "static", "constexpr"]);

const spaceRegex = /^\s+/;
const endCommentRegex = /\*\//;
const continueCommentRegex = /^\s*\*\s?/;

class Tokenizer {
  /** @param {string[]} lines */
  constructor(lines) {
    this.lines = lines;
    this.lineCount = 0;
    this.lastLine = "";
  }

  /** @private */
  nextLine() {
    if (!this.lines?.length) return null;
    this.lineCount++;
    this.lastLine = this.lines.shift();
    return this.lastLine;
  }

  /** @private */
  peekLine() {
    if (!this.lines?.length) return null;
    return this.lines[0];
  }

  nonEmptyLine() {
    let line;
    do {
      line = this.nextLine()?.trim();
    } while (line === "" || hasIgnoredPrefix(line));
    return line;
  }

  finish() {
    this.lines = [];
    return null;
  }


  /**
   * Next line
   * 
   * @returns {FileToken}
   */
  next() {
    const line = this.nonEmptyLine();
    if (line == null) return null;

    let m = spaceRegex.exec(this.lastLine);
    /** @type {FileToken} */
    const token = {
      name: undefined,
      begin: this.lineCount,
      end: null,
      spaces: m?.[0]?.length ?? 0,
      kind: null,
      value: "",
    };

    if (/^\/\/\s*Forward decl/.test(line)) {
      token.kind = "doc";
    } else if (/^#pragma\s+region\s+impl/.test(line)) {
      return this.finish();
    } else if (line.startsWith("}") || line.startsWith("{}")) {
      token.kind = "endStruct";
    } else if (line.startsWith("//")) {
      return this.next();
    } else if (m = /^\/\*(\*)?/.exec(line)) {
      let doc = '';
      const isDoc = !!m[1];
      let ln = line;
      while (ln !== null) {
        if (endCommentRegex.test(ln)) {
          break;
        }
        const m = continueCommentRegex.exec(ln);
        doc += m ? ln.slice(m[0].length) : ln;
        doc += '\n';
        ln = this.nextLine();
      }
      if (isDoc) {
        token.kind = "doc";
        token.value = doc.replace('/**', '').trim();
      } else {
        return this.next();
      }
    } else if (m = /^#define\s+(\w+)(\(([\w\s,]*)\))?/.exec(line)) {
      token.kind = "def";
      token.value = m[1];
      if (m[2]) token.parameters = m[3]?.trim();
      let ln = line;
      while (ln.endsWith('\\')) {
        ln = this.nextLine();
        if (ln === null) break;
      }
      if (token.value.endsWith('_')) return this.next();
      token.doc = checkInlineDoc(line);
    } else if (m = /^using\s+(\w+)\s*=\s*([^;]*)(;?)/.exec(line)) {
      token.kind = "alias";
      token.value = m[1];
      if (m[3]) {
        token.type = m[2].trimEnd();
      } else {
        let type = m[2];
        let line;
        while ((line = this.nextLine()) !== null) {
          if (line.endsWith(";")) {
            type += " " + line.slice(0, line.length - 1);
            break;
          }
          type += " " + line;
        }
        token.type = type.trim();
      }
    } else if (m = /^using\s+([\w:]+)\s*;/.exec(line)) {
      token.kind = "alias";
      token.value = m[1];
    } else if (m = /^(?:struct|class)\s+([\w<>]+);/.exec(line)) {
      token.kind = "forward";
      token.value = m[1];
    } else if ((m = /^(?:constexpr )?(?:struct|class)\s+([\w<>]+)\s*(:\s*([\w<>,\s]+))?/.exec(line)) && !line.includes(";")) {
      token.kind = "struct";
      token.value = m[1];
      if (m[3]) {
        token.type = m[3].trim();
      } else if (this.peekLine()?.trimStart().startsWith(":")) {
        token.type = this.nextLine().trimStart().slice(1).trim();
      }
      if (this.peekLine()?.endsWith("{")) this.nextLine();
    } else if (m = /^template</.exec(line)) {
      token.kind = "template";
      if (line.endsWith(">")) {
        token.parameters = line.slice(m[0].length, line.length - 1).trim();
      } else {
        let parameters = line.slice(m[0].length) ?? "";
        for (let line = this.nextLine(); line !== null; line = this.nextLine()) {
          if (line.endsWith('>')) {
            parameters += '\n' + line.slice(0, line.length - 1);
            break;
          }
          parameters += '\n' + line;
        }
        token.parameters = parameters;
      }
    } else if (m = /^namespace\s+([^{]*)\{/.exec(line)) {
      token.kind = "ns";
      token.value = m[1]?.trim();
    } else if (line.startsWith('#')) {
      let ln = line;
      while (ln.endsWith('\\')) {
        ln = this.nextLine();
        if (ln === null) break;
      }
      return this.next();
    } else {
      const member = line.replaceAll(ignoreInSignature, "").trimStart();
      m = /^(([\w*&:<>,\[\]]+\s+)*)(operator(?:\(\)|\[\]|<=>|[-+<>=!%]{1,2})|[\w*&~:<>]+)(\s*\()?/.exec(member);
      if (!m) {
        system.warn(`Unknown token at line ${this.lineCount}: ${member}`);
        return this.next();
      }
      let name = m[3];
      const typeWords = m[1]?.trim()?.split(/\s+/) ?? [];
      let type = "";
      for (let i = 0; i < typeWords.length; i++) {
        const word = typeWords[i];
        if (!memberSpecifiers.has(word)) {
          type = typeWords.slice(i).join(" ");
          break;
        }
        switch (word) {
          case "constexpr": token.constexpr = true; break;
          case "static": token.static = true; break;
        }
      }
      while (type.endsWith(',')) {
        const ind = type.lastIndexOf(' ');
        if (ind === -1) {
          name = type + " " + name;
          type = "";
          break;
        }
        name = type.slice(ind + 1) + " " + name;
        type = type.slice(0, ind);
      }
      token.value = name;
      token.type = type;

      if (m[4]) {
        token.kind = "function";
        let parameters = member.slice(m[0].length);
        const endBracket = parameters.indexOf(")");
        if (endBracket < 0) {
          let line;
          while ((line = this.nextLine()) !== null) {
            const m = /\)\s*(const)?(&{0,2});?$/.exec(line);
            if (m) {
              parameters += '\n' + line.slice(0, line.length - m[0].length);
              if (m[1]) token.immutable = true;
              if (m?.[2]) token.reference = m[2].length;
              break;
            }
            parameters += '\n' + line;
          }
        } else {
          const details = parameters.slice(endBracket + 1);
          parameters = parameters.slice(0, endBracket);
          const m = /^\s+(const)?(&{0,2})/.exec(details);
          if (m?.[1]) token.immutable = true;
          if (m?.[2]) token.reference = m[2].length;
        }
        token.parameters = parameters;
        if (type.startsWith("operator")) {
          token.type = "";
          token.value = type + " " + (token.value.replace(/(\w+)([*&])/g, "$1 $2"));
        }
      } else {
        token.kind = "var";
        token.doc = checkInlineDoc(member);
      }
      if (m = /^((?:[*&]\s*)+)(\w+)\s*$/.exec(token.value)) {
        token.type += " " + m[1];
        token.value = m[2];
      }
      this.ignoreBody(token.spaces);
    }
    this.extendToNextStart();
    token.end = this.lineCount + 1;
    if (checkTokenTooLarge(token)) {
      system.warn(`Warning: Token at ${token.begin} seems very large ${token.value} (${token.end - token.begin} lines)`);
    }
    return token;
  }

  extendToNextStart() {
    let line = this.peekLine();
    while (line !== null) {
      if (line.trim()) break;
      this.nextLine();
      line = this.peekLine();
    }
  }

  /**
   * 
   * @param {number} spaces 
   */
  ignoreBody(spaces) {
    let opened = false;
    if (!this.lastLine.endsWith("{")) {
      const line = this.peekLine().trim();
      if (line.startsWith("{")) {
        this.nextLine();
        if (line.endsWith("}")) return;
        opened = true;
      }
    } else {
      opened = true;
    }
    const spaceRegex = /^\s+/;
    let line = this.peekLine();
    while (line !== null) {
      if (line.trim()) {
        const indentation = spaceRegex.exec(line)?.[0]?.length ?? 0;
        if (indentation <= spaces) {
          if (line.slice(indentation).startsWith("{")) {
            opened = true;
            if (line.endsWith("}") || line.endsWith("};")) {
              return;
            }
          } else if (line.slice(indentation).startsWith("}") && opened) {
            this.nextLine();
            return;
          } else return;
        }
      }
      this.nextLine();
      line = this.peekLine();
    }
  }
}

/**
 * 
 * @param {string[]} lines
 */
function tokenize(lines) {
  /** @type {FileToken[]} */
  const result = [];
  const tokenizer = new Tokenizer(lines);
  for (let token = tokenizer.next(); token != null; token = tokenizer.next()) {
    result.push(token);
  }
  return result;
}

/** @param {FileToken} token  */
function checkTokenTooLarge(token) {
  const delta = token.end - token.begin;
  if (token.kind == "doc") return false;
  if (token.kind == "enum") return delta > 200;
  if (token.kind == "union") return delta > 100;
  if (token.kind == "function" || token.kind == "callback" || token.kind == "def") return delta > 30;
  return delta > 10;
}

/** @param {string} str  */
function checkInlineDoc(str) {
  const m = str.match(/\/\*\*<(.*)\*\//);
  if (m) return m[1].trim();
}

/** @param {string} line  */
function hasIgnoredPrefix(line) {
  for (const prefix of ignorePrefixes) {
    if (line?.startsWith(prefix)) {
      return true;
    }
  }
  return false;
}

exports.Tokenizer = Tokenizer;
exports.tokenize = tokenize;
