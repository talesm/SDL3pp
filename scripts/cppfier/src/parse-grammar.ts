import { CharStreams, CommonTokenStream } from 'antlr4ts';
import { CHeaderListener } from './grammar/CHeaderListener';
import { CHeaderParser, DirectiveContext, DocContext, ProgContext } from './grammar/CHeaderParser';
import { CHeaderLexer } from './grammar/CHeaderLexer';
import { ParseTreeWalker } from 'antlr4ts/tree/ParseTreeWalker';
import { TerminalNode } from 'antlr4ts/tree/TerminalNode';
import { readFileSync } from "fs";
import { ApiFile } from './types';


class ProgListener implements CHeaderListener {
  public api: ApiFile;

  constructor(name: string) {
    this.api = { name, doc: undefined, entries: {} };
  }

  // Assuming a parser rule with name: `functionDeclaration`
  enterProg(ctx: ProgContext) {
    const doc = ctx.doc();
    if (doc) this.api.doc = parseDoc(doc.text);
  }

  enterDirective(ctx: DirectiveContext) {
    const directive = ctx.DIRECTIVE().text;
    const m = directive.match(/^#define\s*(\w+)(?:\((\w+(,\s*\w+)*)\))?/);
    if (!m) return;
    const doc = parseDoc(ctx.doc()?.text ?? '');
    const name = m[1];
    const parameters = m[2]?.split(/,\s*/)?.map(p => ({ type: "", name: p }));
    const value = directive.slice(m[0].length).trim();
    this.api.entries[name] = {
      doc,
      name,
      kind: 'def',
      parameters,
      value,
    };
  };

  // other enterX functions...
  visitTerminal(/*@NotNull*/ node: TerminalNode) { }
  // visitErrorNode(/*@NotNull*/ node: ErrorNode) {};
  // enterEveryRule(/*@NotNull*/ ctx: ParserRuleContext) {};
  // exitEveryRule(/*@NotNull*/ ctx: ParserRuleContext) {};
}


function parseContent(name: string, content: string) {
  // Create the lexer and parser
  let lexer = new CHeaderLexer(CharStreams.fromString(content, name));
  let tokenStream = new CommonTokenStream(lexer);
  let parser = new CHeaderParser(tokenStream);

  // Parse the input, where `compilationUnit` is whatever entry point you defined
  let tree = parser.prog();

  // Create the listener
  const listener = new ProgListener(name);
  // Use the entry point for listeners
  ParseTreeWalker.DEFAULT.walk(listener, tree);
  return listener.api;
}
function parseDoc(text: string): string {
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

const file = readFileSync("/home/talesm/dev/SDL3/SDL3pp/external/SDL/include/SDL3/SDL_version.h", 'utf-8');
console.log(parseContent("SDL_version.h", file));
