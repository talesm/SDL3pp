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
export function generateApi(config: GenerateApiConfig): void;
/**
 * @param {ApiParameters} parameters
 * @param {Dict<string>}  replacements
 */
export function generateCallParameters(parameters: ApiParameters, replacements: Dict<string>): string;
/**
 *
 * @param {ApiEntry} entry
 */
export function combineHints(entry: ApiEntry): void;
export type GenerateApiConfig = {
    api: Api;
    baseDir: string;
};
export type GenerateApiFileConfig = {
    paramReplacements?: Dict<string> | undefined;
    delegatedReplacements?: Dict<string> | undefined;
};
import type { ApiParameters } from "./types.js";
import type { Dict } from "./types.js";
import type { ApiEntry } from "./types.js";
import type { Api } from "./types.js";
