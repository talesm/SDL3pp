"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.parseDoc = parseDoc;
const antlr4ts_1 = require("antlr4ts");
const DoxyCommentParser_1 = require("./grammar/DoxyCommentParser");
const DoxyCommentLexer_1 = require("./grammar/DoxyCommentLexer");
const ParseTreeWalker_1 = require("antlr4ts/tree/ParseTreeWalker");
const utils_1 = require("./utils");
function parseDoc(name, content) {
    const lexer = new DoxyCommentLexer_1.DoxyCommentLexer(antlr4ts_1.CharStreams.fromString(content, name));
    const tokenStream = new antlr4ts_1.CommonTokenStream(lexer);
    const parser = new DoxyCommentParser_1.DoxyCommentParser(tokenStream);
    let errors = 0;
    lexer.addErrorListener({
        syntaxError() {
            errors++;
        },
    });
    parser.addErrorListener({
        syntaxError() {
            errors++;
        },
    });
    const tree = parser.doc();
    const listener = new DocListener();
    ParseTreeWalker_1.ParseTreeWalker.DEFAULT.walk(listener, tree);
    if (errors)
        utils_1.system.warn(` found${errors} parsing ${name}`);
    if (!listener.result.length)
        return undefined;
    return listener.result;
}
class DocListener {
    constructor() {
        this.result = [];
    }
    enterBlock(ctx) {
        const paragraph = ctx.paragraph();
        if (paragraph) {
            this.result.push(extractContent(paragraph.text));
        }
        const title = ctx.title();
        if (title) {
            this.result.push({
                tag: title.TITLE_DELIM().text,
                content: extractContent(title.textLine().text),
            });
            return;
        }
        const list = ctx.list();
        if (list) {
            const r = list.listItem().map((item) => {
                const ws = item.WS();
                const spaces = ws.length ? " ".repeat(ws.length) : "";
                return {
                    tag: spaces + item.LIST_DELIM().text,
                    content: extractContent(item.listItemContent().text),
                };
            });
            if (r.length === 1)
                this.result.push(r[0]);
            else
                this.result.push(r);
            return;
        }
        const taggedSection = ctx.taggedSection();
        if (taggedSection) {
            const r = taggedSection.taggedBlock().map((item) => {
                return {
                    tag: item.blockTag().text,
                    content: item
                        .textLine()
                        .map((l) => extractContent(l.text))
                        .join(" "),
                };
            });
            if (r.length === 1)
                this.result.push(r[0]);
            else
                this.result.push(r);
            return;
        }
        const staticSection = ctx.table() ?? ctx.codeblock();
        if (staticSection) {
            const content = staticSection.text.replace(/<EOF>$/, "").trimEnd();
            this.result.push({ content });
        }
    }
    visitTerminal(/*@NotNull*/ node) { }
}
function extractContent(text) {
    return text
        .trim()
        .replaceAll(/\s+/g, " ")
        .replace(/<EOF>$/, "");
}
