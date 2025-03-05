const { system } = require("./utils");

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

/**
 * 
 * @param {string[]} lines
 */
function tokenize(lines) {
  /** @type {FileToken[]} */
  const result = [];
  const spaceRegex = /^\s+/;
  const endCommentRegex = /\*\//;
  const continueCommentRegex = /^\s*\*\s?/;
  for (let i = 0; i < lines.length; i++) {
    const lineUntrimmed = lines[i];
    const line = lineUntrimmed.trim();
    if (!line || hasIgnoredPrefix(line)) continue;

    let m = spaceRegex.exec(lineUntrimmed);
    /** @type {FileToken} */
    const token = {
      begin: i + 1,
      end: null,
      spaces: m?.[0]?.length ?? 0,
      kind: null,
      value: ""
    };

    if (/^\/\/\s*Forward decl/.test(line)) {
      token.kind = "doc";
    } else if (/^#pragma\s+region\s+impl/.test(line)) {
      break;
    } else if (line.startsWith("}") || line.startsWith("{}")) {
      token.kind = "endStruct";
    } else if (line.startsWith("//")) {
      continue;
    } else if (m = /^\/\*(\*)?/.exec(line)) {
      let doc = '';
      const isDoc = !!m[1];
      for (; i < lines.length; ++i) {
        const line = lines[i];
        if (endCommentRegex.test(line)) {
          break;
        }
        const m = continueCommentRegex.exec(line);
        doc += m ? line.slice(m[0].length) : line;
        doc += '\n';
      }
      if (isDoc) {
        token.kind = "doc";
        token.value = doc.replace('/**', '').trim();
      } else continue;
    } else if (m = /^#define\s+(\w+)(\(([\w\s,]+)\))?/.exec(line)) {
      token.kind = "def";
      token.value = m[1];
      if (m[2]) token.parameters = m[3]?.trim();
      let ln = line;
      while (ln.endsWith('\\')) {
        if (i > lines.length) break;
        ln = lines[++i];
      }
      if (token.value.endsWith('_')) continue;
    } else if (m = /^typedef\s+(([\w*]+\s+)+\**)(\w+);/.exec(line)) {
      token.kind = "alias";
      token.value = m[3];
      token.type = m[1].trimEnd();
    } else if (m = /^using\s+(\w+)\s*=\s*([^;]*)(;?)/.exec(line)) {
      token.kind = "alias";
      token.value = m[1];
      if (m[3]) {
        token.type = m[2].trimEnd();
      } else {
        let type = m[2];
        for (i = i + 1; i < lines.length; i++) {
          const line = lines[i];
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
    } else if (m = /^typedef\s+((\w+\s+)+\*?)\((SDLCALL )?\*(\w+)\)\(([^)]*)([\),])/.exec(line)) {
      token.value = m[4];
      token.kind = "callback";
      token.type = m[1].trimEnd();
      token.parameters = m[5]?.trim();
      if (m[6] === ",") {
        token.parameters += ",";
        for (i++; i < lines.length; i++) {
          const line = lines[i].trim();
          if (line.endsWith(");")) {
            token.parameters += " " + line.slice(line.length - 2);
            break;
          }
          token.parameters += " " + line;
        }
      }
    } else if (m = /^typedef\s+(struct|enum|union)\s+(\w+)?$/.exec(line)) {
      // @ts-ignore
      token.kind = m[1];
      token.value = m[2];
      if (token.kind != "struct") {
        i = ignoreBody(lines, i, token.spaces);
      } else if (!line.endsWith("{")) i++;
    } else if (m = /^(?:struct|class)\s+([\w<>]+);/.exec(line)) {
      token.kind = "forward";
      token.value = m[1];
    } else if (m = /^(?:struct|class)\s+([\w<>]+)\s*(:\s*([\w<>,\s]+))?/.exec(line)) {
      token.kind = "struct";
      token.value = m[1];
      if (m[3]) {
        token.type = m[3].trim();
      } else if (lines[i + 1].trimStart().startsWith(":")) {
        token.type = lines[++i].trimStart().slice(1).trim();
      }
      if (lines[i + 1]?.endsWith("{")) i++;
    } else if (m = /^template</.exec(line)) {
      token.kind = "template";
      if (line.endsWith(">")) {
        token.parameters = line.slice(m[0].length, line.length - 1).trim();
      } else {
        let parameters = line.slice(m[0].length) ?? "";
        for (++i; i < lines.length; ++i) {
          const line = lines[i];
          if (line.endsWith('>')) {
            parameters += '\n' + line.slice(0, line.length - 1);
            break;
          }
          parameters += '\n' + line;
        }
        token.parameters = parameters;
      }
    } else if (m = /^namespace\s+([^{]*)\{/.exec(line)) {
      token.kind = "namespace";
      token.value = m[1]?.trim();
    } else if (line.startsWith('#')) {
      let ln = line;
      while (ln.endsWith('\\')) {
        if (i > lines.length) break;
        ln = lines[i++];
      }
      continue;
    } else {
      const member = line.replaceAll(ignoreInSignature, "").trimStart();
      m = /^(([\w*&:<>\[\]]+\s+)*)(operator(?:\(\)|\[\]|<=>|[-+<>=!%]{1,2})|[\w*&~]+)(\s*\()?/.exec(member);
      if (!m) {
        system.warn(`Unknown token at line ${i + 1}: ${member}`);
        continue;
      }
      token.value = m[3];
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
      token.type = type;

      if (m[4]) {
        token.kind = "function";
        let parameters = member.slice(m[0].length);
        const endBracket = parameters.indexOf(")");
        if (endBracket < 0) {
          for (++i; i < lines.length; ++i) {
            const line = lines[i];
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
      }
      if (m = /^((?:[*&]\s*)+)(\w+)\s*$/.exec(token.value)) {
        token.type += " " + m[1];
        token.value = m[2];
      }
      i = ignoreBody(lines, i, token.spaces);
    }
    for (let j = i + 1; j < lines.length - 1; j++) {
      if (!lines[j].trim()) {
        i++;
      } else break;
    }
    token.end = i + 2;
    if (checkTokenTooLarge(token)) {
      system.warn(`Warning: Token at ${token.begin} seems very large ${token.value} (${token.end - token.begin} lines)`);
    }
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

/**
 * 
 * @param {string[]} lines 
 * @param {number} begin 
 * @param {number} spaces 
 */
function ignoreBody(lines, begin, spaces) {
  if (!lines[begin].endsWith("{")) {
    const line = lines[begin + 1].trim();
    if (line.startsWith("{")) {
      begin += 1;
      if (line.endsWith("}")) return begin;
    } else if (line.startsWith(":")) {
      for (begin += 2; begin < lines.length; begin++) {
        const line = lines[begin].trim();
        if (line.startsWith("{")) {
          if (line.endsWith("}")) return begin;
          break;
        }
      }
    } else {
      return begin;
    }
  } else begin++;
  const spaceRegex = /^\s+/;
  for (let i = begin + 1; i < lines.length; i++) {
    const line = lines[i];
    if (!line.trim()) continue;
    const indentation = spaceRegex.exec(line)?.[0]?.length ?? 0;
    if (indentation <= spaces) {
      if (line.slice(indentation).startsWith("}")) return i;
      return i - 1;
    }
  }
  return lines.length;
}

/** @param {string} line  */
function hasIgnoredPrefix(line) {
  for (const prefix of ignorePrefixes) {
    if (line.startsWith(prefix)) {
      return true;
    }
  }
  return false;
}

exports.tokenize = tokenize;
