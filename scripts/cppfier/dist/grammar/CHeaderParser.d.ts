import { ATN } from "antlr4ts/atn/ATN";
import { FailedPredicateException } from "antlr4ts/FailedPredicateException";
import { Parser } from "antlr4ts/Parser";
import { ParserRuleContext } from "antlr4ts/ParserRuleContext";
import { RuleContext } from "antlr4ts/RuleContext";
import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { TokenStream } from "antlr4ts/TokenStream";
import { Vocabulary } from "antlr4ts/Vocabulary";
import { CHeaderListener } from "./CHeaderListener";
export declare class CHeaderParser extends Parser {
    static readonly T__0 = 1;
    static readonly T__1 = 2;
    static readonly T__2 = 3;
    static readonly T__3 = 4;
    static readonly T__4 = 5;
    static readonly T__5 = 6;
    static readonly NEWLINE = 7;
    static readonly INT = 8;
    static readonly RULE_prog = 0;
    static readonly RULE_expr = 1;
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
    expr(): ExprContext;
    expr(_p: number): ExprContext;
    sempred(_localctx: RuleContext, ruleIndex: number, predIndex: number): boolean;
    private expr_sempred;
    static readonly _serializedATN: string;
    static __ATN: ATN;
    static get _ATN(): ATN;
}
export declare class ProgContext extends ParserRuleContext {
    expr(): ExprContext;
    EOF(): TerminalNode;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
export declare class ExprContext extends ParserRuleContext {
    expr(): ExprContext[];
    expr(i: number): ExprContext;
    INT(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: CHeaderListener): void;
    exitRule(listener: CHeaderListener): void;
}
