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
const parseDoc_1 = require("./parseDoc");
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
        this.api = { name, doc: undefined, parsedDoc: undefined, entries: {} };
    }
    enterProg(ctx) {
        const doc = ctx.doc();
        if (doc)
            this.api.doc = extractDoc(doc.text);
    }
    enterDecl(ctx) {
        const doc = ctx.doc();
        if (doc && !this.api.doc)
            this.api.doc = extractDoc(doc.text);
    }
    enterDirective(ctx) {
        const directive = ctx.DEFINE().text;
        const docIndex = directive.indexOf('/**<');
        const doc = extractDoc(ctx.doc()?.text ?? (docIndex === -1 ? '' : directive.slice(docIndex).trim()));
        const m = directive.match(/^#define\s*(\w+)(\([ \t]*(\w+[ \t]*(,[ \t]*\w+[ \t]*)*)?\))?/);
        if (!m)
            return;
        const name = m[1];
        if (name.toLowerCase().endsWith("_h_") || name.endsWith("_h") || name.startsWith("_"))
            return;
        const parameters = m[3]?.split(',')?.map(p => ({ name: p.trim(), type: "" })) ?? (m[2] ? [] : undefined);
        const value = directive.slice(m[0].length, docIndex === -1 ? undefined : docIndex).trim();
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            parsedDoc: undefined,
            name,
            kind: 'def',
            parameters,
            value,
        };
    }
    enterGlobalVar(ctx) {
        const type = extractType(ctx.type());
        const doc = extractDoc(ctx.doc()?.text ?? ctx.trailingDoc()?.text ?? '');
        for (const name of ctx.id().map(id => id.text)) {
            if (this.api.entries[name]?.doc)
                return;
            this.api.entries[name] = {
                doc,
                parsedDoc: undefined,
                name,
                kind: 'var',
                type,
            };
        }
    }
    enterFunctionDecl(ctx) {
        const type = extractType(ctx.type());
        if (type.startsWith('__inline'))
            return;
        const doc = extractDoc(ctx.doc()?.text ?? '');
        const name = ctx.id().text;
        if (name.startsWith('_'))
            return;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            parsedDoc: undefined,
            name,
            kind: 'function',
            type,
            parameters: extractSignature(ctx.signature()),
        };
    }
    enterFunctionDef(ctx) {
        const type = extractType(ctx.type());
        const doc = extractDoc(ctx.doc()?.text ?? '');
        const name = ctx.id().text;
        if (name.startsWith('_'))
            return;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            parsedDoc: undefined,
            name,
            kind: 'function',
            type,
            parameters: extractSignature(ctx.signature()),
            constexpr: true,
        };
    }
    enterAliasDef(ctx) {
        const type = extractType(ctx.type());
        const isStruct = !!ctx.STRUCT();
        const doc = extractDoc(ctx.doc()?.text ?? '');
        const name = ctx.id().text;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            parsedDoc: undefined,
            name,
            kind: 'alias',
            type: isStruct ? `struct ${type}` : type,
        };
    }
    enterUnionDef(ctx) {
        const doc = extractDoc(ctx.doc()?.text ?? '');
        const name = ctx._name.text;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            parsedDoc: undefined,
            name,
            kind: 'union',
        };
    }
    enterEnumDef(ctx) {
        const doc = extractDoc(ctx.doc()?.text ?? '');
        const name = ctx._name.text;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            parsedDoc: undefined,
            name,
            kind: 'enum',
            entries: extractEnumItems(ctx.enumBody()),
        };
    }
    enterStructDef(ctx) {
        const doc = extractDoc(ctx.doc()?.text ?? '');
        const name = ctx._name.text;
        if (this.api.entries[name]?.doc)
            return;
        this.api.entries[name] = {
            doc,
            parsedDoc: undefined,
            name,
            kind: 'struct',
            entries: extractStructItems(ctx.structBody()),
        };
    }
    enterCallbackDef(ctx) {
        const doc = extractDoc(ctx.doc()?.text ?? '');
        const name = ctx.id().text;
        if (this.api.entries[name]?.doc)
            return;
        const type = extractType(ctx.type());
        this.api.entries[name] = {
            doc,
            parsedDoc: undefined,
            name,
            kind: 'callback',
            type,
            parameters: extractSignature(ctx.signature()),
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
    const api = listener.api;
    if (name < "SDL_m") {
        api.parsedDoc = (0, parseDoc_1.parseDoc)(name, api.doc ?? "");
        delete api.doc;
        for (const apiEntry of Object.values(api.entries)) {
            apiEntry.parsedDoc = (0, parseDoc_1.parseDoc)(`${name}@${apiEntry.name}`, apiEntry.doc);
            delete apiEntry.doc;
        }
    }
    return api;
}
function extractDoc(text) {
    if (text.includes('\\name'))
        return "";
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
    const paramsArray = paramsText.map(paramText => {
        const i = paramText.lastIndexOf(' ');
        if (i === -1)
            return { name: paramText, type: "" };
        return {
            name: paramText.slice(i + 1),
            type: paramText.slice(0, i),
        };
    });
    if (ctx.ELLIPSIS())
        paramsArray.push({ name: "...", type: "" });
    return paramsArray;
}
function extractEnumItems(ctx) {
    const entries = {};
    for (const item of ctx.enumItem()) {
        const name = item.id().text;
        entries[name] = {
            doc: extractDoc(item.trailingDoc()?.text ?? item.doc()?.text ?? ''),
            name,
            kind: "var",
            type: "",
        };
    }
    return entries;
}
function extractStructItems(ctx) {
    const entries = {};
    for (const item of ctx.structItem()) {
        const field = item.structVar();
        if (field)
            addVar(field);
        const callback = item.structCallback();
        if (callback)
            addCallback(callback);
    }
    return entries;
    function addVar(item) {
        const type = extractType(item.type());
        const doc = extractDoc(item.trailingDoc()?.text ?? item.doc()?.text ?? '');
        for (const name of item.id().map(id => id.text)) {
            entries[name] = {
                doc,
                name,
                kind: 'var',
                type,
            };
        }
    }
    function addCallback(item) {
        const type = extractType(item.type());
        const doc = extractDoc(item.trailingDoc()?.text ?? item.doc()?.text ?? '');
        const name = item.id().text;
        entries[name] = {
            doc,
            name,
            kind: 'var',
            type,
        };
    }
}
