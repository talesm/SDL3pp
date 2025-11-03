declare const _Tokenizer: typeof Tokenizer;
/**
 *
 * @param {string[]} lines
 */
declare function _tokenize(lines: string[]): FileToken[];
declare class Tokenizer {
    /** @param {string[]} lines */
    constructor(lines: string[]);
    lines: string[];
    lineCount: number;
    lastLine: string;
    /** @type {VersionTag} */
    since: VersionTag;
    /** @private */
    private nextLine;
    /** @private */
    private peekLine;
    nonEmptyLine(): string;
    finish(): any;
    /**
     * Next line
     *
     * @returns {FileToken}
     */
    next(): FileToken;
    /**
     * Tokenize parameters
     * @param {FileToken} token
     * @param {string}    parameters
     */
    tokenizeParameters(token: FileToken, parameters: string): void;
    extendToNextStart(): void;
    /**
     *
     * @param {number} spaces
     */
    ignoreBody(spaces: number): string;
}
import type { FileToken } from "./types";
import type { VersionTag } from "./types";
export { _Tokenizer as Tokenizer, _tokenize as tokenize };
