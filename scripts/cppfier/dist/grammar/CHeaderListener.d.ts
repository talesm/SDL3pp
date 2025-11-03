import { ParseTreeListener } from "antlr4ts/tree/ParseTreeListener";
import { ProgContext } from "./CHeaderParser";
import { ExprContext } from "./CHeaderParser";
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
     * Enter a parse tree produced by `CHeaderParser.expr`.
     * @param ctx the parse tree
     */
    enterExpr?: (ctx: ExprContext) => void;
    /**
     * Exit a parse tree produced by `CHeaderParser.expr`.
     * @param ctx the parse tree
     */
    exitExpr?: (ctx: ExprContext) => void;
}
