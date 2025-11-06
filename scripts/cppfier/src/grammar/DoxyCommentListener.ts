// Generated from src/grammar/DoxyComment.g4 by ANTLR 4.9.0-SNAPSHOT


import { ParseTreeListener } from "antlr4ts/tree/ParseTreeListener";

import { DocContext } from "./DoxyCommentParser";
import { BlockContext } from "./DoxyCommentParser";
import { TitleContext } from "./DoxyCommentParser";
import { ParagraphContext } from "./DoxyCommentParser";
import { ListContext } from "./DoxyCommentParser";
import { TableContext } from "./DoxyCommentParser";
import { CodeblockContext } from "./DoxyCommentParser";
import { TaggedSectionContext } from "./DoxyCommentParser";
import { TaggedBlockContext } from "./DoxyCommentParser";
import { EmptyContext } from "./DoxyCommentParser";
import { TextLineContext } from "./DoxyCommentParser";
import { ListItemContext } from "./DoxyCommentParser";
import { ListItemContentContext } from "./DoxyCommentParser";
import { TableItemContext } from "./DoxyCommentParser";
import { BlockTagContext } from "./DoxyCommentParser";
import { EndLineContext } from "./DoxyCommentParser";


/**
 * This interface defines a complete listener for a parse tree produced by
 * `DoxyCommentParser`.
 */
export interface DoxyCommentListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by `DoxyCommentParser.doc`.
	 * @param ctx the parse tree
	 */
	enterDoc?: (ctx: DocContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.doc`.
	 * @param ctx the parse tree
	 */
	exitDoc?: (ctx: DocContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.block`.
	 * @param ctx the parse tree
	 */
	enterBlock?: (ctx: BlockContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.block`.
	 * @param ctx the parse tree
	 */
	exitBlock?: (ctx: BlockContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.title`.
	 * @param ctx the parse tree
	 */
	enterTitle?: (ctx: TitleContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.title`.
	 * @param ctx the parse tree
	 */
	exitTitle?: (ctx: TitleContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.paragraph`.
	 * @param ctx the parse tree
	 */
	enterParagraph?: (ctx: ParagraphContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.paragraph`.
	 * @param ctx the parse tree
	 */
	exitParagraph?: (ctx: ParagraphContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.list`.
	 * @param ctx the parse tree
	 */
	enterList?: (ctx: ListContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.list`.
	 * @param ctx the parse tree
	 */
	exitList?: (ctx: ListContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.table`.
	 * @param ctx the parse tree
	 */
	enterTable?: (ctx: TableContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.table`.
	 * @param ctx the parse tree
	 */
	exitTable?: (ctx: TableContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.codeblock`.
	 * @param ctx the parse tree
	 */
	enterCodeblock?: (ctx: CodeblockContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.codeblock`.
	 * @param ctx the parse tree
	 */
	exitCodeblock?: (ctx: CodeblockContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.taggedSection`.
	 * @param ctx the parse tree
	 */
	enterTaggedSection?: (ctx: TaggedSectionContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.taggedSection`.
	 * @param ctx the parse tree
	 */
	exitTaggedSection?: (ctx: TaggedSectionContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.taggedBlock`.
	 * @param ctx the parse tree
	 */
	enterTaggedBlock?: (ctx: TaggedBlockContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.taggedBlock`.
	 * @param ctx the parse tree
	 */
	exitTaggedBlock?: (ctx: TaggedBlockContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.empty`.
	 * @param ctx the parse tree
	 */
	enterEmpty?: (ctx: EmptyContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.empty`.
	 * @param ctx the parse tree
	 */
	exitEmpty?: (ctx: EmptyContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.textLine`.
	 * @param ctx the parse tree
	 */
	enterTextLine?: (ctx: TextLineContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.textLine`.
	 * @param ctx the parse tree
	 */
	exitTextLine?: (ctx: TextLineContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.listItem`.
	 * @param ctx the parse tree
	 */
	enterListItem?: (ctx: ListItemContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.listItem`.
	 * @param ctx the parse tree
	 */
	exitListItem?: (ctx: ListItemContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.listItemContent`.
	 * @param ctx the parse tree
	 */
	enterListItemContent?: (ctx: ListItemContentContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.listItemContent`.
	 * @param ctx the parse tree
	 */
	exitListItemContent?: (ctx: ListItemContentContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.tableItem`.
	 * @param ctx the parse tree
	 */
	enterTableItem?: (ctx: TableItemContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.tableItem`.
	 * @param ctx the parse tree
	 */
	exitTableItem?: (ctx: TableItemContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.blockTag`.
	 * @param ctx the parse tree
	 */
	enterBlockTag?: (ctx: BlockTagContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.blockTag`.
	 * @param ctx the parse tree
	 */
	exitBlockTag?: (ctx: BlockTagContext) => void;

	/**
	 * Enter a parse tree produced by `DoxyCommentParser.endLine`.
	 * @param ctx the parse tree
	 */
	enterEndLine?: (ctx: EndLineContext) => void;
	/**
	 * Exit a parse tree produced by `DoxyCommentParser.endLine`.
	 * @param ctx the parse tree
	 */
	exitEndLine?: (ctx: EndLineContext) => void;
}

