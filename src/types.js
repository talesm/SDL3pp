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
 */

/**
 * @typedef {{[name: string]: ApiEntry|ApiEntry[]}} ApiEntries
 */

/**
 * @typedef {object} ApiEntry
 * @property {string} name
 * @property {'alias'|'callback'|'def'|'enum'|'function'|'struct'|'union'|'forward'} kind
 * @property {string=} type
 * @property {(string|ApiParameter)[]=} parameters
 * @property {(string|ApiParameter)[]=} template
 * @property {string=} sourceName
 * @property {boolean=} constexpr
 * @property {string} doc
 * @property {number} begin
 * @property {number} decl
 * @property {number} end
 */


/**
 * @typedef {object} ApiParameter
 * @property {string}  name
 * @property {string=} type
 */
