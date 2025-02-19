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
 * @property {boolean=}       immutable
 * @property {boolean=}       static
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

/**
 * @typedef {object} ApiTransform
 * @prop {FileTransformMap}      files
 * @prop {(string|string[])=}    prefixes
 * @prop {ReplacementRule[]=}    renameRules
 * @prop {ReplacementRule[]=}    docRules
 * @prop {StringMap=}            typeMap
 * @prop {StringMap=}            paramTypeMap
 * @prop {StringMap=}            returnTypeMap
 */


/**
 * @typedef {object} FileTransform
 * @prop {string[]=}   includeDefs
 * @prop {string[]=}   ignoreEntries
 * @prop {ApiEntries=} transform
 * @prop {ApiEntries=} includeAfter
 */


/**
 * @typedef {{[name: string]: FileTransform}} FileTransformMap
 */

/**
 * @typedef {object} ReplacementRule
 * @prop {RegExp} pattern
 * @prop {string} replacement
 */


/**
 * @typedef {ApiEntryKind|"doc"|"namespace"|"template"|"endStruct"} FileTokenKind
 */

/**
 * @typedef {object} FileToken
 * @property {string}         value
 * @property {FileTokenKind}  kind
 * @property {string=}        parameters
 * @property {string=}        type
 * @property {boolean=}       constexpr
 * @property {boolean=}       immutable
 * @property {boolean=}       static
 * @property {number}         begin
 * @property {number}         end
 * @property {number}         spaces
 */
