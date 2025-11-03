import { PathOrFileDescriptor } from "node:fs";
/**
 * Read a file as an array of lines
 * @param path
 */
export declare function readLinesSync(path: PathOrFileDescriptor): string[];
/**
 * Write an array of lines to a file
 * @param path
 * @param data
 */
export declare function writeLinesSync(path: PathOrFileDescriptor, data: string[]): void;
/**
 * Read a file as a JSON
 * @param path
 */
export declare function readJSONSync(path: PathOrFileDescriptor): any;
/**
 * Write an array of lines to a file
 * @param path
 * @param data
 */
export declare function writeJSONSync(path: PathOrFileDescriptor, data: any): void;
/**
 * Combine source into target
 * @param target
 * @param source
 * @returns
 */
export declare function combineArray(target: any[], source: any): any[];
/**
 *
 * @param target
 * @param source
 * @returns
 */
export declare function combineObject(target: {
    [key: string]: any;
}, source: any): {
    [key: string]: any;
};
/**
 * Returns true if this seems like a free() function
 * @param name
 */
export declare function looksLikeFreeFunction(name: string): boolean;
export declare const system: {
    silent: boolean;
    stopOnWarn: boolean;
    log(...data: any[]): void;
    warn(...data: any[]): void;
};
/**
 * Clone object, using .clone() if provided
 * @template {any} T the type
 * @param {T} obj the object to bee cloned
 * @returns {T}
 */
export declare function deepClone<T>(obj: T): T;
