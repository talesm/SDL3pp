import { Api, ApiEntry, ApiParameters, Dict } from "./types.js";
export interface GenerateApiConfig {
    api?: Api;
    baseDir?: string;
}
/**
 * Generate api headers from config
 * @param {GenerateApiConfig} config
 */
export declare function generateApi(config: any): void;
export interface GenerateApiFileConfig {
    paramReplacements?: Dict<string>;
    delegatedReplacements?: Dict<string>;
}
export declare function generateCallParameters(parameters: ApiParameters, replacements: Dict<string>): string;
export declare function combineHints(entry: ApiEntry): void;
