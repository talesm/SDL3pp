"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const antlr4ts_1 = require("antlr4ts");
const CHeaderParser_1 = require("./grammar/CHeaderParser");
const CHeaderLexer_1 = require("./grammar/CHeaderLexer");
const ParseTreeWalker_1 = require("antlr4ts/tree/ParseTreeWalker");
// Create the lexer and parser
let lexer = new CHeaderLexer_1.CHeaderLexer(antlr4ts_1.CharStreams.fromString('1+1\n'));
let tokenStream = new antlr4ts_1.CommonTokenStream(lexer);
let parser = new CHeaderParser_1.CHeaderParser(tokenStream);
// Parse the input, where `compilationUnit` is whatever entry point you defined
let tree = parser.prog();
class ProgListener {
    // Assuming a parser rule with name: `functionDeclaration`
    enterProg(context) {
        console.log(`Function start line number ${context._start.line}`);
        // ...
    }
    exitProg(context) { }
    // other enterX functions...
    visitTerminal(/*@NotNull*/ node) { }
}
// Create the listener
const listener = new ProgListener();
// Use the entry point for listeners
ParseTreeWalker_1.ParseTreeWalker.DEFAULT.walk(listener, tree);
