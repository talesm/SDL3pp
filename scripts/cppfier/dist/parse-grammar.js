"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const antlr4ts_1 = require("antlr4ts");
const CHeaderParser_1 = require("./grammar/CHeaderParser");
const CHeaderLexer_1 = require("./grammar/CHeaderLexer");
const ParseTreeWalker_1 = require("antlr4ts/tree/ParseTreeWalker");
const fs_1 = require("fs");
class ProgListener {
    constructor(name) {
        this.api = { name };
    }
    // Assuming a parser rule with name: `functionDeclaration`
    enterProg(ctx) {
        const doc = ctx.doc();
        if (doc)
            this.api.doc = parseDoc(doc.text);
    }
    // other enterX functions...
    visitTerminal(/*@NotNull*/ node) { }
}
function parseContent(name, content) {
    // Create the lexer and parser
    let lexer = new CHeaderLexer_1.CHeaderLexer(antlr4ts_1.CharStreams.fromString(content, name));
    let tokenStream = new antlr4ts_1.CommonTokenStream(lexer);
    let parser = new CHeaderParser_1.CHeaderParser(tokenStream);
    // Parse the input, where `compilationUnit` is whatever entry point you defined
    let tree = parser.prog();
    // Create the listener
    const listener = new ProgListener(name);
    // Use the entry point for listeners
    ParseTreeWalker_1.ParseTreeWalker.DEFAULT.walk(listener, tree);
    return listener.api;
}
function parseDoc(text) {
    if (text.startsWith('/**<')) {
        return text.slice(4, text.length - 2)
            .replaceAll(/^[ \t]*\*[ \t]?/mg, '')
            .trim();
    }
    if (text.startsWith('/**')) {
        return text.slice(3, text.length - 2)
            .replaceAll(/^[ \t]*\*[ \t]?/mg, '')
            .trim();
    }
    if (text.startsWith('///<')) {
        text.slice(4)
            .replaceAll(/^[ \t]\/\/\/[ \t]?/mg, '')
            .trim();
    }
    if (text.startsWith('///')) {
        text.slice(3)
            .replaceAll(/^[ \t]\/\/\/[ \t]?/mg, '')
            .trim();
    }
    return text;
}
const file = (0, fs_1.readFileSync)("/home/talesm/dev/SDL3/SDL3pp/external/SDL/include/SDL3/SDL_version.h", 'utf-8');
console.log(parseContent("SDL_version.h", file));
