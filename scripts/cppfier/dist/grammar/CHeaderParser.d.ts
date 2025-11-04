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
    static readonly ENUM = 10;
    static readonly EXTERN = 11;
    static readonly INLINE = 12;
    static readonly SDL_NOISE = 13;
    static readonly SDL_INLINE = 14;
    static readonly STATIC = 15;
    static readonly STRUCT = 16;
    static readonly TYPEDEF = 17;
    static readonly UNION = 18;
    static readonly VOID = 19;
    static readonly CURLY_B = 20;
    static readonly CURLY_E = 21;
    static readonly ROUND_B = 22;
    static readonly ROUND_E = 23;
    static readonly SQUARE_B = 24;
    static readonly SQUARE_E = 25;
    static readonly COLON = 26;
    static readonly SEMI = 27;
    static readonly COMMA = 28;
    static readonly DOT = 29;
    static readonly STAR = 30;
    static readonly EQ = 31;
    static readonly PUNCT_EXTRA = 32;
    static readonly STRING = 33;
    static readonly ID = 34;
    static readonly NUMBER = 35;
    static readonly RULE_prog = 0;
    static readonly RULE_decl = 1;
    static readonly RULE_externC = 2;
    static readonly RULE_directive = 3;
    static readonly RULE_functionDecl = 4;
    static readonly RULE_functionDef = 5;
    static readonly RULE_aliasDef = 6;
    static readonly RULE_enumDef = 7;
    static readonly RULE_structDef = 8;
    static readonly RULE_callbackDef = 9;
    static readonly RULE_inline = 10;
    static readonly RULE_block = 11;
    static readonly RULE_group = 12;
    static readonly RULE_indexing = 13;
    static readonly RULE_stm = 14;
    static readonly RULE_expr = 15;
    static readonly RULE_word = 16;
    static readonly RULE_punct = 17;
    static readonly RULE_enumBody = 18;
    static readonly RULE_enumItem = 19;
    static readonly RULE_structBody = 20;
    static readonly RULE_structItem = 21;
    static readonly RULE_id = 22;
    static readonly RULE_type = 23;
    static readonly RULE_typeEl = 24;
    static readonly RULE_signature = 25;
    static readonly RULE_attribute = 26;
    static readonly RULE_doc = 27;
    static readonly RULE_trailingDoc = 28;
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
    enumDef(): EnumDefContext;
    structDef(): StructDefContext;
    callbackDef(): CallbackDefContext;
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
    enumDef(): EnumDefContext | undefined;
    structDef(): StructDefContext | undefined;
    callbackDef(): CallbackDefContext | undefined;
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
    EXTERN(): TerminalNode;
    type(): TypeContext;
    id(): IdContext;
    signature(): SignatureContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
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
    VOID(): TerminalNode | undefined;
    STATIC(): TerminalNode | undefined;
    ENUM(): TerminalNode | undefined;
    STRUCT(): TerminalNode | undefined;
    UNION(): TerminalNode | undefined;
    NUMBER(): TerminalNode | undefined;
    STRING(): TerminalNode | undefined;
    DEFINE(): TerminalNode | undefined;
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
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class StructItemContext extends ParserRuleContext {
    type(): TypeContext;
    id(): IdContext[];
    id(i: number): IdContext;
    SEMI(): TerminalNode;
    doc(): DocContext | undefined;
    COMMA(): TerminalNode[];
    COMMA(i: number): TerminalNode;
    indexing(): IndexingContext[];
    indexing(i: number): IndexingContext;
    trailingDoc(): TrailingDocContext | undefined;
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
    STAR(): TerminalNode[];
    STAR(i: number): TerminalNode;
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
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class AttributeContext extends ParserRuleContext {
    ATTRIBUTE(): TerminalNode;
    group(): GroupContext;
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
