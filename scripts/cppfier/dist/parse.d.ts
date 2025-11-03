export type ParseConfig = {
    baseDir: string[];
    sources: string[];
    storeLineNumbers?: boolean | undefined;
};
export type ParseContentConfig = {
    storeLineNumbers?: boolean | undefined;
};
/**
 * @import { ApiEntries, ApiEntry, ApiEntryKind, ApiParameters, FileTokenKind, Api, ApiFile } from "./types"
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
declare function _parseApi(config: ParseConfig): Api;
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
declare function _parseContent(name: string, content: string[], config?: ParseContentConfig | undefined): ApiFile;
/**
 * Insert entry into entries
 *
 * @param {ApiEntries}          entries
 * @param {ApiEntry|ApiEntry[]} entry
 * @param {string}              defaultName
 */
declare function _insertEntry(entries: ApiEntries, entry: ApiEntry | ApiEntry[], defaultName?: string): ApiEntries;
/**
 * The content to read
 * @param {string} name
 * @param {string[]} baseDirs
 */
declare function _readContent(name: string, baseDirs: string[]): string[];
/**
 *
 * @param {string} params
 * @returns {ApiParameters}
 */
declare function _parseParams(params: string): ApiParameters;
/**
 * Remove line numbers from entry
 * @param {ApiEntry} entry
 */
declare function _removeEntryLineNumbers(entry: ApiEntry): void;
import type { Api } from "./types";
import type { ApiFile } from "./types";
import type { ApiEntries } from "./types";
import type { ApiEntry } from "./types";
import type { ApiParameters } from "./types";
export { _parseApi as parseApi, _parseContent as parseContent, _insertEntry as insertEntry, _readContent as readContent, _parseParams as parseParams, _removeEntryLineNumbers as removeEntryLineNumbers };
