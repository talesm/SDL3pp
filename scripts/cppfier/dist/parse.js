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
        this.api = { name, doc: undefined, entries: {} };
    }
    enterProg(ctx) {
        const doc = ctx.doc();
        if (doc)
            this.api.doc = extractDoc(this.api.name, doc.text);
    }
    enterDecl(ctx) {
        const doc = ctx.doc();
        if (doc && !this.api.doc)
            this.api.doc = extractDoc(this.api.name, doc.text);
    }
    enterDirective(ctx) {
        const directive = ctx.DEFINE().text;
        const docIndex = directive.indexOf('/**<');
        const m = directive.match(/^#define\s*(\w+)(\([ \t]*(\w+[ \t]*(,[ \t]*\w+[ \t]*)*)?\))?/);
        if (!m)
            return;
        const name = m[1];
        if (name.toLowerCase().endsWith("_h_") || name.endsWith("_h") || name.startsWith("_"))
            return;
        const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? (docIndex === -1 ? '' : directive.slice(docIndex).trim()));
        const parameters = m[3]?.split(',')?.map(p => ({ name: p.trim(), type: "" })) ?? (m[2] ? [] : undefined);
        const value = directive.slice(m[0].length, docIndex === -1 ? undefined : docIndex).trim();
        if (this.api.entries[name]?.parsedDoc)
            return;
        this.api.entries[name] = {
            parsedDoc: doc,
            name,
            kind: 'def',
            parameters,
            value,
        };
    }
    enterGlobalVar(ctx) {
        const type = extractType(ctx.type());
        const names = ctx.id().map(id => id.text);
        const doc = extractDoc(`${this.api.name}@${names[0]}`, ctx.doc()?.text ?? ctx.trailingDoc()?.text ?? '');
        for (const name of names) {
            if (this.api.entries[name]?.parsedDoc)
                return;
            this.api.entries[name] = {
                parsedDoc: doc,
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
        const name = ctx.id().text;
        const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? '');
        if (name.startsWith('_'))
            return;
        if (this.api.entries[name]?.parsedDoc)
            return;
        this.api.entries[name] = {
            parsedDoc: doc,
            name,
            kind: 'function',
            type,
            parameters: extractSignature(ctx.signature()),
        };
    }
    enterFunctionDef(ctx) {
        const type = extractType(ctx.type());
        const name = ctx.id().text;
        const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? '');
        if (name.startsWith('_'))
            return;
        if (this.api.entries[name]?.parsedDoc)
            return;
        this.api.entries[name] = {
            parsedDoc: doc,
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
        const name = ctx.id().text;
        const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? '');
        if (this.api.entries[name]?.parsedDoc)
            return;
        this.api.entries[name] = {
            parsedDoc: doc,
            name,
            kind: 'alias',
            type: isStruct ? `struct ${type}` : type,
        };
    }
    enterUnionDef(ctx) {
        const name = ctx._name.text;
        const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? '');
        if (this.api.entries[name]?.parsedDoc)
            return;
        this.api.entries[name] = {
            parsedDoc: doc,
            name,
            kind: 'union',
        };
    }
    enterEnumDef(ctx) {
        const name = ctx._name.text;
        const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? '');
        if (this.api.entries[name]?.parsedDoc)
            return;
        this.api.entries[name] = {
            parsedDoc: doc,
            name,
            kind: 'enum',
            entries: extractEnumItems(`${this.api.name}@${name}`, ctx.enumBody()),
        };
    }
    enterStructDef(ctx) {
        const name = ctx._name.text;
        const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? '');
        if (this.api.entries[name]?.parsedDoc)
            return;
        this.api.entries[name] = {
            parsedDoc: doc,
            name,
            kind: 'struct',
            entries: extractStructItems(`${this.api.name}@${name}`, ctx.structBody()),
        };
    }
    enterCallbackDef(ctx) {
        const name = ctx.id().text;
        const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? '');
        if (this.api.entries[name]?.parsedDoc)
            return;
        const type = extractType(ctx.type());
        this.api.entries[name] = {
            parsedDoc: doc,
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
    return listener.api;
}
function extractDoc(name, text) {
    text = extractDocText(text);
    if (text)
        return (0, parseDoc_1.parseDoc)(name, text);
    function extractDocText(text) {
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
function extractEnumItems(prefix, ctx) {
    const entries = {};
    for (const item of ctx.enumItem()) {
        const name = item.id().text;
        entries[name] = {
            parsedDoc: extractDoc(`${prefix}@${name}`, item.trailingDoc()?.text ?? item.doc()?.text ?? ''),
            name,
            kind: "var",
            type: "",
        };
    }
    return entries;
}
function extractStructItems(prefix, ctx) {
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
        const names = item.id().map(id => id.text);
        const doc = extractDoc(`${prefix}@${names[0]}`, item.trailingDoc()?.text ?? item.doc()?.text ?? '');
        for (const name of names) {
            entries[name] = {
                parsedDoc: doc,
                name,
                kind: 'var',
                type,
            };
        }
    }
    function addCallback(item) {
        const type = extractType(item.type());
        const name = item.id().text;
        const doc = extractDoc(`${prefix}@${name}`, item.trailingDoc()?.text ?? item.doc()?.text ?? '');
        entries[name] = {
            parsedDoc: doc,
            name,
            kind: 'var',
            type,
        };
    }
}
