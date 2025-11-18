import { Api } from "./types";
export interface ParseConfig {
    baseDir: string[];
    sources: string[];
}
export declare function parseApi({ baseDir, sources }: ParseConfig): Api;
export declare function normalizeType(typeString: string): string;
