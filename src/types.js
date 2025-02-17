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
 * @typedef {"alias"|"callback"|"def"|"enum"|"forward"|"function"|"struct"|"union"|"var"} ApiEntryKind
 */

/**
 * @typedef {object} ApiEntry
 * @property {string}         name
 * @property {ApiEntryKind}   kind
 * @property {string=}        type
 * @property {ApiParameters=} parameters
 * @property {ApiParameters=} template
 * @property {ApiEntries}     entries
 * @property {string=}        sourceName
 * @property {boolean=}       constexpr
 * @property {string}         doc
 * @property {number=}        begin
 * @property {number=}        decl
 * @property {number=}        end
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
