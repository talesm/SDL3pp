import { CharStreams, CommonTokenStream } from "antlr4ts";
import { CHeaderListener } from "./grammar/CHeaderListener";
import {
  AliasDefContext,
  CallbackDefContext,
  CHeaderParser,
  DeclContext,
  DirectiveContext,
  EnumBodyContext,
  EnumDefContext,
  FunctionDeclContext,
  FunctionDefContext,
  GlobalVarContext,
  ProgContext,
  SignatureContext,
  StructBodyContext,
  StructCallbackContext,
  StructDefContext,
  StructVarContext,
  TypeContext,
  UnionDefContext,
} from "./grammar/CHeaderParser";
import { CHeaderLexer } from "./grammar/CHeaderLexer";
import { ParseTreeWalker } from "antlr4ts/tree/ParseTreeWalker";
import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { readFileSync } from "node:fs";
import { Api, ApiEntries, ApiFile, ApiParameters, ParsedDoc } from "./types";
import { system } from "./utils";
import { parseDoc } from "./parseDoc";

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

  enterProg(ctx: ProgContext) {
    const doc = ctx.doc();
    if (doc) this.api.doc = extractDoc(this.api.name, doc.text);
  }

  enterDecl(ctx: DeclContext) {
    const doc = ctx.doc();
    if (doc && !this.api.doc)
      this.api.doc = extractDoc(this.api.name, doc.text);
  }

  enterDirective(ctx: DirectiveContext) {
    const directive = ctx.DEFINE().text;
    const docIndex = directive.indexOf("/**<");
    const m = directive.match(
      /^#define\s*(\w+)(\([ \t]*(\w+[ \t]*(,[ \t]*\w+[ \t]*)*)?\))?/
    );
    if (!m) return;
    const name = m[1];
    if (
      name.toLowerCase().endsWith("_h_") ||
      name.endsWith("_h") ||
      name.startsWith("_")
    )
      return;
    const doc = extractDoc(
      `${this.api.name}@${name}`,
      ctx.doc()?.text ??
        (docIndex === -1 ? "" : directive.slice(docIndex).trim())
    );
    const parameters =
      m[3]?.split(",")?.map((p) => ({ name: p.trim(), type: "" })) ??
      (m[2] ? [] : undefined);
    const value = directive
      .slice(m[0].length, docIndex === -1 ? undefined : docIndex)
      .trim();
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc: doc,
      name,
      kind: "def",
      parameters,
      value,
    };
  }

  enterGlobalVar(ctx: GlobalVarContext) {
    const type = extractType(ctx.type());
    const names = ctx.id().map((id) => id.text);
    const doc = extractDoc(
      `${this.api.name}@${names[0]}`,
      ctx.doc()?.text ?? ctx.trailingDoc()?.text ?? ""
    );
    for (const name of names) {
      if (this.api.entries[name]?.doc) return;
      this.api.entries[name] = {
        doc: doc,
        name,
        kind: "var",
        type,
      };
    }
  }

  enterFunctionDecl(ctx: FunctionDeclContext) {
    const type = extractType(ctx.type());
    if (type.startsWith("__inline")) return;
    const name = ctx.id().text;
    const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? "");
    if (name.startsWith("_")) return;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc: doc,
      name,
      kind: "function",
      type,
      parameters: extractSignature(ctx.signature()),
    };
  }

  enterFunctionDef(ctx: FunctionDefContext) {
    const type = extractType(ctx.type());
    const name = ctx.id().text;
    const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? "");
    if (name.startsWith("_")) return;
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc: doc,
      name,
      kind: "function",
      type,
      parameters: extractSignature(ctx.signature()),
      constexpr: true,
    };
  }

  enterAliasDef(ctx: AliasDefContext) {
    const type = extractType(ctx.type());
    const isStruct = !!ctx.STRUCT();
    const name = ctx.id().text;
    const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? "");
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc: doc,
      name,
      kind: "alias",
      type: isStruct ? `struct ${type}` : type,
    };
  }

  enterUnionDef(ctx: UnionDefContext) {
    const name = ctx._name.text;
    const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? "");
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc: doc,
      name,
      kind: "union",
    };
  }

  enterEnumDef(ctx: EnumDefContext) {
    const name = ctx._name.text;
    const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? "");
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc: doc,
      name,
      kind: "enum",
      entries: extractEnumItems(`${this.api.name}@${name}`, ctx.enumBody()),
    };
  }

  enterStructDef(ctx: StructDefContext) {
    const name = ctx._name.text;
    const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? "");
    if (this.api.entries[name]?.doc) return;
    this.api.entries[name] = {
      doc: doc,
      name,
      kind: "struct",
      entries: extractStructItems(`${this.api.name}@${name}`, ctx.structBody()),
    };
  }

  enterCallbackDef(ctx: CallbackDefContext) {
    const name = ctx.id().text;
    const doc = extractDoc(`${this.api.name}@${name}`, ctx.doc()?.text ?? "");
    if (this.api.entries[name]?.doc) return;
    const type = extractType(ctx.type());
    this.api.entries[name] = {
      doc: doc,
      name,
      kind: "callback",
      type,
      parameters: extractSignature(ctx.signature()),
    };
  }

  // other enterX functions...
  visitTerminal(/*@NotNull*/ node: TerminalNode) {}
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

