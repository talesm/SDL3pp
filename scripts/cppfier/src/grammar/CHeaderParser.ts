// Generated from src/grammar/CHeader.g4 by ANTLR 4.9.0-SNAPSHOT


import { ATN } from "antlr4ts/atn/ATN";
import { ATNDeserializer } from "antlr4ts/atn/ATNDeserializer";
import { FailedPredicateException } from "antlr4ts/FailedPredicateException";
import { NotNull } from "antlr4ts/Decorators";
import { NoViableAltException } from "antlr4ts/NoViableAltException";
import { Override } from "antlr4ts/Decorators";
import { Parser } from "antlr4ts/Parser";
import { ParserRuleContext } from "antlr4ts/ParserRuleContext";
import { ParserATNSimulator } from "antlr4ts/atn/ParserATNSimulator";
import { ParseTreeListener } from "antlr4ts/tree/ParseTreeListener";
import { ParseTreeVisitor } from "antlr4ts/tree/ParseTreeVisitor";
import { RecognitionException } from "antlr4ts/RecognitionException";
import { RuleContext } from "antlr4ts/RuleContext";
//import { RuleVersion } from "antlr4ts/RuleVersion";
import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { Token } from "antlr4ts/Token";
import { TokenStream } from "antlr4ts/TokenStream";
import { Vocabulary } from "antlr4ts/Vocabulary";
import { VocabularyImpl } from "antlr4ts/VocabularyImpl";

import * as Utils from "antlr4ts/misc/Utils";

import { CHeaderListener } from "./CHeaderListener";

export class CHeaderParser extends Parser {
	public static readonly WS = 1;
	public static readonly LONG_COMMENT = 2;
	public static readonly SHORT_COMMENT = 3;
	public static readonly LONG_DOC = 4;
	public static readonly SHORT_DOC = 5;
	public static readonly DIRECTIVE = 6;
	public static readonly ATTRIBUTE = 7;
	public static readonly EXTERN = 8;
	public static readonly INLINE = 9;
	public static readonly SDL_NOISE = 10;
	public static readonly SDL_INLINE = 11;
	public static readonly STATIC = 12;
	public static readonly TYPEDEF = 13;
	public static readonly VOID = 14;
	public static readonly CURLY_B = 15;
	public static readonly CURLY_E = 16;
	public static readonly ROUND_B = 17;
	public static readonly ROUND_E = 18;
	public static readonly SQUARE_B = 19;
	public static readonly SQUARE_E = 20;
	public static readonly COLON = 21;
	public static readonly SEMI = 22;
	public static readonly COMMA = 23;
	public static readonly DOT = 24;
	public static readonly STAR = 25;
	public static readonly PUNCT_EXTRA = 26;
	public static readonly STRING = 27;
	public static readonly ID = 28;
	public static readonly NUMBER = 29;
	public static readonly RULE_prog = 0;
	public static readonly RULE_decl = 1;
	public static readonly RULE_externC = 2;
	public static readonly RULE_directive = 3;
	public static readonly RULE_functionDecl = 4;
	public static readonly RULE_functionDef = 5;
	public static readonly RULE_aliasDef = 6;
	public static readonly RULE_inline = 7;
	public static readonly RULE_block = 8;
	public static readonly RULE_group = 9;
	public static readonly RULE_indexing = 10;
	public static readonly RULE_stm = 11;
	public static readonly RULE_word = 12;
	public static readonly RULE_punct = 13;
	public static readonly RULE_id = 14;
	public static readonly RULE_type = 15;
	public static readonly RULE_typeEl = 16;
	public static readonly RULE_signature = 17;
	public static readonly RULE_attribute = 18;
	public static readonly RULE_doc = 19;
	// tslint:disable:no-trailing-whitespace
	public static readonly ruleNames: string[] = [
		"prog", "decl", "externC", "directive", "functionDecl", "functionDef", 
		"aliasDef", "inline", "block", "group", "indexing", "stm", "word", "punct", 
		"id", "type", "typeEl", "signature", "attribute", "doc",
	];

	private static readonly _LITERAL_NAMES: Array<string | undefined> = [
		undefined, undefined, undefined, undefined, undefined, undefined, undefined, 
		"'__attribute__'", "'extern'", "'__inline__'", undefined, "'SDL_FORCE_INLINE'", 
		"'static'", "'typedef'", "'void'", "'{'", "'}'", "'('", "')'", "'['", 
		"']'", "':'", "';'", "','", "'.'", "'*'",
	];
	private static readonly _SYMBOLIC_NAMES: Array<string | undefined> = [
		undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "LONG_DOC", "SHORT_DOC", 
		"DIRECTIVE", "ATTRIBUTE", "EXTERN", "INLINE", "SDL_NOISE", "SDL_INLINE", 
		"STATIC", "TYPEDEF", "VOID", "CURLY_B", "CURLY_E", "ROUND_B", "ROUND_E", 
		"SQUARE_B", "SQUARE_E", "COLON", "SEMI", "COMMA", "DOT", "STAR", "PUNCT_EXTRA", 
		"STRING", "ID", "NUMBER",
	];
	public static readonly VOCABULARY: Vocabulary = new VocabularyImpl(CHeaderParser._LITERAL_NAMES, CHeaderParser._SYMBOLIC_NAMES, []);

