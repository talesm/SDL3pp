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
    static readonly SDL_NOISE = 9;
    static readonly CURLY_B = 10;
    static readonly CURLY_E = 11;
    static readonly ROUND_B = 12;
    static readonly ROUND_E = 13;
    static readonly SEMI = 14;
    static readonly STAR = 15;
    static readonly STRING = 16;
    static readonly ID = 17;
    static readonly RULE_prog = 0;
    static readonly RULE_stm = 1;
    static readonly RULE_externC = 2;
    static readonly RULE_directive = 3;
    static readonly RULE_functionDecl = 4;
    static readonly RULE_type = 5;
    static readonly RULE_typeEl = 6;
    static readonly RULE_signature = 7;
    static readonly RULE_signatureEl = 8;
    static readonly RULE_doc = 9;
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
    stm(): StmContext;
    externC(): ExternCContext;
    directive(): DirectiveContext;
    functionDecl(): FunctionDeclContext;
    type(): TypeContext;
    typeEl(): TypeElContext;
    signature(): SignatureContext;
    signatureEl(): SignatureElContext;
    doc(): DocContext;
    static readonly _serializedATN: string;
    static __ATN: ATN;
    static get _ATN(): ATN;
}
export declare class ProgContext extends ParserRuleContext {
    EOF(): TerminalNode;
    doc(): DocContext | undefined;
    stm(): StmContext[];
    stm(i: number): StmContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class StmContext extends ParserRuleContext {
    directive(): DirectiveContext | undefined;
    externC(): ExternCContext | undefined;
    functionDecl(): FunctionDeclContext | undefined;
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
    stm(): StmContext[];
    stm(i: number): StmContext;
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
    VOID(): TerminalNode | undefined;
    signatureEl(): SignatureElContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class SignatureElContext extends ParserRuleContext {
    type(): TypeContext;
    ID(): TerminalNode;
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
