import { CharStreams, CommonTokenStream } from "antlr4ts";
import { DoxyCommentListener } from "./grammar/DoxyCommentListener";
import { BlockContext, DoxyCommentParser } from "./grammar/DoxyCommentParser";
import { DoxyCommentLexer } from "./grammar/DoxyCommentLexer";
import { ParseTreeWalker } from "antlr4ts/tree/ParseTreeWalker";
import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { system } from "./utils";
import { ParsedDoc } from "./types";

export function parseDoc(name: string, content: string): ParsedDoc {
  const lexer = new DoxyCommentLexer(CharStreams.fromString(content, name));
  const tokenStream = new CommonTokenStream(lexer);
  const parser = new DoxyCommentParser(tokenStream);
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

  ParseTreeWalker.DEFAULT.walk(listener, tree);
  if (errors) system.warn(` found${errors} parsing ${name}`);

  if (!listener.result.length) return undefined;
  return listener.result;
}

class DocListener implements DoxyCommentListener {
  public result: ParsedDoc;

  constructor() {
    this.result = [];
  }

  enterBlock(ctx: BlockContext) {
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
      if (r.length === 1) this.result.push(r[0]);
      else this.result.push(r);
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
      if (r.length === 1) this.result.push(r[0]);
      else this.result.push(r);
      return;
    }
    const staticSection = ctx.table() ?? ctx.codeblock();
    if (staticSection) {
      const content = staticSection.text.replace(/<EOF>$/, "").trimEnd();
      this.result.push({ content });
    }
  }

  visitTerminal(/*@NotNull*/ node: TerminalNode) {}
}

function extractContent(text: string): string {
  return text
    .trim()
    .replaceAll(/\s+/g, " ")
    .replace(/<EOF>$/, "");
}