	// @Override
	// @NotNull
	public get vocabulary(): Vocabulary {
		return CHeaderParser.VOCABULARY;
	}
	// tslint:enable:no-trailing-whitespace

	// @Override
	public get grammarFileName(): string { return "CHeader.g4"; }

	// @Override
	public get ruleNames(): string[] { return CHeaderParser.ruleNames; }

	// @Override
	public get serializedATN(): string { return CHeaderParser._serializedATN; }

	protected createFailedPredicateException(predicate?: string, message?: string): FailedPredicateException {
		return new FailedPredicateException(this, predicate, message);
	}

	constructor(input: TokenStream) {
		super(input);
		this._interp = new ParserATNSimulator(CHeaderParser._ATN, this);
	}
	// @RuleVersion(0)
	public prog(): ProgContext {
		let _localctx: ProgContext = new ProgContext(this._ctx, this.state);
		this.enterRule(_localctx, 0, CHeaderParser.RULE_prog);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 41;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 0, this._ctx) ) {
			case 1:
				{
				this.state = 40;
				this.doc();
				}
				break;
			}
			this.state = 46;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF))) !== 0)) {
				{
				{
				this.state = 43;
				this.decl();
				}
				}
				this.state = 48;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 49;
			this.match(CHeaderParser.EOF);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public decl(): DeclContext {
		let _localctx: DeclContext = new DeclContext(this._ctx, this.state);
		this.enterRule(_localctx, 2, CHeaderParser.RULE_decl);
		try {
			this.state = 57;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 2, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 51;
				this.directive();
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 52;
				this.externC();
				}
				break;

			case 3:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 53;
				this.functionDecl();
				}
				break;

			case 4:
				this.enterOuterAlt(_localctx, 4);
				{
				this.state = 54;
				this.functionDef();
				}
				break;

			case 5:
				this.enterOuterAlt(_localctx, 5);
				{
				this.state = 55;
				this.aliasDef();
				}
				break;

			case 6:
				this.enterOuterAlt(_localctx, 6);
				{
				this.state = 56;
				this.doc();
				}
				break;
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public externC(): ExternCContext {
		let _localctx: ExternCContext = new ExternCContext(this._ctx, this.state);
		this.enterRule(_localctx, 4, CHeaderParser.RULE_externC);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 59;
			this.match(CHeaderParser.EXTERN);
			this.state = 60;
			this.match(CHeaderParser.STRING);
			this.state = 61;
			this.match(CHeaderParser.CURLY_B);
			this.state = 65;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF))) !== 0)) {
				{
				{
				this.state = 62;
				this.decl();
				}
				}
				this.state = 67;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 68;
			this.match(CHeaderParser.CURLY_E);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public directive(): DirectiveContext {
		let _localctx: DirectiveContext = new DirectiveContext(this._ctx, this.state);
		this.enterRule(_localctx, 6, CHeaderParser.RULE_directive);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 71;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 70;
				this.doc();
				}
			}

			this.state = 73;
			this.match(CHeaderParser.DIRECTIVE);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public functionDecl(): FunctionDeclContext {
		let _localctx: FunctionDeclContext = new FunctionDeclContext(this._ctx, this.state);
		this.enterRule(_localctx, 8, CHeaderParser.RULE_functionDecl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 76;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 75;
				this.doc();
				}
			}

			this.state = 78;
			this.match(CHeaderParser.EXTERN);
			this.state = 79;
			this.type();
			this.state = 81;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.ATTRIBUTE) {
				{
				this.state = 80;
				this.attribute();
				}
			}

			this.state = 83;
			this.match(CHeaderParser.ID);
			this.state = 84;
			this.signature();
			this.state = 85;
			this.match(CHeaderParser.SEMI);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public functionDef(): FunctionDefContext {
		let _localctx: FunctionDefContext = new FunctionDefContext(this._ctx, this.state);
		this.enterRule(_localctx, 10, CHeaderParser.RULE_functionDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 88;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 87;
				this.doc();
				}
			}

			this.state = 90;
			this.inline();
			this.state = 91;
			this.type();
			this.state = 93;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.ATTRIBUTE) {
				{
				this.state = 92;
				this.attribute();
				}
			}

			this.state = 95;
			this.match(CHeaderParser.ID);
			this.state = 96;
			this.signature();
			this.state = 97;
			this.block();
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public aliasDef(): AliasDefContext {
		let _localctx: AliasDefContext = new AliasDefContext(this._ctx, this.state);
		this.enterRule(_localctx, 12, CHeaderParser.RULE_aliasDef);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 99;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 100;
			this.type();
			this.state = 101;
			this.match(CHeaderParser.ID);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public inline(): InlineContext {
		let _localctx: InlineContext = new InlineContext(this._ctx, this.state);
		this.enterRule(_localctx, 14, CHeaderParser.RULE_inline);
		try {
			this.state = 106;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.SDL_INLINE:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 103;
				this.match(CHeaderParser.SDL_INLINE);
				}
				break;
			case CHeaderParser.STATIC:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 104;
				this.match(CHeaderParser.STATIC);
				this.state = 105;
				this.match(CHeaderParser.INLINE);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public block(): BlockContext {
		let _localctx: BlockContext = new BlockContext(this._ctx, this.state);
		this.enterRule(_localctx, 16, CHeaderParser.RULE_block);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 108;
			this.match(CHeaderParser.CURLY_B);
			this.state = 112;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA) | (1 << CHeaderParser.DOT) | (1 << CHeaderParser.STAR) | (1 << CHeaderParser.PUNCT_EXTRA) | (1 << CHeaderParser.STRING) | (1 << CHeaderParser.ID) | (1 << CHeaderParser.NUMBER))) !== 0)) {
				{
				{
				this.state = 109;
				this.stm();
				}
				}
				this.state = 114;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 115;
			this.match(CHeaderParser.CURLY_E);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public group(): GroupContext {
		let _localctx: GroupContext = new GroupContext(this._ctx, this.state);
		this.enterRule(_localctx, 18, CHeaderParser.RULE_group);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 117;
			this.match(CHeaderParser.ROUND_B);
			this.state = 121;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA) | (1 << CHeaderParser.DOT) | (1 << CHeaderParser.STAR) | (1 << CHeaderParser.PUNCT_EXTRA) | (1 << CHeaderParser.STRING) | (1 << CHeaderParser.ID) | (1 << CHeaderParser.NUMBER))) !== 0)) {
				{
				{
				this.state = 118;
				this.stm();
				}
				}
				this.state = 123;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 124;
			this.match(CHeaderParser.ROUND_E);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public indexing(): IndexingContext {
		let _localctx: IndexingContext = new IndexingContext(this._ctx, this.state);
		this.enterRule(_localctx, 20, CHeaderParser.RULE_indexing);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 126;
			this.match(CHeaderParser.SQUARE_B);
			this.state = 130;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA) | (1 << CHeaderParser.DOT) | (1 << CHeaderParser.STAR) | (1 << CHeaderParser.PUNCT_EXTRA) | (1 << CHeaderParser.STRING) | (1 << CHeaderParser.ID) | (1 << CHeaderParser.NUMBER))) !== 0)) {
				{
				{
				this.state = 127;
				this.stm();
				}
				}
				this.state = 132;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 133;
			this.match(CHeaderParser.SQUARE_E);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public stm(): StmContext {
		let _localctx: StmContext = new StmContext(this._ctx, this.state);
		this.enterRule(_localctx, 22, CHeaderParser.RULE_stm);
		try {
			this.state = 140;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.CURLY_B:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 135;
				this.block();
				}
				break;
			case CHeaderParser.ROUND_B:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 136;
				this.group();
				}
				break;
			case CHeaderParser.SQUARE_B:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 137;
				this.indexing();
				}
				break;
			case CHeaderParser.DIRECTIVE:
			case CHeaderParser.STATIC:
			case CHeaderParser.VOID:
			case CHeaderParser.STRING:
			case CHeaderParser.ID:
			case CHeaderParser.NUMBER:
				this.enterOuterAlt(_localctx, 4);
				{
				this.state = 138;
				this.word();
				}
				break;
			case CHeaderParser.COLON:
			case CHeaderParser.SEMI:
			case CHeaderParser.COMMA:
			case CHeaderParser.DOT:
			case CHeaderParser.STAR:
			case CHeaderParser.PUNCT_EXTRA:
				this.enterOuterAlt(_localctx, 5);
				{
				this.state = 139;
				this.punct();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public word(): WordContext {
		let _localctx: WordContext = new WordContext(this._ctx, this.state);
		this.enterRule(_localctx, 24, CHeaderParser.RULE_word);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 142;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.STRING) | (1 << CHeaderParser.ID) | (1 << CHeaderParser.NUMBER))) !== 0))) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public punct(): PunctContext {
		let _localctx: PunctContext = new PunctContext(this._ctx, this.state);
		this.enterRule(_localctx, 26, CHeaderParser.RULE_punct);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 144;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA) | (1 << CHeaderParser.DOT) | (1 << CHeaderParser.STAR) | (1 << CHeaderParser.PUNCT_EXTRA))) !== 0))) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public id(): IdContext {
		let _localctx: IdContext = new IdContext(this._ctx, this.state);
		this.enterRule(_localctx, 28, CHeaderParser.RULE_id);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 146;
			this.match(CHeaderParser.ID);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public type(): TypeContext {
		let _localctx: TypeContext = new TypeContext(this._ctx, this.state);
		this.enterRule(_localctx, 30, CHeaderParser.RULE_type);
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 149;
			this._errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					this.state = 148;
					this.typeEl();
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				this.state = 151;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 14, this._ctx);
			} while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public typeEl(): TypeElContext {
		let _localctx: TypeElContext = new TypeElContext(this._ctx, this.state);
		this.enterRule(_localctx, 32, CHeaderParser.RULE_typeEl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 153;
			_la = this._input.LA(1);
			if (!(_la === CHeaderParser.VOID || _la === CHeaderParser.ID)) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			this.state = 157;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.STAR) {
				{
				{
				this.state = 154;
				this.match(CHeaderParser.STAR);
				}
				}
				this.state = 159;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public signature(): SignatureContext {
		let _localctx: SignatureContext = new SignatureContext(this._ctx, this.state);
		this.enterRule(_localctx, 34, CHeaderParser.RULE_signature);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 160;
			this.match(CHeaderParser.ROUND_B);
			this.state = 169;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.VOID || _la === CHeaderParser.ID) {
				{
				this.state = 161;
				this.type();
				this.state = 166;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la === CHeaderParser.COMMA) {
					{
					{
					this.state = 162;
					this.match(CHeaderParser.COMMA);
					this.state = 163;
					this.type();
					}
					}
					this.state = 168;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 171;
			this.match(CHeaderParser.ROUND_E);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public attribute(): AttributeContext {
		let _localctx: AttributeContext = new AttributeContext(this._ctx, this.state);
		this.enterRule(_localctx, 36, CHeaderParser.RULE_attribute);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 173;
			this.match(CHeaderParser.ATTRIBUTE);
			this.state = 174;
			this.group();
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public doc(): DocContext {
		let _localctx: DocContext = new DocContext(this._ctx, this.state);
		this.enterRule(_localctx, 38, CHeaderParser.RULE_doc);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 176;
			_la = this._input.LA(1);
			if (!(_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC)) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}

	public static readonly _serializedATN: string =
		"\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03\x1F\xB5\x04\x02" +
		"\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
		"\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r\t\r\x04" +
		"\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x04\x11\t\x11\x04\x12\t\x12\x04" +
		"\x13\t\x13\x04\x14\t\x14\x04\x15\t\x15\x03\x02\x05\x02,\n\x02\x03\x02" +
		"\x07\x02/\n\x02\f\x02\x0E\x022\v\x02\x03\x02\x03\x02\x03\x03\x03\x03\x03" +
		"\x03\x03\x03\x03\x03\x03\x03\x05\x03<\n\x03\x03\x04\x03\x04\x03\x04\x03" +
		"\x04\x07\x04B\n\x04\f\x04\x0E\x04E\v\x04\x03\x04\x03\x04\x03\x05\x05\x05" +
		"J\n\x05\x03\x05\x03\x05\x03\x06\x05\x06O\n\x06\x03\x06\x03\x06\x03\x06" +
		"\x05\x06T\n\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x07\x05\x07[\n\x07" +
		"\x03\x07\x03\x07\x03\x07\x05\x07`\n\x07\x03\x07\x03\x07\x03\x07\x03\x07" +
		"\x03\b\x03\b\x03\b\x03\b\x03\t\x03\t\x03\t\x05\tm\n\t\x03\n\x03\n\x07" +
		"\nq\n\n\f\n\x0E\nt\v\n\x03\n\x03\n\x03\v\x03\v\x07\vz\n\v\f\v\x0E\v}\v" +
		"\v\x03\v\x03\v\x03\f\x03\f\x07\f\x83\n\f\f\f\x0E\f\x86\v\f\x03\f\x03\f" +
		"\x03\r\x03\r\x03\r\x03\r\x03\r\x05\r\x8F\n\r\x03\x0E\x03\x0E\x03\x0F\x03" +
		"\x0F\x03\x10\x03\x10\x03\x11\x06\x11\x98\n\x11\r\x11\x0E\x11\x99\x03\x12" +
		"\x03\x12\x07\x12\x9E\n\x12\f\x12\x0E\x12\xA1\v\x12\x03\x13\x03\x13\x03" +
		"\x13\x03\x13\x07\x13\xA7\n\x13\f\x13\x0E\x13\xAA\v\x13\x05\x13\xAC\n\x13" +
		"\x03\x13\x03\x13\x03\x14\x03\x14\x03\x14\x03\x15\x03\x15\x03\x15\x02\x02" +
		"\x02\x16\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E\x02\x10\x02\x12" +
		"\x02\x14\x02\x16\x02\x18\x02\x1A\x02\x1C\x02\x1E\x02 \x02\"\x02$\x02&" +
		"\x02(\x02\x02\x06\x06\x02\b\b\x0E\x0E\x10\x10\x1D\x1F\x03\x02\x17\x1C" +
		"\x04\x02\x10\x10\x1E\x1E\x03\x02\x06\x07\x02\xB9\x02+\x03\x02\x02\x02" +
		"\x04;\x03\x02\x02\x02\x06=\x03\x02\x02\x02\bI\x03\x02\x02\x02\nN\x03\x02" +
		"\x02\x02\fZ\x03\x02\x02\x02\x0Ee\x03\x02\x02\x02\x10l\x03\x02\x02\x02" +
		"\x12n\x03\x02\x02\x02\x14w\x03\x02\x02\x02\x16\x80\x03\x02\x02\x02\x18" +
		"\x8E\x03\x02\x02\x02\x1A\x90\x03\x02\x02\x02\x1C\x92\x03\x02\x02\x02\x1E" +
		"\x94\x03\x02\x02\x02 \x97\x03\x02\x02\x02\"\x9B\x03\x02\x02\x02$\xA2\x03" +
		"\x02\x02\x02&\xAF\x03\x02\x02\x02(\xB2\x03\x02\x02\x02*,\x05(\x15\x02" +
		"+*\x03\x02\x02\x02+,\x03\x02\x02\x02,0\x03\x02\x02\x02-/\x05\x04\x03\x02" +
		".-\x03\x02\x02\x02/2\x03\x02\x02\x020.\x03\x02\x02\x0201\x03\x02\x02\x02" +
		"13\x03\x02\x02\x0220\x03\x02\x02\x0234\x07\x02\x02\x034\x03\x03\x02\x02" +
		"\x025<\x05\b\x05\x026<\x05\x06\x04\x027<\x05\n\x06\x028<\x05\f\x07\x02" +
		"9<\x05\x0E\b\x02:<\x05(\x15\x02;5\x03\x02\x02\x02;6\x03\x02\x02\x02;7" +
		"\x03\x02\x02\x02;8\x03\x02\x02\x02;9\x03\x02\x02\x02;:\x03\x02\x02\x02" +
		"<\x05\x03\x02\x02\x02=>\x07\n\x02\x02>?\x07\x1D\x02\x02?C\x07\x11\x02" +
		"\x02@B\x05\x04\x03\x02A@\x03\x02\x02\x02BE\x03\x02\x02\x02CA\x03\x02\x02" +
		"\x02CD\x03\x02\x02\x02DF\x03\x02\x02\x02EC\x03\x02\x02\x02FG\x07\x12\x02" +
		"\x02G\x07\x03\x02\x02\x02HJ\x05(\x15\x02IH\x03\x02\x02\x02IJ\x03\x02\x02" +
		"\x02JK\x03\x02\x02\x02KL\x07\b\x02\x02L\t\x03\x02\x02\x02MO\x05(\x15\x02" +
		"NM\x03\x02\x02\x02NO\x03\x02\x02\x02OP\x03\x02\x02\x02PQ\x07\n\x02\x02" +
		"QS\x05 \x11\x02RT\x05&\x14\x02SR\x03\x02\x02\x02ST\x03\x02\x02\x02TU\x03" +
		"\x02\x02\x02UV\x07\x1E\x02\x02VW\x05$\x13\x02WX\x07\x18\x02\x02X\v\x03" +
		"\x02\x02\x02Y[\x05(\x15\x02ZY\x03\x02\x02\x02Z[\x03\x02\x02\x02[\\\x03" +
		"\x02\x02\x02\\]\x05\x10\t\x02]_\x05 \x11\x02^`\x05&\x14\x02_^\x03\x02" +
		"\x02\x02_`\x03\x02\x02\x02`a\x03\x02\x02\x02ab\x07\x1E\x02\x02bc\x05$" +
		"\x13\x02cd\x05\x12\n\x02d\r\x03\x02\x02\x02ef\x07\x0F\x02\x02fg\x05 \x11" +
		"\x02gh\x07\x1E\x02\x02h\x0F\x03\x02\x02\x02im\x07\r\x02\x02jk\x07\x0E" +
		"\x02\x02km\x07\v\x02\x02li\x03\x02\x02\x02lj\x03\x02\x02\x02m\x11\x03" +
		"\x02\x02\x02nr\x07\x11\x02\x02oq\x05\x18\r\x02po\x03\x02\x02\x02qt\x03" +
		"\x02\x02\x02rp\x03\x02\x02\x02rs\x03\x02\x02\x02su\x03\x02\x02\x02tr\x03" +
		"\x02\x02\x02uv\x07\x12\x02\x02v\x13\x03\x02\x02\x02w{\x07\x13\x02\x02" +
		"xz\x05\x18\r\x02yx\x03\x02\x02\x02z}\x03\x02\x02\x02{y\x03\x02\x02\x02" +
		"{|\x03\x02\x02\x02|~\x03\x02\x02\x02}{\x03\x02\x02\x02~\x7F\x07\x14\x02" +
		"\x02\x7F\x15\x03\x02\x02\x02\x80\x84\x07\x15\x02\x02\x81\x83\x05\x18\r" +
		"\x02\x82\x81\x03\x02\x02\x02\x83\x86\x03\x02\x02\x02\x84\x82\x03\x02\x02" +
		"\x02\x84\x85\x03\x02\x02\x02\x85\x87\x03\x02\x02\x02\x86\x84\x03\x02\x02" +
		"\x02\x87\x88\x07\x16\x02\x02\x88\x17\x03\x02\x02\x02\x89\x8F\x05\x12\n" +
		"\x02\x8A\x8F\x05\x14\v\x02\x8B\x8F\x05\x16\f\x02\x8C\x8F\x05\x1A\x0E\x02" +
		"\x8D\x8F\x05\x1C\x0F\x02\x8E\x89\x03\x02\x02\x02\x8E\x8A\x03\x02\x02\x02" +
		"\x8E\x8B\x03\x02\x02\x02\x8E\x8C\x03\x02\x02\x02\x8E\x8D\x03\x02\x02\x02" +
		"\x8F\x19\x03\x02\x02\x02\x90\x91\t\x02\x02\x02\x91\x1B\x03\x02\x02\x02" +
		"\x92\x93\t\x03\x02\x02\x93\x1D\x03\x02\x02\x02\x94\x95\x07\x1E\x02\x02" +
		"\x95\x1F\x03\x02\x02\x02\x96\x98\x05\"\x12\x02\x97\x96\x03\x02\x02\x02" +
		"\x98\x99\x03\x02\x02\x02\x99\x97\x03\x02\x02\x02\x99\x9A\x03\x02\x02\x02" +
		"\x9A!\x03\x02\x02\x02\x9B\x9F\t\x04\x02\x02\x9C\x9E\x07\x1B\x02\x02\x9D" +
		"\x9C\x03\x02\x02\x02\x9E\xA1\x03\x02\x02\x02\x9F\x9D\x03\x02\x02\x02\x9F" +
		"\xA0\x03\x02\x02\x02\xA0#\x03\x02\x02\x02\xA1\x9F\x03\x02\x02\x02\xA2" +
		"\xAB\x07\x13\x02\x02\xA3\xA8\x05 \x11\x02\xA4\xA5\x07\x19\x02\x02\xA5" +
		"\xA7\x05 \x11\x02\xA6\xA4\x03\x02\x02\x02\xA7\xAA\x03\x02\x02\x02\xA8" +
		"\xA6\x03\x02\x02\x02\xA8\xA9\x03\x02\x02\x02\xA9\xAC\x03\x02\x02\x02\xAA" +
		"\xA8\x03\x02\x02\x02\xAB\xA3\x03\x02\x02\x02\xAB\xAC\x03\x02\x02\x02\xAC" +
		"\xAD\x03\x02\x02\x02\xAD\xAE\x07\x14\x02\x02\xAE%\x03\x02\x02\x02\xAF" +
		"\xB0\x07\t\x02\x02\xB0\xB1\x05\x14\v\x02\xB1\'\x03\x02\x02\x02\xB2\xB3" +
		"\t\x05\x02\x02\xB3)\x03\x02\x02\x02\x14+0;CINSZ_lr{\x84\x8E\x99\x9F\xA8" +
		"\xAB";
	public static __ATN: ATN;
	public static get _ATN(): ATN {
		if (!CHeaderParser.__ATN) {
			CHeaderParser.__ATN = new ATNDeserializer().deserialize(Utils.toCharArray(CHeaderParser._serializedATN));
		}

		return CHeaderParser.__ATN;
	}

}

export class ProgContext extends ParserRuleContext {
	public EOF(): TerminalNode { return this.getToken(CHeaderParser.EOF, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public decl(): DeclContext[];
	public decl(i: number): DeclContext;
	public decl(i?: number): DeclContext | DeclContext[] {
		if (i === undefined) {
			return this.getRuleContexts(DeclContext);
		} else {
			return this.getRuleContext(i, DeclContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_prog; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterProg) {
			listener.enterProg(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitProg) {
			listener.exitProg(this);
		}
	}
}


export class DeclContext extends ParserRuleContext {
	public directive(): DirectiveContext | undefined {
		return this.tryGetRuleContext(0, DirectiveContext);
	}
	public externC(): ExternCContext | undefined {
		return this.tryGetRuleContext(0, ExternCContext);
	}
	public functionDecl(): FunctionDeclContext | undefined {
		return this.tryGetRuleContext(0, FunctionDeclContext);
	}
	public functionDef(): FunctionDefContext | undefined {
		return this.tryGetRuleContext(0, FunctionDefContext);
	}
	public aliasDef(): AliasDefContext | undefined {
		return this.tryGetRuleContext(0, AliasDefContext);
	}
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_decl; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterDecl) {
			listener.enterDecl(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitDecl) {
			listener.exitDecl(this);
		}
	}
}


export class ExternCContext extends ParserRuleContext {
	public EXTERN(): TerminalNode { return this.getToken(CHeaderParser.EXTERN, 0); }
	public STRING(): TerminalNode { return this.getToken(CHeaderParser.STRING, 0); }
	public CURLY_B(): TerminalNode { return this.getToken(CHeaderParser.CURLY_B, 0); }
	public CURLY_E(): TerminalNode { return this.getToken(CHeaderParser.CURLY_E, 0); }
	public decl(): DeclContext[];
	public decl(i: number): DeclContext;
	public decl(i?: number): DeclContext | DeclContext[] {
		if (i === undefined) {
			return this.getRuleContexts(DeclContext);
		} else {
			return this.getRuleContext(i, DeclContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_externC; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterExternC) {
			listener.enterExternC(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitExternC) {
			listener.exitExternC(this);
		}
	}
}


export class DirectiveContext extends ParserRuleContext {
	public DIRECTIVE(): TerminalNode { return this.getToken(CHeaderParser.DIRECTIVE, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_directive; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterDirective) {
			listener.enterDirective(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitDirective) {
			listener.exitDirective(this);
		}
	}
}


export class FunctionDeclContext extends ParserRuleContext {
	public EXTERN(): TerminalNode { return this.getToken(CHeaderParser.EXTERN, 0); }
	public type(): TypeContext {
		return this.getRuleContext(0, TypeContext);
	}
	public ID(): TerminalNode { return this.getToken(CHeaderParser.ID, 0); }
	public signature(): SignatureContext {
		return this.getRuleContext(0, SignatureContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public attribute(): AttributeContext | undefined {
		return this.tryGetRuleContext(0, AttributeContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_functionDecl; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterFunctionDecl) {
			listener.enterFunctionDecl(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitFunctionDecl) {
			listener.exitFunctionDecl(this);
		}
	}
}


export class FunctionDefContext extends ParserRuleContext {
	public inline(): InlineContext {
		return this.getRuleContext(0, InlineContext);
	}
	public type(): TypeContext {
		return this.getRuleContext(0, TypeContext);
	}
	public ID(): TerminalNode { return this.getToken(CHeaderParser.ID, 0); }
	public signature(): SignatureContext {
		return this.getRuleContext(0, SignatureContext);
	}
	public block(): BlockContext {
		return this.getRuleContext(0, BlockContext);
	}
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public attribute(): AttributeContext | undefined {
		return this.tryGetRuleContext(0, AttributeContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_functionDef; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterFunctionDef) {
			listener.enterFunctionDef(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitFunctionDef) {
			listener.exitFunctionDef(this);
		}
	}
}


export class AliasDefContext extends ParserRuleContext {
	public TYPEDEF(): TerminalNode { return this.getToken(CHeaderParser.TYPEDEF, 0); }
	public type(): TypeContext {
		return this.getRuleContext(0, TypeContext);
	}
	public ID(): TerminalNode { return this.getToken(CHeaderParser.ID, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_aliasDef; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterAliasDef) {
			listener.enterAliasDef(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitAliasDef) {
			listener.exitAliasDef(this);
		}
	}
}


export class InlineContext extends ParserRuleContext {
	public SDL_INLINE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.SDL_INLINE, 0); }
	public STATIC(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STATIC, 0); }
	public INLINE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.INLINE, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_inline; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterInline) {
			listener.enterInline(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitInline) {
			listener.exitInline(this);
		}
	}
}


export class BlockContext extends ParserRuleContext {
	public CURLY_B(): TerminalNode { return this.getToken(CHeaderParser.CURLY_B, 0); }
	public CURLY_E(): TerminalNode { return this.getToken(CHeaderParser.CURLY_E, 0); }
	public stm(): StmContext[];
	public stm(i: number): StmContext;
	public stm(i?: number): StmContext | StmContext[] {
		if (i === undefined) {
			return this.getRuleContexts(StmContext);
		} else {
			return this.getRuleContext(i, StmContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_block; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterBlock) {
			listener.enterBlock(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitBlock) {
			listener.exitBlock(this);
		}
	}
}


export class GroupContext extends ParserRuleContext {
	public ROUND_B(): TerminalNode { return this.getToken(CHeaderParser.ROUND_B, 0); }
	public ROUND_E(): TerminalNode { return this.getToken(CHeaderParser.ROUND_E, 0); }
	public stm(): StmContext[];
	public stm(i: number): StmContext;
	public stm(i?: number): StmContext | StmContext[] {
		if (i === undefined) {
			return this.getRuleContexts(StmContext);
		} else {
			return this.getRuleContext(i, StmContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_group; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterGroup) {
			listener.enterGroup(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitGroup) {
			listener.exitGroup(this);
		}
	}
}


export class IndexingContext extends ParserRuleContext {
	public SQUARE_B(): TerminalNode { return this.getToken(CHeaderParser.SQUARE_B, 0); }
	public SQUARE_E(): TerminalNode { return this.getToken(CHeaderParser.SQUARE_E, 0); }
	public stm(): StmContext[];
	public stm(i: number): StmContext;
	public stm(i?: number): StmContext | StmContext[] {
		if (i === undefined) {
			return this.getRuleContexts(StmContext);
		} else {
			return this.getRuleContext(i, StmContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_indexing; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterIndexing) {
			listener.enterIndexing(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitIndexing) {
			listener.exitIndexing(this);
		}
	}
}


export class StmContext extends ParserRuleContext {
	public block(): BlockContext | undefined {
		return this.tryGetRuleContext(0, BlockContext);
	}
	public group(): GroupContext | undefined {
		return this.tryGetRuleContext(0, GroupContext);
	}
	public indexing(): IndexingContext | undefined {
		return this.tryGetRuleContext(0, IndexingContext);
	}
	public word(): WordContext | undefined {
		return this.tryGetRuleContext(0, WordContext);
	}
	public punct(): PunctContext | undefined {
		return this.tryGetRuleContext(0, PunctContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_stm; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterStm) {
			listener.enterStm(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitStm) {
			listener.exitStm(this);
		}
	}
}


export class WordContext extends ParserRuleContext {
	public ID(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.ID, 0); }
	public VOID(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.VOID, 0); }
	public STATIC(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STATIC, 0); }
	public NUMBER(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.NUMBER, 0); }
	public STRING(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STRING, 0); }
	public DIRECTIVE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.DIRECTIVE, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_word; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterWord) {
			listener.enterWord(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitWord) {
			listener.exitWord(this);
		}
	}
}


export class PunctContext extends ParserRuleContext {
	public COLON(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.COLON, 0); }
	public SEMI(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.SEMI, 0); }
	public COMMA(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.COMMA, 0); }
	public DOT(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.DOT, 0); }
	public STAR(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STAR, 0); }
	public PUNCT_EXTRA(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.PUNCT_EXTRA, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_punct; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterPunct) {
			listener.enterPunct(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitPunct) {
			listener.exitPunct(this);
		}
	}
}


export class IdContext extends ParserRuleContext {
	public ID(): TerminalNode { return this.getToken(CHeaderParser.ID, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_id; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterId) {
			listener.enterId(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitId) {
			listener.exitId(this);
		}
	}
}


export class TypeContext extends ParserRuleContext {
	public typeEl(): TypeElContext[];
	public typeEl(i: number): TypeElContext;
	public typeEl(i?: number): TypeElContext | TypeElContext[] {
		if (i === undefined) {
			return this.getRuleContexts(TypeElContext);
		} else {
			return this.getRuleContext(i, TypeElContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_type; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterType) {
			listener.enterType(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitType) {
			listener.exitType(this);
		}
	}
}


export class TypeElContext extends ParserRuleContext {
	public VOID(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.VOID, 0); }
	public ID(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.ID, 0); }
	public STAR(): TerminalNode[];
	public STAR(i: number): TerminalNode;
	public STAR(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(CHeaderParser.STAR);
		} else {
			return this.getToken(CHeaderParser.STAR, i);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_typeEl; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterTypeEl) {
			listener.enterTypeEl(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitTypeEl) {
			listener.exitTypeEl(this);
		}
	}
}


export class SignatureContext extends ParserRuleContext {
	public ROUND_B(): TerminalNode { return this.getToken(CHeaderParser.ROUND_B, 0); }
	public ROUND_E(): TerminalNode { return this.getToken(CHeaderParser.ROUND_E, 0); }
	public type(): TypeContext[];
	public type(i: number): TypeContext;
	public type(i?: number): TypeContext | TypeContext[] {
		if (i === undefined) {
			return this.getRuleContexts(TypeContext);
		} else {
			return this.getRuleContext(i, TypeContext);
		}
	}
	public COMMA(): TerminalNode[];
	public COMMA(i: number): TerminalNode;
	public COMMA(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(CHeaderParser.COMMA);
		} else {
			return this.getToken(CHeaderParser.COMMA, i);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_signature; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterSignature) {
			listener.enterSignature(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitSignature) {
			listener.exitSignature(this);
		}
	}
}


export class AttributeContext extends ParserRuleContext {
	public ATTRIBUTE(): TerminalNode { return this.getToken(CHeaderParser.ATTRIBUTE, 0); }
	public group(): GroupContext {
		return this.getRuleContext(0, GroupContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_attribute; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterAttribute) {
			listener.enterAttribute(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitAttribute) {
			listener.exitAttribute(this);
		}
	}
}


export class DocContext extends ParserRuleContext {
	public SHORT_DOC(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.SHORT_DOC, 0); }
	public LONG_DOC(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.LONG_DOC, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_doc; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterDoc) {
			listener.enterDoc(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitDoc) {
			listener.exitDoc(this);
		}
	}
}


