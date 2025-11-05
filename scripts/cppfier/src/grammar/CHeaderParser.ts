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
	public static readonly CURLY_B = 25;
	public static readonly CURLY_E = 26;
	public static readonly ROUND_B = 27;
	public static readonly ROUND_E = 28;
	public static readonly SQUARE_B = 29;
	public static readonly SQUARE_E = 30;
	public static readonly COLON = 31;
	public static readonly SEMI = 32;
	public static readonly COMMA = 33;
	public static readonly DOT = 34;
	public static readonly STAR = 35;
	public static readonly EQ = 36;
	public static readonly ELLIPSIS = 37;
	public static readonly PUNCT_EXTRA = 38;
	public static readonly STRING = 39;
	public static readonly ID = 40;
	public static readonly NUMBER = 41;
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
	public static readonly RULE_structDef = 10;
	public static readonly RULE_callbackDef = 11;
	public static readonly RULE_compileTimeAssert = 12;
	public static readonly RULE_inline = 13;
	public static readonly RULE_block = 14;
	public static readonly RULE_group = 15;
	public static readonly RULE_indexing = 16;
	public static readonly RULE_stm = 17;
	public static readonly RULE_expr = 18;
	public static readonly RULE_word = 19;
	public static readonly RULE_punct = 20;
	public static readonly RULE_enumBody = 21;
	public static readonly RULE_enumItem = 22;
	public static readonly RULE_structBody = 23;
	public static readonly RULE_structItem = 24;
	public static readonly RULE_structVar = 25;
	public static readonly RULE_structCallback = 26;
	public static readonly RULE_unionInlineType = 27;
	public static readonly RULE_id = 28;
	public static readonly RULE_type = 29;
	public static readonly RULE_typeEl = 30;
	public static readonly RULE_signature = 31;
	public static readonly RULE_attribute = 32;
	public static readonly RULE_doc = 33;
	public static readonly RULE_trailingDoc = 34;
	// tslint:disable:no-trailing-whitespace
	public static readonly ruleNames: string[] = [
		"prog", "decl", "directive", "externC", "globalVar", "functionDecl", "functionDef", 
		"aliasDef", "unionDef", "enumDef", "structDef", "callbackDef", "compileTimeAssert", 
		"inline", "block", "group", "indexing", "stm", "expr", "word", "punct", 
		"enumBody", "enumItem", "structBody", "structItem", "structVar", "structCallback", 
		"unionInlineType", "id", "type", "typeEl", "signature", "attribute", "doc", 
		"trailingDoc",
	];

	private static readonly _LITERAL_NAMES: Array<string | undefined> = [
		undefined, undefined, undefined, undefined, undefined, undefined, undefined, 
		undefined, undefined, "'__attribute__'", "'const'", "'enum'", "'extern'", 
		"'__inline__'", undefined, "'SDL_FORCE_INLINE'", "'static'", "'struct'", 
		"'typedef'", "'union'", "'void'", undefined, undefined, undefined, "'SDL_COMPILE_TIME_ASSERT'", 
		"'{'", "'}'", "'('", "')'", "'['", "']'", "':'", "';'", "','", "'.'", 
		"'*'", "'='", "'...'",
	];
	private static readonly _SYMBOLIC_NAMES: Array<string | undefined> = [
		undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "TRAILING_DOC", "LONG_DOC", 
		"SHORT_DOC", "DEFINE", "DIRECTIVE", "ATTRIBUTE", "CONST", "ENUM", "EXTERN", 
		"INLINE", "SDL_NOISE", "SDL_INLINE", "STATIC", "STRUCT", "TYPEDEF", "UNION", 
		"VOID", "SDL_VARARG_ATTRIB", "SDL_ACQUIRE", "SDL_RELEASE", "SDL_COMPILE_TIME_ASSERT", 
		"CURLY_B", "CURLY_E", "ROUND_B", "ROUND_E", "SQUARE_B", "SQUARE_E", "COLON", 
		"SEMI", "COMMA", "DOT", "STAR", "EQ", "ELLIPSIS", "PUNCT_EXTRA", "STRING", 
		"ID", "NUMBER",
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
			this.state = 71;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 0, this._ctx) ) {
			case 1:
				{
				this.state = 70;
				this.doc();
				}
				break;
			}
			this.state = 76;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || _la === CHeaderParser.ID) {
				{
				{
				this.state = 73;
				this.decl();
				}
				}
				this.state = 78;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 79;
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
			this.state = 93;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 2, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 81;
				this.directive();
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 82;
				this.externC();
				}
				break;

			case 3:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 83;
				this.globalVar();
				}
				break;

			case 4:
				this.enterOuterAlt(_localctx, 4);
				{
				this.state = 84;
				this.functionDecl();
				}
				break;

			case 5:
				this.enterOuterAlt(_localctx, 5);
				{
				this.state = 85;
				this.functionDef();
				}
				break;

			case 6:
				this.enterOuterAlt(_localctx, 6);
				{
				this.state = 86;
				this.aliasDef();
				}
				break;

			case 7:
				this.enterOuterAlt(_localctx, 7);
				{
				this.state = 87;
				this.unionDef();
				}
				break;

			case 8:
				this.enterOuterAlt(_localctx, 8);
				{
				this.state = 88;
				this.enumDef();
				}
				break;

			case 9:
				this.enterOuterAlt(_localctx, 9);
				{
				this.state = 89;
				this.structDef();
				}
				break;

			case 10:
				this.enterOuterAlt(_localctx, 10);
				{
				this.state = 90;
				this.callbackDef();
				}
				break;

			case 11:
				this.enterOuterAlt(_localctx, 11);
				{
				this.state = 91;
				this.compileTimeAssert();
				}
				break;

			case 12:
				this.enterOuterAlt(_localctx, 12);
				{
				this.state = 92;
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
			this.state = 96;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 95;
				this.doc();
				}
			}

			this.state = 98;
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
			this.state = 100;
			this.match(CHeaderParser.EXTERN);
			this.state = 101;
			this.match(CHeaderParser.STRING);
			this.state = 102;
			this.match(CHeaderParser.CURLY_B);
			this.state = 106;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || _la === CHeaderParser.ID) {
				{
				{
				this.state = 103;
				this.decl();
				}
				}
				this.state = 108;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 109;
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
			this.state = 112;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 111;
				this.doc();
				}
			}

			this.state = 115;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.EXTERN) {
				{
				this.state = 114;
				this.match(CHeaderParser.EXTERN);
				}
			}

			this.state = 117;
			this.type();
			this.state = 118;
			this.id();
			this.state = 123;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.COMMA) {
				{
				{
				this.state = 119;
				this.match(CHeaderParser.COMMA);
				this.state = 120;
				this.id();
				}
				}
				this.state = 125;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 129;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.SQUARE_B) {
				{
				{
				this.state = 126;
				this.indexing();
				}
				}
				this.state = 131;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 132;
			this.match(CHeaderParser.SEMI);
			this.state = 134;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 133;
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
			this.state = 137;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 136;
				this.doc();
				}
			}

			this.state = 140;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.EXTERN) {
				{
				this.state = 139;
				this.match(CHeaderParser.EXTERN);
				}
			}

			this.state = 142;
			this.type();
			this.state = 144;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE))) !== 0)) {
				{
				this.state = 143;
				this.attribute();
				}
			}

			this.state = 146;
			this.id();
			this.state = 147;
			this.signature();
			this.state = 148;
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
			this.state = 151;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 150;
				this.doc();
				}
			}

			this.state = 153;
			this.inline();
			this.state = 154;
			this.type();
			this.state = 156;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE))) !== 0)) {
				{
				this.state = 155;
				this.attribute();
				}
			}

			this.state = 158;
			this.id();
			this.state = 159;
			this.signature();
			this.state = 160;
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
			this.match(CHeaderParser.TYPEDEF);
			this.state = 167;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.STRUCT || _la === CHeaderParser.UNION) {
				{
				this.state = 166;
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
			}

			this.state = 169;
			this.type();
			this.state = 170;
			this.id();
			this.state = 171;
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
			this.state = 174;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 173;
				this.doc();
				}
			}

			this.state = 176;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 177;
			this.match(CHeaderParser.UNION);
			this.state = 178;
			this.id();
			this.state = 179;
			this.block();
			this.state = 180;
			_localctx._name = this.id();
			this.state = 181;
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
			this.state = 184;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 183;
				this.doc();
				}
			}

			this.state = 186;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 187;
			this.match(CHeaderParser.ENUM);
			this.state = 188;
			this.id();
			this.state = 189;
			this.enumBody();
			this.state = 190;
			_localctx._name = this.id();
			this.state = 191;
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
		this.enterRule(_localctx, 20, CHeaderParser.RULE_structDef);
		let _la: number;
		try {
			this.state = 211;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 21, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 194;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
					{
					this.state = 193;
					this.doc();
					}
				}

				this.state = 196;
				this.match(CHeaderParser.STRUCT);
				this.state = 197;
				_localctx._name = this.id();
				this.state = 198;
				this.structBody();
				this.state = 199;
				this.match(CHeaderParser.SEMI);
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 202;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
					{
					this.state = 201;
					this.doc();
					}
				}

				this.state = 204;
				this.match(CHeaderParser.TYPEDEF);
				this.state = 205;
				this.match(CHeaderParser.STRUCT);
				this.state = 206;
				this.id();
				this.state = 207;
				this.structBody();
				this.state = 208;
				_localctx._name = this.id();
				this.state = 209;
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
		this.enterRule(_localctx, 22, CHeaderParser.RULE_callbackDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 214;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 213;
				this.doc();
				}
			}

			this.state = 216;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 217;
			this.type();
			this.state = 218;
			this.match(CHeaderParser.ROUND_B);
			this.state = 219;
			this.match(CHeaderParser.STAR);
			this.state = 220;
			this.id();
			this.state = 221;
			this.match(CHeaderParser.ROUND_E);
			this.state = 222;
			this.signature();
			this.state = 223;
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
		this.enterRule(_localctx, 24, CHeaderParser.RULE_compileTimeAssert);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 225;
			this.match(CHeaderParser.SDL_COMPILE_TIME_ASSERT);
			this.state = 226;
			this.group();
			this.state = 227;
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
		this.enterRule(_localctx, 26, CHeaderParser.RULE_inline);
		try {
			this.state = 232;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.SDL_INLINE:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 229;
				this.match(CHeaderParser.SDL_INLINE);
				}
				break;
			case CHeaderParser.STATIC:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 230;
				this.match(CHeaderParser.STATIC);
				this.state = 231;
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
		this.enterRule(_localctx, 28, CHeaderParser.RULE_block);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 234;
			this.match(CHeaderParser.CURLY_B);
			this.state = 238;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.SEMI - 32)) | (1 << (CHeaderParser.COMMA - 32)) | (1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
				{
				{
				this.state = 235;
				this.stm();
				}
				}
				this.state = 240;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 241;
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
		this.enterRule(_localctx, 30, CHeaderParser.RULE_group);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 243;
			this.match(CHeaderParser.ROUND_B);
			this.state = 247;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.SEMI - 32)) | (1 << (CHeaderParser.COMMA - 32)) | (1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
				{
				{
				this.state = 244;
				this.stm();
				}
				}
				this.state = 249;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 250;
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
		this.enterRule(_localctx, 32, CHeaderParser.RULE_indexing);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 252;
			this.match(CHeaderParser.SQUARE_B);
			this.state = 256;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.SEMI - 32)) | (1 << (CHeaderParser.COMMA - 32)) | (1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
				{
				{
				this.state = 253;
				this.stm();
				}
				}
				this.state = 258;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 259;
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
		this.enterRule(_localctx, 34, CHeaderParser.RULE_stm);
		try {
			this.state = 267;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.CURLY_B:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 261;
				this.block();
				}
				break;
			case CHeaderParser.SQUARE_B:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 262;
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
				this.state = 263;
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
				this.state = 264;
				this.punct();
				}
				break;
			case CHeaderParser.LONG_DOC:
			case CHeaderParser.SHORT_DOC:
				this.enterOuterAlt(_localctx, 5);
				{
				this.state = 265;
				this.doc();
				}
				break;
			case CHeaderParser.TRAILING_DOC:
				this.enterOuterAlt(_localctx, 6);
				{
				this.state = 266;
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
		this.enterRule(_localctx, 36, CHeaderParser.RULE_expr);
		try {
			this.state = 271;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.ROUND_B:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 269;
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
				this.state = 270;
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
		this.enterRule(_localctx, 38, CHeaderParser.RULE_word);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 273;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || ((((_la - 39)) & ~0x1F) === 0 && ((1 << (_la - 39)) & ((1 << (CHeaderParser.STRING - 39)) | (1 << (CHeaderParser.ID - 39)) | (1 << (CHeaderParser.NUMBER - 39)))) !== 0))) {
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
		this.enterRule(_localctx, 40, CHeaderParser.RULE_punct);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 275;
			_la = this._input.LA(1);
			if (!(((((_la - 31)) & ~0x1F) === 0 && ((1 << (_la - 31)) & ((1 << (CHeaderParser.COLON - 31)) | (1 << (CHeaderParser.SEMI - 31)) | (1 << (CHeaderParser.COMMA - 31)) | (1 << (CHeaderParser.DOT - 31)) | (1 << (CHeaderParser.STAR - 31)) | (1 << (CHeaderParser.EQ - 31)) | (1 << (CHeaderParser.ELLIPSIS - 31)) | (1 << (CHeaderParser.PUNCT_EXTRA - 31)))) !== 0))) {
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
		this.enterRule(_localctx, 42, CHeaderParser.RULE_enumBody);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 277;
			this.match(CHeaderParser.CURLY_B);
			this.state = 281;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC || _la === CHeaderParser.ID) {
				{
				{
				this.state = 278;
				this.enumItem();
				}
				}
				this.state = 283;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 284;
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
		this.enterRule(_localctx, 44, CHeaderParser.RULE_enumItem);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 287;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 286;
				this.doc();
				}
			}

			this.state = 289;
			this.id();
			this.state = 292;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.EQ) {
				{
				this.state = 290;
				this.match(CHeaderParser.EQ);
				this.state = 291;
				this.expr();
				}
			}

			this.state = 295;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.COMMA) {
				{
				this.state = 294;
				this.match(CHeaderParser.COMMA);
				}
			}

			this.state = 298;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 297;
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
		this.enterRule(_localctx, 46, CHeaderParser.RULE_structBody);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 300;
			this.match(CHeaderParser.CURLY_B);
			this.state = 305;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID))) !== 0) || _la === CHeaderParser.ID) {
				{
				this.state = 303;
				this._errHandler.sync(this);
				switch ( this.interpreter.adaptivePredict(this._input, 34, this._ctx) ) {
				case 1:
					{
					this.state = 301;
					this.structItem();
					}
					break;

				case 2:
					{
					this.state = 302;
					this.unionInlineType();
					}
					break;
				}
				}
				this.state = 307;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 308;
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
		this.enterRule(_localctx, 48, CHeaderParser.RULE_structItem);
		try {
			this.state = 312;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 36, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 310;
				this.structVar();
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 311;
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
		this.enterRule(_localctx, 50, CHeaderParser.RULE_structVar);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 315;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 314;
				this.doc();
				}
			}

			this.state = 321;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 39, this._ctx) ) {
			case 1:
				{
				this.state = 318;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if (_la === CHeaderParser.CONST) {
					{
					this.state = 317;
					this.match(CHeaderParser.CONST);
					}
				}

				this.state = 320;
				this.match(CHeaderParser.STRUCT);
				}
				break;
			}
			this.state = 323;
			this.type();
			this.state = 324;
			this.id();
			this.state = 329;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.COMMA) {
				{
				{
				this.state = 325;
				this.match(CHeaderParser.COMMA);
				this.state = 326;
				this.id();
				}
				}
				this.state = 331;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 335;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.SQUARE_B) {
				{
				{
				this.state = 332;
				this.indexing();
				}
				}
				this.state = 337;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 338;
			this.match(CHeaderParser.SEMI);
			this.state = 340;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 339;
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
		this.enterRule(_localctx, 52, CHeaderParser.RULE_structCallback);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 343;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 342;
				this.doc();
				}
			}

			this.state = 345;
			this.type();
			this.state = 346;
			this.match(CHeaderParser.ROUND_B);
			this.state = 347;
			this.match(CHeaderParser.STAR);
			this.state = 348;
			this.id();
			this.state = 349;
			this.match(CHeaderParser.ROUND_E);
			this.state = 350;
			this.signature();
			this.state = 351;
			this.match(CHeaderParser.SEMI);
			this.state = 353;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 352;
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
		this.enterRule(_localctx, 54, CHeaderParser.RULE_unionInlineType);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 356;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 355;
				this.doc();
				}
			}

			this.state = 358;
			this.match(CHeaderParser.UNION);
			this.state = 359;
			this.block();
			this.state = 360;
			this.id();
			this.state = 361;
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
		this.enterRule(_localctx, 56, CHeaderParser.RULE_id);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 363;
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
		this.enterRule(_localctx, 58, CHeaderParser.RULE_type);
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 366;
			this._errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					this.state = 365;
					this.typeEl();
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				this.state = 368;
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
		this.enterRule(_localctx, 60, CHeaderParser.RULE_typeEl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 370;
			_la = this._input.LA(1);
			if (!(((((_la - 10)) & ~0x1F) === 0 && ((1 << (_la - 10)) & ((1 << (CHeaderParser.CONST - 10)) | (1 << (CHeaderParser.VOID - 10)) | (1 << (CHeaderParser.ID - 10)))) !== 0))) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			this.state = 375;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.SQUARE_B || _la === CHeaderParser.STAR) {
				{
				this.state = 373;
				this._errHandler.sync(this);
				switch (this._input.LA(1)) {
				case CHeaderParser.STAR:
					{
					this.state = 371;
					this.match(CHeaderParser.STAR);
					}
					break;
				case CHeaderParser.SQUARE_B:
					{
					this.state = 372;
					this.indexing();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				this.state = 377;
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
		this.enterRule(_localctx, 62, CHeaderParser.RULE_signature);
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 378;
			this.match(CHeaderParser.ROUND_B);
			this.state = 387;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (((((_la - 10)) & ~0x1F) === 0 && ((1 << (_la - 10)) & ((1 << (CHeaderParser.CONST - 10)) | (1 << (CHeaderParser.VOID - 10)) | (1 << (CHeaderParser.ID - 10)))) !== 0)) {
				{
				this.state = 379;
				this.type();
				this.state = 384;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 49, this._ctx);
				while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
					if (_alt === 1) {
						{
						{
						this.state = 380;
						this.match(CHeaderParser.COMMA);
						this.state = 381;
						this.type();
						}
						}
					}
					this.state = 386;
					this._errHandler.sync(this);
					_alt = this.interpreter.adaptivePredict(this._input, 49, this._ctx);
				}
				}
			}

			this.state = 391;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.COMMA) {
				{
				this.state = 389;
				this.match(CHeaderParser.COMMA);
				this.state = 390;
				this.match(CHeaderParser.ELLIPSIS);
				}
			}

			this.state = 393;
			this.match(CHeaderParser.ROUND_E);
			this.state = 395;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE))) !== 0)) {
				{
				this.state = 394;
				this.attribute();
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
	public attribute(): AttributeContext {
		let _localctx: AttributeContext = new AttributeContext(this._ctx, this.state);
		this.enterRule(_localctx, 64, CHeaderParser.RULE_attribute);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 397;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE))) !== 0))) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			this.state = 398;
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
		this.enterRule(_localctx, 66, CHeaderParser.RULE_doc);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 400;
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
		this.enterRule(_localctx, 68, CHeaderParser.RULE_trailingDoc);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 402;
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
		"\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03+\u0197\x04\x02" +
		"\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
		"\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r\t\r\x04" +
		"\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x04\x11\t\x11\x04\x12\t\x12\x04" +
		"\x13\t\x13\x04\x14\t\x14\x04\x15\t\x15\x04\x16\t\x16\x04\x17\t\x17\x04" +
		"\x18\t\x18\x04\x19\t\x19\x04\x1A\t\x1A\x04\x1B\t\x1B\x04\x1C\t\x1C\x04" +
		"\x1D\t\x1D\x04\x1E\t\x1E\x04\x1F\t\x1F\x04 \t \x04!\t!\x04\"\t\"\x04#" +
		"\t#\x04$\t$\x03\x02\x05\x02J\n\x02\x03\x02\x07\x02M\n\x02\f\x02\x0E\x02" +
		"P\v\x02\x03\x02\x03\x02\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03" +
		"\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x03`\n\x03\x03\x04" +
		"\x05\x04c\n\x04\x03\x04\x03\x04\x03\x05\x03\x05\x03\x05\x03\x05\x07\x05" +
		"k\n\x05\f\x05\x0E\x05n\v\x05\x03\x05\x03\x05\x03\x06\x05\x06s\n\x06\x03" +
		"\x06\x05\x06v\n\x06\x03\x06\x03\x06\x03\x06\x03\x06\x07\x06|\n\x06\f\x06" +
		"\x0E\x06\x7F\v\x06\x03\x06\x07\x06\x82\n\x06\f\x06\x0E\x06\x85\v\x06\x03" +
		"\x06\x03\x06\x05\x06\x89\n\x06\x03\x07\x05\x07\x8C\n\x07\x03\x07\x05\x07" +
		"\x8F\n\x07\x03\x07\x03\x07\x05\x07\x93\n\x07\x03\x07\x03\x07\x03\x07\x03" +
		"\x07\x03\b\x05\b\x9A\n\b\x03\b\x03\b\x03\b\x05\b\x9F\n\b\x03\b\x03\b\x03" +
		"\b\x03\b\x03\t\x05\t\xA6\n\t\x03\t\x03\t\x05\t\xAA\n\t\x03\t\x03\t\x03" +
		"\t\x03\t\x03\n\x05\n\xB1\n\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n" +
		"\x03\v\x05\v\xBB\n\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\f\x05" +
		"\f\xC5\n\f\x03\f\x03\f\x03\f\x03\f\x03\f\x03\f\x05\f\xCD\n\f\x03\f\x03" +
		"\f\x03\f\x03\f\x03\f\x03\f\x03\f\x05\f\xD6\n\f\x03\r\x05\r\xD9\n\r\x03" +
		"\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\x0E\x03\x0E\x03" +
		"\x0E\x03\x0E\x03\x0F\x03\x0F\x03\x0F\x05\x0F\xEB\n\x0F\x03\x10\x03\x10" +
		"\x07\x10\xEF\n\x10\f\x10\x0E\x10\xF2\v\x10\x03\x10\x03\x10\x03\x11\x03" +
		"\x11\x07\x11\xF8\n\x11\f\x11\x0E\x11\xFB\v\x11\x03\x11\x03\x11\x03\x12" +
		"\x03\x12\x07\x12\u0101\n\x12\f\x12\x0E\x12\u0104\v\x12\x03\x12\x03\x12" +
		"\x03\x13\x03\x13\x03\x13\x03\x13\x03\x13\x03\x13\x05\x13\u010E\n\x13\x03" +
		"\x14\x03\x14\x05\x14\u0112\n\x14\x03\x15\x03\x15\x03\x16\x03\x16\x03\x17" +
		"\x03\x17\x07\x17\u011A\n\x17\f\x17\x0E\x17\u011D\v\x17\x03\x17\x03\x17" +
		"\x03\x18\x05\x18\u0122\n\x18\x03\x18\x03\x18\x03\x18\x05\x18\u0127\n\x18" +
		"\x03\x18\x05\x18\u012A\n\x18\x03\x18\x05\x18\u012D\n\x18\x03\x19\x03\x19" +
		"\x03\x19\x07\x19\u0132\n\x19\f\x19\x0E\x19\u0135\v\x19\x03\x19\x03\x19" +
		"\x03\x1A\x03\x1A\x05\x1A\u013B\n\x1A\x03\x1B\x05\x1B\u013E\n\x1B\x03\x1B" +
		"\x05\x1B\u0141\n\x1B\x03\x1B\x05\x1B\u0144\n\x1B\x03\x1B\x03\x1B\x03\x1B" +
		"\x03\x1B\x07\x1B\u014A\n\x1B\f\x1B\x0E\x1B\u014D\v\x1B\x03\x1B\x07\x1B" +
		"\u0150\n\x1B\f\x1B\x0E\x1B\u0153\v\x1B\x03\x1B\x03\x1B\x05\x1B\u0157\n" +
		"\x1B\x03\x1C\x05\x1C\u015A\n\x1C\x03\x1C\x03\x1C\x03\x1C\x03\x1C\x03\x1C" +
		"\x03\x1C\x03\x1C\x03\x1C\x05\x1C\u0164\n\x1C\x03\x1D\x05\x1D\u0167\n\x1D" +
		"\x03\x1D\x03\x1D\x03\x1D\x03\x1D\x03\x1D\x03\x1E\x03\x1E\x03\x1F\x06\x1F" +
		"\u0171\n\x1F\r\x1F\x0E\x1F\u0172\x03 \x03 \x03 \x07 \u0178\n \f \x0E " +
		"\u017B\v \x03!\x03!\x03!\x03!\x07!\u0181\n!\f!\x0E!\u0184\v!\x05!\u0186" +
		"\n!\x03!\x03!\x05!\u018A\n!\x03!\x03!\x05!\u018E\n!\x03\"\x03\"\x03\"" +
		"\x03#\x03#\x03$\x03$\x03$\x02\x02\x02%\x02\x02\x04\x02\x06\x02\b\x02\n" +
		"\x02\f\x02\x0E\x02\x10\x02\x12\x02\x14\x02\x16\x02\x18\x02\x1A\x02\x1C" +
		"\x02\x1E\x02 \x02\"\x02$\x02&\x02(\x02*\x02,\x02.\x020\x022\x024\x026" +
		"\x028\x02:\x02<\x02>\x02@\x02B\x02D\x02F\x02\x02\b\x04\x02\x13\x13\x15" +
		"\x15\b\x02\t\t\f\r\x12\x13\x15\x16\x1A\x1A)+\x03\x02!(\x05\x02\f\f\x16" +
		"\x16**\x04\x02\v\v\x17\x19\x03\x02\x07\b\x02\u01B6\x02I\x03\x02\x02\x02" +
		"\x04_\x03\x02\x02\x02\x06b\x03\x02\x02\x02\bf\x03\x02\x02\x02\nr\x03\x02" +
		"\x02\x02\f\x8B\x03\x02\x02\x02\x0E\x99\x03\x02\x02\x02\x10\xA5\x03\x02" +
		"\x02\x02\x12\xB0\x03\x02\x02\x02\x14\xBA\x03\x02\x02\x02\x16\xD5\x03\x02" +
		"\x02\x02\x18\xD8\x03\x02\x02\x02\x1A\xE3\x03\x02\x02\x02\x1C\xEA\x03\x02" +
		"\x02\x02\x1E\xEC\x03\x02\x02\x02 \xF5\x03\x02\x02\x02\"\xFE\x03\x02\x02" +
		"\x02$\u010D\x03\x02\x02\x02&\u0111\x03\x02\x02\x02(\u0113\x03\x02\x02" +
		"\x02*\u0115\x03\x02\x02\x02,\u0117\x03\x02\x02\x02.\u0121\x03\x02\x02" +
		"\x020\u012E\x03\x02\x02\x022\u013A\x03\x02\x02\x024\u013D\x03\x02\x02" +
		"\x026\u0159\x03\x02\x02\x028\u0166\x03\x02\x02\x02:\u016D\x03\x02\x02" +
		"\x02<\u0170\x03\x02\x02\x02>\u0174\x03\x02\x02\x02@\u017C\x03\x02\x02" +
		"\x02B\u018F\x03\x02\x02\x02D\u0192\x03\x02\x02\x02F\u0194\x03\x02\x02" +
		"\x02HJ\x05D#\x02IH\x03\x02\x02\x02IJ\x03\x02\x02\x02JN\x03\x02\x02\x02" +
		"KM\x05\x04\x03\x02LK\x03\x02\x02\x02MP\x03\x02\x02\x02NL\x03\x02\x02\x02" +
		"NO\x03\x02\x02\x02OQ\x03\x02\x02\x02PN\x03\x02\x02\x02QR\x07\x02\x02\x03" +
		"R\x03\x03\x02\x02\x02S`\x05\x06\x04\x02T`\x05\b\x05\x02U`\x05\n\x06\x02" +
		"V`\x05\f\x07\x02W`\x05\x0E\b\x02X`\x05\x10\t\x02Y`\x05\x12\n\x02Z`\x05" +
		"\x14\v\x02[`\x05\x16\f\x02\\`\x05\x18\r\x02]`\x05\x1A\x0E\x02^`\x05D#" +
		"\x02_S\x03\x02\x02\x02_T\x03\x02\x02\x02_U\x03\x02\x02\x02_V\x03\x02\x02" +
		"\x02_W\x03\x02\x02\x02_X\x03\x02\x02\x02_Y\x03\x02\x02\x02_Z\x03\x02\x02" +
		"\x02_[\x03\x02\x02\x02_\\\x03\x02\x02\x02_]\x03\x02\x02\x02_^\x03\x02" +
		"\x02\x02`\x05\x03\x02\x02\x02ac\x05D#\x02ba\x03\x02\x02\x02bc\x03\x02" +
		"\x02\x02cd\x03\x02\x02\x02de\x07\t\x02\x02e\x07\x03\x02\x02\x02fg\x07" +
		"\x0E\x02\x02gh\x07)\x02\x02hl\x07\x1B\x02\x02ik\x05\x04\x03\x02ji\x03" +
		"\x02\x02\x02kn\x03\x02\x02\x02lj\x03\x02\x02\x02lm\x03\x02\x02\x02mo\x03" +
		"\x02\x02\x02nl\x03\x02\x02\x02op\x07\x1C\x02\x02p\t\x03\x02\x02\x02qs" +
		"\x05D#\x02rq\x03\x02\x02\x02rs\x03\x02\x02\x02su\x03\x02\x02\x02tv\x07" +
		"\x0E\x02\x02ut\x03\x02\x02\x02uv\x03\x02\x02\x02vw\x03\x02\x02\x02wx\x05" +
		"<\x1F\x02x}\x05:\x1E\x02yz\x07#\x02\x02z|\x05:\x1E\x02{y\x03\x02\x02\x02" +
		"|\x7F\x03\x02\x02\x02}{\x03\x02\x02\x02}~\x03\x02\x02\x02~\x83\x03\x02" +
		"\x02\x02\x7F}\x03\x02\x02\x02\x80\x82\x05\"\x12\x02\x81\x80\x03\x02\x02" +
		"\x02\x82\x85\x03\x02\x02\x02\x83\x81\x03\x02\x02\x02\x83\x84\x03\x02\x02" +
		"\x02\x84\x86\x03\x02\x02\x02\x85\x83\x03\x02\x02\x02\x86\x88\x07\"\x02" +
		"\x02\x87\x89\x05F$\x02\x88\x87\x03\x02\x02\x02\x88\x89\x03\x02\x02\x02" +
		"\x89\v\x03\x02\x02\x02\x8A\x8C\x05D#\x02\x8B\x8A\x03\x02\x02\x02\x8B\x8C" +
		"\x03\x02\x02\x02\x8C\x8E\x03\x02\x02\x02\x8D\x8F\x07\x0E\x02\x02\x8E\x8D" +
		"\x03\x02\x02\x02\x8E\x8F\x03\x02\x02\x02\x8F\x90\x03\x02\x02\x02\x90\x92" +
		"\x05<\x1F\x02\x91\x93\x05B\"\x02\x92\x91\x03\x02\x02\x02\x92\x93\x03\x02" +
		"\x02\x02\x93\x94\x03\x02\x02\x02\x94\x95\x05:\x1E\x02\x95\x96\x05@!\x02" +
		"\x96\x97\x07\"\x02\x02\x97\r\x03\x02\x02\x02\x98\x9A\x05D#\x02\x99\x98" +
		"\x03\x02\x02\x02\x99\x9A\x03\x02\x02\x02\x9A\x9B\x03\x02\x02\x02\x9B\x9C" +
		"\x05\x1C\x0F\x02\x9C\x9E\x05<\x1F\x02\x9D\x9F\x05B\"\x02\x9E\x9D\x03\x02" +
		"\x02\x02\x9E\x9F\x03\x02\x02\x02\x9F\xA0\x03\x02\x02\x02\xA0\xA1\x05:" +
		"\x1E\x02\xA1\xA2\x05@!\x02\xA2\xA3\x05\x1E\x10\x02\xA3\x0F\x03\x02\x02" +
		"\x02\xA4\xA6\x05D#\x02\xA5\xA4\x03\x02\x02\x02\xA5\xA6\x03\x02\x02\x02" +
		"\xA6\xA7\x03\x02\x02\x02\xA7\xA9\x07\x14\x02\x02\xA8\xAA\t\x02\x02\x02" +
		"\xA9\xA8\x03\x02\x02\x02\xA9\xAA\x03\x02\x02\x02\xAA\xAB\x03\x02\x02\x02" +
		"\xAB\xAC\x05<\x1F\x02\xAC\xAD\x05:\x1E\x02\xAD\xAE\x07\"\x02\x02\xAE\x11" +
		"\x03\x02\x02\x02\xAF\xB1\x05D#\x02\xB0\xAF\x03\x02\x02\x02\xB0\xB1\x03" +
		"\x02\x02\x02\xB1\xB2\x03\x02\x02\x02\xB2\xB3\x07\x14\x02\x02\xB3\xB4\x07" +
		"\x15\x02\x02\xB4\xB5\x05:\x1E\x02\xB5\xB6\x05\x1E\x10\x02\xB6\xB7\x05" +
		":\x1E\x02\xB7\xB8\x07\"\x02\x02\xB8\x13\x03\x02\x02\x02\xB9\xBB\x05D#" +
		"\x02\xBA\xB9\x03\x02\x02\x02\xBA\xBB\x03\x02\x02\x02\xBB\xBC\x03\x02\x02" +
		"\x02\xBC\xBD\x07\x14\x02\x02\xBD\xBE\x07\r\x02\x02\xBE\xBF\x05:\x1E\x02" +
		"\xBF\xC0\x05,\x17\x02\xC0\xC1\x05:\x1E\x02\xC1\xC2\x07\"\x02\x02\xC2\x15" +
		"\x03\x02\x02\x02\xC3\xC5\x05D#\x02\xC4\xC3\x03\x02\x02\x02\xC4\xC5\x03" +
		"\x02\x02\x02\xC5\xC6\x03\x02\x02\x02\xC6\xC7\x07\x13\x02\x02\xC7\xC8\x05" +
		":\x1E\x02\xC8\xC9\x050\x19\x02\xC9\xCA\x07\"\x02\x02\xCA\xD6\x03\x02\x02" +
		"\x02\xCB\xCD\x05D#\x02\xCC\xCB\x03\x02\x02\x02\xCC\xCD\x03\x02\x02\x02" +
		"\xCD\xCE\x03\x02\x02\x02\xCE\xCF\x07\x14\x02\x02\xCF\xD0\x07\x13\x02\x02" +
		"\xD0\xD1\x05:\x1E\x02\xD1\xD2\x050\x19\x02\xD2\xD3\x05:\x1E\x02\xD3\xD4" +
		"\x07\"\x02\x02\xD4\xD6\x03\x02\x02\x02\xD5\xC4\x03\x02\x02\x02\xD5\xCC" +
		"\x03\x02\x02\x02\xD6\x17\x03\x02\x02\x02\xD7\xD9\x05D#\x02\xD8\xD7\x03" +
		"\x02\x02\x02\xD8\xD9\x03\x02\x02\x02\xD9\xDA\x03\x02\x02\x02\xDA\xDB\x07" +
		"\x14\x02\x02\xDB\xDC\x05<\x1F\x02\xDC\xDD\x07\x1D\x02\x02\xDD\xDE\x07" +
		"%\x02\x02\xDE\xDF\x05:\x1E\x02\xDF\xE0\x07\x1E\x02\x02\xE0\xE1\x05@!\x02" +
		"\xE1\xE2\x07\"\x02\x02\xE2\x19\x03\x02\x02\x02\xE3\xE4\x07\x1A\x02\x02" +
		"\xE4\xE5\x05 \x11\x02\xE5\xE6\x07\"\x02\x02\xE6\x1B\x03\x02\x02\x02\xE7" +
		"\xEB\x07\x11\x02\x02\xE8\xE9\x07\x12\x02\x02\xE9\xEB\x07\x0F\x02\x02\xEA" +
		"\xE7\x03\x02\x02\x02\xEA\xE8\x03\x02\x02\x02\xEB\x1D\x03\x02\x02\x02\xEC" +
		"\xF0\x07\x1B\x02\x02\xED\xEF\x05$\x13\x02\xEE\xED\x03\x02\x02\x02\xEF" +
		"\xF2\x03\x02\x02\x02\xF0\xEE\x03\x02\x02\x02\xF0\xF1\x03\x02\x02\x02\xF1" +
		"\xF3\x03\x02\x02\x02\xF2\xF0\x03\x02\x02\x02\xF3\xF4\x07\x1C\x02\x02\xF4" +
		"\x1F\x03\x02\x02\x02\xF5\xF9\x07\x1D\x02\x02\xF6\xF8\x05$\x13\x02\xF7" +
		"\xF6\x03\x02\x02\x02\xF8\xFB\x03\x02\x02\x02\xF9\xF7\x03\x02\x02\x02\xF9" +
		"\xFA\x03\x02\x02\x02\xFA\xFC\x03\x02\x02\x02\xFB\xF9\x03\x02\x02\x02\xFC" +
		"\xFD\x07\x1E\x02\x02\xFD!\x03\x02\x02\x02\xFE\u0102\x07\x1F\x02\x02\xFF" +
		"\u0101\x05$\x13\x02\u0100\xFF\x03\x02\x02\x02\u0101\u0104\x03\x02\x02" +
		"\x02\u0102\u0100\x03\x02\x02\x02\u0102\u0103\x03\x02\x02\x02\u0103\u0105" +
		"\x03\x02\x02\x02\u0104\u0102\x03\x02\x02\x02\u0105\u0106\x07 \x02\x02" +
		"\u0106#\x03\x02\x02\x02\u0107\u010E\x05\x1E\x10\x02\u0108\u010E\x05\"" +
		"\x12\x02\u0109\u010E\x05&\x14\x02\u010A\u010E\x05*\x16\x02\u010B\u010E" +
		"\x05D#\x02\u010C\u010E\x05F$\x02\u010D\u0107\x03\x02\x02\x02\u010D\u0108" +
		"\x03\x02\x02\x02\u010D\u0109\x03\x02\x02\x02\u010D\u010A\x03\x02\x02\x02" +
		"\u010D\u010B\x03\x02\x02\x02\u010D\u010C\x03\x02\x02\x02\u010E%\x03\x02" +
		"\x02\x02\u010F\u0112\x05 \x11\x02\u0110\u0112\x05(\x15\x02\u0111\u010F" +
		"\x03\x02\x02\x02\u0111\u0110\x03\x02\x02\x02\u0112\'\x03\x02\x02\x02\u0113" +
		"\u0114\t\x03\x02\x02\u0114)\x03\x02\x02\x02\u0115\u0116\t\x04\x02\x02" +
		"\u0116+\x03\x02\x02\x02\u0117\u011B\x07\x1B\x02\x02\u0118\u011A\x05.\x18" +
		"\x02\u0119\u0118\x03\x02\x02\x02\u011A\u011D\x03\x02\x02\x02\u011B\u0119" +
		"\x03\x02\x02\x02\u011B\u011C\x03\x02\x02\x02\u011C\u011E\x03\x02\x02\x02" +
		"\u011D\u011B\x03\x02\x02\x02\u011E\u011F\x07\x1C\x02\x02\u011F-\x03\x02" +
		"\x02\x02\u0120\u0122\x05D#\x02\u0121\u0120\x03\x02\x02\x02\u0121\u0122" +
		"\x03\x02\x02\x02\u0122\u0123\x03\x02\x02\x02\u0123\u0126\x05:\x1E\x02" +
		"\u0124\u0125\x07&\x02\x02\u0125\u0127\x05&\x14\x02\u0126\u0124\x03\x02" +
		"\x02\x02\u0126\u0127\x03\x02\x02\x02\u0127\u0129\x03\x02\x02\x02\u0128" +
		"\u012A\x07#\x02\x02\u0129\u0128\x03\x02\x02\x02\u0129\u012A\x03\x02\x02" +
		"\x02\u012A\u012C\x03\x02\x02\x02\u012B\u012D\x05F$\x02\u012C\u012B\x03" +
		"\x02\x02\x02\u012C\u012D\x03\x02\x02\x02\u012D/\x03\x02\x02\x02\u012E" +
		"\u0133\x07\x1B\x02\x02\u012F\u0132\x052\x1A\x02\u0130\u0132\x058\x1D\x02" +
		"\u0131\u012F\x03\x02\x02\x02\u0131\u0130\x03\x02\x02\x02\u0132\u0135\x03" +
		"\x02\x02\x02\u0133\u0131\x03\x02\x02\x02\u0133\u0134\x03\x02\x02\x02\u0134" +
		"\u0136\x03\x02\x02\x02\u0135\u0133\x03\x02\x02\x02\u0136\u0137\x07\x1C" +
		"\x02\x02\u01371\x03\x02\x02\x02\u0138\u013B\x054\x1B\x02\u0139\u013B\x05" +
		"6\x1C\x02\u013A\u0138\x03\x02\x02\x02\u013A\u0139\x03\x02\x02\x02\u013B" +
		"3\x03\x02\x02\x02\u013C\u013E\x05D#\x02\u013D\u013C\x03\x02\x02\x02\u013D" +
		"\u013E\x03\x02\x02\x02\u013E\u0143\x03\x02\x02\x02\u013F\u0141\x07\f\x02" +
		"\x02\u0140\u013F\x03\x02\x02\x02\u0140\u0141\x03\x02\x02\x02\u0141\u0142" +
		"\x03\x02\x02\x02\u0142\u0144\x07\x13\x02\x02\u0143\u0140\x03\x02\x02\x02" +
		"\u0143\u0144\x03\x02\x02\x02\u0144\u0145\x03\x02\x02\x02\u0145\u0146\x05" +
		"<\x1F\x02\u0146\u014B\x05:\x1E\x02\u0147\u0148\x07#\x02\x02\u0148\u014A" +
		"\x05:\x1E\x02\u0149\u0147\x03\x02\x02\x02\u014A\u014D\x03\x02\x02\x02" +
		"\u014B\u0149\x03\x02\x02\x02\u014B\u014C\x03\x02\x02\x02\u014C\u0151\x03" +
		"\x02\x02\x02\u014D\u014B\x03\x02\x02\x02\u014E\u0150\x05\"\x12\x02\u014F" +
		"\u014E\x03\x02\x02\x02\u0150\u0153\x03\x02\x02\x02\u0151\u014F\x03\x02" +
		"\x02\x02\u0151\u0152\x03\x02\x02\x02\u0152\u0154\x03\x02\x02\x02\u0153" +
		"\u0151\x03\x02\x02\x02\u0154\u0156\x07\"\x02\x02\u0155\u0157\x05F$\x02" +
		"\u0156\u0155\x03\x02\x02\x02\u0156\u0157\x03\x02\x02\x02\u01575\x03\x02" +
		"\x02\x02\u0158\u015A\x05D#\x02\u0159\u0158\x03\x02\x02\x02\u0159\u015A" +
		"\x03\x02\x02\x02\u015A\u015B\x03\x02\x02\x02\u015B\u015C\x05<\x1F\x02" +
		"\u015C\u015D\x07\x1D\x02\x02\u015D\u015E\x07%\x02\x02\u015E\u015F\x05" +
		":\x1E\x02\u015F\u0160\x07\x1E\x02\x02\u0160\u0161\x05@!\x02\u0161\u0163" +
		"\x07\"\x02\x02\u0162\u0164\x05F$\x02\u0163\u0162\x03\x02\x02\x02\u0163" +
		"\u0164\x03\x02\x02\x02\u01647\x03\x02\x02\x02\u0165\u0167\x05D#\x02\u0166" +
		"\u0165\x03\x02\x02\x02\u0166\u0167\x03\x02\x02\x02\u0167\u0168\x03\x02" +
		"\x02\x02\u0168\u0169\x07\x15\x02\x02\u0169\u016A\x05\x1E\x10\x02\u016A" +
		"\u016B\x05:\x1E\x02\u016B\u016C\x07\"\x02\x02\u016C9\x03\x02\x02\x02\u016D" +
		"\u016E\x07*\x02\x02\u016E;\x03\x02\x02\x02\u016F\u0171\x05> \x02\u0170" +
		"\u016F\x03\x02\x02\x02\u0171\u0172\x03\x02\x02\x02\u0172\u0170\x03\x02" +
		"\x02\x02\u0172\u0173\x03\x02\x02\x02\u0173=\x03\x02\x02\x02\u0174\u0179" +
		"\t\x05\x02\x02\u0175\u0178\x07%\x02\x02\u0176\u0178\x05\"\x12\x02\u0177" +
		"\u0175\x03\x02\x02\x02\u0177\u0176\x03\x02\x02\x02\u0178\u017B\x03\x02" +
		"\x02\x02\u0179\u0177\x03\x02\x02\x02\u0179\u017A\x03\x02\x02\x02\u017A" +
		"?\x03\x02\x02\x02\u017B\u0179\x03\x02\x02\x02\u017C\u0185\x07\x1D\x02" +
		"\x02\u017D\u0182\x05<\x1F\x02\u017E\u017F\x07#\x02\x02\u017F\u0181\x05" +
		"<\x1F\x02\u0180\u017E\x03\x02\x02\x02\u0181\u0184\x03\x02\x02\x02\u0182" +
		"\u0180\x03\x02\x02\x02\u0182\u0183\x03\x02\x02\x02\u0183\u0186\x03\x02" +
		"\x02\x02\u0184\u0182\x03\x02\x02\x02\u0185\u017D\x03\x02\x02\x02\u0185" +
		"\u0186\x03\x02\x02\x02\u0186\u0189\x03\x02\x02\x02\u0187\u0188\x07#\x02" +
		"\x02\u0188\u018A\x07\'\x02\x02\u0189\u0187\x03\x02\x02\x02\u0189\u018A" +
		"\x03\x02\x02\x02\u018A\u018B\x03\x02\x02\x02\u018B\u018D\x07\x1E\x02\x02" +
		"\u018C\u018E\x05B\"\x02\u018D\u018C\x03\x02\x02\x02\u018D\u018E\x03\x02" +
		"\x02\x02\u018EA\x03\x02\x02\x02\u018F\u0190\t\x06\x02\x02\u0190\u0191" +
		"\x05 \x11\x02\u0191C\x03\x02\x02\x02\u0192\u0193\t\x07\x02\x02\u0193E" +
		"\x03\x02\x02\x02\u0194\u0195\x07\x06\x02\x02\u0195G\x03\x02\x02\x027I" +
		"N_blru}\x83\x88\x8B\x8E\x92\x99\x9E\xA5\xA9\xB0\xBA\xC4\xCC\xD5\xD8\xEA" +
		"\xF0\xF9\u0102\u010D\u0111\u011B\u0121\u0126\u0129\u012C\u0131\u0133\u013A" +
		"\u013D\u0140\u0143\u014B\u0151\u0156\u0159\u0163\u0166\u0172\u0177\u0179" +
		"\u0182\u0185\u0189\u018D";
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
	public STRUCT(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STRUCT, 0); }
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
	public CONST(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.CONST, 0); }
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
	public attribute(): AttributeContext | undefined {
		return this.tryGetRuleContext(0, AttributeContext);
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


