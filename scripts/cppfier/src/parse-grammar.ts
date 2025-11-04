import { CharStreams, CommonTokenStream } from 'antlr4ts';
import { CHeaderListener } from './grammar/CHeaderListener';
import { AliasDefContext, CHeaderParser, DirectiveContext, DocContext, EnumBodyContext, EnumDefContext, EnumItemContext, EnumItemLastContext, FunctionDeclContext, FunctionDefContext, ProgContext, SignatureContext, TypeContext } from './grammar/CHeaderParser';
import { CHeaderLexer } from './grammar/CHeaderLexer';
import { ParseTreeWalker } from 'antlr4ts/tree/ParseTreeWalker';
import { TerminalNode } from 'antlr4ts/tree/TerminalNode';
import { readFileSync } from "fs";
import { Api, ApiEntries, ApiFile, ApiParameters } from './types';
import { system } from './utils';

export interface ParseConfig {
  baseDir: string[];
  sources: string[];
}

export function parseApi({ baseDir, sources }: ParseConfig) {
  const api: Api = { files: {} };

  for (const name of sources) {
    system.log(`Reading file ${name}`);
    const content = readContent(name, baseDir);
    api.files[name] = parseContent(name, content);
  }
  return api;
}

function readContent(name: string, baseDirs: string[]) {
  for (const baseDir of baseDirs) {
    try {
      return readFileSync(baseDir + name, "utf-8");
    } catch (err) {
      system.log(`${name} not found at ${baseDir}, looking at next one`);
    }
  }
  throw new Error("File not found: " + name);
}

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
    const doc = parseDoc(ctx.doc()?.text ?? '');
    const m = directive.match(/^#define\s*(\w+)(?:\((\w+(,\s*\w+)*)\))?/);
    if (!m) return;
    const name = m[1];
    if (name.endsWith("_h_") || name.startsWith("_")) return;
    const parameters = m[2]?.split(/,\s*/)?.map(p => ({ name: p, type: "" }));
    const value = directive.slice(m[0].length).trim();
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      name,
      kind: 'def',
      parameters,
      value,
    };
  }

  enterFunctionDecl(ctx: FunctionDeclContext) {
    const type = extractType(ctx.type());
    if (type.startsWith('__inline')) return;
    const doc = parseDoc(ctx.doc()?.text ?? '');
    const name = ctx.id().text;
    if (name.startsWith('_')) return;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      name,
      kind: 'function',
      type,
      parameters: extractSignature(ctx.signature()),
    };
  }

  enterFunctionDef(ctx: FunctionDefContext) {
    const type = extractType(ctx.type());
    const doc = parseDoc(ctx.doc()?.text ?? '');
    const name = ctx.id().text;
    if (name.startsWith('_')) return;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      name,
      kind: 'function',
      type,
      parameters: extractSignature(ctx.signature()),
    };
  }

  enterAliasDef(ctx: AliasDefContext) {
    const type = extractType(ctx.type());
    const doc = parseDoc(ctx.doc()?.text ?? '');
    const name = ctx.id().text;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      name,
      kind: 'alias',
      type,
    };
  }

  enterEnumDef(ctx: EnumDefContext) {
    const doc = parseDoc(ctx.doc()?.text ?? '');
    const name = ctx.id(1).text;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      name,
      kind: 'enum',
      entries: extractEnumItems(ctx.enumBody()),
    };
  }

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

export function normalizeType(typeString: string) {
  if (!typeString) return "";
  return typeString
    .replace(/(\w+)\s*([&*])/g, "$1 $2")
    .replace(/([&*])\s*(\w+)/g, "$1 $2")
    .replace(/([*&])\s+[*&]/g, "$1$2")
    .replace(/([<(\[])\s+/g, "$1")
    .replace(/\s+([>)\]])/g, "$1")
    .replace(/\s\s+/g, " ");
}

function extractType(ctx: TypeContext): string {
  return normalizeType(ctx.typeEl().map(el => el.text).join(" "));
}

function extractSignature(ctx: SignatureContext): ApiParameters {
  const params = ctx.type();
  if (params?.length == 0) return [];
  const paramsText = params.map(param => extractType(param));
  if (paramsText.length === 1 && paramsText[0] === "void") return [];
  return paramsText.map(paramText => {
    const i = paramText.lastIndexOf(' ');
    if (i === -1) return { name: paramText, type: "" };
    return {
      name: paramText.slice(i + 1),
      type: paramText.slice(0, i),
    };
  });
}

function extractEnumItems(ctx: EnumBodyContext): ApiEntries {
  const entries: ApiEntries = {};
  ctx.enumItem().forEach(item => addEnumItem(item));
  addEnumItem(ctx.enumItemLast());
  return entries;

  function addEnumItem(ctx: EnumItemContext | EnumItemLastContext) {
    const name = ctx.id().text;
    entries[name] = {
      doc: parseDoc(ctx.doc()?.text ?? ctx.trailingDoc()?.text ?? ''),
      name,
      kind: "var",
      type: "",
    };
  }
}

