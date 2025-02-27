const { readLinesSync, system } = require("./utils");
const { tokenize } = require("./tokenize.js");

/**
 * @typedef {object} ParseConfig
 * @prop {string}   baseDir
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
    const content = readLinesSync(baseDir + name);
    api.files[name] = parseContent(name, content, config);
  }
  return api;
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
  const tokens = tokenize(content);
  if (!config) config = {};

  /** @type {ApiFile} */
  const apiFile = {
    name,
    doc: '',
    entries: {},
  };
  const parser = new ContentParser(tokens, config);
  const entryArray = parser.parseEntries();
  insertEntry(apiFile.entries, entryArray);
  if (config.storeLineNumbers) {
    apiFile.docBegin = parser.docBegin || 1;
    apiFile.docEnd = parser.docEnd || apiFile.docBegin;
    apiFile.entriesBegin = parser.entriesBegin || entryArray[0]?.begin || apiFile.docEnd;
    apiFile.entriesEnd = parser.entriesEnd || entryArray[entryArray.length - 1]?.end || apiFile.entriesBegin;
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
      if (entry.doc || !currEntry.doc) entries[name] = entry;
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
   * @param {FileToken[]}        tokens the tokens
   * @param {ParseContentConfig} config the configuration
   */
  constructor(tokens, config) {
    /** @private */
    this.tokens = tokens ?? [];
    this.storeLineNumbers = config.storeLineNumbers;
    this.docBegin = 0;
    this.doc = "";
    this.docEnd = 0;
    this.entriesBegin = 0;
    this.entriesEnd = tokens[tokens.length - 1]?.end;
  }

  /**
   * @private
   * @returns next token
   */
  next() { return this.tokens.shift(); }

  /**
  * @private
  * @returns next token, but does not consume it
  */
  lookup() { return this.tokens[0]; }

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

    while (this.lookup()?.kind === "doc") {
      const token = this.expect("doc");
      this.checkFileDoc(lastBegin, lastEnd, lastDoc);
      lastDoc = token.value;
      lastEnd = token.end;
      lastBegin = token.begin;
    }

    if (this.lookup()?.kind === "namespace") {
      const token = this.expect("namespace");
      this.checkFileDoc(lastBegin, lastEnd, lastDoc);
      if (!this.entriesBegin) {
        this.entriesBegin = token.end;
      }
      return this.parseEntry();
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
      doc: '',
      name: token.value,
      kind: /** @type {ApiEntryKind}*/(token.kind),
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
        if (token.parameters) entry.parameters = parseParams(token.parameters);
        break;
      case "enum":
        break;
      case "function":
        entry.type = normalizeType(token.type);
        entry.parameters = parseParams(token.parameters);
        if (token.constexpr) entry.constexpr = token.constexpr;
        if (token.immutable) entry.immutable = token.immutable;
        if (token.static) entry.static = token.static;
        break;
      case "struct":
        if (token.type) entry.type = normalizeType(token.type);
        if (!lastDecl) lastDecl = token.begin;
        entry.entries = insertEntry({}, this.parseEntries(token.spaces + 1));
        entryEnd = this.expect("endStruct").end;
        break;
      case "var":
        entry.type = normalizeType(token.type);
        break;
      case "forward":
        break;
      case "endStruct":
        if (lastTemplate) throw new Error(`Error at ${lastEnd}: Expected an entity after template signature`);
        this.entriesEnd = token.begin;
        this.tokens = [];
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
    if (defaultValue) return { name, type, default: defaultValue };
    return { name, type };
  });

}

/** @param {string} typeString  */
function normalizeType(typeString) {
  if (!typeString) return "";
  return typeString.replace(/(\w+)\s*([&*])/g, "$1 $2").replace(/([*&])\s+(&*)/g, "$1$2");
}

exports.parseApi = parseApi;
exports.parseContent = parseContent;
exports.insertEntry = insertEntry;
exports.removeEntryLineNumbers = removeEntryLineNumbers;
