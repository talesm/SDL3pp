import { ATN } from "antlr4ts/atn/ATN";
import { FailedPredicateException } from "antlr4ts/FailedPredicateException";
import { Parser } from "antlr4ts/Parser";
import { ParserRuleContext } from "antlr4ts/ParserRuleContext";
import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { TokenStream } from "antlr4ts/TokenStream";
import { Vocabulary } from "antlr4ts/Vocabulary";
import { CHeaderListener } from "./CHeaderListener";
export declare class CHeaderParser extends Parser {
    static readonly WS = 1;
    static readonly LONG_COMMENT = 2;
    static readonly SHORT_COMMENT = 3;
    static readonly TRAILING_DOC = 4;
    static readonly LONG_DOC = 5;
    static readonly SHORT_DOC = 6;
    static readonly DEFINE = 7;
    static readonly DIRECTIVE = 8;
    static readonly ATTRIBUTE = 9;
    static readonly CONST = 10;
    static readonly ENUM = 11;
    static readonly EXTERN = 12;
    static readonly INLINE = 13;
    static readonly SDL_NOISE = 14;
    static readonly SDL_INLINE = 15;
    static readonly STATIC = 16;
    static readonly STRUCT = 17;
    static readonly TYPEDEF = 18;
    static readonly UNION = 19;
    static readonly VOID = 20;
    static readonly SDL_VARARG_ATTRIB = 21;
    static readonly SDL_COMPILE_TIME_ASSERT = 22;
    static readonly CURLY_B = 23;
    static readonly CURLY_E = 24;
    static readonly ROUND_B = 25;
    static readonly ROUND_E = 26;
    static readonly SQUARE_B = 27;
    static readonly SQUARE_E = 28;
    static readonly COLON = 29;
    static readonly SEMI = 30;
    static readonly COMMA = 31;
    static readonly DOT = 32;
    static readonly STAR = 33;
    static readonly EQ = 34;
    static readonly ELLIPSIS = 35;
    static readonly PUNCT_EXTRA = 36;
    static readonly STRING = 37;
    static readonly ID = 38;
    static readonly NUMBER = 39;
    static readonly RULE_prog = 0;
    static readonly RULE_decl = 1;
    static readonly RULE_externC = 2;
    static readonly RULE_directive = 3;
    static readonly RULE_functionDecl = 4;
    static readonly RULE_functionDef = 5;
    static readonly RULE_aliasDef = 6;
    static readonly RULE_unionDef = 7;
    static readonly RULE_enumDef = 8;
    static readonly RULE_structDef = 9;
    static readonly RULE_callbackDef = 10;
    static readonly RULE_compileTimeAssert = 11;
    static readonly RULE_inline = 12;
    static readonly RULE_block = 13;
    static readonly RULE_group = 14;
    static readonly RULE_indexing = 15;
    static readonly RULE_stm = 16;
    static readonly RULE_expr = 17;
    static readonly RULE_word = 18;
    static readonly RULE_punct = 19;
    static readonly RULE_enumBody = 20;
    static readonly RULE_enumItem = 21;
    static readonly RULE_structBody = 22;
    static readonly RULE_structItem = 23;
    static readonly RULE_structVar = 24;
    static readonly RULE_structCallback = 25;
    static readonly RULE_unionInlineType = 26;
    static readonly RULE_id = 27;
    static readonly RULE_type = 28;
    static readonly RULE_typeEl = 29;
    static readonly RULE_signature = 30;
    static readonly RULE_attribute = 31;
    static readonly RULE_doc = 32;
    static readonly RULE_trailingDoc = 33;
    static readonly ruleNames: string[];
    private static readonly _LITERAL_NAMES;
    private static readonly _SYMBOLIC_NAMES;
    static readonly VOCABULARY: Vocabulary;
    get vocabulary(): Vocabulary;
    get grammarFileName(): string;
    get ruleNames(): string[];
    get serializedATN(): string;
    protected createFailedPredicateException(predicate?: string, message?: string): FailedPredicateException;
    constructor(input: TokenStream);
    prog(): ProgContext;
    decl(): DeclContext;
    externC(): ExternCContext;
    directive(): DirectiveContext;
    functionDecl(): FunctionDeclContext;
    functionDef(): FunctionDefContext;
    aliasDef(): AliasDefContext;
    unionDef(): UnionDefContext;
    enumDef(): EnumDefContext;
    structDef(): StructDefContext;
    callbackDef(): CallbackDefContext;
    compileTimeAssert(): CompileTimeAssertContext;
    inline(): InlineContext;
    block(): BlockContext;
    group(): GroupContext;
    indexing(): IndexingContext;
    stm(): StmContext;
    expr(): ExprContext;
    word(): WordContext;
    punct(): PunctContext;
    enumBody(): EnumBodyContext;
    enumItem(): EnumItemContext;
    structBody(): StructBodyContext;
    structItem(): StructItemContext;
    structVar(): StructVarContext;
    structCallback(): StructCallbackContext;
    unionInlineType(): UnionInlineTypeContext;
    id(): IdContext;
    type(): TypeContext;
    typeEl(): TypeElContext;
    signature(): SignatureContext;
    attribute(): AttributeContext;
    doc(): DocContext;
    trailingDoc(): TrailingDocContext;
    static readonly _serializedATN: string;
    static __ATN: ATN;
    static get _ATN(): ATN;
}
export declare class ProgContext extends ParserRuleContext {
    EOF(): TerminalNode;
    doc(): DocContext | undefined;
    decl(): DeclContext[];
    decl(i: number): DeclContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class DeclContext extends ParserRuleContext {
    directive(): DirectiveContext | undefined;
    externC(): ExternCContext | undefined;
    functionDecl(): FunctionDeclContext | undefined;
    functionDef(): FunctionDefContext | undefined;
    aliasDef(): AliasDefContext | undefined;
    unionDef(): UnionDefContext | undefined;
    enumDef(): EnumDefContext | undefined;
    structDef(): StructDefContext | undefined;
    callbackDef(): CallbackDefContext | undefined;
    compileTimeAssert(): CompileTimeAssertContext | undefined;
    doc(): DocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class ExternCContext extends ParserRuleContext {
    EXTERN(): TerminalNode;
    STRING(): TerminalNode;
    CURLY_B(): TerminalNode;
    CURLY_E(): TerminalNode;
    decl(): DeclContext[];
    decl(i: number): DeclContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class DirectiveContext extends ParserRuleContext {
    DEFINE(): TerminalNode;
    doc(): DocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class FunctionDeclContext extends ParserRuleContext {
    type(): TypeContext;
    id(): IdContext;
    signature(): SignatureContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    EXTERN(): TerminalNode | undefined;
    attribute(): AttributeContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class FunctionDefContext extends ParserRuleContext {
    inline(): InlineContext;
    type(): TypeContext;
    id(): IdContext;
    signature(): SignatureContext;
    block(): BlockContext;
    doc(): DocContext | undefined;
    attribute(): AttributeContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class AliasDefContext extends ParserRuleContext {
    TYPEDEF(): TerminalNode;
    type(): TypeContext;
    id(): IdContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    UNION(): TerminalNode | undefined;
    STRUCT(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class UnionDefContext extends ParserRuleContext {
    TYPEDEF(): TerminalNode;
    UNION(): TerminalNode;
    id(): IdContext[];
    id(i: number): IdContext;
    block(): BlockContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class EnumDefContext extends ParserRuleContext {
    TYPEDEF(): TerminalNode;
    ENUM(): TerminalNode;
    id(): IdContext[];
    id(i: number): IdContext;
    enumBody(): EnumBodyContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class StructDefContext extends ParserRuleContext {
    TYPEDEF(): TerminalNode;
    STRUCT(): TerminalNode;
    id(): IdContext[];
    id(i: number): IdContext;
    structBody(): StructBodyContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class CallbackDefContext extends ParserRuleContext {
    TYPEDEF(): TerminalNode;
    type(): TypeContext;
    ROUND_B(): TerminalNode;
    STAR(): TerminalNode;
    id(): IdContext;
    ROUND_E(): TerminalNode;
    signature(): SignatureContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class CompileTimeAssertContext extends ParserRuleContext {
    SDL_COMPILE_TIME_ASSERT(): TerminalNode;
    group(): GroupContext;
    SEMI(): TerminalNode;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class InlineContext extends ParserRuleContext {
    SDL_INLINE(): TerminalNode | undefined;
    STATIC(): TerminalNode | undefined;
    INLINE(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class BlockContext extends ParserRuleContext {
    CURLY_B(): TerminalNode;
    CURLY_E(): TerminalNode;
    stm(): StmContext[];
    stm(i: number): StmContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class GroupContext extends ParserRuleContext {
    ROUND_B(): TerminalNode;
    ROUND_E(): TerminalNode;
    stm(): StmContext[];
    stm(i: number): StmContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class IndexingContext extends ParserRuleContext {
    SQUARE_B(): TerminalNode;
    SQUARE_E(): TerminalNode;
    stm(): StmContext[];
    stm(i: number): StmContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class StmContext extends ParserRuleContext {
    block(): BlockContext | undefined;
    indexing(): IndexingContext | undefined;
    expr(): ExprContext | undefined;
    punct(): PunctContext | undefined;
    doc(): DocContext | undefined;
    trailingDoc(): TrailingDocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class ExprContext extends ParserRuleContext {
    group(): GroupContext | undefined;
    word(): WordContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class WordContext extends ParserRuleContext {
    ID(): TerminalNode | undefined;
    CONST(): TerminalNode | undefined;
    VOID(): TerminalNode | undefined;
    STATIC(): TerminalNode | undefined;
    ENUM(): TerminalNode | undefined;
    STRUCT(): TerminalNode | undefined;
    UNION(): TerminalNode | undefined;
    NUMBER(): TerminalNode | undefined;
    STRING(): TerminalNode | undefined;
    DEFINE(): TerminalNode | undefined;
    SDL_COMPILE_TIME_ASSERT(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class PunctContext extends ParserRuleContext {
    COLON(): TerminalNode | undefined;
    SEMI(): TerminalNode | undefined;
    COMMA(): TerminalNode | undefined;
    DOT(): TerminalNode | undefined;
    STAR(): TerminalNode | undefined;
    EQ(): TerminalNode | undefined;
    ELLIPSIS(): TerminalNode | undefined;
    PUNCT_EXTRA(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class EnumBodyContext extends ParserRuleContext {
    CURLY_B(): TerminalNode;
    CURLY_E(): TerminalNode;
    enumItem(): EnumItemContext[];
    enumItem(i: number): EnumItemContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class EnumItemContext extends ParserRuleContext {
    id(): IdContext;
    doc(): DocContext | undefined;
    EQ(): TerminalNode | undefined;
    expr(): ExprContext | undefined;
    COMMA(): TerminalNode | undefined;
    trailingDoc(): TrailingDocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class StructBodyContext extends ParserRuleContext {
    CURLY_B(): TerminalNode;
    CURLY_E(): TerminalNode;
    structItem(): StructItemContext[];
    structItem(i: number): StructItemContext;
    unionInlineType(): UnionInlineTypeContext[];
    unionInlineType(i: number): UnionInlineTypeContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class StructItemContext extends ParserRuleContext {
    structVar(): StructVarContext | undefined;
    structCallback(): StructCallbackContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class StructVarContext extends ParserRuleContext {
    type(): TypeContext;
    id(): IdContext[];
    id(i: number): IdContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    STRUCT(): TerminalNode | undefined;
    COMMA(): TerminalNode[];
    COMMA(i: number): TerminalNode;
    indexing(): IndexingContext[];
    indexing(i: number): IndexingContext;
    trailingDoc(): TrailingDocContext | undefined;
    CONST(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class StructCallbackContext extends ParserRuleContext {
    type(): TypeContext;
    ROUND_B(): TerminalNode;
    STAR(): TerminalNode;
    id(): IdContext;
    ROUND_E(): TerminalNode;
    signature(): SignatureContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    trailingDoc(): TrailingDocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class UnionInlineTypeContext extends ParserRuleContext {
    UNION(): TerminalNode;
    block(): BlockContext;
    id(): IdContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class IdContext extends ParserRuleContext {
    ID(): TerminalNode;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class TypeContext extends ParserRuleContext {
    typeEl(): TypeElContext[];
    typeEl(i: number): TypeElContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class TypeElContext extends ParserRuleContext {
    VOID(): TerminalNode | undefined;
    ID(): TerminalNode | undefined;
    CONST(): TerminalNode | undefined;
    STAR(): TerminalNode[];
    STAR(i: number): TerminalNode;
    indexing(): IndexingContext[];
    indexing(i: number): IndexingContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class SignatureContext extends ParserRuleContext {
    ROUND_B(): TerminalNode;
    ROUND_E(): TerminalNode;
    type(): TypeContext[];
    type(i: number): TypeContext;
    COMMA(): TerminalNode[];
    COMMA(i: number): TerminalNode;
    ELLIPSIS(): TerminalNode | undefined;
    attribute(): AttributeContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class AttributeContext extends ParserRuleContext {
    group(): GroupContext;
    ATTRIBUTE(): TerminalNode | undefined;
    SDL_VARARG_ATTRIB(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class DocContext extends ParserRuleContext {
    SHORT_DOC(): TerminalNode | undefined;
    LONG_DOC(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class TrailingDocContext extends ParserRuleContext {
    TRAILING_DOC(): TerminalNode;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
