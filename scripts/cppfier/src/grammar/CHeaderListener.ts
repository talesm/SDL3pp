// Generated from src/grammar/CHeader.g4 by ANTLR 4.9.0-SNAPSHOT


import { ParseTreeListener } from "antlr4ts/tree/ParseTreeListener";

import { ProgContext } from "./CHeaderParser";
import { StmContext } from "./CHeaderParser";
import { ExternCContext } from "./CHeaderParser";
import { DirectiveContext } from "./CHeaderParser";
import { FunctionDeclContext } from "./CHeaderParser";
import { FunctionDefContext } from "./CHeaderParser";
import { TypeContext } from "./CHeaderParser";
import { TypeElContext } from "./CHeaderParser";
import { SignatureContext } from "./CHeaderParser";
import { SignatureElContext } from "./CHeaderParser";
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
	 * Enter a parse tree produced by `CHeaderParser.signatureEl`.
	 * @param ctx the parse tree
	 */
	enterSignatureEl?: (ctx: SignatureElContext) => void;
	/**
	 * Exit a parse tree produced by `CHeaderParser.signatureEl`.
	 * @param ctx the parse tree
	 */
	exitSignatureEl?: (ctx: SignatureElContext) => void;

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

