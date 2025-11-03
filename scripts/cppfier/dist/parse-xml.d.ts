/**
 * @import { Api, ApiEntries, ApiEntry, ApiFile } from "./types"
 */
/**
 * @typedef {object} ParseXmlConfig
 * @prop {string[]} xmlDir
 * @prop {string[]} baseDir
 * @prop {string[]} sources
 */
/**
 * @param {ParseXmlConfig} config
 */
export function parseXmlApi(config: ParseXmlConfig): Promise<Api>;
/**
 * @typedef {object} ParseXmlFileConfig
 * @prop {string[]=} xmlDir
 * @prop {string[]=} baseDir
 */
/**
 *
 * @param {string} name
 * @param {ParseXmlFileConfig} config
 */
export function parseXmlFile(name: string, config?: ParseXmlFileConfig): Promise<ApiFile>;
export type ParseXmlConfig = {
    xmlDir: string[];
    baseDir: string[];
    sources: string[];
};
export type ParseXmlFileConfig = {
    xmlDir?: string[] | undefined;
    baseDir?: string[] | undefined;
};
export type ParseXmlContentConfig = {
    baseDir?: string[] | undefined;
};
import type { Api } from "./types";
import type { ApiFile } from "./types";
