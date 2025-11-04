"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.parseApi = parseApi;
exports.normalizeType = normalizeType;
const antlr4ts_1 = require("antlr4ts");
const CHeaderParser_1 = require("./grammar/CHeaderParser");
const CHeaderLexer_1 = require("./grammar/CHeaderLexer");
const ParseTreeWalker_1 = require("antlr4ts/tree/ParseTreeWalker");
const fs_1 = require("fs");
const utils_1 = require("./utils");
function parseApi({ baseDir, sources }) {
    const api = { files: {} };
    for (const name of sources) {
        utils_1.system.log(`Reading file ${name}`);
        const content = readContent(name, baseDir);
        api.files[name] = parseContent(name, content);
    }
    return api;
}
function readContent(name, baseDirs) {
    for (const baseDir of baseDirs) {
        try {
            return (0, fs_1.readFileSync)(baseDir + name, "utf-8");
        }
        catch (err) {
            utils_1.system.log(`${name} not found at ${baseDir}, looking at next one`);
        }
    }
    throw new Error("File not found: " + name);
}
class ProgListener {
    constructor(name) {
        this.api = { name, doc: undefined, entries: {} };
    }
    // Assuming a parser rule with name: `functionDeclaration`
    enterProg(ctx) {
        const doc = ctx.doc();
        if (doc)
            this.api.doc = parseDoc(doc.text);
    }
    enterDirective(ctx) {
        const directive = ctx.DIRECTIVE().text;
        const docIndex = directive.indexOf('/**<');
        const doc = parseDoc(ctx.doc()?.text ?? (docIndex === -1 ? '' : directive.slice(docIndex).trim()));
        const m = directive.match(/^#define\s*(\w+)(?:\((\w+(,\s*\w+)*)\))?/);
        if (!m)
            return;
        const name = m[1];
        if (name.endsWith("_h_") || name.startsWith("_"))
            return;
        const parameters = m[2]?.split(/,\s*/)?.map(p => ({ name: p, type: "" }));
        const value = directive.slice(m[0].length, docIndex === -1 ? undefined : docIndex).trim();
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            name,
            kind: 'def',
            parameters,
            value,
        };
    }
    enterFunctionDecl(ctx) {
        const type = extractType(ctx.type());
        if (type.startsWith('__inline'))
            return;
        const doc = parseDoc(ctx.doc()?.text ?? '');
        const name = ctx.id().text;
        if (name.startsWith('_'))
            return;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            name,
            kind: 'function',
            type,
            parameters: extractSignature(ctx.signature()),
        };
    }
    enterFunctionDef(ctx) {
        const type = extractType(ctx.type());
        const doc = parseDoc(ctx.doc()?.text ?? '');
        const name = ctx.id().text;
        if (name.startsWith('_'))
            return;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            name,
            kind: 'function',
            type,
            parameters: extractSignature(ctx.signature()),
        };
    }
    enterAliasDef(ctx) {
        const type = extractType(ctx.type());
        const doc = parseDoc(ctx.doc()?.text ?? '');
        const name = ctx.id().text;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            name,
            kind: 'alias',
            type,
        };
    }
    enterEnumDef(ctx) {
        const doc = parseDoc(ctx.doc()?.text ?? '');
        const name = ctx.id(1).text;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            name,
            kind: 'enum',
            entries: extractEnumItems(ctx.enumBody()),
        };
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
function normalizeType(typeString) {
    if (!typeString)
        return "";
    return typeString
        .replace(/(\w+)\s*([&*])/g, "$1 $2")
        .replace(/([&*])\s*(\w+)/g, "$1 $2")
        .replace(/([*&])\s+[*&]/g, "$1$2")
        .replace(/([<(\[])\s+/g, "$1")
        .replace(/\s+([>)\]])/g, "$1")
        .replace(/\s\s+/g, " ");
}
function extractType(ctx) {
    return normalizeType(ctx.typeEl().map(el => el.text).join(" "));
}
function extractSignature(ctx) {
    const params = ctx.type();
    if (params?.length == 0)
        return [];
    const paramsText = params.map(param => extractType(param));
    if (paramsText.length === 1 && paramsText[0] === "void")
        return [];
    return paramsText.map(paramText => {
        const i = paramText.lastIndexOf(' ');
        if (i === -1)
            return { name: paramText, type: "" };
        return {
            name: paramText.slice(i + 1),
            type: paramText.slice(0, i),
        };
    });
}
function extractEnumItems(ctx) {
    const entries = {};
    ctx.enumItem().forEach(item => addEnumItem(item));
    addEnumItem(ctx.enumItemLast());
    return entries;
    function addEnumItem(ctx) {
        const name = ctx.id().text;
        entries[name] = {
            doc: parseDoc(ctx.doc()?.text ?? ctx.trailingDoc()?.text ?? ''),
            name,
            kind: "var",
            type: "",
        };
    }
}
