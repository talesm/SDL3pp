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
	public static readonly TRAILING_DOC = 4;
	public static readonly LONG_DOC = 5;
	public static readonly SHORT_DOC = 6;
	public static readonly DEFINE = 7;
	public static readonly DIRECTIVE = 8;
	public static readonly ATTRIBUTE = 9;
	public static readonly CONST = 10;
	public static readonly ENUM = 11;
	public static readonly EXTERN = 12;
	public static readonly INLINE = 13;
	public static readonly SDL_NOISE = 14;
	public static readonly SDL_INLINE = 15;
	public static readonly STATIC = 16;
	public static readonly STRUCT = 17;
	public static readonly TYPEDEF = 18;
	public static readonly UNION = 19;
	public static readonly VOID = 20;
	public static readonly SDL_VARARG_ATTRIB = 21;
	public static readonly SDL_ACQUIRE = 22;
	public static readonly SDL_RELEASE = 23;
	public static readonly SDL_COMPILE_TIME_ASSERT = 24;
	public static readonly SDL_ALLOC_SIZE = 25;
	public static readonly SDL_INOUT = 26;
	public static readonly VK_DEFINE_HANDLE = 27;
	public static readonly CURLY_B = 28;
	public static readonly CURLY_E = 29;
	public static readonly ROUND_B = 30;
	public static readonly ROUND_E = 31;
	public static readonly SQUARE_B = 32;
	public static readonly SQUARE_E = 33;
	public static readonly COLON = 34;
	public static readonly SEMI = 35;
	public static readonly COMMA = 36;
	public static readonly DOT = 37;
	public static readonly STAR = 38;
	public static readonly EQ = 39;
	public static readonly ELLIPSIS = 40;
	public static readonly PUNCT_EXTRA = 41;
	public static readonly STRING = 42;
	public static readonly ID = 43;
	public static readonly NUMBER = 44;
	public static readonly RULE_prog = 0;
	public static readonly RULE_decl = 1;
	public static readonly RULE_directive = 2;
	public static readonly RULE_externC = 3;
	public static readonly RULE_globalVar = 4;
	public static readonly RULE_functionDecl = 5;
	public static readonly RULE_functionDef = 6;
	public static readonly RULE_aliasDef = 7;
	public static readonly RULE_unionDef = 8;
	public static readonly RULE_enumDef = 9;
	public static readonly RULE_structDecl = 10;
	public static readonly RULE_structDef = 11;
	public static readonly RULE_callbackDef = 12;
	public static readonly RULE_compileTimeAssert = 13;
	public static readonly RULE_inline = 14;
	public static readonly RULE_block = 15;
	public static readonly RULE_group = 16;
	public static readonly RULE_indexing = 17;
	public static readonly RULE_stm = 18;
	public static readonly RULE_expr = 19;
	public static readonly RULE_word = 20;
	public static readonly RULE_punct = 21;
	public static readonly RULE_enumBody = 22;
	public static readonly RULE_enumItem = 23;
	public static readonly RULE_structBody = 24;
	public static readonly RULE_structItem = 25;
	public static readonly RULE_structVar = 26;
	public static readonly RULE_structCallback = 27;
	public static readonly RULE_unionInlineType = 28;
	public static readonly RULE_id = 29;
	public static readonly RULE_type = 30;
	public static readonly RULE_typeEl = 31;
	public static readonly RULE_signature = 32;
	public static readonly RULE_attribute = 33;
	public static readonly RULE_doc = 34;
	public static readonly RULE_trailingDoc = 35;
	// tslint:disable:no-trailing-whitespace
	public static readonly ruleNames: string[] = [
		"prog", "decl", "directive", "externC", "globalVar", "functionDecl", "functionDef", 
		"aliasDef", "unionDef", "enumDef", "structDecl", "structDef", "callbackDef", 
		"compileTimeAssert", "inline", "block", "group", "indexing", "stm", "expr", 
		"word", "punct", "enumBody", "enumItem", "structBody", "structItem", "structVar", 
		"structCallback", "unionInlineType", "id", "type", "typeEl", "signature", 
		"attribute", "doc", "trailingDoc",
	];

	private static readonly _LITERAL_NAMES: Array<string | undefined> = [
		undefined, undefined, undefined, undefined, undefined, undefined, undefined, 
		undefined, undefined, "'__attribute__'", "'const'", "'enum'", "'extern'", 
		"'__inline__'", undefined, "'SDL_FORCE_INLINE'", "'static'", "'struct'", 
		"'typedef'", "'union'", "'void'", undefined, undefined, undefined, "'SDL_COMPILE_TIME_ASSERT'", 
		undefined, undefined, undefined, "'{'", "'}'", "'('", "')'", "'['", "']'", 
		"':'", "';'", "','", "'.'", "'*'", "'='", "'...'",
	];
	private static readonly _SYMBOLIC_NAMES: Array<string | undefined> = [
		undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "TRAILING_DOC", "LONG_DOC", 
		"SHORT_DOC", "DEFINE", "DIRECTIVE", "ATTRIBUTE", "CONST", "ENUM", "EXTERN", 
		"INLINE", "SDL_NOISE", "SDL_INLINE", "STATIC", "STRUCT", "TYPEDEF", "UNION", 
		"VOID", "SDL_VARARG_ATTRIB", "SDL_ACQUIRE", "SDL_RELEASE", "SDL_COMPILE_TIME_ASSERT", 
		"SDL_ALLOC_SIZE", "SDL_INOUT", "VK_DEFINE_HANDLE", "CURLY_B", "CURLY_E", 
		"ROUND_B", "ROUND_E", "SQUARE_B", "SQUARE_E", "COLON", "SEMI", "COMMA", 
		"DOT", "STAR", "EQ", "ELLIPSIS", "PUNCT_EXTRA", "STRING", "ID", "NUMBER",
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
			this.state = 73;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 0, this._ctx) ) {
			case 1:
				{
				this.state = 72;
				this.doc();
				}
				break;
			}
			this.state = 78;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0) || _la === CHeaderParser.ID) {
				{
				{
				this.state = 75;
				this.decl();
				}
				}
				this.state = 80;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 81;
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
			this.state = 96;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 2, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 83;
				this.directive();
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 84;
				this.externC();
				}
				break;

			case 3:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 85;
				this.globalVar();
				}
				break;

			case 4:
				this.enterOuterAlt(_localctx, 4);
				{
				this.state = 86;
				this.functionDecl();
				}
				break;

			case 5:
				this.enterOuterAlt(_localctx, 5);
				{
				this.state = 87;
				this.functionDef();
				}
				break;

			case 6:
				this.enterOuterAlt(_localctx, 6);
				{
				this.state = 88;
				this.aliasDef();
				}
				break;

			case 7:
				this.enterOuterAlt(_localctx, 7);
				{
				this.state = 89;
				this.unionDef();
				}
				break;

			case 8:
				this.enterOuterAlt(_localctx, 8);
				{
				this.state = 90;
				this.enumDef();
				}
				break;

			case 9:
				this.enterOuterAlt(_localctx, 9);
				{
				this.state = 91;
				this.structDecl();
				}
				break;

			case 10:
				this.enterOuterAlt(_localctx, 10);
				{
				this.state = 92;
				this.structDef();
				}
				break;

			case 11:
				this.enterOuterAlt(_localctx, 11);
				{
				this.state = 93;
				this.callbackDef();
				}
				break;

			case 12:
				this.enterOuterAlt(_localctx, 12);
				{
				this.state = 94;
				this.compileTimeAssert();
				}
				break;

			case 13:
				this.enterOuterAlt(_localctx, 13);
				{
				this.state = 95;
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
	public directive(): DirectiveContext {
		let _localctx: DirectiveContext = new DirectiveContext(this._ctx, this.state);
		this.enterRule(_localctx, 4, CHeaderParser.RULE_directive);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 99;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 98;
				this.doc();
				}
			}

			this.state = 101;
			this.match(CHeaderParser.DEFINE);
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
		this.enterRule(_localctx, 6, CHeaderParser.RULE_externC);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 103;
			this.match(CHeaderParser.EXTERN);
			this.state = 104;
			this.match(CHeaderParser.STRING);
			this.state = 105;
			this.match(CHeaderParser.CURLY_B);
			this.state = 109;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0) || _la === CHeaderParser.ID) {
				{
				{
				this.state = 106;
				this.decl();
				}
				}
				this.state = 111;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 112;
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
	public globalVar(): GlobalVarContext {
		let _localctx: GlobalVarContext = new GlobalVarContext(this._ctx, this.state);
		this.enterRule(_localctx, 8, CHeaderParser.RULE_globalVar);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 115;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 114;
				this.doc();
				}
			}

			this.state = 118;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.EXTERN) {
				{
				this.state = 117;
				this.match(CHeaderParser.EXTERN);
				}
			}

			this.state = 120;
			this.type();
			this.state = 121;
			this.id();
			this.state = 126;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.COMMA) {
				{
				{
				this.state = 122;
				this.match(CHeaderParser.COMMA);
				this.state = 123;
				this.id();
				}
				}
				this.state = 128;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 132;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.SQUARE_B) {
				{
				{
				this.state = 129;
				this.indexing();
				}
				}
				this.state = 134;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 135;
			this.match(CHeaderParser.SEMI);
			this.state = 137;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 136;
				this.trailingDoc();
				}
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
	public functionDecl(): FunctionDeclContext {
		let _localctx: FunctionDeclContext = new FunctionDeclContext(this._ctx, this.state);
		this.enterRule(_localctx, 10, CHeaderParser.RULE_functionDecl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 140;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 139;
				this.doc();
				}
			}

			this.state = 143;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.EXTERN) {
				{
				this.state = 142;
				this.match(CHeaderParser.EXTERN);
				}
			}

			this.state = 148;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0)) {
				{
				{
				this.state = 145;
				this.attribute();
				}
				}
				this.state = 150;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 151;
			this.type();
			this.state = 155;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0)) {
				{
				{
				this.state = 152;
				this.attribute();
				}
				}
				this.state = 157;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 158;
			this.id();
			this.state = 159;
			this.signature();
			this.state = 160;
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
		this.enterRule(_localctx, 12, CHeaderParser.RULE_functionDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 163;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 162;
				this.doc();
				}
			}

			this.state = 165;
			this.inline();
			this.state = 166;
			this.type();
			this.state = 170;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0)) {
				{
				{
				this.state = 167;
				this.attribute();
				}
				}
				this.state = 172;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 173;
			this.id();
			this.state = 174;
			this.signature();
			this.state = 175;
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
		this.enterRule(_localctx, 14, CHeaderParser.RULE_aliasDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 178;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 177;
				this.doc();
				}
			}

			this.state = 180;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 182;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 17, this._ctx) ) {
			case 1:
				{
				this.state = 181;
				_la = this._input.LA(1);
				if (!(_la === CHeaderParser.STRUCT || _la === CHeaderParser.UNION)) {
				this._errHandler.recoverInline(this);
				} else {
					if (this._input.LA(1) === Token.EOF) {
						this.matchedEOF = true;
					}

					this._errHandler.reportMatch(this);
					this.consume();
				}
				}
				break;
			}
			this.state = 184;
			this.type();
			this.state = 185;
			this.id();
			this.state = 186;
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
	public unionDef(): UnionDefContext {
		let _localctx: UnionDefContext = new UnionDefContext(this._ctx, this.state);
		this.enterRule(_localctx, 16, CHeaderParser.RULE_unionDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 189;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 188;
				this.doc();
				}
			}

			this.state = 191;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 192;
			this.match(CHeaderParser.UNION);
			this.state = 193;
			this.id();
			this.state = 194;
			this.block();
			this.state = 195;
			_localctx._name = this.id();
			this.state = 196;
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
	public enumDef(): EnumDefContext {
		let _localctx: EnumDefContext = new EnumDefContext(this._ctx, this.state);
		this.enterRule(_localctx, 18, CHeaderParser.RULE_enumDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 199;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 198;
				this.doc();
				}
			}

			this.state = 201;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 202;
			this.match(CHeaderParser.ENUM);
			this.state = 203;
			this.id();
			this.state = 204;
			this.enumBody();
			this.state = 205;
			_localctx._name = this.id();
			this.state = 206;
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
	public structDecl(): StructDeclContext {
		let _localctx: StructDeclContext = new StructDeclContext(this._ctx, this.state);
		this.enterRule(_localctx, 20, CHeaderParser.RULE_structDecl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 211;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0)) {
				{
				{
				this.state = 208;
				this.attribute();
				}
				}
				this.state = 213;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 214;
			this.match(CHeaderParser.STRUCT);
			this.state = 215;
			this.id();
			this.state = 216;
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
	public structDef(): StructDefContext {
		let _localctx: StructDefContext = new StructDefContext(this._ctx, this.state);
		this.enterRule(_localctx, 22, CHeaderParser.RULE_structDef);
		let _la: number;
		try {
			this.state = 236;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 23, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 219;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
					{
					this.state = 218;
					this.doc();
					}
				}

				this.state = 221;
				this.match(CHeaderParser.STRUCT);
				this.state = 222;
				_localctx._name = this.id();
				this.state = 223;
				this.structBody();
				this.state = 224;
				this.match(CHeaderParser.SEMI);
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 227;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
					{
					this.state = 226;
					this.doc();
					}
				}

				this.state = 229;
				this.match(CHeaderParser.TYPEDEF);
				this.state = 230;
				this.match(CHeaderParser.STRUCT);
				this.state = 231;
				this.id();
				this.state = 232;
				this.structBody();
				this.state = 233;
				_localctx._name = this.id();
				this.state = 234;
				this.match(CHeaderParser.SEMI);
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
	public callbackDef(): CallbackDefContext {
		let _localctx: CallbackDefContext = new CallbackDefContext(this._ctx, this.state);
		this.enterRule(_localctx, 24, CHeaderParser.RULE_callbackDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 239;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 238;
				this.doc();
				}
			}

			this.state = 241;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 242;
			this.type();
			this.state = 243;
			this.match(CHeaderParser.ROUND_B);
			this.state = 244;
			this.match(CHeaderParser.STAR);
			this.state = 245;
			this.id();
			this.state = 246;
			this.match(CHeaderParser.ROUND_E);
			this.state = 247;
			this.signature();
			this.state = 248;
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
	public compileTimeAssert(): CompileTimeAssertContext {
		let _localctx: CompileTimeAssertContext = new CompileTimeAssertContext(this._ctx, this.state);
		this.enterRule(_localctx, 26, CHeaderParser.RULE_compileTimeAssert);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 250;
			this.match(CHeaderParser.SDL_COMPILE_TIME_ASSERT);
			this.state = 251;
			this.group();
			this.state = 252;
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
	public inline(): InlineContext {
		let _localctx: InlineContext = new InlineContext(this._ctx, this.state);
		this.enterRule(_localctx, 28, CHeaderParser.RULE_inline);
		try {
			this.state = 257;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.SDL_INLINE:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 254;
				this.match(CHeaderParser.SDL_INLINE);
				}
				break;
			case CHeaderParser.STATIC:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 255;
				this.match(CHeaderParser.STATIC);
				this.state = 256;
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
		this.enterRule(_localctx, 30, CHeaderParser.RULE_block);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 259;
			this.match(CHeaderParser.CURLY_B);
			this.state = 263;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.SQUARE_B - 32)) | (1 << (CHeaderParser.COLON - 32)) | (1 << (CHeaderParser.SEMI - 32)) | (1 << (CHeaderParser.COMMA - 32)) | (1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
				{
				{
				this.state = 260;
				this.stm();
				}
				}
				this.state = 265;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 266;
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
		this.enterRule(_localctx, 32, CHeaderParser.RULE_group);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 268;
			this.match(CHeaderParser.ROUND_B);
			this.state = 272;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.SQUARE_B - 32)) | (1 << (CHeaderParser.COLON - 32)) | (1 << (CHeaderParser.SEMI - 32)) | (1 << (CHeaderParser.COMMA - 32)) | (1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
				{
				{
				this.state = 269;
				this.stm();
				}
				}
				this.state = 274;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 275;
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
		this.enterRule(_localctx, 34, CHeaderParser.RULE_indexing);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 277;
			this.match(CHeaderParser.SQUARE_B);
			this.state = 281;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.SQUARE_B - 32)) | (1 << (CHeaderParser.COLON - 32)) | (1 << (CHeaderParser.SEMI - 32)) | (1 << (CHeaderParser.COMMA - 32)) | (1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
				{
				{
				this.state = 278;
				this.stm();
				}
				}
				this.state = 283;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 284;
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
		this.enterRule(_localctx, 36, CHeaderParser.RULE_stm);
		try {
			this.state = 292;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.CURLY_B:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 286;
				this.block();
				}
				break;
			case CHeaderParser.SQUARE_B:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 287;
				this.indexing();
				}
				break;
			case CHeaderParser.DEFINE:
			case CHeaderParser.CONST:
			case CHeaderParser.ENUM:
			case CHeaderParser.STATIC:
			case CHeaderParser.STRUCT:
			case CHeaderParser.UNION:
			case CHeaderParser.VOID:
			case CHeaderParser.SDL_COMPILE_TIME_ASSERT:
			case CHeaderParser.ROUND_B:
			case CHeaderParser.STRING:
			case CHeaderParser.ID:
			case CHeaderParser.NUMBER:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 288;
				this.expr();
				}
				break;
			case CHeaderParser.COLON:
			case CHeaderParser.SEMI:
			case CHeaderParser.COMMA:
			case CHeaderParser.DOT:
			case CHeaderParser.STAR:
			case CHeaderParser.EQ:
			case CHeaderParser.ELLIPSIS:
			case CHeaderParser.PUNCT_EXTRA:
				this.enterOuterAlt(_localctx, 4);
				{
				this.state = 289;
				this.punct();
				}
				break;
			case CHeaderParser.LONG_DOC:
			case CHeaderParser.SHORT_DOC:
				this.enterOuterAlt(_localctx, 5);
				{
				this.state = 290;
				this.doc();
				}
				break;
			case CHeaderParser.TRAILING_DOC:
				this.enterOuterAlt(_localctx, 6);
				{
				this.state = 291;
				this.trailingDoc();
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
	public expr(): ExprContext {
		let _localctx: ExprContext = new ExprContext(this._ctx, this.state);
		this.enterRule(_localctx, 38, CHeaderParser.RULE_expr);
		try {
			this.state = 296;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.ROUND_B:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 294;
				this.group();
				}
				break;
			case CHeaderParser.DEFINE:
			case CHeaderParser.CONST:
			case CHeaderParser.ENUM:
			case CHeaderParser.STATIC:
			case CHeaderParser.STRUCT:
			case CHeaderParser.UNION:
			case CHeaderParser.VOID:
			case CHeaderParser.SDL_COMPILE_TIME_ASSERT:
			case CHeaderParser.STRING:
			case CHeaderParser.ID:
			case CHeaderParser.NUMBER:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 295;
				this.word();
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
		this.enterRule(_localctx, 40, CHeaderParser.RULE_word);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 298;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || ((((_la - 42)) & ~0x1F) === 0 && ((1 << (_la - 42)) & ((1 << (CHeaderParser.STRING - 42)) | (1 << (CHeaderParser.ID - 42)) | (1 << (CHeaderParser.NUMBER - 42)))) !== 0))) {
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
		this.enterRule(_localctx, 42, CHeaderParser.RULE_punct);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 300;
			_la = this._input.LA(1);
			if (!(((((_la - 34)) & ~0x1F) === 0 && ((1 << (_la - 34)) & ((1 << (CHeaderParser.COLON - 34)) | (1 << (CHeaderParser.SEMI - 34)) | (1 << (CHeaderParser.COMMA - 34)) | (1 << (CHeaderParser.DOT - 34)) | (1 << (CHeaderParser.STAR - 34)) | (1 << (CHeaderParser.EQ - 34)) | (1 << (CHeaderParser.ELLIPSIS - 34)) | (1 << (CHeaderParser.PUNCT_EXTRA - 34)))) !== 0))) {
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
	public enumBody(): EnumBodyContext {
		let _localctx: EnumBodyContext = new EnumBodyContext(this._ctx, this.state);
		this.enterRule(_localctx, 44, CHeaderParser.RULE_enumBody);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 302;
			this.match(CHeaderParser.CURLY_B);
			this.state = 306;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC || _la === CHeaderParser.ID) {
				{
				{
				this.state = 303;
				this.enumItem();
				}
				}
				this.state = 308;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 309;
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
	public enumItem(): EnumItemContext {
		let _localctx: EnumItemContext = new EnumItemContext(this._ctx, this.state);
		this.enterRule(_localctx, 46, CHeaderParser.RULE_enumItem);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 312;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 311;
				this.doc();
				}
			}

			this.state = 314;
			this.id();
			this.state = 317;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.EQ) {
				{
				this.state = 315;
				this.match(CHeaderParser.EQ);
				this.state = 316;
				this.expr();
				}
			}

			this.state = 320;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.COMMA) {
				{
				this.state = 319;
				this.match(CHeaderParser.COMMA);
				}
			}

			this.state = 323;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 322;
				this.trailingDoc();
				}
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
	public structBody(): StructBodyContext {
		let _localctx: StructBodyContext = new StructBodyContext(this._ctx, this.state);
		this.enterRule(_localctx, 48, CHeaderParser.RULE_structBody);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 325;
			this.match(CHeaderParser.CURLY_B);
			this.state = 330;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID))) !== 0) || _la === CHeaderParser.ID) {
				{
				this.state = 328;
				this._errHandler.sync(this);
				switch ( this.interpreter.adaptivePredict(this._input, 36, this._ctx) ) {
				case 1:
					{
					this.state = 326;
					this.structItem();
					}
					break;

				case 2:
					{
					this.state = 327;
					this.unionInlineType();
					}
					break;
				}
				}
				this.state = 332;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 333;
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
	public structItem(): StructItemContext {
		let _localctx: StructItemContext = new StructItemContext(this._ctx, this.state);
		this.enterRule(_localctx, 50, CHeaderParser.RULE_structItem);
		try {
			this.state = 337;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 38, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 335;
				this.structVar();
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 336;
				this.structCallback();
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
	public structVar(): StructVarContext {
		let _localctx: StructVarContext = new StructVarContext(this._ctx, this.state);
		this.enterRule(_localctx, 52, CHeaderParser.RULE_structVar);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 340;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 339;
				this.doc();
				}
			}

			this.state = 342;
			this.type();
			this.state = 343;
			this.id();
			this.state = 348;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.COMMA) {
				{
				{
				this.state = 344;
				this.match(CHeaderParser.COMMA);
				this.state = 345;
				this.id();
				}
				}
				this.state = 350;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 354;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.SQUARE_B) {
				{
				{
				this.state = 351;
				this.indexing();
				}
				}
				this.state = 356;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 357;
			this.match(CHeaderParser.SEMI);
			this.state = 359;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 358;
				this.trailingDoc();
				}
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
	public structCallback(): StructCallbackContext {
		let _localctx: StructCallbackContext = new StructCallbackContext(this._ctx, this.state);
		this.enterRule(_localctx, 54, CHeaderParser.RULE_structCallback);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 362;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 361;
				this.doc();
				}
			}

			this.state = 364;
			this.type();
			this.state = 365;
			this.match(CHeaderParser.ROUND_B);
			this.state = 366;
			this.match(CHeaderParser.STAR);
			this.state = 367;
			this.id();
			this.state = 368;
			this.match(CHeaderParser.ROUND_E);
			this.state = 369;
			this.signature();
			this.state = 370;
			this.match(CHeaderParser.SEMI);
			this.state = 372;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 371;
				this.trailingDoc();
				}
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
	public unionInlineType(): UnionInlineTypeContext {
		let _localctx: UnionInlineTypeContext = new UnionInlineTypeContext(this._ctx, this.state);
		this.enterRule(_localctx, 56, CHeaderParser.RULE_unionInlineType);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 375;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 374;
				this.doc();
				}
			}

			this.state = 377;
			this.match(CHeaderParser.UNION);
			this.state = 378;
			this.block();
			this.state = 379;
			this.id();
			this.state = 380;
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
	public id(): IdContext {
		let _localctx: IdContext = new IdContext(this._ctx, this.state);
		this.enterRule(_localctx, 58, CHeaderParser.RULE_id);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 382;
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
		this.enterRule(_localctx, 60, CHeaderParser.RULE_type);
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 385;
			this._errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					this.state = 384;
					this.typeEl();
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				this.state = 387;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 46, this._ctx);
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
		this.enterRule(_localctx, 62, CHeaderParser.RULE_typeEl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 389;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.CONST) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.VOID))) !== 0) || _la === CHeaderParser.ID)) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			this.state = 394;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.SQUARE_B || _la === CHeaderParser.STAR) {
				{
				this.state = 392;
				this._errHandler.sync(this);
				switch (this._input.LA(1)) {
				case CHeaderParser.STAR:
					{
					this.state = 390;
					this.match(CHeaderParser.STAR);
					}
					break;
				case CHeaderParser.SQUARE_B:
					{
					this.state = 391;
					this.indexing();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				this.state = 396;
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
		this.enterRule(_localctx, 64, CHeaderParser.RULE_signature);
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 397;
			this.match(CHeaderParser.ROUND_B);
			this.state = 418;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0) || _la === CHeaderParser.ID) {
				{
				this.state = 401;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0)) {
					{
					{
					this.state = 398;
					this.attribute();
					}
					}
					this.state = 403;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				this.state = 404;
				this.type();
				this.state = 415;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 51, this._ctx);
				while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
					if (_alt === 1) {
						{
						{
						this.state = 405;
						this.match(CHeaderParser.COMMA);
						this.state = 409;
						this._errHandler.sync(this);
						_la = this._input.LA(1);
						while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0)) {
							{
							{
							this.state = 406;
							this.attribute();
							}
							}
							this.state = 411;
							this._errHandler.sync(this);
							_la = this._input.LA(1);
						}
						this.state = 412;
						this.type();
						}
						}
					}
					this.state = 417;
					this._errHandler.sync(this);
					_alt = this.interpreter.adaptivePredict(this._input, 51, this._ctx);
				}
				}
			}

			this.state = 422;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.COMMA) {
				{
				this.state = 420;
				this.match(CHeaderParser.COMMA);
				this.state = 421;
				this.match(CHeaderParser.ELLIPSIS);
				}
			}

			this.state = 424;
			this.match(CHeaderParser.ROUND_E);
			this.state = 428;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0)) {
				{
				{
				this.state = 425;
				this.attribute();
				}
				}
				this.state = 430;
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
	public attribute(): AttributeContext {
		let _localctx: AttributeContext = new AttributeContext(this._ctx, this.state);
		this.enterRule(_localctx, 66, CHeaderParser.RULE_attribute);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 431;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT) | (1 << CHeaderParser.VK_DEFINE_HANDLE))) !== 0))) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			this.state = 432;
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
		this.enterRule(_localctx, 68, CHeaderParser.RULE_doc);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 434;
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
	// @RuleVersion(0)
	public trailingDoc(): TrailingDocContext {
		let _localctx: TrailingDocContext = new TrailingDocContext(this._ctx, this.state);
		this.enterRule(_localctx, 70, CHeaderParser.RULE_trailingDoc);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 436;
			this.match(CHeaderParser.TRAILING_DOC);
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
		"\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03.\u01B9\x04\x02" +
		"\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
		"\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r\t\r\x04" +
		"\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x04\x11\t\x11\x04\x12\t\x12\x04" +
		"\x13\t\x13\x04\x14\t\x14\x04\x15\t\x15\x04\x16\t\x16\x04\x17\t\x17\x04" +
		"\x18\t\x18\x04\x19\t\x19\x04\x1A\t\x1A\x04\x1B\t\x1B\x04\x1C\t\x1C\x04" +
		"\x1D\t\x1D\x04\x1E\t\x1E\x04\x1F\t\x1F\x04 \t \x04!\t!\x04\"\t\"\x04#" +
		"\t#\x04$\t$\x04%\t%\x03\x02\x05\x02L\n\x02\x03\x02\x07\x02O\n\x02\f\x02" +
		"\x0E\x02R\v\x02\x03\x02\x03\x02\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03" +
		"\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x03" +
		"c\n\x03\x03\x04\x05\x04f\n\x04\x03\x04\x03\x04\x03\x05\x03\x05\x03\x05" +
		"\x03\x05\x07\x05n\n\x05\f\x05\x0E\x05q\v\x05\x03\x05\x03\x05\x03\x06\x05" +
		"\x06v\n\x06\x03\x06\x05\x06y\n\x06\x03\x06\x03\x06\x03\x06\x03\x06\x07" +
		"\x06\x7F\n\x06\f\x06\x0E\x06\x82\v\x06\x03\x06\x07\x06\x85\n\x06\f\x06" +
		"\x0E\x06\x88\v\x06\x03\x06\x03\x06\x05\x06\x8C\n\x06\x03\x07\x05\x07\x8F" +
		"\n\x07\x03\x07\x05\x07\x92\n\x07\x03\x07\x07\x07\x95\n\x07\f\x07\x0E\x07" +
		"\x98\v\x07\x03\x07\x03\x07\x07\x07\x9C\n\x07\f\x07\x0E\x07\x9F\v\x07\x03" +
		"\x07\x03\x07\x03\x07\x03\x07\x03\b\x05\b\xA6\n\b\x03\b\x03\b\x03\b\x07" +
		"\b\xAB\n\b\f\b\x0E\b\xAE\v\b\x03\b\x03\b\x03\b\x03\b\x03\t\x05\t\xB5\n" +
		"\t\x03\t\x03\t\x05\t\xB9\n\t\x03\t\x03\t\x03\t\x03\t\x03\n\x05\n\xC0\n" +
		"\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\v\x05\v\xCA\n\v\x03\v" +
		"\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\f\x07\f\xD4\n\f\f\f\x0E\f\xD7" +
		"\v\f\x03\f\x03\f\x03\f\x03\f\x03\r\x05\r\xDE\n\r\x03\r\x03\r\x03\r\x03" +
		"\r\x03\r\x03\r\x05\r\xE6\n\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r" +
		"\x05\r\xEF\n\r\x03\x0E\x05\x0E\xF2\n\x0E\x03\x0E\x03\x0E\x03\x0E\x03\x0E" +
		"\x03\x0E\x03\x0E\x03\x0E\x03\x0E\x03\x0E\x03\x0F\x03\x0F\x03\x0F\x03\x0F" +
		"\x03\x10\x03\x10\x03\x10\x05\x10\u0104\n\x10\x03\x11\x03\x11\x07\x11\u0108" +
		"\n\x11\f\x11\x0E\x11\u010B\v\x11\x03\x11\x03\x11\x03\x12\x03\x12\x07\x12" +
		"\u0111\n\x12\f\x12\x0E\x12\u0114\v\x12\x03\x12\x03\x12\x03\x13\x03\x13" +
		"\x07\x13\u011A\n\x13\f\x13\x0E\x13\u011D\v\x13\x03\x13\x03\x13\x03\x14" +
		"\x03\x14\x03\x14\x03\x14\x03\x14\x03\x14\x05\x14\u0127\n\x14\x03\x15\x03" +
		"\x15\x05\x15\u012B\n\x15\x03\x16\x03\x16\x03\x17\x03\x17\x03\x18\x03\x18" +
		"\x07\x18\u0133\n\x18\f\x18\x0E\x18\u0136\v\x18\x03\x18\x03\x18\x03\x19" +
		"\x05\x19\u013B\n\x19\x03\x19\x03\x19\x03\x19\x05\x19\u0140\n\x19\x03\x19" +
		"\x05\x19\u0143\n\x19\x03\x19\x05\x19\u0146\n\x19\x03\x1A\x03\x1A\x03\x1A" +
		"\x07\x1A\u014B\n\x1A\f\x1A\x0E\x1A\u014E\v\x1A\x03\x1A\x03\x1A\x03\x1B" +
		"\x03\x1B\x05\x1B\u0154\n\x1B\x03\x1C\x05\x1C\u0157\n\x1C\x03\x1C\x03\x1C" +
		"\x03\x1C\x03\x1C\x07\x1C\u015D\n\x1C\f\x1C\x0E\x1C\u0160\v\x1C\x03\x1C" +
		"\x07\x1C\u0163\n\x1C\f\x1C\x0E\x1C\u0166\v\x1C\x03\x1C\x03\x1C\x05\x1C" +
		"\u016A\n\x1C\x03\x1D\x05\x1D\u016D\n\x1D\x03\x1D\x03\x1D\x03\x1D\x03\x1D" +
		"\x03\x1D\x03\x1D\x03\x1D\x03\x1D\x05\x1D\u0177\n\x1D\x03\x1E\x05\x1E\u017A" +
		"\n\x1E\x03\x1E\x03\x1E\x03\x1E\x03\x1E\x03\x1E\x03\x1F\x03\x1F\x03 \x06" +
		" \u0184\n \r \x0E \u0185\x03!\x03!\x03!\x07!\u018B\n!\f!\x0E!\u018E\v" +
		"!\x03\"\x03\"\x07\"\u0192\n\"\f\"\x0E\"\u0195\v\"\x03\"\x03\"\x03\"\x07" +
		"\"\u019A\n\"\f\"\x0E\"\u019D\v\"\x03\"\x07\"\u01A0\n\"\f\"\x0E\"\u01A3" +
		"\v\"\x05\"\u01A5\n\"\x03\"\x03\"\x05\"\u01A9\n\"\x03\"\x03\"\x07\"\u01AD" +
		"\n\"\f\"\x0E\"\u01B0\v\"\x03#\x03#\x03#\x03$\x03$\x03%\x03%\x03%\x02\x02" +
		"\x02&\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E\x02\x10\x02\x12\x02" +
		"\x14\x02\x16\x02\x18\x02\x1A\x02\x1C\x02\x1E\x02 \x02\"\x02$\x02&\x02" +
		"(\x02*\x02,\x02.\x020\x022\x024\x026\x028\x02:\x02<\x02>\x02@\x02B\x02" +
		"D\x02F\x02H\x02\x02\b\x04\x02\x13\x13\x15\x15\b\x02\t\t\f\r\x12\x13\x15" +
		"\x16\x1A\x1A,.\x03\x02$+\x06\x02\f\f\x13\x13\x16\x16--\x05\x02\v\v\x17" +
		"\x19\x1B\x1D\x03\x02\x07\b\x02\u01DA\x02K\x03\x02\x02\x02\x04b\x03\x02" +
		"\x02\x02\x06e\x03\x02\x02\x02\bi\x03\x02\x02\x02\nu\x03\x02\x02\x02\f" +
		"\x8E\x03\x02\x02\x02\x0E\xA5\x03\x02\x02\x02\x10\xB4\x03\x02\x02\x02\x12" +
		"\xBF\x03\x02\x02\x02\x14\xC9\x03\x02\x02\x02\x16\xD5\x03\x02\x02\x02\x18" +
		"\xEE\x03\x02\x02\x02\x1A\xF1\x03\x02\x02\x02\x1C\xFC\x03\x02\x02\x02\x1E" +
		"\u0103\x03\x02\x02\x02 \u0105\x03\x02\x02\x02\"\u010E\x03\x02\x02\x02" +
		"$\u0117\x03\x02\x02\x02&\u0126\x03\x02\x02\x02(\u012A\x03\x02\x02\x02" +
		"*\u012C\x03\x02\x02\x02,\u012E\x03\x02\x02\x02.\u0130\x03\x02\x02\x02" +
		"0\u013A\x03\x02\x02\x022\u0147\x03\x02\x02\x024\u0153\x03\x02\x02\x02" +
		"6\u0156\x03\x02\x02\x028\u016C\x03\x02\x02\x02:\u0179\x03\x02\x02\x02" +
		"<\u0180\x03\x02\x02\x02>\u0183\x03\x02\x02\x02@\u0187\x03\x02\x02\x02" +
		"B\u018F\x03\x02\x02\x02D\u01B1\x03\x02\x02\x02F\u01B4\x03\x02\x02\x02" +
		"H\u01B6\x03\x02\x02\x02JL\x05F$\x02KJ\x03\x02\x02\x02KL\x03\x02\x02\x02" +
		"LP\x03\x02\x02\x02MO\x05\x04\x03\x02NM\x03\x02\x02\x02OR\x03\x02\x02\x02" +
		"PN\x03\x02\x02\x02PQ\x03\x02\x02\x02QS\x03\x02\x02\x02RP\x03\x02\x02\x02" +
		"ST\x07\x02\x02\x03T\x03\x03\x02\x02\x02Uc\x05\x06\x04\x02Vc\x05\b\x05" +
		"\x02Wc\x05\n\x06\x02Xc\x05\f\x07\x02Yc\x05\x0E\b\x02Zc\x05\x10\t\x02[" +
		"c\x05\x12\n\x02\\c\x05\x14\v\x02]c\x05\x16\f\x02^c\x05\x18\r\x02_c\x05" +
		"\x1A\x0E\x02`c\x05\x1C\x0F\x02ac\x05F$\x02bU\x03\x02\x02\x02bV\x03\x02" +
		"\x02\x02bW\x03\x02\x02\x02bX\x03\x02\x02\x02bY\x03\x02\x02\x02bZ\x03\x02" +
		"\x02\x02b[\x03\x02\x02\x02b\\\x03\x02\x02\x02b]\x03\x02\x02\x02b^\x03" +
		"\x02\x02\x02b_\x03\x02\x02\x02b`\x03\x02\x02\x02ba\x03\x02\x02\x02c\x05" +
		"\x03\x02\x02\x02df\x05F$\x02ed\x03\x02\x02\x02ef\x03\x02\x02\x02fg\x03" +
		"\x02\x02\x02gh\x07\t\x02\x02h\x07\x03\x02\x02\x02ij\x07\x0E\x02\x02jk" +
		"\x07,\x02\x02ko\x07\x1E\x02\x02ln\x05\x04\x03\x02ml\x03\x02\x02\x02nq" +
		"\x03\x02\x02\x02om\x03\x02\x02\x02op\x03\x02\x02\x02pr\x03\x02\x02\x02" +
		"qo\x03\x02\x02\x02rs\x07\x1F\x02\x02s\t\x03\x02\x02\x02tv\x05F$\x02ut" +
		"\x03\x02\x02\x02uv\x03\x02\x02\x02vx\x03\x02\x02\x02wy\x07\x0E\x02\x02" +
		"xw\x03\x02\x02\x02xy\x03\x02\x02\x02yz\x03\x02\x02\x02z{\x05> \x02{\x80" +
		"\x05<\x1F\x02|}\x07&\x02\x02}\x7F\x05<\x1F\x02~|\x03\x02\x02\x02\x7F\x82" +
		"\x03\x02\x02\x02\x80~\x03\x02\x02\x02\x80\x81\x03\x02\x02\x02\x81\x86" +
		"\x03\x02\x02\x02\x82\x80\x03\x02\x02\x02\x83\x85\x05$\x13\x02\x84\x83" +
		"\x03\x02\x02\x02\x85\x88\x03\x02\x02\x02\x86\x84\x03\x02\x02\x02\x86\x87" +
		"\x03\x02\x02\x02\x87\x89\x03\x02\x02\x02\x88\x86\x03\x02\x02\x02\x89\x8B" +
		"\x07%\x02\x02\x8A\x8C\x05H%\x02\x8B\x8A\x03\x02\x02\x02\x8B\x8C\x03\x02" +
		"\x02\x02\x8C\v\x03\x02\x02\x02\x8D\x8F\x05F$\x02\x8E\x8D\x03\x02\x02\x02" +
		"\x8E\x8F\x03\x02\x02\x02\x8F\x91\x03\x02\x02\x02\x90\x92\x07\x0E\x02\x02" +
		"\x91\x90\x03\x02\x02\x02\x91\x92\x03\x02\x02\x02\x92\x96\x03\x02\x02\x02" +
		"\x93\x95\x05D#\x02\x94\x93\x03\x02\x02\x02\x95\x98\x03\x02\x02\x02\x96" +
		"\x94\x03\x02\x02\x02\x96\x97\x03\x02\x02\x02\x97\x99\x03\x02\x02\x02\x98" +
		"\x96\x03\x02\x02\x02\x99\x9D\x05> \x02\x9A\x9C\x05D#\x02\x9B\x9A\x03\x02" +
		"\x02\x02\x9C\x9F\x03\x02\x02\x02\x9D\x9B\x03\x02\x02\x02\x9D\x9E\x03\x02" +
		"\x02\x02\x9E\xA0\x03\x02\x02\x02\x9F\x9D\x03\x02\x02\x02\xA0\xA1\x05<" +
		"\x1F\x02\xA1\xA2\x05B\"\x02\xA2\xA3\x07%\x02\x02\xA3\r\x03\x02\x02\x02" +
		"\xA4\xA6\x05F$\x02\xA5\xA4\x03\x02\x02\x02\xA5\xA6\x03\x02\x02\x02\xA6" +
		"\xA7\x03\x02\x02\x02\xA7\xA8\x05\x1E\x10\x02\xA8\xAC\x05> \x02\xA9\xAB" +
		"\x05D#\x02\xAA\xA9\x03\x02\x02\x02\xAB\xAE\x03\x02\x02\x02\xAC\xAA\x03" +
		"\x02\x02\x02\xAC\xAD\x03\x02\x02\x02\xAD\xAF\x03\x02\x02\x02\xAE\xAC\x03" +
		"\x02\x02\x02\xAF\xB0\x05<\x1F\x02\xB0\xB1\x05B\"\x02\xB1\xB2\x05 \x11" +
		"\x02\xB2\x0F\x03\x02\x02\x02\xB3\xB5\x05F$\x02\xB4\xB3\x03\x02\x02\x02" +
		"\xB4\xB5\x03\x02\x02\x02\xB5\xB6\x03\x02\x02\x02\xB6\xB8\x07\x14\x02\x02" +
		"\xB7\xB9\t\x02\x02\x02\xB8\xB7\x03\x02\x02\x02\xB8\xB9\x03\x02\x02\x02" +
		"\xB9\xBA\x03\x02\x02\x02\xBA\xBB\x05> \x02\xBB\xBC\x05<\x1F\x02\xBC\xBD" +
		"\x07%\x02\x02\xBD\x11\x03\x02\x02\x02\xBE\xC0\x05F$\x02\xBF\xBE\x03\x02" +
		"\x02\x02\xBF\xC0\x03\x02\x02\x02\xC0\xC1\x03\x02\x02\x02\xC1\xC2\x07\x14" +
		"\x02\x02\xC2\xC3\x07\x15\x02\x02\xC3\xC4\x05<\x1F\x02\xC4\xC5\x05 \x11" +
		"\x02\xC5\xC6\x05<\x1F\x02\xC6\xC7\x07%\x02\x02\xC7\x13\x03\x02\x02\x02" +
		"\xC8\xCA\x05F$\x02\xC9\xC8\x03\x02\x02\x02\xC9\xCA\x03\x02\x02\x02\xCA" +
		"\xCB\x03\x02\x02\x02\xCB\xCC\x07\x14\x02\x02\xCC\xCD\x07\r\x02\x02\xCD" +
		"\xCE\x05<\x1F\x02\xCE\xCF\x05.\x18\x02\xCF\xD0\x05<\x1F\x02\xD0\xD1\x07" +
		"%\x02\x02\xD1\x15\x03\x02\x02\x02\xD2\xD4\x05D#\x02\xD3\xD2\x03\x02\x02" +
		"\x02\xD4\xD7\x03\x02\x02\x02\xD5\xD3\x03\x02\x02\x02\xD5\xD6\x03\x02\x02" +
		"\x02\xD6\xD8\x03\x02\x02\x02\xD7\xD5\x03\x02\x02\x02\xD8\xD9\x07\x13\x02" +
		"\x02\xD9\xDA\x05<\x1F\x02\xDA\xDB\x07%\x02\x02\xDB\x17\x03\x02\x02\x02" +
		"\xDC\xDE\x05F$\x02\xDD\xDC\x03\x02\x02\x02\xDD\xDE\x03\x02\x02\x02\xDE" +
		"\xDF\x03\x02\x02\x02\xDF\xE0\x07\x13\x02\x02\xE0\xE1\x05<\x1F\x02\xE1" +
		"\xE2\x052\x1A\x02\xE2\xE3\x07%\x02\x02\xE3\xEF\x03\x02\x02\x02\xE4\xE6" +
		"\x05F$\x02\xE5\xE4\x03\x02\x02\x02\xE5\xE6\x03\x02\x02\x02\xE6\xE7\x03" +
		"\x02\x02\x02\xE7\xE8\x07\x14\x02\x02\xE8\xE9\x07\x13\x02\x02\xE9\xEA\x05" +
		"<\x1F\x02\xEA\xEB\x052\x1A\x02\xEB\xEC\x05<\x1F\x02\xEC\xED\x07%\x02\x02" +
		"\xED\xEF\x03\x02\x02\x02\xEE\xDD\x03\x02\x02\x02\xEE\xE5\x03\x02\x02\x02" +
		"\xEF\x19\x03\x02\x02\x02\xF0\xF2\x05F$\x02\xF1\xF0\x03\x02\x02\x02\xF1" +
		"\xF2\x03\x02\x02\x02\xF2\xF3\x03\x02\x02\x02\xF3\xF4\x07\x14\x02\x02\xF4" +
		"\xF5\x05> \x02\xF5\xF6\x07 \x02\x02\xF6\xF7\x07(\x02\x02\xF7\xF8\x05<" +
		"\x1F\x02\xF8\xF9\x07!\x02\x02\xF9\xFA\x05B\"\x02\xFA\xFB\x07%\x02\x02" +
		"\xFB\x1B\x03\x02\x02\x02\xFC\xFD\x07\x1A\x02\x02\xFD\xFE\x05\"\x12\x02" +
		"\xFE\xFF\x07%\x02\x02\xFF\x1D\x03\x02\x02\x02\u0100\u0104\x07\x11\x02" +
		"\x02\u0101\u0102\x07\x12\x02\x02\u0102\u0104\x07\x0F\x02\x02\u0103\u0100" +
		"\x03\x02\x02\x02\u0103\u0101\x03\x02\x02\x02\u0104\x1F\x03\x02\x02\x02" +
		"\u0105\u0109\x07\x1E\x02\x02\u0106\u0108\x05&\x14\x02\u0107\u0106\x03" +
		"\x02\x02\x02\u0108\u010B\x03\x02\x02\x02\u0109\u0107\x03\x02\x02\x02\u0109" +
		"\u010A\x03\x02\x02\x02\u010A\u010C\x03\x02\x02\x02\u010B\u0109\x03\x02" +
		"\x02\x02\u010C\u010D\x07\x1F\x02\x02\u010D!\x03\x02\x02\x02\u010E\u0112" +
		"\x07 \x02\x02\u010F\u0111\x05&\x14\x02\u0110\u010F\x03\x02\x02\x02\u0111" +
		"\u0114\x03\x02\x02\x02\u0112\u0110\x03\x02\x02\x02\u0112\u0113\x03\x02" +
		"\x02\x02\u0113\u0115\x03\x02\x02\x02\u0114\u0112\x03\x02\x02\x02\u0115" +
		"\u0116\x07!\x02\x02\u0116#\x03\x02\x02\x02\u0117\u011B\x07\"\x02\x02\u0118" +
		"\u011A\x05&\x14\x02\u0119\u0118\x03\x02\x02\x02\u011A\u011D\x03\x02\x02" +
		"\x02\u011B\u0119\x03\x02\x02\x02\u011B\u011C\x03\x02\x02\x02\u011C\u011E" +
		"\x03\x02\x02\x02\u011D\u011B\x03\x02\x02\x02\u011E\u011F\x07#\x02\x02" +
		"\u011F%\x03\x02\x02\x02\u0120\u0127\x05 \x11\x02\u0121\u0127\x05$\x13" +
		"\x02\u0122\u0127\x05(\x15\x02\u0123\u0127\x05,\x17\x02\u0124\u0127\x05" +
		"F$\x02\u0125\u0127\x05H%\x02\u0126\u0120\x03\x02\x02\x02\u0126\u0121\x03" +
		"\x02\x02\x02\u0126\u0122\x03\x02\x02\x02\u0126\u0123\x03\x02\x02\x02\u0126" +
		"\u0124\x03\x02\x02\x02\u0126\u0125\x03\x02\x02\x02\u0127\'\x03\x02\x02" +
		"\x02\u0128\u012B\x05\"\x12\x02\u0129\u012B\x05*\x16\x02\u012A\u0128\x03" +
		"\x02\x02\x02\u012A\u0129\x03\x02\x02\x02\u012B)\x03\x02\x02\x02\u012C" +
		"\u012D\t\x03\x02\x02\u012D+\x03\x02\x02\x02\u012E\u012F\t\x04\x02\x02" +
		"\u012F-\x03\x02\x02\x02\u0130\u0134\x07\x1E\x02\x02\u0131\u0133\x050\x19" +
		"\x02\u0132\u0131\x03\x02\x02\x02\u0133\u0136\x03\x02\x02\x02\u0134\u0132" +
		"\x03\x02\x02\x02\u0134\u0135\x03\x02\x02\x02\u0135\u0137\x03\x02\x02\x02" +
		"\u0136\u0134\x03\x02\x02\x02\u0137\u0138\x07\x1F\x02\x02\u0138/\x03\x02" +
		"\x02\x02\u0139\u013B\x05F$\x02\u013A\u0139\x03\x02\x02\x02\u013A\u013B" +
		"\x03\x02\x02\x02\u013B\u013C\x03\x02\x02\x02\u013C\u013F\x05<\x1F\x02" +
		"\u013D\u013E\x07)\x02\x02\u013E\u0140\x05(\x15\x02\u013F\u013D\x03\x02" +
		"\x02\x02\u013F\u0140\x03\x02\x02\x02\u0140\u0142\x03\x02\x02\x02\u0141" +
		"\u0143\x07&\x02\x02\u0142\u0141\x03\x02\x02\x02\u0142\u0143\x03\x02\x02" +
		"\x02\u0143\u0145\x03\x02\x02\x02\u0144\u0146\x05H%\x02\u0145\u0144\x03" +
		"\x02\x02\x02\u0145\u0146\x03\x02\x02\x02\u01461\x03\x02\x02\x02\u0147" +
		"\u014C\x07\x1E\x02\x02\u0148\u014B\x054\x1B\x02\u0149\u014B\x05:\x1E\x02" +
		"\u014A\u0148\x03\x02\x02\x02\u014A\u0149\x03\x02\x02\x02\u014B\u014E\x03" +
		"\x02\x02\x02\u014C\u014A\x03\x02\x02\x02\u014C\u014D\x03\x02\x02\x02\u014D" +
		"\u014F\x03\x02\x02\x02\u014E\u014C\x03\x02\x02\x02\u014F\u0150\x07\x1F" +
		"\x02\x02\u01503\x03\x02\x02\x02\u0151\u0154\x056\x1C\x02\u0152\u0154\x05" +
		"8\x1D\x02\u0153\u0151\x03\x02\x02\x02\u0153\u0152\x03\x02\x02\x02\u0154" +
		"5\x03\x02\x02\x02\u0155\u0157\x05F$\x02\u0156\u0155\x03\x02\x02\x02\u0156" +
		"\u0157\x03\x02\x02\x02\u0157\u0158\x03\x02\x02\x02\u0158\u0159\x05> \x02" +
		"\u0159\u015E\x05<\x1F\x02\u015A\u015B\x07&\x02\x02\u015B\u015D\x05<\x1F" +
		"\x02\u015C\u015A\x03\x02\x02\x02\u015D\u0160\x03\x02\x02\x02\u015E\u015C" +
		"\x03\x02\x02\x02\u015E\u015F\x03\x02\x02\x02\u015F\u0164\x03\x02\x02\x02" +
		"\u0160\u015E\x03\x02\x02\x02\u0161\u0163\x05$\x13\x02\u0162\u0161\x03" +
		"\x02\x02\x02\u0163\u0166\x03\x02\x02\x02\u0164\u0162\x03\x02\x02\x02\u0164" +
		"\u0165\x03\x02\x02\x02\u0165\u0167\x03\x02\x02\x02\u0166\u0164\x03\x02" +
		"\x02\x02\u0167\u0169\x07%\x02\x02\u0168\u016A\x05H%\x02\u0169\u0168\x03" +
		"\x02\x02\x02\u0169\u016A\x03\x02\x02\x02\u016A7\x03\x02\x02\x02\u016B" +
		"\u016D\x05F$\x02\u016C\u016B\x03\x02\x02\x02\u016C\u016D\x03\x02\x02\x02" +
		"\u016D\u016E\x03\x02\x02\x02\u016E\u016F\x05> \x02\u016F\u0170\x07 \x02" +
		"\x02\u0170\u0171\x07(\x02\x02\u0171\u0172\x05<\x1F\x02\u0172\u0173\x07" +
		"!\x02\x02\u0173\u0174\x05B\"\x02\u0174\u0176\x07%\x02\x02\u0175\u0177" +
		"\x05H%\x02\u0176\u0175\x03\x02\x02\x02\u0176\u0177\x03\x02\x02\x02\u0177" +
		"9\x03\x02\x02\x02\u0178\u017A\x05F$\x02\u0179\u0178\x03\x02\x02\x02\u0179" +
		"\u017A\x03\x02\x02\x02\u017A\u017B\x03\x02\x02\x02\u017B\u017C\x07\x15" +
		"\x02\x02\u017C\u017D\x05 \x11\x02\u017D\u017E\x05<\x1F\x02\u017E\u017F" +
		"\x07%\x02\x02\u017F;\x03\x02\x02\x02\u0180\u0181\x07-\x02\x02\u0181=\x03" +
		"\x02\x02\x02\u0182\u0184\x05@!\x02\u0183\u0182\x03\x02\x02\x02\u0184\u0185" +
		"\x03\x02\x02\x02\u0185\u0183\x03\x02\x02\x02\u0185\u0186\x03\x02\x02\x02" +
		"\u0186?\x03\x02\x02\x02\u0187\u018C\t\x05\x02\x02\u0188\u018B\x07(\x02" +
		"\x02\u0189\u018B\x05$\x13\x02\u018A\u0188\x03\x02\x02\x02\u018A\u0189" +
		"\x03\x02\x02\x02\u018B\u018E\x03\x02\x02\x02\u018C\u018A\x03\x02\x02\x02" +
		"\u018C\u018D\x03\x02\x02\x02\u018DA\x03\x02\x02\x02\u018E\u018C\x03\x02" +
		"\x02\x02\u018F\u01A4\x07 \x02\x02\u0190\u0192\x05D#\x02\u0191\u0190\x03" +
		"\x02\x02\x02\u0192\u0195\x03\x02\x02\x02\u0193\u0191\x03\x02\x02\x02\u0193" +
		"\u0194\x03\x02\x02\x02\u0194\u0196\x03\x02\x02\x02\u0195\u0193\x03\x02" +
		"\x02\x02\u0196\u01A1\x05> \x02\u0197\u019B\x07&\x02\x02\u0198\u019A\x05" +
		"D#\x02\u0199\u0198\x03\x02\x02\x02\u019A\u019D\x03\x02\x02\x02\u019B\u0199" +
		"\x03\x02\x02\x02\u019B\u019C\x03\x02\x02\x02\u019C\u019E\x03\x02\x02\x02" +
		"\u019D\u019B\x03\x02\x02\x02\u019E\u01A0\x05> \x02\u019F\u0197\x03\x02" +
		"\x02\x02\u01A0\u01A3\x03\x02\x02\x02\u01A1\u019F\x03\x02\x02\x02\u01A1" +
		"\u01A2\x03\x02\x02\x02\u01A2\u01A5\x03\x02\x02\x02\u01A3\u01A1\x03\x02" +
		"\x02\x02\u01A4\u0193\x03\x02\x02\x02\u01A4\u01A5\x03\x02\x02\x02\u01A5" +
		"\u01A8\x03\x02\x02\x02\u01A6\u01A7\x07&\x02\x02\u01A7\u01A9\x07*\x02\x02" +
		"\u01A8\u01A6\x03\x02\x02\x02\u01A8\u01A9\x03\x02\x02\x02\u01A9\u01AA\x03" +
		"\x02\x02\x02\u01AA\u01AE\x07!\x02\x02\u01AB\u01AD\x05D#\x02\u01AC\u01AB" +
		"\x03\x02\x02\x02\u01AD\u01B0\x03\x02\x02\x02\u01AE\u01AC\x03\x02\x02\x02" +
		"\u01AE\u01AF\x03\x02\x02\x02\u01AFC\x03\x02\x02\x02\u01B0\u01AE\x03\x02" +
		"\x02\x02\u01B1\u01B2\t\x06\x02\x02\u01B2\u01B3\x05\"\x12\x02\u01B3E\x03" +
		"\x02\x02\x02\u01B4\u01B5\t\x07\x02\x02\u01B5G\x03\x02\x02\x02\u01B6\u01B7" +
		"\x07\x06\x02\x02\u01B7I\x03\x02\x02\x029KPbeoux\x80\x86\x8B\x8E\x91\x96" +
		"\x9D\xA5\xAC\xB4\xB8\xBF\xC9\xD5\xDD\xE5\xEE\xF1\u0103\u0109\u0112\u011B" +
		"\u0126\u012A\u0134\u013A\u013F\u0142\u0145\u014A\u014C\u0153\u0156\u015E" +
		"\u0164\u0169\u016C\u0176\u0179\u0185\u018A\u018C\u0193\u019B\u01A1\u01A4" +
		"\u01A8\u01AE";
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
	public globalVar(): GlobalVarContext | undefined {
		return this.tryGetRuleContext(0, GlobalVarContext);
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
	public unionDef(): UnionDefContext | undefined {
		return this.tryGetRuleContext(0, UnionDefContext);
	}
	public enumDef(): EnumDefContext | undefined {
		return this.tryGetRuleContext(0, EnumDefContext);
	}
	public structDecl(): StructDeclContext | undefined {
		return this.tryGetRuleContext(0, StructDeclContext);
	}
	public structDef(): StructDefContext | undefined {
		return this.tryGetRuleContext(0, StructDefContext);
	}
	public callbackDef(): CallbackDefContext | undefined {
		return this.tryGetRuleContext(0, CallbackDefContext);
	}
	public compileTimeAssert(): CompileTimeAssertContext | undefined {
		return this.tryGetRuleContext(0, CompileTimeAssertContext);
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


export class DirectiveContext extends ParserRuleContext {
	public DEFINE(): TerminalNode { return this.getToken(CHeaderParser.DEFINE, 0); }
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


export class GlobalVarContext extends ParserRuleContext {
	public type(): TypeContext {
		return this.getRuleContext(0, TypeContext);
	}
	public id(): IdContext[];
	public id(i: number): IdContext;
	public id(i?: number): IdContext | IdContext[] {
		if (i === undefined) {
			return this.getRuleContexts(IdContext);
		} else {
			return this.getRuleContext(i, IdContext);
		}
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public EXTERN(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.EXTERN, 0); }
	public COMMA(): TerminalNode[];
	public COMMA(i: number): TerminalNode;
	public COMMA(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(CHeaderParser.COMMA);
		} else {
			return this.getToken(CHeaderParser.COMMA, i);
		}
	}
	public indexing(): IndexingContext[];
	public indexing(i: number): IndexingContext;
	public indexing(i?: number): IndexingContext | IndexingContext[] {
		if (i === undefined) {
			return this.getRuleContexts(IndexingContext);
		} else {
			return this.getRuleContext(i, IndexingContext);
		}
	}
	public trailingDoc(): TrailingDocContext | undefined {
		return this.tryGetRuleContext(0, TrailingDocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_globalVar; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterGlobalVar) {
			listener.enterGlobalVar(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitGlobalVar) {
			listener.exitGlobalVar(this);
		}
	}
}


export class FunctionDeclContext extends ParserRuleContext {
	public type(): TypeContext {
		return this.getRuleContext(0, TypeContext);
	}
	public id(): IdContext {
		return this.getRuleContext(0, IdContext);
	}
	public signature(): SignatureContext {
		return this.getRuleContext(0, SignatureContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public EXTERN(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.EXTERN, 0); }
	public attribute(): AttributeContext[];
	public attribute(i: number): AttributeContext;
	public attribute(i?: number): AttributeContext | AttributeContext[] {
		if (i === undefined) {
			return this.getRuleContexts(AttributeContext);
		} else {
			return this.getRuleContext(i, AttributeContext);
		}
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
	public id(): IdContext {
		return this.getRuleContext(0, IdContext);
	}
	public signature(): SignatureContext {
		return this.getRuleContext(0, SignatureContext);
	}
	public block(): BlockContext {
		return this.getRuleContext(0, BlockContext);
	}
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public attribute(): AttributeContext[];
	public attribute(i: number): AttributeContext;
	public attribute(i?: number): AttributeContext | AttributeContext[] {
		if (i === undefined) {
			return this.getRuleContexts(AttributeContext);
		} else {
			return this.getRuleContext(i, AttributeContext);
		}
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
	public id(): IdContext {
		return this.getRuleContext(0, IdContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public UNION(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.UNION, 0); }
	public STRUCT(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STRUCT, 0); }
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


export class UnionDefContext extends ParserRuleContext {
	public _name!: IdContext;
	public TYPEDEF(): TerminalNode { return this.getToken(CHeaderParser.TYPEDEF, 0); }
	public UNION(): TerminalNode { return this.getToken(CHeaderParser.UNION, 0); }
	public id(): IdContext[];
	public id(i: number): IdContext;
	public id(i?: number): IdContext | IdContext[] {
		if (i === undefined) {
			return this.getRuleContexts(IdContext);
		} else {
			return this.getRuleContext(i, IdContext);
		}
	}
	public block(): BlockContext {
		return this.getRuleContext(0, BlockContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_unionDef; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterUnionDef) {
			listener.enterUnionDef(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitUnionDef) {
			listener.exitUnionDef(this);
		}
	}
}


export class EnumDefContext extends ParserRuleContext {
	public _name!: IdContext;
	public TYPEDEF(): TerminalNode { return this.getToken(CHeaderParser.TYPEDEF, 0); }
	public ENUM(): TerminalNode { return this.getToken(CHeaderParser.ENUM, 0); }
	public id(): IdContext[];
	public id(i: number): IdContext;
	public id(i?: number): IdContext | IdContext[] {
		if (i === undefined) {
			return this.getRuleContexts(IdContext);
		} else {
			return this.getRuleContext(i, IdContext);
		}
	}
	public enumBody(): EnumBodyContext {
		return this.getRuleContext(0, EnumBodyContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_enumDef; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterEnumDef) {
			listener.enterEnumDef(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitEnumDef) {
			listener.exitEnumDef(this);
		}
	}
}


export class StructDeclContext extends ParserRuleContext {
	public STRUCT(): TerminalNode { return this.getToken(CHeaderParser.STRUCT, 0); }
	public id(): IdContext {
		return this.getRuleContext(0, IdContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public attribute(): AttributeContext[];
	public attribute(i: number): AttributeContext;
	public attribute(i?: number): AttributeContext | AttributeContext[] {
		if (i === undefined) {
			return this.getRuleContexts(AttributeContext);
		} else {
			return this.getRuleContext(i, AttributeContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_structDecl; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterStructDecl) {
			listener.enterStructDecl(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitStructDecl) {
			listener.exitStructDecl(this);
		}
	}
}


export class StructDefContext extends ParserRuleContext {
	public _name!: IdContext;
	public STRUCT(): TerminalNode { return this.getToken(CHeaderParser.STRUCT, 0); }
	public structBody(): StructBodyContext {
		return this.getRuleContext(0, StructBodyContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public id(): IdContext[];
	public id(i: number): IdContext;
	public id(i?: number): IdContext | IdContext[] {
		if (i === undefined) {
			return this.getRuleContexts(IdContext);
		} else {
			return this.getRuleContext(i, IdContext);
		}
	}
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public TYPEDEF(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.TYPEDEF, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_structDef; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterStructDef) {
			listener.enterStructDef(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitStructDef) {
			listener.exitStructDef(this);
		}
	}
}


export class CallbackDefContext extends ParserRuleContext {
	public TYPEDEF(): TerminalNode { return this.getToken(CHeaderParser.TYPEDEF, 0); }
	public type(): TypeContext {
		return this.getRuleContext(0, TypeContext);
	}
	public ROUND_B(): TerminalNode { return this.getToken(CHeaderParser.ROUND_B, 0); }
	public STAR(): TerminalNode { return this.getToken(CHeaderParser.STAR, 0); }
	public id(): IdContext {
		return this.getRuleContext(0, IdContext);
	}
	public ROUND_E(): TerminalNode { return this.getToken(CHeaderParser.ROUND_E, 0); }
	public signature(): SignatureContext {
		return this.getRuleContext(0, SignatureContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_callbackDef; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterCallbackDef) {
			listener.enterCallbackDef(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitCallbackDef) {
			listener.exitCallbackDef(this);
		}
	}
}


export class CompileTimeAssertContext extends ParserRuleContext {
	public SDL_COMPILE_TIME_ASSERT(): TerminalNode { return this.getToken(CHeaderParser.SDL_COMPILE_TIME_ASSERT, 0); }
	public group(): GroupContext {
		return this.getRuleContext(0, GroupContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_compileTimeAssert; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterCompileTimeAssert) {
			listener.enterCompileTimeAssert(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitCompileTimeAssert) {
			listener.exitCompileTimeAssert(this);
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
	public indexing(): IndexingContext | undefined {
		return this.tryGetRuleContext(0, IndexingContext);
	}
	public expr(): ExprContext | undefined {
		return this.tryGetRuleContext(0, ExprContext);
	}
	public punct(): PunctContext | undefined {
		return this.tryGetRuleContext(0, PunctContext);
	}
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public trailingDoc(): TrailingDocContext | undefined {
		return this.tryGetRuleContext(0, TrailingDocContext);
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


export class ExprContext extends ParserRuleContext {
	public group(): GroupContext | undefined {
		return this.tryGetRuleContext(0, GroupContext);
	}
	public word(): WordContext | undefined {
		return this.tryGetRuleContext(0, WordContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_expr; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterExpr) {
			listener.enterExpr(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitExpr) {
			listener.exitExpr(this);
		}
	}
}


export class WordContext extends ParserRuleContext {
	public ID(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.ID, 0); }
	public CONST(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.CONST, 0); }
	public VOID(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.VOID, 0); }
	public STATIC(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STATIC, 0); }
	public ENUM(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.ENUM, 0); }
	public STRUCT(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STRUCT, 0); }
	public UNION(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.UNION, 0); }
	public NUMBER(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.NUMBER, 0); }
	public STRING(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STRING, 0); }
	public DEFINE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.DEFINE, 0); }
	public SDL_COMPILE_TIME_ASSERT(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.SDL_COMPILE_TIME_ASSERT, 0); }
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
	public EQ(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.EQ, 0); }
	public ELLIPSIS(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.ELLIPSIS, 0); }
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


export class EnumBodyContext extends ParserRuleContext {
	public CURLY_B(): TerminalNode { return this.getToken(CHeaderParser.CURLY_B, 0); }
	public CURLY_E(): TerminalNode { return this.getToken(CHeaderParser.CURLY_E, 0); }
	public enumItem(): EnumItemContext[];
	public enumItem(i: number): EnumItemContext;
	public enumItem(i?: number): EnumItemContext | EnumItemContext[] {
		if (i === undefined) {
			return this.getRuleContexts(EnumItemContext);
		} else {
			return this.getRuleContext(i, EnumItemContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_enumBody; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterEnumBody) {
			listener.enterEnumBody(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitEnumBody) {
			listener.exitEnumBody(this);
		}
	}
}


export class EnumItemContext extends ParserRuleContext {
	public id(): IdContext {
		return this.getRuleContext(0, IdContext);
	}
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public EQ(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.EQ, 0); }
	public expr(): ExprContext | undefined {
		return this.tryGetRuleContext(0, ExprContext);
	}
	public COMMA(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.COMMA, 0); }
	public trailingDoc(): TrailingDocContext | undefined {
		return this.tryGetRuleContext(0, TrailingDocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_enumItem; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterEnumItem) {
			listener.enterEnumItem(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitEnumItem) {
			listener.exitEnumItem(this);
		}
	}
}


export class StructBodyContext extends ParserRuleContext {
	public CURLY_B(): TerminalNode { return this.getToken(CHeaderParser.CURLY_B, 0); }
	public CURLY_E(): TerminalNode { return this.getToken(CHeaderParser.CURLY_E, 0); }
	public structItem(): StructItemContext[];
	public structItem(i: number): StructItemContext;
	public structItem(i?: number): StructItemContext | StructItemContext[] {
		if (i === undefined) {
			return this.getRuleContexts(StructItemContext);
		} else {
			return this.getRuleContext(i, StructItemContext);
		}
	}
	public unionInlineType(): UnionInlineTypeContext[];
	public unionInlineType(i: number): UnionInlineTypeContext;
	public unionInlineType(i?: number): UnionInlineTypeContext | UnionInlineTypeContext[] {
		if (i === undefined) {
			return this.getRuleContexts(UnionInlineTypeContext);
		} else {
			return this.getRuleContext(i, UnionInlineTypeContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_structBody; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterStructBody) {
			listener.enterStructBody(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitStructBody) {
			listener.exitStructBody(this);
		}
	}
}


export class StructItemContext extends ParserRuleContext {
	public structVar(): StructVarContext | undefined {
		return this.tryGetRuleContext(0, StructVarContext);
	}
	public structCallback(): StructCallbackContext | undefined {
		return this.tryGetRuleContext(0, StructCallbackContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_structItem; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterStructItem) {
			listener.enterStructItem(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitStructItem) {
			listener.exitStructItem(this);
		}
	}
}


export class StructVarContext extends ParserRuleContext {
	public type(): TypeContext {
		return this.getRuleContext(0, TypeContext);
	}
	public id(): IdContext[];
	public id(i: number): IdContext;
	public id(i?: number): IdContext | IdContext[] {
		if (i === undefined) {
			return this.getRuleContexts(IdContext);
		} else {
			return this.getRuleContext(i, IdContext);
		}
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
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
	public indexing(): IndexingContext[];
	public indexing(i: number): IndexingContext;
	public indexing(i?: number): IndexingContext | IndexingContext[] {
		if (i === undefined) {
			return this.getRuleContexts(IndexingContext);
		} else {
			return this.getRuleContext(i, IndexingContext);
		}
	}
	public trailingDoc(): TrailingDocContext | undefined {
		return this.tryGetRuleContext(0, TrailingDocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_structVar; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterStructVar) {
			listener.enterStructVar(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitStructVar) {
			listener.exitStructVar(this);
		}
	}
}


export class StructCallbackContext extends ParserRuleContext {
	public type(): TypeContext {
		return this.getRuleContext(0, TypeContext);
	}
	public ROUND_B(): TerminalNode { return this.getToken(CHeaderParser.ROUND_B, 0); }
	public STAR(): TerminalNode { return this.getToken(CHeaderParser.STAR, 0); }
	public id(): IdContext {
		return this.getRuleContext(0, IdContext);
	}
	public ROUND_E(): TerminalNode { return this.getToken(CHeaderParser.ROUND_E, 0); }
	public signature(): SignatureContext {
		return this.getRuleContext(0, SignatureContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	public trailingDoc(): TrailingDocContext | undefined {
		return this.tryGetRuleContext(0, TrailingDocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_structCallback; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterStructCallback) {
			listener.enterStructCallback(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitStructCallback) {
			listener.exitStructCallback(this);
		}
	}
}


export class UnionInlineTypeContext extends ParserRuleContext {
	public UNION(): TerminalNode { return this.getToken(CHeaderParser.UNION, 0); }
	public block(): BlockContext {
		return this.getRuleContext(0, BlockContext);
	}
	public id(): IdContext {
		return this.getRuleContext(0, IdContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_unionInlineType; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterUnionInlineType) {
			listener.enterUnionInlineType(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitUnionInlineType) {
			listener.exitUnionInlineType(this);
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
	public CONST(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.CONST, 0); }
	public STRUCT(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STRUCT, 0); }
	public STAR(): TerminalNode[];
	public STAR(i: number): TerminalNode;
	public STAR(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(CHeaderParser.STAR);
		} else {
			return this.getToken(CHeaderParser.STAR, i);
		}
	}
	public indexing(): IndexingContext[];
	public indexing(i: number): IndexingContext;
	public indexing(i?: number): IndexingContext | IndexingContext[] {
		if (i === undefined) {
			return this.getRuleContexts(IndexingContext);
		} else {
			return this.getRuleContext(i, IndexingContext);
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
	public ELLIPSIS(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.ELLIPSIS, 0); }
	public attribute(): AttributeContext[];
	public attribute(i: number): AttributeContext;
	public attribute(i?: number): AttributeContext | AttributeContext[] {
		if (i === undefined) {
			return this.getRuleContexts(AttributeContext);
		} else {
			return this.getRuleContext(i, AttributeContext);
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
	public group(): GroupContext {
		return this.getRuleContext(0, GroupContext);
	}
	public ATTRIBUTE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.ATTRIBUTE, 0); }
	public SDL_VARARG_ATTRIB(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.SDL_VARARG_ATTRIB, 0); }
	public SDL_ACQUIRE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.SDL_ACQUIRE, 0); }
	public SDL_RELEASE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.SDL_RELEASE, 0); }
	public SDL_ALLOC_SIZE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.SDL_ALLOC_SIZE, 0); }
	public SDL_INOUT(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.SDL_INOUT, 0); }
	public VK_DEFINE_HANDLE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.VK_DEFINE_HANDLE, 0); }
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


export class TrailingDocContext extends ParserRuleContext {
	public TRAILING_DOC(): TerminalNode { return this.getToken(CHeaderParser.TRAILING_DOC, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public get ruleIndex(): number { return CHeaderParser.RULE_trailingDoc; }
	// @Override
	public enterRule(listener: CHeaderListener): void {
		if (listener.enterTrailingDoc) {
			listener.enterTrailingDoc(this);
		}
	}
	// @Override
	public exitRule(listener: CHeaderListener): void {
		if (listener.exitTrailingDoc) {
			listener.exitTrailingDoc(this);
		}
	}
}


