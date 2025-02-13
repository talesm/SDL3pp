/**
 * @typedef {object} Api
 * @property {ApiFileMap} files
 */

/**
 * @typedef {{[file: string]: ApiFile}} ApiFileMap
 */

/**
 * @typedef {object} ApiFile
 * @property {string} name
 * @property {string=} doc
 * @property {ApiEntries} entries
 * @property {number=} docBegin
 * @property {number=} docEnd
 * @property {number=} entriesBegin
 * @property {number=} entriesEnd
 */

/**
 * @typedef {{[name: string]: ApiEntry|ApiEntry[]}} ApiEntries
 */

/**
 * @typedef {object} ApiEntry
 * @property {string} name
 * @property {'alias'|'callback'|'def'|'enum'|'function'|'struct'|'union'|'forward'} kind
 * @property {string=} type
 * @property {ApiParameters=} parameters
 * @property {ApiParameters=} template
 * @property {string=} sourceName
 * @property {boolean=} constexpr
 * @property {string} doc
 * @property {number=} begin
 * @property {number=} decl
 * @property {number=} end
 */

/**
 * @typedef {(string|ApiParameter)[]} ApiParameters
 */

/**
 * @typedef {object} ApiParameter
 * @property {string}  name
 * @property {string=} type
 */

/**
 * @typedef {{[name: string]: string}} StringMap
 */
