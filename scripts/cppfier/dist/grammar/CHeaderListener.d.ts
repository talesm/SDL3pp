import { ParseTreeListener } from "antlr4ts/tree/ParseTreeListener";
import { ProgContext } from "./CHeaderParser";
import { DeclContext } from "./CHeaderParser";
import { ExternCContext } from "./CHeaderParser";
import { DirectiveContext } from "./CHeaderParser";
import { FunctionDeclContext } from "./CHeaderParser";
import { FunctionDefContext } from "./CHeaderParser";
import { InlineContext } from "./CHeaderParser";
import { BlockContext } from "./CHeaderParser";
import { GroupContext } from "./CHeaderParser";
import { IndexingContext } from "./CHeaderParser";
import { StmContext } from "./CHeaderParser";
import { WordContext } from "./CHeaderParser";
import { PunctContext } from "./CHeaderParser";
import { TypeContext } from "./CHeaderParser";
import { TypeElContext } from "./CHeaderParser";
import { SignatureContext } from "./CHeaderParser";
import { AttributeContext } from "./CHeaderParser";
import { DocContext } from "./CHeaderParser";
/**
 * This interface defines a complete listener for a parse tree produced by
 * `CHeaderParser`.
 */
export interface CHeaderListener extends ParseTreeListener {
    /**
     * Enter a parse tree produced by `CHeaderParser.prog`.
     * @param ctx the parse tree
     */
    enterProg?: (ctx: ProgContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.prog`.
     * @param ctx the parse tree
     */
    exitProg?: (ctx: ProgContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.decl`.
     * @param ctx the parse tree
     */
    enterDecl?: (ctx: DeclContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.decl`.
     * @param ctx the parse tree
     */
    exitDecl?: (ctx: DeclContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.externC`.
     * @param ctx the parse tree
     */
    enterExternC?: (ctx: ExternCContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.externC`.
     * @param ctx the parse tree
     */
    exitExternC?: (ctx: ExternCContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.directive`.
     * @param ctx the parse tree
     */
    enterDirective?: (ctx: DirectiveContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.directive`.
     * @param ctx the parse tree
     */
    exitDirective?: (ctx: DirectiveContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.functionDecl`.
     * @param ctx the parse tree
     */
    enterFunctionDecl?: (ctx: FunctionDeclContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.functionDecl`.
     * @param ctx the parse tree
     */
    exitFunctionDecl?: (ctx: FunctionDeclContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.functionDef`.
     * @param ctx the parse tree
     */
    enterFunctionDef?: (ctx: FunctionDefContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.functionDef`.
     * @param ctx the parse tree
     */
    exitFunctionDef?: (ctx: FunctionDefContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.inline`.
     * @param ctx the parse tree
     */
    enterInline?: (ctx: InlineContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.inline`.
     * @param ctx the parse tree
     */
    exitInline?: (ctx: InlineContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.block`.
     * @param ctx the parse tree
     */
    enterBlock?: (ctx: BlockContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.block`.
     * @param ctx the parse tree
     */
    exitBlock?: (ctx: BlockContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.group`.
     * @param ctx the parse tree
     */
    enterGroup?: (ctx: GroupContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.group`.
     * @param ctx the parse tree
     */
    exitGroup?: (ctx: GroupContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.indexing`.
     * @param ctx the parse tree
     */
    enterIndexing?: (ctx: IndexingContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.indexing`.
     * @param ctx the parse tree
     */
    exitIndexing?: (ctx: IndexingContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.stm`.
     * @param ctx the parse tree
     */
    enterStm?: (ctx: StmContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.stm`.
     * @param ctx the parse tree
     */
    exitStm?: (ctx: StmContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.word`.
     * @param ctx the parse tree
     */
    enterWord?: (ctx: WordContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.word`.
     * @param ctx the parse tree
     */
    exitWord?: (ctx: WordContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.punct`.
     * @param ctx the parse tree
     */
    enterPunct?: (ctx: PunctContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.punct`.
     * @param ctx the parse tree
     */
    exitPunct?: (ctx: PunctContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.type`.
     * @param ctx the parse tree
     */
    enterType?: (ctx: TypeContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.type`.
     * @param ctx the parse tree
     */
    exitType?: (ctx: TypeContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.typeEl`.
     * @param ctx the parse tree
     */
    enterTypeEl?: (ctx: TypeElContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.typeEl`.
     * @param ctx the parse tree
     */
    exitTypeEl?: (ctx: TypeElContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.signature`.
     * @param ctx the parse tree
     */
    enterSignature?: (ctx: SignatureContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.signature`.
     * @param ctx the parse tree
     */
    exitSignature?: (ctx: SignatureContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.attribute`.
     * @param ctx the parse tree
     */
    enterAttribute?: (ctx: AttributeContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.attribute`.
     * @param ctx the parse tree
     */
    exitAttribute?: (ctx: AttributeContext) => void;
    /**
     * Enter a parse tree produced by `CHeaderParser.doc`.
     * @param ctx the parse tree
     */
    enterDoc?: (ctx: DocContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.doc`.
     * @param ctx the parse tree
     */
    exitDoc?: (ctx: DocContext) => void;
}