function extractDoc(name: string, text: string): ParsedDoc {
  text = extractDocText(text);
  if (text) return parseDoc(name, text);
  function extractDocText(text: string): string {
    if (text.includes("\\name")) return "";
    if (text.startsWith("/**<")) {
      return text
        .slice(4, text.length - 2)
        .replaceAll(/^[ \t]*\*[ \t]?/gm, "")
        .trim();
    }
    if (text.startsWith("/**")) {
      return text
        .slice(3, text.length - 2)
        .replaceAll(/^[ \t]*\*[ \t]?/gm, "")
        .trim();
    }
    if (text.startsWith("///<")) {
      text
        .slice(4)
        .replaceAll(/^[ \t]\/\/\/[ \t]?/gm, "")
        .trim();
    }
    if (text.startsWith("///")) {
      text
        .slice(3)
        .replaceAll(/^[ \t]\/\/\/[ \t]?/gm, "")
        .trim();
    }
    return text;
  }
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
  return normalizeType(
    ctx
      .typeEl()
      .map((el) => el.text)
      .join(" ")
  );
}

function extractSignature(ctx: SignatureContext): ApiParameters {
  const params = ctx.type();
  if (params?.length == 0) return [];
  const paramsText = params.map((param) => extractType(param));
  if (paramsText.length === 1 && paramsText[0] === "void") return [];
  const paramsArray = paramsText.map((paramText) => {
    const i = paramText.lastIndexOf(" ");
    if (i === -1) return { name: paramText, type: "" };
    return {
      name: paramText.slice(i + 1),
      type: paramText.slice(0, i),
    };
  });
  if (ctx.ELLIPSIS()) paramsArray.push({ name: "...", type: "" });
  return paramsArray;
}

function extractEnumItems(prefix: string, ctx: EnumBodyContext): ApiEntries {
  const entries: ApiEntries = {};
  for (const item of ctx.enumItem()) {
    const name = item.id().text;
    entries[name] = {
      doc: extractDoc(
        `${prefix}@${name}`,
        item.trailingDoc()?.text ?? item.doc()?.text ?? ""
      ),
      name,
      kind: "var",
      type: "",
    };
  }
  return entries;
}

function extractStructItems(
  prefix: string,
  ctx: StructBodyContext
): ApiEntries {
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
    const names = item.id().map((id) => id.text);
    const doc = extractDoc(
      `${prefix}@${names[0]}`,
      item.trailingDoc()?.text ?? item.doc()?.text ?? ""
    );
    for (const name of names) {
      entries[name] = {
        doc: doc,
        name,
        kind: "var",
        type,
      };
    }
  }
  function addCallback(item: StructCallbackContext) {
    const type = extractType(item.type());
    const name = item.id().text;
    const doc = extractDoc(
      `${prefix}@${name}`,
      item.trailingDoc()?.text ?? item.doc()?.text ?? ""
    );
    entries[name] = {
      doc: doc,
      name,
      kind: "var",
      type,
    };
  }
}
