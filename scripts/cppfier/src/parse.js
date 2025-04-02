const { readLinesSync, system } = require("./utils");
const { Tokenizer } = require("./tokenize.js");

/**
 * @import { Api, ApiEntries, ApiEntry, ApiEntryKind, ApiFile, ApiParameters, FileTokenKind } from "./types"
 */

/**
 * @typedef {object} ParseConfig
 * @prop {string[]} baseDir
 * @prop {string[]} sources
 * @prop {boolean=} storeLineNumbers
 */

/**
 * @param {ParseConfig} config
 * 
 */
function parseApi(config) {
  const { baseDir, sources } = config;

  /** @type {Api} */
  const api = { files: {} };
  for (const name of sources) {
    system.log(`Reading file ${name}`);
    const content = readContent(name, baseDir);
    api.files[name] = parseContent(name, content, config);
  }
  return api;
}

/**
 * The content to read
 * @param {string} name 
 * @param {string[]} baseDirs 
 */
function readContent(name, baseDirs) {
  for (const baseDir of baseDirs) {
    try {
      const content = readLinesSync(baseDir + name);
      return content;
    } catch (err) {
      system.log(`${name} not found at ${baseDir}, looking at next one`);
    }
  }
  throw new Error("File not found: " + name);
}

/**
 * @typedef {object} ParseContentConfig
 * @prop {boolean=} storeLineNumbers
 */

/**
 * 
 * @param {string} name
 * @param {string[]} content 
 * @param {ParseContentConfig=} config 
 */
function parseContent(name, content, config) {
  if (!config) config = {};

  /** @type {ApiFile} */
  const apiFile = {
    name,
    doc: '',
    entries: {},
  };
  const parser = new ContentParser(content, config);
  const entryArray = parser.parseEntries();
  insertEntry(apiFile.entries, entryArray);
  if (config.storeLineNumbers) {
    apiFile.docBegin = parser.docBegin || 1;
    apiFile.docEnd = parser.docEnd || apiFile.docBegin;
    apiFile.entriesBegin = apiFile.docEnd || parser.entriesBegin || entryArray[0]?.begin;
    apiFile.entriesEnd = parser.entriesEnd || entryArray[entryArray.length - 1]?.end || apiFile.entriesBegin;
    apiFile.namespace = parser.namespace || undefined;
  }
  apiFile.doc = parser.doc;
  return apiFile;
}

/**
 * Insert entry into entries
 * 
 * @param {ApiEntries}                 entries 
 * @param {ApiEntry|ApiEntry[]|string} entry 
 * @param {string}                     defaultName
 */
function insertEntry(entries, entry, defaultName = "") {
  if (Array.isArray(entry)) {
    entry.forEach(e => insertEntry(entries, e, defaultName));
    return entries;
  }
  if (typeof entry === "string") {
    entry = /** @type {ApiEntry} */({
      kind: entry,
      name: defaultName
    });
  }
  else if (!entry.name) entry.name = defaultName;
  fixEntry(entry);
  const name = entry.kind == "forward" ? entry.name + "-forward" : entry.name;
  if (entries[name]) {
    const currEntry = entries[name];
    if (Array.isArray(currEntry)) {
      currEntry.push(entry);
    } else if (currEntry.kind != 'function') {
      if (entry.doc || !currEntry.doc) {
        if (entry.kind === "def") {
          currEntry.doc = entry.doc;
        } else {
          entries[name] = entry;
        }
      }
    } else if (entry.kind == 'function') {
      entries[name] = [currEntry, entry];
    }
  } else {
    entries[name] = entry;
  }
  return entries;
}

/**
 * Add missing fields
 * @param {ApiEntry} entry 
 */
function fixEntry(entry) {
  if (typeof entry.doc != "string") entry.doc = "";
  if (entry.entries) {
    for (const subEntry of Object.values(entry.entries)) {
      if (Array.isArray(subEntry)) {
        subEntry.forEach(fixEntry);
      } else if (typeof subEntry === 'object') {
        fixEntry(subEntry);
      }
    }
  } else if (entry.kind == "struct") {
    entry.entries = {};
  }
}

