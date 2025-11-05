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
	public static readonly SDL_COMPILE_TIME_ASSERT = 22;
	public static readonly CURLY_B = 23;
	public static readonly CURLY_E = 24;
	public static readonly ROUND_B = 25;
	public static readonly ROUND_E = 26;
	public static readonly SQUARE_B = 27;
	public static readonly SQUARE_E = 28;
	public static readonly COLON = 29;
	public static readonly SEMI = 30;
	public static readonly COMMA = 31;
	public static readonly DOT = 32;
	public static readonly STAR = 33;
	public static readonly EQ = 34;
	public static readonly ELLIPSIS = 35;
	public static readonly PUNCT_EXTRA = 36;
	public static readonly STRING = 37;
	public static readonly ID = 38;
	public static readonly NUMBER = 39;
	public static readonly RULE_prog = 0;
	public static readonly RULE_decl = 1;
	public static readonly RULE_externC = 2;
	public static readonly RULE_directive = 3;
	public static readonly RULE_functionDecl = 4;
	public static readonly RULE_functionDef = 5;
	public static readonly RULE_aliasDef = 6;
	public static readonly RULE_unionDef = 7;
	public static readonly RULE_enumDef = 8;
	public static readonly RULE_structDef = 9;
	public static readonly RULE_callbackDef = 10;
	public static readonly RULE_compileTimeAssert = 11;
	public static readonly RULE_inline = 12;
	public static readonly RULE_block = 13;
	public static readonly RULE_group = 14;
	public static readonly RULE_indexing = 15;
	public static readonly RULE_stm = 16;
	public static readonly RULE_expr = 17;
	public static readonly RULE_word = 18;
	public static readonly RULE_punct = 19;
	public static readonly RULE_enumBody = 20;
	public static readonly RULE_enumItem = 21;
	public static readonly RULE_structBody = 22;
	public static readonly RULE_structItem = 23;
	public static readonly RULE_unionInlineType = 24;
	public static readonly RULE_id = 25;
	public static readonly RULE_type = 26;
	public static readonly RULE_typeEl = 27;
	public static readonly RULE_signature = 28;
	public static readonly RULE_attribute = 29;
	public static readonly RULE_doc = 30;
	public static readonly RULE_trailingDoc = 31;
	// tslint:disable:no-trailing-whitespace
	public static readonly ruleNames: string[] = [
		"prog", "decl", "externC", "directive", "functionDecl", "functionDef", 
		"aliasDef", "unionDef", "enumDef", "structDef", "callbackDef", "compileTimeAssert", 
		"inline", "block", "group", "indexing", "stm", "expr", "word", "punct", 
		"enumBody", "enumItem", "structBody", "structItem", "unionInlineType", 
		"id", "type", "typeEl", "signature", "attribute", "doc", "trailingDoc",
	];

	private static readonly _LITERAL_NAMES: Array<string | undefined> = [
		undefined, undefined, undefined, undefined, undefined, undefined, undefined, 
		undefined, undefined, "'__attribute__'", "'const'", "'enum'", "'extern'", 
		"'__inline__'", undefined, "'SDL_FORCE_INLINE'", "'static'", "'struct'", 
		"'typedef'", "'union'", "'void'", undefined, "'SDL_COMPILE_TIME_ASSERT'", 
		"'{'", "'}'", "'('", "')'", "'['", "']'", "':'", "';'", "','", "'.'", 
		"'*'", "'='", "'...'",
	];
	private static readonly _SYMBOLIC_NAMES: Array<string | undefined> = [
		undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "TRAILING_DOC", "LONG_DOC", 
		"SHORT_DOC", "DEFINE", "DIRECTIVE", "ATTRIBUTE", "CONST", "ENUM", "EXTERN", 
		"INLINE", "SDL_NOISE", "SDL_INLINE", "STATIC", "STRUCT", "TYPEDEF", "UNION", 
		"VOID", "SDL_VARARG_ATTRIB", "SDL_COMPILE_TIME_ASSERT", "CURLY_B", "CURLY_E", 
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
			this.state = 65;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 0, this._ctx) ) {
			case 1:
				{
				this.state = 64;
				this.doc();
				}
				break;
			}
			this.state = 70;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || _la === CHeaderParser.ID) {
				{
				{
				this.state = 67;
				this.decl();
				}
				}
				this.state = 72;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 73;
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
			this.state = 86;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 2, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 75;
				this.directive();
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 76;
				this.externC();
				}
				break;

			case 3:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 77;
				this.functionDecl();
				}
				break;

			case 4:
				this.enterOuterAlt(_localctx, 4);
				{
				this.state = 78;
				this.functionDef();
				}
				break;

			case 5:
				this.enterOuterAlt(_localctx, 5);
				{
				this.state = 79;
				this.aliasDef();
				}
				break;

			case 6:
				this.enterOuterAlt(_localctx, 6);
				{
				this.state = 80;
				this.unionDef();
				}
				break;

			case 7:
				this.enterOuterAlt(_localctx, 7);
				{
				this.state = 81;
				this.enumDef();
				}
				break;

			case 8:
				this.enterOuterAlt(_localctx, 8);
				{
				this.state = 82;
				this.structDef();
				}
				break;

			case 9:
				this.enterOuterAlt(_localctx, 9);
				{
				this.state = 83;
				this.callbackDef();
				}
				break;

			case 10:
				this.enterOuterAlt(_localctx, 10);
				{
				this.state = 84;
				this.compileTimeAssert();
				}
				break;

			case 11:
				this.enterOuterAlt(_localctx, 11);
				{
				this.state = 85;
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
			this.state = 88;
			this.match(CHeaderParser.EXTERN);
			this.state = 89;
			this.match(CHeaderParser.STRING);
			this.state = 90;
			this.match(CHeaderParser.CURLY_B);
			this.state = 94;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || _la === CHeaderParser.ID) {
				{
				{
				this.state = 91;
				this.decl();
				}
				}
				this.state = 96;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 97;
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
			this.state = 100;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 99;
				this.doc();
				}
			}

			this.state = 102;
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
	public functionDecl(): FunctionDeclContext {
		let _localctx: FunctionDeclContext = new FunctionDeclContext(this._ctx, this.state);
		this.enterRule(_localctx, 8, CHeaderParser.RULE_functionDecl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 105;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 104;
				this.doc();
				}
			}

			this.state = 108;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.EXTERN) {
				{
				this.state = 107;
				this.match(CHeaderParser.EXTERN);
				}
			}

			this.state = 110;
			this.type();
			this.state = 112;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.ATTRIBUTE || _la === CHeaderParser.SDL_VARARG_ATTRIB) {
				{
				this.state = 111;
				this.attribute();
				}
			}

			this.state = 114;
			this.id();
			this.state = 115;
			this.signature();
			this.state = 116;
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
			this.state = 119;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 118;
				this.doc();
				}
			}

			this.state = 121;
			this.inline();
			this.state = 122;
			this.type();
			this.state = 124;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.ATTRIBUTE || _la === CHeaderParser.SDL_VARARG_ATTRIB) {
				{
				this.state = 123;
				this.attribute();
				}
			}

			this.state = 126;
			this.id();
			this.state = 127;
			this.signature();
			this.state = 128;
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
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 131;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 130;
				this.doc();
				}
			}

			this.state = 133;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 135;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.STRUCT || _la === CHeaderParser.UNION) {
				{
				this.state = 134;
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

			this.state = 137;
			this.type();
			this.state = 138;
			this.id();
			this.state = 139;
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
		this.enterRule(_localctx, 14, CHeaderParser.RULE_unionDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 142;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 141;
				this.doc();
				}
			}

			this.state = 144;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 145;
			this.match(CHeaderParser.UNION);
			this.state = 146;
			this.id();
			this.state = 147;
			this.block();
			this.state = 148;
			this.id();
			this.state = 149;
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
		this.enterRule(_localctx, 16, CHeaderParser.RULE_enumDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 152;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 151;
				this.doc();
				}
			}

			this.state = 154;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 155;
			this.match(CHeaderParser.ENUM);
			this.state = 156;
			this.id();
			this.state = 157;
			this.enumBody();
			this.state = 158;
			this.id();
			this.state = 159;
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
		this.enterRule(_localctx, 18, CHeaderParser.RULE_structDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 162;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 161;
				this.doc();
				}
			}

			this.state = 164;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 165;
			this.match(CHeaderParser.STRUCT);
			this.state = 166;
			this.id();
			this.state = 167;
			this.structBody();
			this.state = 168;
			this.id();
			this.state = 169;
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
	public callbackDef(): CallbackDefContext {
		let _localctx: CallbackDefContext = new CallbackDefContext(this._ctx, this.state);
		this.enterRule(_localctx, 20, CHeaderParser.RULE_callbackDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 172;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 171;
				this.doc();
				}
			}

			this.state = 174;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 175;
			this.type();
			this.state = 176;
			this.match(CHeaderParser.ROUND_B);
			this.state = 177;
			this.match(CHeaderParser.STAR);
			this.state = 178;
			this.id();
			this.state = 179;
			this.match(CHeaderParser.ROUND_E);
			this.state = 180;
			this.signature();
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
	public compileTimeAssert(): CompileTimeAssertContext {
		let _localctx: CompileTimeAssertContext = new CompileTimeAssertContext(this._ctx, this.state);
		this.enterRule(_localctx, 22, CHeaderParser.RULE_compileTimeAssert);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 183;
			this.match(CHeaderParser.SDL_COMPILE_TIME_ASSERT);
			this.state = 184;
			this.group();
			this.state = 185;
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
		this.enterRule(_localctx, 24, CHeaderParser.RULE_inline);
		try {
			this.state = 190;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.SDL_INLINE:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 187;
				this.match(CHeaderParser.SDL_INLINE);
				}
				break;
			case CHeaderParser.STATIC:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 188;
				this.match(CHeaderParser.STATIC);
				this.state = 189;
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
		this.enterRule(_localctx, 26, CHeaderParser.RULE_block);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 192;
			this.match(CHeaderParser.CURLY_B);
			this.state = 196;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
				{
				{
				this.state = 193;
				this.stm();
				}
				}
				this.state = 198;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 199;
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
		this.enterRule(_localctx, 28, CHeaderParser.RULE_group);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 201;
			this.match(CHeaderParser.ROUND_B);
			this.state = 205;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
				{
				{
				this.state = 202;
				this.stm();
				}
				}
				this.state = 207;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 208;
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
		this.enterRule(_localctx, 30, CHeaderParser.RULE_indexing);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 210;
			this.match(CHeaderParser.SQUARE_B);
			this.state = 214;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
				{
				{
				this.state = 211;
				this.stm();
				}
				}
				this.state = 216;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 217;
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
		this.enterRule(_localctx, 32, CHeaderParser.RULE_stm);
		try {
			this.state = 225;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.CURLY_B:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 219;
				this.block();
				}
				break;
			case CHeaderParser.SQUARE_B:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 220;
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
				this.state = 221;
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
				this.state = 222;
				this.punct();
				}
				break;
			case CHeaderParser.LONG_DOC:
			case CHeaderParser.SHORT_DOC:
				this.enterOuterAlt(_localctx, 5);
				{
				this.state = 223;
				this.doc();
				}
				break;
			case CHeaderParser.TRAILING_DOC:
				this.enterOuterAlt(_localctx, 6);
				{
				this.state = 224;
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
		this.enterRule(_localctx, 34, CHeaderParser.RULE_expr);
		try {
			this.state = 229;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.ROUND_B:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 227;
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
				this.state = 228;
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
		this.enterRule(_localctx, 36, CHeaderParser.RULE_word);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 231;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || ((((_la - 37)) & ~0x1F) === 0 && ((1 << (_la - 37)) & ((1 << (CHeaderParser.STRING - 37)) | (1 << (CHeaderParser.ID - 37)) | (1 << (CHeaderParser.NUMBER - 37)))) !== 0))) {
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
		this.enterRule(_localctx, 38, CHeaderParser.RULE_punct);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 233;
			_la = this._input.LA(1);
			if (!(((((_la - 29)) & ~0x1F) === 0 && ((1 << (_la - 29)) & ((1 << (CHeaderParser.COLON - 29)) | (1 << (CHeaderParser.SEMI - 29)) | (1 << (CHeaderParser.COMMA - 29)) | (1 << (CHeaderParser.DOT - 29)) | (1 << (CHeaderParser.STAR - 29)) | (1 << (CHeaderParser.EQ - 29)) | (1 << (CHeaderParser.ELLIPSIS - 29)) | (1 << (CHeaderParser.PUNCT_EXTRA - 29)))) !== 0))) {
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
		this.enterRule(_localctx, 40, CHeaderParser.RULE_enumBody);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 235;
			this.match(CHeaderParser.CURLY_B);
			this.state = 239;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC || _la === CHeaderParser.ID) {
				{
				{
				this.state = 236;
				this.enumItem();
				}
				}
				this.state = 241;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 242;
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
		this.enterRule(_localctx, 42, CHeaderParser.RULE_enumItem);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 245;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 244;
				this.doc();
				}
			}

			this.state = 247;
			this.id();
			this.state = 250;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.EQ) {
				{
				this.state = 248;
				this.match(CHeaderParser.EQ);
				this.state = 249;
				this.expr();
				}
			}

			this.state = 253;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.COMMA) {
				{
				this.state = 252;
				this.match(CHeaderParser.COMMA);
				}
			}

			this.state = 256;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 255;
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
		this.enterRule(_localctx, 44, CHeaderParser.RULE_structBody);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 258;
			this.match(CHeaderParser.CURLY_B);
			this.state = 263;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID))) !== 0) || _la === CHeaderParser.ID) {
				{
				this.state = 261;
				this._errHandler.sync(this);
				switch ( this.interpreter.adaptivePredict(this._input, 27, this._ctx) ) {
				case 1:
					{
					this.state = 259;
					this.structItem();
					}
					break;

				case 2:
					{
					this.state = 260;
					this.unionInlineType();
					}
					break;
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
	public structItem(): StructItemContext {
		let _localctx: StructItemContext = new StructItemContext(this._ctx, this.state);
		this.enterRule(_localctx, 46, CHeaderParser.RULE_structItem);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 269;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 268;
				this.doc();
				}
			}

			this.state = 275;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 31, this._ctx) ) {
			case 1:
				{
				this.state = 272;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				if (_la === CHeaderParser.CONST) {
					{
					this.state = 271;
					this.match(CHeaderParser.CONST);
					}
				}

				this.state = 274;
				this.match(CHeaderParser.STRUCT);
				}
				break;
			}
			this.state = 277;
			this.type();
			this.state = 278;
			this.id();
			this.state = 283;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.COMMA) {
				{
				{
				this.state = 279;
				this.match(CHeaderParser.COMMA);
				this.state = 280;
				this.id();
				}
				}
				this.state = 285;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 289;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.SQUARE_B) {
				{
				{
				this.state = 286;
				this.indexing();
				}
				}
				this.state = 291;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 292;
			this.match(CHeaderParser.SEMI);
			this.state = 294;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 293;
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
		this.enterRule(_localctx, 48, CHeaderParser.RULE_unionInlineType);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 297;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 296;
				this.doc();
				}
			}

			this.state = 299;
			this.match(CHeaderParser.UNION);
			this.state = 300;
			this.block();
			this.state = 301;
			this.id();
			this.state = 302;
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
		this.enterRule(_localctx, 50, CHeaderParser.RULE_id);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 304;
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
		this.enterRule(_localctx, 52, CHeaderParser.RULE_type);
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 307;
			this._errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					this.state = 306;
					this.typeEl();
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				this.state = 309;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 36, this._ctx);
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
		this.enterRule(_localctx, 54, CHeaderParser.RULE_typeEl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 311;
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
			this.state = 316;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.SQUARE_B || _la === CHeaderParser.STAR) {
				{
				this.state = 314;
				this._errHandler.sync(this);
				switch (this._input.LA(1)) {
				case CHeaderParser.STAR:
					{
					this.state = 312;
					this.match(CHeaderParser.STAR);
					}
					break;
				case CHeaderParser.SQUARE_B:
					{
					this.state = 313;
					this.indexing();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				this.state = 318;
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
		this.enterRule(_localctx, 56, CHeaderParser.RULE_signature);
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 319;
			this.match(CHeaderParser.ROUND_B);
			this.state = 328;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (((((_la - 10)) & ~0x1F) === 0 && ((1 << (_la - 10)) & ((1 << (CHeaderParser.CONST - 10)) | (1 << (CHeaderParser.VOID - 10)) | (1 << (CHeaderParser.ID - 10)))) !== 0)) {
				{
				this.state = 320;
				this.type();
				this.state = 325;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 39, this._ctx);
				while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
					if (_alt === 1) {
						{
						{
						this.state = 321;
						this.match(CHeaderParser.COMMA);
						this.state = 322;
						this.type();
						}
						}
					}
					this.state = 327;
					this._errHandler.sync(this);
					_alt = this.interpreter.adaptivePredict(this._input, 39, this._ctx);
				}
				}
			}

			this.state = 332;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.COMMA) {
				{
				this.state = 330;
				this.match(CHeaderParser.COMMA);
				this.state = 331;
				this.match(CHeaderParser.ELLIPSIS);
				}
			}

			this.state = 334;
			this.match(CHeaderParser.ROUND_E);
			this.state = 336;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.ATTRIBUTE || _la === CHeaderParser.SDL_VARARG_ATTRIB) {
				{
				this.state = 335;
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
		this.enterRule(_localctx, 58, CHeaderParser.RULE_attribute);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 338;
			_la = this._input.LA(1);
			if (!(_la === CHeaderParser.ATTRIBUTE || _la === CHeaderParser.SDL_VARARG_ATTRIB)) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			this.state = 339;
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
		this.enterRule(_localctx, 60, CHeaderParser.RULE_doc);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 341;
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
		this.enterRule(_localctx, 62, CHeaderParser.RULE_trailingDoc);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 343;
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
		"\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03)\u015C\x04\x02" +
		"\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
		"\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r\t\r\x04" +
		"\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x04\x11\t\x11\x04\x12\t\x12\x04" +
		"\x13\t\x13\x04\x14\t\x14\x04\x15\t\x15\x04\x16\t\x16\x04\x17\t\x17\x04" +
		"\x18\t\x18\x04\x19\t\x19\x04\x1A\t\x1A\x04\x1B\t\x1B\x04\x1C\t\x1C\x04" +
		"\x1D\t\x1D\x04\x1E\t\x1E\x04\x1F\t\x1F\x04 \t \x04!\t!\x03\x02\x05\x02" +
		"D\n\x02\x03\x02\x07\x02G\n\x02\f\x02\x0E\x02J\v\x02\x03\x02\x03\x02\x03" +
		"\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03" +
		"\x03\x03\x03\x05\x03Y\n\x03\x03\x04\x03\x04\x03\x04\x03\x04\x07\x04_\n" +
		"\x04\f\x04\x0E\x04b\v\x04\x03\x04\x03\x04\x03\x05\x05\x05g\n\x05\x03\x05" +
		"\x03\x05\x03\x06\x05\x06l\n\x06\x03\x06\x05\x06o\n\x06\x03\x06\x03\x06" +
		"\x05\x06s\n\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x07\x05\x07z\n\x07" +
		"\x03\x07\x03\x07\x03\x07\x05\x07\x7F\n\x07\x03\x07\x03\x07\x03\x07\x03" +
		"\x07\x03\b\x05\b\x86\n\b\x03\b\x03\b\x05\b\x8A\n\b\x03\b\x03\b\x03\b\x03" +
		"\b\x03\t\x05\t\x91\n\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\n" +
		"\x05\n\x9B\n\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\v\x05\v\xA5" +
		"\n\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\f\x05\f\xAF\n\f\x03" +
		"\f\x03\f\x03\f\x03\f\x03\f\x03\f\x03\f\x03\f\x03\f\x03\r\x03\r\x03\r\x03" +
		"\r\x03\x0E\x03\x0E\x03\x0E\x05\x0E\xC1\n\x0E\x03\x0F\x03\x0F\x07\x0F\xC5" +
		"\n\x0F\f\x0F\x0E\x0F\xC8\v\x0F\x03\x0F\x03\x0F\x03\x10\x03\x10\x07\x10" +
		"\xCE\n\x10\f\x10\x0E\x10\xD1\v\x10\x03\x10\x03\x10\x03\x11\x03\x11\x07" +
		"\x11\xD7\n\x11\f\x11\x0E\x11\xDA\v\x11\x03\x11\x03\x11\x03\x12\x03\x12" +
		"\x03\x12\x03\x12\x03\x12\x03\x12\x05\x12\xE4\n\x12\x03\x13\x03\x13\x05" +
		"\x13\xE8\n\x13\x03\x14\x03\x14\x03\x15\x03\x15\x03\x16\x03\x16\x07\x16" +
		"\xF0\n\x16\f\x16\x0E\x16\xF3\v\x16\x03\x16\x03\x16\x03\x17\x05\x17\xF8" +
		"\n\x17\x03\x17\x03\x17\x03\x17\x05\x17\xFD\n\x17\x03\x17\x05\x17\u0100" +
		"\n\x17\x03\x17\x05\x17\u0103\n\x17\x03\x18\x03\x18\x03\x18\x07\x18\u0108" +
		"\n\x18\f\x18\x0E\x18\u010B\v\x18\x03\x18\x03\x18\x03\x19\x05\x19\u0110" +
		"\n\x19\x03\x19\x05\x19\u0113\n\x19\x03\x19\x05\x19\u0116\n\x19\x03\x19" +
		"\x03\x19\x03\x19\x03\x19\x07\x19\u011C\n\x19\f\x19\x0E\x19\u011F\v\x19" +
		"\x03\x19\x07\x19\u0122\n\x19\f\x19\x0E\x19\u0125\v\x19\x03\x19\x03\x19" +
		"\x05\x19\u0129\n\x19\x03\x1A\x05\x1A\u012C\n\x1A\x03\x1A\x03\x1A\x03\x1A" +
		"\x03\x1A\x03\x1A\x03\x1B\x03\x1B\x03\x1C\x06\x1C\u0136\n\x1C\r\x1C\x0E" +
		"\x1C\u0137\x03\x1D\x03\x1D\x03\x1D\x07\x1D\u013D\n\x1D\f\x1D\x0E\x1D\u0140" +
		"\v\x1D\x03\x1E\x03\x1E\x03\x1E\x03\x1E\x07\x1E\u0146\n\x1E\f\x1E\x0E\x1E" +
		"\u0149\v\x1E\x05\x1E\u014B\n\x1E\x03\x1E\x03\x1E\x05\x1E\u014F\n\x1E\x03" +
		"\x1E\x03\x1E\x05\x1E\u0153\n\x1E\x03\x1F\x03\x1F\x03\x1F\x03 \x03 \x03" +
		"!\x03!\x03!\x02\x02\x02\"\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E" +
		"\x02\x10\x02\x12\x02\x14\x02\x16\x02\x18\x02\x1A\x02\x1C\x02\x1E\x02 " +
		"\x02\"\x02$\x02&\x02(\x02*\x02,\x02.\x020\x022\x024\x026\x028\x02:\x02" +
		"<\x02>\x02@\x02\x02\b\x04\x02\x13\x13\x15\x15\b\x02\t\t\f\r\x12\x13\x15" +
		"\x16\x18\x18\')\x03\x02\x1F&\x05\x02\f\f\x16\x16((\x04\x02\v\v\x17\x17" +
		"\x03\x02\x07\b\x02\u0173\x02C\x03\x02\x02\x02\x04X\x03\x02\x02\x02\x06" +
		"Z\x03\x02\x02\x02\bf\x03\x02\x02\x02\nk\x03\x02\x02\x02\fy\x03\x02\x02" +
		"\x02\x0E\x85\x03\x02\x02\x02\x10\x90\x03\x02\x02\x02\x12\x9A\x03\x02\x02" +
		"\x02\x14\xA4\x03\x02\x02\x02\x16\xAE\x03\x02\x02\x02\x18\xB9\x03\x02\x02" +
		"\x02\x1A\xC0\x03\x02\x02\x02\x1C\xC2\x03\x02\x02\x02\x1E\xCB\x03\x02\x02" +
		"\x02 \xD4\x03\x02\x02\x02\"\xE3\x03\x02\x02\x02$\xE7\x03\x02\x02\x02&" +
		"\xE9\x03\x02\x02\x02(\xEB\x03\x02\x02\x02*\xED\x03\x02\x02\x02,\xF7\x03" +
		"\x02\x02\x02.\u0104\x03\x02\x02\x020\u010F\x03\x02\x02\x022\u012B\x03" +
		"\x02\x02\x024\u0132\x03\x02\x02\x026\u0135\x03\x02\x02\x028\u0139\x03" +
		"\x02\x02\x02:\u0141\x03\x02\x02\x02<\u0154\x03\x02\x02\x02>\u0157\x03" +
		"\x02\x02\x02@\u0159\x03\x02\x02\x02BD\x05> \x02CB\x03\x02\x02\x02CD\x03" +
		"\x02\x02\x02DH\x03\x02\x02\x02EG\x05\x04\x03\x02FE\x03\x02\x02\x02GJ\x03" +
		"\x02\x02\x02HF\x03\x02\x02\x02HI\x03\x02\x02\x02IK\x03\x02\x02\x02JH\x03" +
		"\x02\x02\x02KL\x07\x02\x02\x03L\x03\x03\x02\x02\x02MY\x05\b\x05\x02NY" +
		"\x05\x06\x04\x02OY\x05\n\x06\x02PY\x05\f\x07\x02QY\x05\x0E\b\x02RY\x05" +
		"\x10\t\x02SY\x05\x12\n\x02TY\x05\x14\v\x02UY\x05\x16\f\x02VY\x05\x18\r" +
		"\x02WY\x05> \x02XM\x03\x02\x02\x02XN\x03\x02\x02\x02XO\x03\x02\x02\x02" +
		"XP\x03\x02\x02\x02XQ\x03\x02\x02\x02XR\x03\x02\x02\x02XS\x03\x02\x02\x02" +
		"XT\x03\x02\x02\x02XU\x03\x02\x02\x02XV\x03\x02\x02\x02XW\x03\x02\x02\x02" +
		"Y\x05\x03\x02\x02\x02Z[\x07\x0E\x02\x02[\\\x07\'\x02\x02\\`\x07\x19\x02" +
		"\x02]_\x05\x04\x03\x02^]\x03\x02\x02\x02_b\x03\x02\x02\x02`^\x03\x02\x02" +
		"\x02`a\x03\x02\x02\x02ac\x03\x02\x02\x02b`\x03\x02\x02\x02cd\x07\x1A\x02" +
		"\x02d\x07\x03\x02\x02\x02eg\x05> \x02fe\x03\x02\x02\x02fg\x03\x02\x02" +
		"\x02gh\x03\x02\x02\x02hi\x07\t\x02\x02i\t\x03\x02\x02\x02jl\x05> \x02" +
		"kj\x03\x02\x02\x02kl\x03\x02\x02\x02ln\x03\x02\x02\x02mo\x07\x0E\x02\x02" +
		"nm\x03\x02\x02\x02no\x03\x02\x02\x02op\x03\x02\x02\x02pr\x056\x1C\x02" +
		"qs\x05<\x1F\x02rq\x03\x02\x02\x02rs\x03\x02\x02\x02st\x03\x02\x02\x02" +
		"tu\x054\x1B\x02uv\x05:\x1E\x02vw\x07 \x02\x02w\v\x03\x02\x02\x02xz\x05" +
		"> \x02yx\x03\x02\x02\x02yz\x03\x02\x02\x02z{\x03\x02\x02\x02{|\x05\x1A" +
		"\x0E\x02|~\x056\x1C\x02}\x7F\x05<\x1F\x02~}\x03\x02\x02\x02~\x7F\x03\x02" +
		"\x02\x02\x7F\x80\x03\x02\x02\x02\x80\x81\x054\x1B\x02\x81\x82\x05:\x1E" +
		"\x02\x82\x83\x05\x1C\x0F\x02\x83\r\x03\x02\x02\x02\x84\x86\x05> \x02\x85" +
		"\x84\x03\x02\x02\x02\x85\x86\x03\x02\x02\x02\x86\x87\x03\x02\x02\x02\x87" +
		"\x89\x07\x14\x02\x02\x88\x8A\t\x02\x02\x02\x89\x88\x03\x02\x02\x02\x89" +
		"\x8A\x03\x02\x02\x02\x8A\x8B\x03\x02\x02\x02\x8B\x8C\x056\x1C\x02\x8C" +
		"\x8D\x054\x1B\x02\x8D\x8E\x07 \x02\x02\x8E\x0F\x03\x02\x02\x02\x8F\x91" +
		"\x05> \x02\x90\x8F\x03\x02\x02\x02\x90\x91\x03\x02\x02\x02\x91\x92\x03" +
		"\x02\x02\x02\x92\x93\x07\x14\x02\x02\x93\x94\x07\x15\x02\x02\x94\x95\x05" +
		"4\x1B\x02\x95\x96\x05\x1C\x0F\x02\x96\x97\x054\x1B\x02\x97\x98\x07 \x02" +
		"\x02\x98\x11\x03\x02\x02\x02\x99\x9B\x05> \x02\x9A\x99\x03\x02\x02\x02" +
		"\x9A\x9B\x03\x02\x02\x02\x9B\x9C\x03\x02\x02\x02\x9C\x9D\x07\x14\x02\x02" +
		"\x9D\x9E\x07\r\x02\x02\x9E\x9F\x054\x1B\x02\x9F\xA0\x05*\x16\x02\xA0\xA1" +
		"\x054\x1B\x02\xA1\xA2\x07 \x02\x02\xA2\x13\x03\x02\x02\x02\xA3\xA5\x05" +
		"> \x02\xA4\xA3\x03\x02\x02\x02\xA4\xA5\x03\x02\x02\x02\xA5\xA6\x03\x02" +
		"\x02\x02\xA6\xA7\x07\x14\x02\x02\xA7\xA8\x07\x13\x02\x02\xA8\xA9\x054" +
		"\x1B\x02\xA9\xAA\x05.\x18\x02\xAA\xAB\x054\x1B\x02\xAB\xAC\x07 \x02\x02" +
		"\xAC\x15\x03\x02\x02\x02\xAD\xAF\x05> \x02\xAE\xAD\x03\x02\x02\x02\xAE" +
		"\xAF\x03\x02\x02\x02\xAF\xB0\x03\x02\x02\x02\xB0\xB1\x07\x14\x02\x02\xB1" +
		"\xB2\x056\x1C\x02\xB2\xB3\x07\x1B\x02\x02\xB3\xB4\x07#\x02\x02\xB4\xB5" +
		"\x054\x1B\x02\xB5\xB6\x07\x1C\x02\x02\xB6\xB7\x05:\x1E\x02\xB7\xB8\x07" +
		" \x02\x02\xB8\x17\x03\x02\x02\x02\xB9\xBA\x07\x18\x02\x02\xBA\xBB\x05" +
		"\x1E\x10\x02\xBB\xBC\x07 \x02\x02\xBC\x19\x03\x02\x02\x02\xBD\xC1\x07" +
		"\x11\x02\x02\xBE\xBF\x07\x12\x02\x02\xBF\xC1\x07\x0F\x02\x02\xC0\xBD\x03" +
		"\x02\x02\x02\xC0\xBE\x03\x02\x02\x02\xC1\x1B\x03\x02\x02\x02\xC2\xC6\x07" +
		"\x19\x02\x02\xC3\xC5\x05\"\x12\x02\xC4\xC3\x03\x02\x02\x02\xC5\xC8\x03" +
		"\x02\x02\x02\xC6\xC4\x03\x02\x02\x02\xC6\xC7\x03\x02\x02\x02\xC7\xC9\x03" +
		"\x02\x02\x02\xC8\xC6\x03\x02\x02\x02\xC9\xCA\x07\x1A\x02\x02\xCA\x1D\x03" +
		"\x02\x02\x02\xCB\xCF\x07\x1B\x02\x02\xCC\xCE\x05\"\x12\x02\xCD\xCC\x03" +
		"\x02\x02\x02\xCE\xD1\x03\x02\x02\x02\xCF\xCD\x03\x02\x02\x02\xCF\xD0\x03" +
		"\x02\x02\x02\xD0\xD2\x03\x02\x02\x02\xD1\xCF\x03\x02\x02\x02\xD2\xD3\x07" +
		"\x1C\x02\x02\xD3\x1F\x03\x02\x02\x02\xD4\xD8\x07\x1D\x02\x02\xD5\xD7\x05" +
		"\"\x12\x02\xD6\xD5\x03\x02\x02\x02\xD7\xDA\x03\x02\x02\x02\xD8\xD6\x03" +
		"\x02\x02\x02\xD8\xD9\x03\x02\x02\x02\xD9\xDB\x03\x02\x02\x02\xDA\xD8\x03" +
		"\x02\x02\x02\xDB\xDC\x07\x1E\x02\x02\xDC!\x03\x02\x02\x02\xDD\xE4\x05" +
		"\x1C\x0F\x02\xDE\xE4\x05 \x11\x02\xDF\xE4\x05$\x13\x02\xE0\xE4\x05(\x15" +
		"\x02\xE1\xE4\x05> \x02\xE2\xE4\x05@!\x02\xE3\xDD\x03\x02\x02\x02\xE3\xDE" +
		"\x03\x02\x02\x02\xE3\xDF\x03\x02\x02\x02\xE3\xE0\x03\x02\x02\x02\xE3\xE1" +
		"\x03\x02\x02\x02\xE3\xE2\x03\x02\x02\x02\xE4#\x03\x02\x02\x02\xE5\xE8" +
		"\x05\x1E\x10\x02\xE6\xE8\x05&\x14\x02\xE7\xE5\x03\x02\x02\x02\xE7\xE6" +
		"\x03\x02\x02\x02\xE8%\x03\x02\x02\x02\xE9\xEA\t\x03\x02\x02\xEA\'\x03" +
		"\x02\x02\x02\xEB\xEC\t\x04\x02\x02\xEC)\x03\x02\x02\x02\xED\xF1\x07\x19" +
		"\x02\x02\xEE\xF0\x05,\x17\x02\xEF\xEE\x03\x02\x02\x02\xF0\xF3\x03\x02" +
		"\x02\x02\xF1\xEF\x03\x02\x02\x02\xF1\xF2\x03\x02\x02\x02\xF2\xF4\x03\x02" +
		"\x02\x02\xF3\xF1\x03\x02\x02\x02\xF4\xF5\x07\x1A\x02\x02\xF5+\x03\x02" +
		"\x02\x02\xF6\xF8\x05> \x02\xF7\xF6\x03\x02\x02\x02\xF7\xF8\x03\x02\x02" +
		"\x02\xF8\xF9\x03\x02\x02\x02\xF9\xFC\x054\x1B\x02\xFA\xFB\x07$\x02\x02" +
		"\xFB\xFD\x05$\x13\x02\xFC\xFA\x03\x02\x02\x02\xFC\xFD\x03\x02\x02\x02" +
		"\xFD\xFF\x03\x02\x02\x02\xFE\u0100\x07!\x02\x02\xFF\xFE\x03\x02\x02\x02" +
		"\xFF\u0100\x03\x02\x02\x02\u0100\u0102\x03\x02\x02\x02\u0101\u0103\x05" +
		"@!\x02\u0102\u0101\x03\x02\x02\x02\u0102\u0103\x03\x02\x02\x02\u0103-" +
		"\x03\x02\x02\x02\u0104\u0109\x07\x19\x02\x02\u0105\u0108\x050\x19\x02" +
		"\u0106\u0108\x052\x1A\x02\u0107\u0105\x03\x02\x02\x02\u0107\u0106\x03" +
		"\x02\x02\x02\u0108\u010B\x03\x02\x02\x02\u0109\u0107\x03\x02\x02\x02\u0109" +
		"\u010A\x03\x02\x02\x02\u010A\u010C\x03\x02\x02\x02\u010B\u0109\x03\x02" +
		"\x02\x02\u010C\u010D\x07\x1A\x02\x02\u010D/\x03\x02\x02\x02\u010E\u0110" +
		"\x05> \x02\u010F\u010E\x03\x02\x02\x02\u010F\u0110\x03\x02\x02\x02\u0110" +
		"\u0115\x03\x02\x02\x02\u0111\u0113\x07\f\x02\x02\u0112\u0111\x03\x02\x02" +
		"\x02\u0112\u0113\x03\x02\x02\x02\u0113\u0114\x03\x02\x02\x02\u0114\u0116" +
		"\x07\x13\x02\x02\u0115\u0112\x03\x02\x02\x02\u0115\u0116\x03\x02\x02\x02" +
		"\u0116\u0117\x03\x02\x02\x02\u0117\u0118\x056\x1C\x02\u0118\u011D\x05" +
		"4\x1B\x02\u0119\u011A\x07!\x02\x02\u011A\u011C\x054\x1B\x02\u011B\u0119" +
		"\x03\x02\x02\x02\u011C\u011F\x03\x02\x02\x02\u011D\u011B\x03\x02\x02\x02" +
		"\u011D\u011E\x03\x02\x02\x02\u011E\u0123\x03\x02\x02\x02\u011F\u011D\x03" +
		"\x02\x02\x02\u0120\u0122\x05 \x11\x02\u0121\u0120\x03\x02\x02\x02\u0122" +
		"\u0125\x03\x02\x02\x02\u0123\u0121\x03\x02\x02\x02\u0123\u0124\x03\x02" +
		"\x02\x02\u0124\u0126\x03\x02\x02\x02\u0125\u0123\x03\x02\x02\x02\u0126" +
		"\u0128\x07 \x02\x02\u0127\u0129\x05@!\x02\u0128\u0127\x03\x02\x02\x02" +
		"\u0128\u0129\x03\x02\x02\x02\u01291\x03\x02\x02\x02\u012A\u012C\x05> " +
		"\x02\u012B\u012A\x03\x02\x02\x02\u012B\u012C\x03\x02\x02\x02\u012C\u012D" +
		"\x03\x02\x02\x02\u012D\u012E\x07\x15\x02\x02\u012E\u012F\x05\x1C\x0F\x02" +
		"\u012F\u0130\x054\x1B\x02\u0130\u0131\x07 \x02\x02\u01313\x03\x02\x02" +
		"\x02\u0132\u0133\x07(\x02\x02\u01335\x03\x02\x02\x02\u0134\u0136\x058" +
		"\x1D\x02\u0135\u0134\x03\x02\x02\x02\u0136\u0137\x03\x02\x02\x02\u0137" +
		"\u0135\x03\x02\x02\x02\u0137\u0138\x03\x02\x02\x02\u01387\x03\x02\x02" +
		"\x02\u0139\u013E\t\x05\x02\x02\u013A\u013D\x07#\x02\x02\u013B\u013D\x05" +
		" \x11\x02\u013C\u013A\x03\x02\x02\x02\u013C\u013B\x03\x02\x02\x02\u013D" +
		"\u0140\x03\x02\x02\x02\u013E\u013C\x03\x02\x02\x02\u013E\u013F\x03\x02" +
		"\x02\x02\u013F9\x03\x02\x02\x02\u0140\u013E\x03\x02\x02\x02\u0141\u014A" +
		"\x07\x1B\x02\x02\u0142\u0147\x056\x1C\x02\u0143\u0144\x07!\x02\x02\u0144" +
		"\u0146\x056\x1C\x02\u0145\u0143\x03\x02\x02\x02\u0146\u0149\x03\x02\x02" +
		"\x02\u0147\u0145\x03\x02\x02\x02\u0147\u0148\x03\x02\x02\x02\u0148\u014B" +
		"\x03\x02\x02\x02\u0149\u0147\x03\x02\x02\x02\u014A\u0142\x03\x02\x02\x02" +
		"\u014A\u014B\x03\x02\x02\x02\u014B\u014E\x03\x02\x02\x02\u014C\u014D\x07" +
		"!\x02\x02\u014D\u014F\x07%\x02\x02\u014E\u014C\x03\x02\x02\x02\u014E\u014F" +
		"\x03\x02\x02\x02\u014F\u0150\x03\x02\x02\x02\u0150\u0152\x07\x1C\x02\x02" +
		"\u0151\u0153\x05<\x1F\x02\u0152\u0151\x03\x02\x02\x02\u0152\u0153\x03" +
		"\x02\x02\x02\u0153;\x03\x02\x02\x02\u0154\u0155\t\x06\x02\x02\u0155\u0156" +
		"\x05\x1E\x10\x02\u0156=\x03\x02\x02\x02\u0157\u0158\t\x07\x02\x02\u0158" +
		"?\x03\x02\x02\x02\u0159\u015A\x07\x06\x02\x02\u015AA\x03\x02\x02\x02-" +
		"CHX`fknry~\x85\x89\x90\x9A\xA4\xAE\xC0\xC6\xCF\xD8\xE3\xE7\xF1\xF7\xFC" +
		"\xFF\u0102\u0107\u0109\u010F\u0112\u0115\u011D\u0123\u0128\u012B\u0137" +
		"\u013C\u013E\u0147\u014A\u014E\u0152";
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
	public TYPEDEF(): TerminalNode { return this.getToken(CHeaderParser.TYPEDEF, 0); }
	public STRUCT(): TerminalNode { return this.getToken(CHeaderParser.STRUCT, 0); }
	public id(): IdContext[];
	public id(i: number): IdContext;
	public id(i?: number): IdContext | IdContext[] {
		if (i === undefined) {
			return this.getRuleContexts(IdContext);
		} else {
			return this.getRuleContext(i, IdContext);
		}
	}
	public structBody(): StructBodyContext {
		return this.getRuleContext(0, StructBodyContext);
	}
	public SEMI(): TerminalNode { return this.getToken(CHeaderParser.SEMI, 0); }
	public doc(): DocContext | undefined {
		return this.tryGetRuleContext(0, DocContext);
	}
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


