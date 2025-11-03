export type GenerateApiConfig = {
    api: Api;
    baseDir: string;
};
export type GenerateApiFileConfig = {
    paramReplacements?: Dict<string> | undefined;
    delegatedReplacements?: Dict<string> | undefined;
};
/**
 * @import { Api, ApiEntries, ApiEntry, ApiFile, ApiParameter, ApiParameters, Dict } from "./types.js"
 */
/**
 * @typedef {object} GenerateApiConfig
 * @prop {Api}      api
 * @prop {string}   baseDir
 */
/**
 * Generate api headers from config
 * @param {GenerateApiConfig} config
 */
declare function _generateApi(config: GenerateApiConfig): void;
/**
 * @param {ApiParameters} parameters
 * @param {Dict<string>}  replacements
 */
declare function _generateCallParameters(parameters: ApiParameters, replacements: Dict<string>): string;
/**
 *
 * @param {ApiEntry} entry
 */
declare function _combineHints(entry: ApiEntry): void;
import type { Api } from "./types.js";
import type { Dict } from "./types.js";
import type { ApiParameters } from "./types.js";
import type { ApiEntry } from "./types.js";
export { _generateApi as generateApi, _generateCallParameters as generateCallParameters, _combineHints as combineHints };