/**
 * Parses a list of tokens into Entry
 */
class ContentParser {
  /**
   * 
   * @param {string[]}           lines the tokens
   * @param {ParseContentConfig} config the configuration
   */
  constructor(lines, config) {
    /** @private */
    this.tokenizer = new Tokenizer(lines.slice());
    this.nextToken = this.tokenizer.next();
    this.storeLineNumbers = config.storeLineNumbers;
    this.docBegin = 0;
    this.doc = "";
    this.docEnd = 0;
    this.entriesBegin = 0;
    this.entriesEnd = 0;
    this.namespace = "";
  }

  /**
   * @private
   * @returns next token
   */
  next() {
    const token = this.nextToken;
    if (token !== null) {
      this.entriesEnd = token.end;
      this.nextToken = this.tokenizer.next();
    }
    return token;
  }

  /**
  * @private
  * @returns next token, but does not consume it
  */
  lookup() { return this.nextToken; }

  /**
   * Parse all entries
   * @param {number} identLevel the level of ident
   */
  parseEntries(identLevel = 0) {
    const entries = [];
    for (let entry = this.parseEntry(identLevel); !!entry; entry = this.parseEntry(identLevel)) {
      entries.push(entry);
    }
    return entries;
  }

  /**
   * Parses a single entry
   * @param {number} identLevel the level of ident
   * @returns {ApiEntry}
   */
  parseEntry(identLevel = 0) {
    let lastEnd = 0;
    let lastDecl = 0;
    let lastBegin = 0;
    let lastDoc = "";
    /** @type {ApiParameters} */
    let lastTemplate = null;

    if ((this.lookup()?.spaces ?? -1) < identLevel) return undefined;
    if (this.lookup()?.kind === "endStruct") return undefined;

    while (this.lookup()?.kind === "doc") {
      const token = this.expect("doc");
      this.checkFileDoc(lastBegin, lastEnd, lastDoc);
      lastDoc = token.value;
      lastEnd = token.end;
      lastBegin = token.begin;
    }
    if (this.lookup()?.kind === "template") {
      const token = this.expect("template");
      if (lastEnd != token.begin || lastTemplate) {
        this.checkFileDoc(lastBegin, lastEnd, lastDoc);
        lastBegin = token.begin;
        lastDoc = null;
      }
      lastTemplate = parseParams(token.parameters);
      if (!lastDecl) lastDecl = token.begin;
      lastEnd = token.end;
    }
    const token = this.next();
    if (!token) {
      if (lastTemplate) throw new Error(`Error at ${lastEnd}: Expected an entity after template signature`);
      return undefined;
    }
    let entryEnd = token.end;

    /** @type {ApiEntry} */
    const entry = {
      doc: token.doc ?? '',
      name: token.name,
      kind: /** @type {ApiEntryKind}*/(token.kind),
      value: token.value,
    };
    switch (token.kind) {
      case "alias":
        if (token.type) entry.type = normalizeType(token.type);
        break;
      case "callback":
        entry.type = normalizeType(token.type);
        entry.parameters = parseParams(token.parameters);
        break;
      case "def":
        if (token.parameters != null) entry.parameters = parseParams(token.parameters);
        break;
      case "enum":
        if (!lastDecl) lastDecl = token.begin;
        entry.entries = insertEntry({}, this.parseEntries(token.spaces + 1));
        entryEnd = this.expect("endStruct").end;
        break;
      case "function":
        entry.type = normalizeType(token.type);
        entry.parameters = parseParams(token.parameters);
        if (token.constexpr) entry.constexpr = token.constexpr;
        if (token.immutable) entry.immutable = token.immutable;
        if (token.static) entry.static = token.static;
        if (token.reference) entry.reference = token.reference;
        break;
      case "struct":
        if (token.type) entry.type = normalizeType(token.type);
        if (!lastDecl) lastDecl = token.begin;
        entry.entries = insertEntry({}, this.parseEntries(token.spaces + 1));
        entryEnd = this.expect("endStruct").end;
        break;
      case "ns":
        if (!lastDecl) lastDecl = token.begin;
        if (!this.namespace) {
          this.namespace = entry.name;
          this.checkFileDoc(lastBegin, lastEnd, lastDoc);
          if (!this.entriesBegin) {
            this.entriesBegin = token.end;
          }
          return this.parseEntry();
        }
        entry.entries = insertEntry({}, this.parseEntries(token.spaces));
        entryEnd = this.expect("endStruct").end;
        break;
      case "union": break;
      case "var":
        entry.type = normalizeType(token.type);
        if (token.constexpr) entry.constexpr = token.constexpr;
        if (token.static) entry.static = token.static;
        break;
      case "forward":
        break;
      case "endStruct":
        if (lastTemplate) throw new Error(`Error at ${lastEnd}: Expected an entity after template signature`);
        this.entriesEnd = token.begin;
        this.nextToken = null;
        return undefined;
      default:
        throw new Error(`Error at ${token.begin}: Unexpected ${token.kind}`);
    }
    if (token.begin == lastEnd) {
      if (lastDoc) entry.doc = lastDoc;
      if (lastTemplate) entry.template = lastTemplate;
      if (this.storeLineNumbers) {
        entry.begin = lastBegin;
        entry.decl = lastDecl || token.begin;
        entry.end = entryEnd;
      }
    } else {
      if (lastDoc) this.checkFileDoc(lastBegin, lastEnd, lastDoc);
      if (this.storeLineNumbers) {
        entry.begin = token.begin;
        entry.decl = token.begin;
        entry.end = entryEnd;
      }
    }
    return entry;
  }

