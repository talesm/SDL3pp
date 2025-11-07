import { CharStreams, CommonTokenStream } from 'antlr4ts';
import { CHeaderListener } from './grammar/CHeaderListener';
import { AliasDefContext, CallbackDefContext, CHeaderParser, DeclContext, DirectiveContext, EnumBodyContext, EnumDefContext, EnumItemContext, FunctionDeclContext, FunctionDefContext, GlobalVarContext, ProgContext, SignatureContext, StructBodyContext, StructCallbackContext, StructDefContext, StructVarContext, TypeContext, UnionDefContext } from './grammar/CHeaderParser';
import { CHeaderLexer } from './grammar/CHeaderLexer';
import { ParseTreeWalker } from 'antlr4ts/tree/ParseTreeWalker';
import { TerminalNode } from 'antlr4ts/tree/TerminalNode';
import { readFileSync } from "fs";
import { Api, ApiEntries, ApiFile, ApiParameters } from './types';
import { system } from './utils';
import { parseDoc } from './parseDoc';

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
    this.api = { name, doc: undefined, parsedDoc: undefined, entries: {} };
  }

  enterProg(ctx: ProgContext) {
    const doc = ctx.doc();
    if (doc) this.api.doc = extractDoc(doc.text);
  }

  enterDecl(ctx: DeclContext) {
    const doc = ctx.doc();
    if (doc && !this.api.doc) this.api.doc = extractDoc(doc.text);
  }

  enterDirective(ctx: DirectiveContext) {
    const directive = ctx.DEFINE().text;
    const docIndex = directive.indexOf('/**<');
    const doc = extractDoc(ctx.doc()?.text ?? (docIndex === -1 ? '' : directive.slice(docIndex).trim()));
    const m = directive.match(/^#define\s*(\w+)(\([ \t]*(\w+[ \t]*(,[ \t]*\w+[ \t]*)*)?\))?/);
    if (!m) return;
    const name = m[1];
    if (name.toLowerCase().endsWith("_h_") || name.endsWith("_h") || name.startsWith("_")) return;
    const parameters = m[3]?.split(',')?.map(p => ({ name: p.trim(), type: "" })) ?? (m[2] ? [] : undefined);
    const value = directive.slice(m[0].length, docIndex === -1 ? undefined : docIndex).trim();
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      parsedDoc: undefined,
      name,
      kind: 'def',
      parameters,
      value,
    };
  }

  enterGlobalVar(ctx: GlobalVarContext) {
    const type = extractType(ctx.type());
    const doc = extractDoc(ctx.doc()?.text ?? ctx.trailingDoc()?.text ?? '');
    for (const name of ctx.id().map(id => id.text)) {
      if (this.api.entries[name]?.doc) return;
      this.api.entries[name] = {
        doc,
        parsedDoc: undefined,
        name,
        kind: 'var',
        type,
      };
    }
  }

  enterFunctionDecl(ctx: FunctionDeclContext) {
    const type = extractType(ctx.type());
    if (type.startsWith('__inline')) return;
    const doc = extractDoc(ctx.doc()?.text ?? '');
    const name = ctx.id().text;
    if (name.startsWith('_')) return;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      parsedDoc: undefined,
      name,
      kind: 'function',
      type,
      parameters: extractSignature(ctx.signature()),
    };
  }

  enterFunctionDef(ctx: FunctionDefContext) {
    const type = extractType(ctx.type());
    const doc = extractDoc(ctx.doc()?.text ?? '');
    const name = ctx.id().text;
    if (name.startsWith('_')) return;
    if (this.api.entries[name]?.doc) return;
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

  enterAliasDef(ctx: AliasDefContext) {
    const type = extractType(ctx.type());
    const isStruct = !!ctx.STRUCT();
    const doc = extractDoc(ctx.doc()?.text ?? '');
    const name = ctx.id().text;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      parsedDoc: undefined,
      name,
      kind: 'alias',
      type: isStruct ? `struct ${type}` : type,
    };
  }

  enterUnionDef(ctx: UnionDefContext) {
    const doc = extractDoc(ctx.doc()?.text ?? '');
    const name = ctx._name.text;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      parsedDoc: undefined,
      name,
      kind: 'union',
    };
  }

  enterEnumDef(ctx: EnumDefContext) {
    const doc = extractDoc(ctx.doc()?.text ?? '');
    const name = ctx._name.text;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      parsedDoc: undefined,
      name,
      kind: 'enum',
      entries: extractEnumItems(ctx.enumBody()),
    };
  }

  enterStructDef(ctx: StructDefContext) {
    const doc = extractDoc(ctx.doc()?.text ?? '');
    const name = ctx._name.text;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc,
      parsedDoc: undefined,
      name,
      kind: 'struct',
      entries: extractStructItems(ctx.structBody()),
    };
  }

  enterCallbackDef(ctx: CallbackDefContext) {
    const doc = extractDoc(ctx.doc()?.text ?? '');
    const name = ctx.id().text;
    if (this.api.entries[name]?.doc) return;
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
  const api = listener.api;
  if (name < "SDL_n") {
    api.parsedDoc = parseDoc(name, api.doc ?? "");
    delete api.doc;
    for (const apiEntry of Object.values(api.entries)) {
      apiEntry.parsedDoc = parseDoc(`${name}@${apiEntry.name}`, apiEntry.doc);
      delete apiEntry.doc;
    }
  }
  return api;
}
function extractDoc(text: string): string {
  if (text.includes('\\name')) return "";
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
  const paramsArray = paramsText.map(paramText => {
    const i = paramText.lastIndexOf(' ');
    if (i === -1) return { name: paramText, type: "" };
    return {
      name: paramText.slice(i + 1),
      type: paramText.slice(0, i),
    };
  });
  if (ctx.ELLIPSIS()) paramsArray.push({ name: "...", type: "" });
  return paramsArray;
}

function extractEnumItems(ctx: EnumBodyContext): ApiEntries {
  const entries: ApiEntries = {};
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

function extractStructItems(ctx: StructBodyContext): ApiEntries {
  const entries: ApiEntries = {};
  for (const item of ctx.structItem()) {
    const field = item.structVar();
    if (field) addVar(field);
    const callback = item.structCallback();
    if (callback) addCallback(callback);
  }
  return entries;

  function addVar(item: StructVarContext) {
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
  function addCallback(item: StructCallbackContext) {
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


