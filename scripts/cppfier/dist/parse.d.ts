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
export function parseApi(config: ParseConfig): Api;
/**
 * The content to read
 * @param {string} name
 * @param {string[]} baseDirs
 */
export function readContent(name: string, baseDirs: string[]): string[];
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
export function parseContent(name: string, content: string[], config?: ParseContentConfig | undefined): ApiFile;
/**
 * Insert entry into entries
 *
 * @param {ApiEntries}          entries
 * @param {ApiEntry|ApiEntry[]} entry
 * @param {string}              defaultName
 */
export function insertEntry(entries: ApiEntries, entry: ApiEntry | ApiEntry[], defaultName?: string): ApiEntries;
/**
 * Remove line numbers from entry
 * @param {ApiEntry} entry
 */
export function removeEntryLineNumbers(entry: ApiEntry): void;
/**
 *
 * @param {string} params
 * @returns {ApiParameters}
 */
export function parseParams(params: string): ApiParameters;
export type ParseConfig = {
    baseDir: string[];
    sources: string[];
    storeLineNumbers?: boolean | undefined;
};
export type ParseContentConfig = {
    storeLineNumbers?: boolean | undefined;
};
import type { Api } from "./types";
import type { ApiFile } from "./types";
import type { ApiEntries } from "./types";
import type { ApiEntry } from "./types";
import type { ApiParameters } from "./types";
