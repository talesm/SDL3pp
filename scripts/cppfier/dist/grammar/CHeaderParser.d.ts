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
    static readonly LONG_DOC = 4;
    static readonly SHORT_DOC = 5;
    static readonly DIRECTIVE = 6;
    static readonly EXTERN = 7;
    static readonly VOID = 8;
    static readonly ATTRIBUTE = 9;
    static readonly SDL_NOISE = 10;
    static readonly STATIC = 11;
    static readonly INLINE = 12;
    static readonly SDL_INLINE = 13;
    static readonly CURLY_B = 14;
    static readonly CURLY_E = 15;
    static readonly ROUND_B = 16;
    static readonly ROUND_E = 17;
    static readonly SQUARE_B = 18;
    static readonly SQUARE_E = 19;
    static readonly COLON = 20;
    static readonly SEMI = 21;
    static readonly COMMA = 22;
    static readonly DOT = 23;
    static readonly STAR = 24;
    static readonly PUNCT_EXTRA = 25;
    static readonly STRING = 26;
    static readonly ID = 27;
    static readonly NUMBER = 28;
    static readonly RULE_prog = 0;
    static readonly RULE_decl = 1;
    static readonly RULE_externC = 2;
    static readonly RULE_directive = 3;
    static readonly RULE_functionDecl = 4;
    static readonly RULE_functionDef = 5;
    static readonly RULE_inline = 6;
    static readonly RULE_block = 7;
    static readonly RULE_group = 8;
    static readonly RULE_indexing = 9;
    static readonly RULE_stm = 10;
    static readonly RULE_word = 11;
    static readonly RULE_punct = 12;
    static readonly RULE_type = 13;
    static readonly RULE_typeEl = 14;
    static readonly RULE_signature = 15;
    static readonly RULE_attribute = 16;
    static readonly RULE_doc = 17;
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
    inline(): InlineContext;
    block(): BlockContext;
    group(): GroupContext;
    indexing(): IndexingContext;
    stm(): StmContext;
    word(): WordContext;
    punct(): PunctContext;
    type(): TypeContext;
    typeEl(): TypeElContext;
    signature(): SignatureContext;
    attribute(): AttributeContext;
    doc(): DocContext;
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
    DIRECTIVE(): TerminalNode;
    doc(): DocContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class FunctionDeclContext extends ParserRuleContext {
    EXTERN(): TerminalNode;
    type(): TypeContext;
    ID(): TerminalNode;
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
    ID(): TerminalNode;
    signature(): SignatureContext;
    block(): BlockContext;
    doc(): DocContext | undefined;
    attribute(): AttributeContext | undefined;
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
    group(): GroupContext | undefined;
    indexing(): IndexingContext | undefined;
    word(): WordContext | undefined;
    punct(): PunctContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class WordContext extends ParserRuleContext {
    ID(): TerminalNode | undefined;
    VOID(): TerminalNode | undefined;
    STATIC(): TerminalNode | undefined;
    NUMBER(): TerminalNode | undefined;
    STRING(): TerminalNode | undefined;
    DIRECTIVE(): TerminalNode | undefined;
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
    PUNCT_EXTRA(): TerminalNode | undefined;
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
    type(): TypeContext | undefined;
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