  /**
   * Check if this is the file doc
   * @private
   * @param {number} begin 
   * @param {number} end 
   * @param {string} doc 
   */
  checkFileDoc(begin, end, doc) {
    if (!this.docBegin && begin) {
      this.docBegin = begin;
      this.docEnd = end;
      this.doc = doc;
    }
  }

  /**
   * Returns token only if matches the specific kind
   * @param {FileTokenKind} kind 
   */
  expect(kind) {
    const token = this.next();
    if (!token) throw new Error(`Unexpected premature end of file`);
    if (token.kind !== kind) throw new Error(`Error at ${token.begin}: expected ${kind} got ${token.kind}`);
    return token;
  }
}

/**
 * Remove line numbers from entry
 * @param {ApiEntry} entry 
 */
function removeEntryLineNumbers(entry) {
  delete entry.decl;
  delete entry.begin;
  delete entry.end;
}

/**
 * 
 * @param {string} params 
 * @returns {ApiParameters}
 */
function parseParams(params) {
  if (!params?.length || params == 'void') {
    return [];
  }
  return params.split(',').map(param => {
    param = param.trim();
    let defaultValue;
    const defaultIndex = param.indexOf('=');
    if (defaultIndex > -1) {
      defaultValue = param.slice(defaultIndex + 1).trimStart();
      param = param.slice(0, defaultIndex).trimEnd();
    }
    const nameIndex = param.lastIndexOf(' ');
    if (nameIndex == -1) return param;
    let name = param.slice(nameIndex + 1).trim();
    let type = param.slice(0, nameIndex).trim();
    while (name.startsWith('*') || name.startsWith('&')) {
      type += name[0];
      name = name.slice(1);
    }
    type = normalizeType(type);
    if (name.startsWith('(&')) name += ')[N]';
    if (defaultValue) return { name, type, default: defaultValue };
    return { name, type };
  });

}

/** @param {string} typeString  */
function normalizeType(typeString) {
  if (!typeString) return "";
  return typeString
    .replace(/(\w+)\s*([&*])/g, "$1 $2")
    .replace(/([*&])\s+(&*)/g, "$1$2")
    .replace(/([<(\[])\s+/g, "$1")
    .replace(/\s+([>)\]])/g, "$1")
    .replace(/\s\s+/g, " ");
}

exports.parseApi = parseApi;
exports.parseContent = parseContent;
exports.insertEntry = insertEntry;
exports.readContent = readContent;
exports.parseParams = parseParams;
exports.removeEntryLineNumbers = removeEntryLineNumbers;
