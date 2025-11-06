import { ATN } from "antlr4ts/atn/ATN";
import { FailedPredicateException } from "antlr4ts/FailedPredicateException";
import { Parser } from "antlr4ts/Parser";
import { ParserRuleContext } from "antlr4ts/ParserRuleContext";
import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { TokenStream } from "antlr4ts/TokenStream";
import { Vocabulary } from "antlr4ts/Vocabulary";
import { DoxyCommentListener } from "./DoxyCommentListener";
export declare class DoxyCommentParser extends Parser {
    static readonly CODE_DELIM = 1;
    static readonly TABLE_DELIM = 2;
    static readonly LIST_DELIM = 3;
    static readonly TITLE_DELIM = 4;
    static readonly DEFGROUP_TAG = 5;
    static readonly RETURNS_TAG = 6;
    static readonly PARAM_TAG = 7;
    static readonly THREAD_SAFETY_TAG = 8;
    static readonly SA_TAG = 9;
    static readonly SINCE_TAG = 10;
    static readonly POST_TAG = 11;
    static readonly THROWS_TAG = 12;
    static readonly WORD = 13;
    static readonly WS = 14;
    static readonly NL = 15;
    static readonly RULE_doc = 0;
    static readonly RULE_block = 1;
    static readonly RULE_title = 2;
    static readonly RULE_paragraph = 3;
    static readonly RULE_list = 4;
    static readonly RULE_table = 5;
    static readonly RULE_codeblock = 6;
    static readonly RULE_taggedSection = 7;
    static readonly RULE_taggedBlock = 8;
    static readonly RULE_empty = 9;
    static readonly RULE_textLine = 10;
    static readonly RULE_listItem = 11;
    static readonly RULE_listItemContent = 12;
    static readonly RULE_tableItem = 13;
    static readonly RULE_blockTag = 14;
    static readonly RULE_endLine = 15;
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
    doc(): DocContext;
    block(): BlockContext;
    title(): TitleContext;
    paragraph(): ParagraphContext;
    list(): ListContext;
    table(): TableContext;
    codeblock(): CodeblockContext;
    taggedSection(): TaggedSectionContext;
    taggedBlock(): TaggedBlockContext;
    empty(): EmptyContext;
    textLine(): TextLineContext;
    listItem(): ListItemContext;
    listItemContent(): ListItemContentContext;
    tableItem(): TableItemContext;
    blockTag(): BlockTagContext;
    endLine(): EndLineContext;
    static readonly _serializedATN: string;
    static __ATN: ATN;
    static get _ATN(): ATN;
}
export declare class DocContext extends ParserRuleContext {
    EOF(): TerminalNode;
    block(): BlockContext[];
    block(i: number): BlockContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class BlockContext extends ParserRuleContext {
    title(): TitleContext | undefined;
    paragraph(): ParagraphContext | undefined;
    list(): ListContext | undefined;
    table(): TableContext | undefined;
    codeblock(): CodeblockContext | undefined;
    taggedSection(): TaggedSectionContext | undefined;
    empty(): EmptyContext | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class TitleContext extends ParserRuleContext {
    TITLE_DELIM(): TerminalNode;
    textLine(): TextLineContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class ParagraphContext extends ParserRuleContext {
    textLine(): TextLineContext[];
    textLine(i: number): TextLineContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class ListContext extends ParserRuleContext {
    listItem(): ListItemContext[];
    listItem(i: number): ListItemContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class TableContext extends ParserRuleContext {
    endLine(): EndLineContext;
    tableItem(): TableItemContext[];
    tableItem(i: number): TableItemContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class CodeblockContext extends ParserRuleContext {
    CODE_DELIM(): TerminalNode[];
    CODE_DELIM(i: number): TerminalNode;
    endLine(): EndLineContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class TaggedSectionContext extends ParserRuleContext {
    taggedBlock(): TaggedBlockContext[];
    taggedBlock(i: number): TaggedBlockContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class TaggedBlockContext extends ParserRuleContext {
    blockTag(): BlockTagContext;
    textLine(): TextLineContext[];
    textLine(i: number): TextLineContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class EmptyContext extends ParserRuleContext {
    NL(): TerminalNode;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class TextLineContext extends ParserRuleContext {
    endLine(): EndLineContext;
    WS(): TerminalNode | undefined;
    WORD(): TerminalNode | undefined;
    NL(): TerminalNode[];
    NL(i: number): TerminalNode;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class ListItemContext extends ParserRuleContext {
    LIST_DELIM(): TerminalNode;
    listItemContent(): ListItemContentContext;
    WS(): TerminalNode[];
    WS(i: number): TerminalNode;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class ListItemContentContext extends ParserRuleContext {
    textLine(): TextLineContext[];
    textLine(i: number): TextLineContext;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class TableItemContext extends ParserRuleContext {
    TABLE_DELIM(): TerminalNode[];
    TABLE_DELIM(i: number): TerminalNode;
    endLine(): EndLineContext;
    NL(): TerminalNode[];
    NL(i: number): TerminalNode;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class BlockTagContext extends ParserRuleContext {
    PARAM_TAG(): TerminalNode | undefined;
    WORD(): TerminalNode | undefined;
    WS(): TerminalNode[];
    WS(i: number): TerminalNode;
    DEFGROUP_TAG(): TerminalNode | undefined;
    RETURNS_TAG(): TerminalNode | undefined;
    THREAD_SAFETY_TAG(): TerminalNode | undefined;
    SA_TAG(): TerminalNode | undefined;
    SINCE_TAG(): TerminalNode | undefined;
    POST_TAG(): TerminalNode | undefined;
    THROWS_TAG(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
export declare class EndLineContext extends ParserRuleContext {
    NL(): TerminalNode | undefined;
    EOF(): TerminalNode | undefined;
    constructor(parent: ParserRuleContext | undefined, invokingState: number);
    get ruleIndex(): number;
    enterRule(listener: DoxyCommentListener): void;
    exitRule(listener: DoxyCommentListener): void;
}
