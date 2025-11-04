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
	public static readonly ENUM = 10;
	public static readonly EXTERN = 11;
	public static readonly INLINE = 12;
	public static readonly SDL_NOISE = 13;
	public static readonly SDL_INLINE = 14;
	public static readonly STATIC = 15;
	public static readonly STRUCT = 16;
	public static readonly TYPEDEF = 17;
	public static readonly UNION = 18;
	public static readonly VOID = 19;
	public static readonly CURLY_B = 20;
	public static readonly CURLY_E = 21;
	public static readonly ROUND_B = 22;
	public static readonly ROUND_E = 23;
	public static readonly SQUARE_B = 24;
	public static readonly SQUARE_E = 25;
	public static readonly COLON = 26;
	public static readonly SEMI = 27;
	public static readonly COMMA = 28;
	public static readonly DOT = 29;
	public static readonly STAR = 30;
	public static readonly EQ = 31;
	public static readonly PUNCT_EXTRA = 32;
	public static readonly STRING = 33;
	public static readonly ID = 34;
	public static readonly NUMBER = 35;
	public static readonly RULE_prog = 0;
	public static readonly RULE_decl = 1;
	public static readonly RULE_externC = 2;
	public static readonly RULE_directive = 3;
	public static readonly RULE_functionDecl = 4;
	public static readonly RULE_functionDef = 5;
	public static readonly RULE_aliasDef = 6;
	public static readonly RULE_enumDef = 7;
	public static readonly RULE_structDef = 8;
	public static readonly RULE_callbackDef = 9;
	public static readonly RULE_inline = 10;
	public static readonly RULE_block = 11;
	public static readonly RULE_group = 12;
	public static readonly RULE_indexing = 13;
	public static readonly RULE_stm = 14;
	public static readonly RULE_expr = 15;
	public static readonly RULE_word = 16;
	public static readonly RULE_punct = 17;
	public static readonly RULE_enumBody = 18;
	public static readonly RULE_enumItem = 19;
	public static readonly RULE_structBody = 20;
	public static readonly RULE_structItem = 21;
	public static readonly RULE_id = 22;
	public static readonly RULE_type = 23;
	public static readonly RULE_typeEl = 24;
	public static readonly RULE_signature = 25;
	public static readonly RULE_attribute = 26;
	public static readonly RULE_doc = 27;
	public static readonly RULE_trailingDoc = 28;
	// tslint:disable:no-trailing-whitespace
	public static readonly ruleNames: string[] = [
		"prog", "decl", "externC", "directive", "functionDecl", "functionDef", 
		"aliasDef", "enumDef", "structDef", "callbackDef", "inline", "block", 
		"group", "indexing", "stm", "expr", "word", "punct", "enumBody", "enumItem", 
		"structBody", "structItem", "id", "type", "typeEl", "signature", "attribute", 
		"doc", "trailingDoc",
	];

	private static readonly _LITERAL_NAMES: Array<string | undefined> = [
		undefined, undefined, undefined, undefined, undefined, undefined, undefined, 
		undefined, undefined, "'__attribute__'", "'enum'", "'extern'", "'__inline__'", 
		undefined, "'SDL_FORCE_INLINE'", "'static'", "'struct'", "'typedef'", 
		"'union'", "'void'", "'{'", "'}'", "'('", "')'", "'['", "']'", "':'", 
		"';'", "','", "'.'", "'*'", "'='",
	];
	private static readonly _SYMBOLIC_NAMES: Array<string | undefined> = [
		undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "TRAILING_DOC", "LONG_DOC", 
		"SHORT_DOC", "DEFINE", "DIRECTIVE", "ATTRIBUTE", "ENUM", "EXTERN", "INLINE", 
		"SDL_NOISE", "SDL_INLINE", "STATIC", "STRUCT", "TYPEDEF", "UNION", "VOID", 
		"CURLY_B", "CURLY_E", "ROUND_B", "ROUND_E", "SQUARE_B", "SQUARE_E", "COLON", 
		"SEMI", "COMMA", "DOT", "STAR", "EQ", "PUNCT_EXTRA", "STRING", "ID", "NUMBER",
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
			this.state = 59;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 0, this._ctx) ) {
			case 1:
				{
				this.state = 58;
				this.doc();
				}
				break;
			}
			this.state = 64;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF))) !== 0)) {
				{
				{
				this.state = 61;
				this.decl();
				}
				}
				this.state = 66;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 67;
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
			this.state = 78;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 2, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 69;
				this.directive();
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 70;
				this.externC();
				}
				break;

			case 3:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 71;
				this.functionDecl();
				}
				break;

			case 4:
				this.enterOuterAlt(_localctx, 4);
				{
				this.state = 72;
				this.functionDef();
				}
				break;

			case 5:
				this.enterOuterAlt(_localctx, 5);
				{
				this.state = 73;
				this.aliasDef();
				}
				break;

			case 6:
				this.enterOuterAlt(_localctx, 6);
				{
				this.state = 74;
				this.enumDef();
				}
				break;

			case 7:
				this.enterOuterAlt(_localctx, 7);
				{
				this.state = 75;
				this.structDef();
				}
				break;

			case 8:
				this.enterOuterAlt(_localctx, 8);
				{
				this.state = 76;
				this.callbackDef();
				}
				break;

			case 9:
				this.enterOuterAlt(_localctx, 9);
				{
				this.state = 77;
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
			this.state = 80;
			this.match(CHeaderParser.EXTERN);
			this.state = 81;
			this.match(CHeaderParser.STRING);
			this.state = 82;
			this.match(CHeaderParser.CURLY_B);
			this.state = 86;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF))) !== 0)) {
				{
				{
				this.state = 83;
				this.decl();
				}
				}
				this.state = 88;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 89;
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
			this.state = 92;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 91;
				this.doc();
				}
			}

			this.state = 94;
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
			this.state = 97;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 96;
				this.doc();
				}
			}

			this.state = 99;
			this.match(CHeaderParser.EXTERN);
			this.state = 100;
			this.type();
			this.state = 102;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.ATTRIBUTE) {
				{
				this.state = 101;
				this.attribute();
				}
			}

			this.state = 104;
			this.id();
			this.state = 105;
			this.signature();
			this.state = 106;
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
			this.state = 109;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 108;
				this.doc();
				}
			}

			this.state = 111;
			this.inline();
			this.state = 112;
			this.type();
			this.state = 114;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.ATTRIBUTE) {
				{
				this.state = 113;
				this.attribute();
				}
			}

			this.state = 116;
			this.id();
			this.state = 117;
			this.signature();
			this.state = 118;
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
			this.state = 121;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 120;
				this.doc();
				}
			}

			this.state = 123;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 125;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.STRUCT || _la === CHeaderParser.UNION) {
				{
				this.state = 124;
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

			this.state = 127;
			this.type();
			this.state = 128;
			this.id();
			this.state = 129;
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
		this.enterRule(_localctx, 14, CHeaderParser.RULE_enumDef);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 132;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 131;
				this.doc();
				}
			}

			this.state = 134;
			this.match(CHeaderParser.TYPEDEF);
			this.state = 135;
			this.match(CHeaderParser.ENUM);
			this.state = 136;
			this.id();
			this.state = 137;
			this.enumBody();
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
	public structDef(): StructDefContext {
		let _localctx: StructDefContext = new StructDefContext(this._ctx, this.state);
		this.enterRule(_localctx, 16, CHeaderParser.RULE_structDef);
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
			this.match(CHeaderParser.STRUCT);
			this.state = 146;
			this.id();
			this.state = 147;
			this.structBody();
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
	public callbackDef(): CallbackDefContext {
		let _localctx: CallbackDefContext = new CallbackDefContext(this._ctx, this.state);
		this.enterRule(_localctx, 18, CHeaderParser.RULE_callbackDef);
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
			this.type();
			this.state = 156;
			this.match(CHeaderParser.ROUND_B);
			this.state = 157;
			this.match(CHeaderParser.STAR);
			this.state = 158;
			this.id();
			this.state = 159;
			this.match(CHeaderParser.ROUND_E);
			this.state = 160;
			this.signature();
			this.state = 161;
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
		this.enterRule(_localctx, 20, CHeaderParser.RULE_inline);
		try {
			this.state = 166;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.SDL_INLINE:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 163;
				this.match(CHeaderParser.SDL_INLINE);
				}
				break;
			case CHeaderParser.STATIC:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 164;
				this.match(CHeaderParser.STATIC);
				this.state = 165;
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
		this.enterRule(_localctx, 22, CHeaderParser.RULE_block);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 168;
			this.match(CHeaderParser.CURLY_B);
			this.state = 172;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (((((_la - 7)) & ~0x1F) === 0 && ((1 << (_la - 7)) & ((1 << (CHeaderParser.DEFINE - 7)) | (1 << (CHeaderParser.ENUM - 7)) | (1 << (CHeaderParser.STATIC - 7)) | (1 << (CHeaderParser.STRUCT - 7)) | (1 << (CHeaderParser.UNION - 7)) | (1 << (CHeaderParser.VOID - 7)) | (1 << (CHeaderParser.CURLY_B - 7)) | (1 << (CHeaderParser.ROUND_B - 7)) | (1 << (CHeaderParser.SQUARE_B - 7)) | (1 << (CHeaderParser.COLON - 7)) | (1 << (CHeaderParser.SEMI - 7)) | (1 << (CHeaderParser.COMMA - 7)) | (1 << (CHeaderParser.DOT - 7)) | (1 << (CHeaderParser.STAR - 7)) | (1 << (CHeaderParser.EQ - 7)) | (1 << (CHeaderParser.PUNCT_EXTRA - 7)) | (1 << (CHeaderParser.STRING - 7)) | (1 << (CHeaderParser.ID - 7)) | (1 << (CHeaderParser.NUMBER - 7)))) !== 0)) {
				{
				{
				this.state = 169;
				this.stm();
				}
				}
				this.state = 174;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 175;
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
		this.enterRule(_localctx, 24, CHeaderParser.RULE_group);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 177;
			this.match(CHeaderParser.ROUND_B);
			this.state = 181;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (((((_la - 7)) & ~0x1F) === 0 && ((1 << (_la - 7)) & ((1 << (CHeaderParser.DEFINE - 7)) | (1 << (CHeaderParser.ENUM - 7)) | (1 << (CHeaderParser.STATIC - 7)) | (1 << (CHeaderParser.STRUCT - 7)) | (1 << (CHeaderParser.UNION - 7)) | (1 << (CHeaderParser.VOID - 7)) | (1 << (CHeaderParser.CURLY_B - 7)) | (1 << (CHeaderParser.ROUND_B - 7)) | (1 << (CHeaderParser.SQUARE_B - 7)) | (1 << (CHeaderParser.COLON - 7)) | (1 << (CHeaderParser.SEMI - 7)) | (1 << (CHeaderParser.COMMA - 7)) | (1 << (CHeaderParser.DOT - 7)) | (1 << (CHeaderParser.STAR - 7)) | (1 << (CHeaderParser.EQ - 7)) | (1 << (CHeaderParser.PUNCT_EXTRA - 7)) | (1 << (CHeaderParser.STRING - 7)) | (1 << (CHeaderParser.ID - 7)) | (1 << (CHeaderParser.NUMBER - 7)))) !== 0)) {
				{
				{
				this.state = 178;
				this.stm();
				}
				}
				this.state = 183;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 184;
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
		this.enterRule(_localctx, 26, CHeaderParser.RULE_indexing);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 186;
			this.match(CHeaderParser.SQUARE_B);
			this.state = 190;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (((((_la - 7)) & ~0x1F) === 0 && ((1 << (_la - 7)) & ((1 << (CHeaderParser.DEFINE - 7)) | (1 << (CHeaderParser.ENUM - 7)) | (1 << (CHeaderParser.STATIC - 7)) | (1 << (CHeaderParser.STRUCT - 7)) | (1 << (CHeaderParser.UNION - 7)) | (1 << (CHeaderParser.VOID - 7)) | (1 << (CHeaderParser.CURLY_B - 7)) | (1 << (CHeaderParser.ROUND_B - 7)) | (1 << (CHeaderParser.SQUARE_B - 7)) | (1 << (CHeaderParser.COLON - 7)) | (1 << (CHeaderParser.SEMI - 7)) | (1 << (CHeaderParser.COMMA - 7)) | (1 << (CHeaderParser.DOT - 7)) | (1 << (CHeaderParser.STAR - 7)) | (1 << (CHeaderParser.EQ - 7)) | (1 << (CHeaderParser.PUNCT_EXTRA - 7)) | (1 << (CHeaderParser.STRING - 7)) | (1 << (CHeaderParser.ID - 7)) | (1 << (CHeaderParser.NUMBER - 7)))) !== 0)) {
				{
				{
				this.state = 187;
				this.stm();
				}
				}
				this.state = 192;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 193;
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
		this.enterRule(_localctx, 28, CHeaderParser.RULE_stm);
		try {
			this.state = 199;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.CURLY_B:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 195;
				this.block();
				}
				break;
			case CHeaderParser.SQUARE_B:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 196;
				this.indexing();
				}
				break;
			case CHeaderParser.DEFINE:
			case CHeaderParser.ENUM:
			case CHeaderParser.STATIC:
			case CHeaderParser.STRUCT:
			case CHeaderParser.UNION:
			case CHeaderParser.VOID:
			case CHeaderParser.ROUND_B:
			case CHeaderParser.STRING:
			case CHeaderParser.ID:
			case CHeaderParser.NUMBER:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 197;
				this.expr();
				}
				break;
			case CHeaderParser.COLON:
			case CHeaderParser.SEMI:
			case CHeaderParser.COMMA:
			case CHeaderParser.DOT:
			case CHeaderParser.STAR:
			case CHeaderParser.EQ:
			case CHeaderParser.PUNCT_EXTRA:
				this.enterOuterAlt(_localctx, 4);
				{
				this.state = 198;
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
	public expr(): ExprContext {
		let _localctx: ExprContext = new ExprContext(this._ctx, this.state);
		this.enterRule(_localctx, 30, CHeaderParser.RULE_expr);
		try {
			this.state = 203;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.ROUND_B:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 201;
				this.group();
				}
				break;
			case CHeaderParser.DEFINE:
			case CHeaderParser.ENUM:
			case CHeaderParser.STATIC:
			case CHeaderParser.STRUCT:
			case CHeaderParser.UNION:
			case CHeaderParser.VOID:
			case CHeaderParser.STRING:
			case CHeaderParser.ID:
			case CHeaderParser.NUMBER:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 202;
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
		this.enterRule(_localctx, 32, CHeaderParser.RULE_word);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 205;
			_la = this._input.LA(1);
			if (!(((((_la - 7)) & ~0x1F) === 0 && ((1 << (_la - 7)) & ((1 << (CHeaderParser.DEFINE - 7)) | (1 << (CHeaderParser.ENUM - 7)) | (1 << (CHeaderParser.STATIC - 7)) | (1 << (CHeaderParser.STRUCT - 7)) | (1 << (CHeaderParser.UNION - 7)) | (1 << (CHeaderParser.VOID - 7)) | (1 << (CHeaderParser.STRING - 7)) | (1 << (CHeaderParser.ID - 7)) | (1 << (CHeaderParser.NUMBER - 7)))) !== 0))) {
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
		this.enterRule(_localctx, 34, CHeaderParser.RULE_punct);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 207;
			_la = this._input.LA(1);
			if (!(((((_la - 26)) & ~0x1F) === 0 && ((1 << (_la - 26)) & ((1 << (CHeaderParser.COLON - 26)) | (1 << (CHeaderParser.SEMI - 26)) | (1 << (CHeaderParser.COMMA - 26)) | (1 << (CHeaderParser.DOT - 26)) | (1 << (CHeaderParser.STAR - 26)) | (1 << (CHeaderParser.EQ - 26)) | (1 << (CHeaderParser.PUNCT_EXTRA - 26)))) !== 0))) {
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
		this.enterRule(_localctx, 36, CHeaderParser.RULE_enumBody);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 209;
			this.match(CHeaderParser.CURLY_B);
			this.state = 213;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (((((_la - 5)) & ~0x1F) === 0 && ((1 << (_la - 5)) & ((1 << (CHeaderParser.LONG_DOC - 5)) | (1 << (CHeaderParser.SHORT_DOC - 5)) | (1 << (CHeaderParser.ID - 5)))) !== 0)) {
				{
				{
				this.state = 210;
				this.enumItem();
				}
				}
				this.state = 215;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 216;
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
		this.enterRule(_localctx, 38, CHeaderParser.RULE_enumItem);
		let _la: number;
		try {
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
			this.id();
			this.state = 224;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.EQ) {
				{
				this.state = 222;
				this.match(CHeaderParser.EQ);
				this.state = 223;
				this.expr();
				}
			}

			this.state = 227;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.COMMA) {
				{
				this.state = 226;
				this.match(CHeaderParser.COMMA);
				}
			}

			this.state = 230;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 229;
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
		this.enterRule(_localctx, 40, CHeaderParser.RULE_structBody);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 232;
			this.match(CHeaderParser.CURLY_B);
			this.state = 236;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (((((_la - 5)) & ~0x1F) === 0 && ((1 << (_la - 5)) & ((1 << (CHeaderParser.LONG_DOC - 5)) | (1 << (CHeaderParser.SHORT_DOC - 5)) | (1 << (CHeaderParser.STRUCT - 5)) | (1 << (CHeaderParser.VOID - 5)) | (1 << (CHeaderParser.ID - 5)))) !== 0)) {
				{
				{
				this.state = 233;
				this.structItem();
				}
				}
				this.state = 238;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 239;
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
		this.enterRule(_localctx, 42, CHeaderParser.RULE_structItem);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 242;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 241;
				this.doc();
				}
			}

			this.state = 245;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.STRUCT) {
				{
				this.state = 244;
				this.match(CHeaderParser.STRUCT);
				}
			}

			this.state = 247;
			this.type();
			this.state = 248;
			this.id();
			this.state = 253;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.COMMA) {
				{
				{
				this.state = 249;
				this.match(CHeaderParser.COMMA);
				this.state = 250;
				this.id();
				}
				}
				this.state = 255;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 259;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.SQUARE_B) {
				{
				{
				this.state = 256;
				this.indexing();
				}
				}
				this.state = 261;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 262;
			this.match(CHeaderParser.SEMI);
			this.state = 264;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.TRAILING_DOC) {
				{
				this.state = 263;
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
	public id(): IdContext {
		let _localctx: IdContext = new IdContext(this._ctx, this.state);
		this.enterRule(_localctx, 44, CHeaderParser.RULE_id);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 266;
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
		this.enterRule(_localctx, 46, CHeaderParser.RULE_type);
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 269;
			this._errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					this.state = 268;
					this.typeEl();
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				this.state = 271;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 31, this._ctx);
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
		this.enterRule(_localctx, 48, CHeaderParser.RULE_typeEl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 273;
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
			this.state = 277;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.STAR) {
				{
				{
				this.state = 274;
				this.match(CHeaderParser.STAR);
				}
				}
				this.state = 279;
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
		this.enterRule(_localctx, 50, CHeaderParser.RULE_signature);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 280;
			this.match(CHeaderParser.ROUND_B);
			this.state = 289;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.VOID || _la === CHeaderParser.ID) {
				{
				this.state = 281;
				this.type();
				this.state = 286;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la === CHeaderParser.COMMA) {
					{
					{
					this.state = 282;
					this.match(CHeaderParser.COMMA);
					this.state = 283;
					this.type();
					}
					}
					this.state = 288;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			this.state = 291;
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
		this.enterRule(_localctx, 52, CHeaderParser.RULE_attribute);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 293;
			this.match(CHeaderParser.ATTRIBUTE);
			this.state = 294;
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
		this.enterRule(_localctx, 54, CHeaderParser.RULE_doc);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 296;
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
		this.enterRule(_localctx, 56, CHeaderParser.RULE_trailingDoc);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 298;
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
		"\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03%\u012F\x04\x02" +
		"\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
		"\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r\t\r\x04" +
		"\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x04\x11\t\x11\x04\x12\t\x12\x04" +
		"\x13\t\x13\x04\x14\t\x14\x04\x15\t\x15\x04\x16\t\x16\x04\x17\t\x17\x04" +
		"\x18\t\x18\x04\x19\t\x19\x04\x1A\t\x1A\x04\x1B\t\x1B\x04\x1C\t\x1C\x04" +
		"\x1D\t\x1D\x04\x1E\t\x1E\x03\x02\x05\x02>\n\x02\x03\x02\x07\x02A\n\x02" +
		"\f\x02\x0E\x02D\v\x02\x03\x02\x03\x02\x03\x03\x03\x03\x03\x03\x03\x03" +
		"\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x03Q\n\x03\x03\x04\x03\x04" +
		"\x03\x04\x03\x04\x07\x04W\n\x04\f\x04\x0E\x04Z\v\x04\x03\x04\x03\x04\x03" +
		"\x05\x05\x05_\n\x05\x03\x05\x03\x05\x03\x06\x05\x06d\n\x06\x03\x06\x03" +
		"\x06\x03\x06\x05\x06i\n\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x07\x05" +
		"\x07p\n\x07\x03\x07\x03\x07\x03\x07\x05\x07u\n\x07\x03\x07\x03\x07\x03" +
		"\x07\x03\x07\x03\b\x05\b|\n\b\x03\b\x03\b\x05\b\x80\n\b\x03\b\x03\b\x03" +
		"\b\x03\b\x03\t\x05\t\x87\n\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\t" +
		"\x03\n\x05\n\x91\n\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\v\x05" +
		"\v\x9B\n\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\f" +
		"\x03\f\x03\f\x05\f\xA9\n\f\x03\r\x03\r\x07\r\xAD\n\r\f\r\x0E\r\xB0\v\r" +
		"\x03\r\x03\r\x03\x0E\x03\x0E\x07\x0E\xB6\n\x0E\f\x0E\x0E\x0E\xB9\v\x0E" +
		"\x03\x0E\x03\x0E\x03\x0F\x03\x0F\x07\x0F\xBF\n\x0F\f\x0F\x0E\x0F\xC2\v" +
		"\x0F\x03\x0F\x03\x0F\x03\x10\x03\x10\x03\x10\x03\x10\x05\x10\xCA\n\x10" +
		"\x03\x11\x03\x11\x05\x11\xCE\n\x11\x03\x12\x03\x12\x03\x13\x03\x13\x03" +
		"\x14\x03\x14\x07\x14\xD6\n\x14\f\x14\x0E\x14\xD9\v\x14\x03\x14\x03\x14" +
		"\x03\x15\x05\x15\xDE\n\x15\x03\x15\x03\x15\x03\x15\x05\x15\xE3\n\x15\x03" +
		"\x15\x05\x15\xE6\n\x15\x03\x15\x05\x15\xE9\n\x15\x03\x16\x03\x16\x07\x16" +
		"\xED\n\x16\f\x16\x0E\x16\xF0\v\x16\x03\x16\x03\x16\x03\x17\x05\x17\xF5" +
		"\n\x17\x03\x17\x05\x17\xF8\n\x17\x03\x17\x03\x17\x03\x17\x03\x17\x07\x17" +
		"\xFE\n\x17\f\x17\x0E\x17\u0101\v\x17\x03\x17\x07\x17\u0104\n\x17\f\x17" +
		"\x0E\x17\u0107\v\x17\x03\x17\x03\x17\x05\x17\u010B\n\x17\x03\x18\x03\x18" +
		"\x03\x19\x06\x19\u0110\n\x19\r\x19\x0E\x19\u0111\x03\x1A\x03\x1A\x07\x1A" +
		"\u0116\n\x1A\f\x1A\x0E\x1A\u0119\v\x1A\x03\x1B\x03\x1B\x03\x1B\x03\x1B" +
		"\x07\x1B\u011F\n\x1B\f\x1B\x0E\x1B\u0122\v\x1B\x05\x1B\u0124\n\x1B\x03" +
		"\x1B\x03\x1B\x03\x1C\x03\x1C\x03\x1C\x03\x1D\x03\x1D\x03\x1E\x03\x1E\x03" +
		"\x1E\x02\x02\x02\x1F\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E\x02" +
		"\x10\x02\x12\x02\x14\x02\x16\x02\x18\x02\x1A\x02\x1C\x02\x1E\x02 \x02" +
		"\"\x02$\x02&\x02(\x02*\x02,\x02.\x020\x022\x024\x026\x028\x02:\x02\x02" +
		"\x07\x04\x02\x12\x12\x14\x14\x07\x02\t\t\f\f\x11\x12\x14\x15#%\x03\x02" +
		"\x1C\"\x04\x02\x15\x15$$\x03\x02\x07\b\x02\u013D\x02=\x03\x02\x02\x02" +
		"\x04P\x03\x02\x02\x02\x06R\x03\x02\x02\x02\b^\x03\x02\x02\x02\nc\x03\x02" +
		"\x02\x02\fo\x03\x02\x02\x02\x0E{\x03\x02\x02\x02\x10\x86\x03\x02\x02\x02" +
		"\x12\x90\x03\x02\x02\x02\x14\x9A\x03\x02\x02\x02\x16\xA8\x03\x02\x02\x02" +
		"\x18\xAA\x03\x02\x02\x02\x1A\xB3\x03\x02\x02\x02\x1C\xBC\x03\x02\x02\x02" +
		"\x1E\xC9\x03\x02\x02\x02 \xCD\x03\x02\x02\x02\"\xCF\x03\x02\x02\x02$\xD1" +
		"\x03\x02\x02\x02&\xD3\x03\x02\x02\x02(\xDD\x03\x02\x02\x02*\xEA\x03\x02" +
		"\x02\x02,\xF4\x03\x02\x02\x02.\u010C\x03\x02\x02\x020\u010F\x03\x02\x02" +
		"\x022\u0113\x03\x02\x02\x024\u011A\x03\x02\x02\x026\u0127\x03\x02\x02" +
		"\x028\u012A\x03\x02\x02\x02:\u012C\x03\x02\x02\x02<>\x058\x1D\x02=<\x03" +
		"\x02\x02\x02=>\x03\x02\x02\x02>B\x03\x02\x02\x02?A\x05\x04\x03\x02@?\x03" +
		"\x02\x02\x02AD\x03\x02\x02\x02B@\x03\x02\x02\x02BC\x03\x02\x02\x02CE\x03" +
		"\x02\x02\x02DB\x03\x02\x02\x02EF\x07\x02\x02\x03F\x03\x03\x02\x02\x02" +
		"GQ\x05\b\x05\x02HQ\x05\x06\x04\x02IQ\x05\n\x06\x02JQ\x05\f\x07\x02KQ\x05" +
		"\x0E\b\x02LQ\x05\x10\t\x02MQ\x05\x12\n\x02NQ\x05\x14\v\x02OQ\x058\x1D" +
		"\x02PG\x03\x02\x02\x02PH\x03\x02\x02\x02PI\x03\x02\x02\x02PJ\x03\x02\x02" +
		"\x02PK\x03\x02\x02\x02PL\x03\x02\x02\x02PM\x03\x02\x02\x02PN\x03\x02\x02" +
		"\x02PO\x03\x02\x02\x02Q\x05\x03\x02\x02\x02RS\x07\r\x02\x02ST\x07#\x02" +
		"\x02TX\x07\x16\x02\x02UW\x05\x04\x03\x02VU\x03\x02\x02\x02WZ\x03\x02\x02" +
		"\x02XV\x03\x02\x02\x02XY\x03\x02\x02\x02Y[\x03\x02\x02\x02ZX\x03\x02\x02" +
		"\x02[\\\x07\x17\x02\x02\\\x07\x03\x02\x02\x02]_\x058\x1D\x02^]\x03\x02" +
		"\x02\x02^_\x03\x02\x02\x02_`\x03\x02\x02\x02`a\x07\t\x02\x02a\t\x03\x02" +
		"\x02\x02bd\x058\x1D\x02cb\x03\x02\x02\x02cd\x03\x02\x02\x02de\x03\x02" +
		"\x02\x02ef\x07\r\x02\x02fh\x050\x19\x02gi\x056\x1C\x02hg\x03\x02\x02\x02" +
		"hi\x03\x02\x02\x02ij\x03\x02\x02\x02jk\x05.\x18\x02kl\x054\x1B\x02lm\x07" +
		"\x1D\x02\x02m\v\x03\x02\x02\x02np\x058\x1D\x02on\x03\x02\x02\x02op\x03" +
		"\x02\x02\x02pq\x03\x02\x02\x02qr\x05\x16\f\x02rt\x050\x19\x02su\x056\x1C" +
		"\x02ts\x03\x02\x02\x02tu\x03\x02\x02\x02uv\x03\x02\x02\x02vw\x05.\x18" +
		"\x02wx\x054\x1B\x02xy\x05\x18\r\x02y\r\x03\x02\x02\x02z|\x058\x1D\x02" +
		"{z\x03\x02\x02\x02{|\x03\x02\x02\x02|}\x03\x02\x02\x02}\x7F\x07\x13\x02" +
		"\x02~\x80\t\x02\x02\x02\x7F~\x03\x02\x02\x02\x7F\x80\x03\x02\x02\x02\x80" +
		"\x81\x03\x02\x02\x02\x81\x82\x050\x19\x02\x82\x83\x05.\x18\x02\x83\x84" +
		"\x07\x1D\x02\x02\x84\x0F\x03\x02\x02\x02\x85\x87\x058\x1D\x02\x86\x85" +
		"\x03\x02\x02\x02\x86\x87\x03\x02\x02\x02\x87\x88\x03\x02\x02\x02\x88\x89" +
		"\x07\x13\x02\x02\x89\x8A\x07\f\x02\x02\x8A\x8B\x05.\x18\x02\x8B\x8C\x05" +
		"&\x14\x02\x8C\x8D\x05.\x18\x02\x8D\x8E\x07\x1D\x02\x02\x8E\x11\x03\x02" +
		"\x02\x02\x8F\x91\x058\x1D\x02\x90\x8F\x03\x02\x02\x02\x90\x91\x03\x02" +
		"\x02\x02\x91\x92\x03\x02\x02\x02\x92\x93\x07\x13\x02\x02\x93\x94\x07\x12" +
		"\x02\x02\x94\x95\x05.\x18\x02\x95\x96\x05*\x16\x02\x96\x97\x05.\x18\x02" +
		"\x97\x98\x07\x1D\x02\x02\x98\x13\x03\x02\x02\x02\x99\x9B\x058\x1D\x02" +
		"\x9A\x99\x03\x02\x02\x02\x9A\x9B\x03\x02\x02\x02\x9B\x9C\x03\x02\x02\x02" +
		"\x9C\x9D\x07\x13\x02\x02\x9D\x9E\x050\x19\x02\x9E\x9F\x07\x18\x02\x02" +
		"\x9F\xA0\x07 \x02\x02\xA0\xA1\x05.\x18\x02\xA1\xA2\x07\x19\x02\x02\xA2" +
		"\xA3\x054\x1B\x02\xA3\xA4\x07\x1D\x02\x02\xA4\x15\x03\x02\x02\x02\xA5" +
		"\xA9\x07\x10\x02\x02\xA6\xA7\x07\x11\x02\x02\xA7\xA9\x07\x0E\x02\x02\xA8" +
		"\xA5\x03\x02\x02\x02\xA8\xA6\x03\x02\x02\x02\xA9\x17\x03\x02\x02\x02\xAA" +
		"\xAE\x07\x16\x02\x02\xAB\xAD\x05\x1E\x10\x02\xAC\xAB\x03\x02\x02\x02\xAD" +
		"\xB0\x03\x02\x02\x02\xAE\xAC\x03\x02\x02\x02\xAE\xAF\x03\x02\x02\x02\xAF" +
		"\xB1\x03\x02\x02\x02\xB0\xAE\x03\x02\x02\x02\xB1\xB2\x07\x17\x02\x02\xB2" +
		"\x19\x03\x02\x02\x02\xB3\xB7\x07\x18\x02\x02\xB4\xB6\x05\x1E\x10\x02\xB5" +
		"\xB4\x03\x02\x02\x02\xB6\xB9\x03\x02\x02\x02\xB7\xB5\x03\x02\x02\x02\xB7" +
		"\xB8\x03\x02\x02\x02\xB8\xBA\x03\x02\x02\x02\xB9\xB7\x03\x02\x02\x02\xBA" +
		"\xBB\x07\x19\x02\x02\xBB\x1B\x03\x02\x02\x02\xBC\xC0\x07\x1A\x02\x02\xBD" +
		"\xBF\x05\x1E\x10\x02\xBE\xBD\x03\x02\x02\x02\xBF\xC2\x03\x02\x02\x02\xC0" +
		"\xBE\x03\x02\x02\x02\xC0\xC1\x03\x02\x02\x02\xC1\xC3\x03\x02\x02\x02\xC2" +
		"\xC0\x03\x02\x02\x02\xC3\xC4\x07\x1B\x02\x02\xC4\x1D\x03\x02\x02\x02\xC5" +
		"\xCA\x05\x18\r\x02\xC6\xCA\x05\x1C\x0F\x02\xC7\xCA\x05 \x11\x02\xC8\xCA" +
		"\x05$\x13\x02\xC9\xC5\x03\x02\x02\x02\xC9\xC6\x03\x02\x02\x02\xC9\xC7" +
		"\x03\x02\x02\x02\xC9\xC8\x03\x02\x02\x02\xCA\x1F\x03\x02\x02\x02\xCB\xCE" +
		"\x05\x1A\x0E\x02\xCC\xCE\x05\"\x12\x02\xCD\xCB\x03\x02\x02\x02\xCD\xCC" +
		"\x03\x02\x02\x02\xCE!\x03\x02\x02\x02\xCF\xD0\t\x03\x02\x02\xD0#\x03\x02" +
		"\x02\x02\xD1\xD2\t\x04\x02\x02\xD2%\x03\x02\x02\x02\xD3\xD7\x07\x16\x02" +
		"\x02\xD4\xD6\x05(\x15\x02\xD5\xD4\x03\x02\x02\x02\xD6\xD9\x03\x02\x02" +
		"\x02\xD7\xD5\x03\x02\x02\x02\xD7\xD8\x03\x02\x02\x02\xD8\xDA\x03\x02\x02" +
		"\x02\xD9\xD7\x03\x02\x02\x02\xDA\xDB\x07\x17\x02\x02\xDB\'\x03\x02\x02" +
		"\x02\xDC\xDE\x058\x1D\x02\xDD\xDC\x03\x02\x02\x02\xDD\xDE\x03\x02\x02" +
		"\x02\xDE\xDF\x03\x02\x02\x02\xDF\xE2\x05.\x18\x02\xE0\xE1\x07!\x02\x02" +
		"\xE1\xE3\x05 \x11\x02\xE2\xE0\x03\x02\x02\x02\xE2\xE3\x03\x02\x02\x02" +
		"\xE3\xE5\x03\x02\x02\x02\xE4\xE6\x07\x1E\x02\x02\xE5\xE4\x03\x02\x02\x02" +
		"\xE5\xE6\x03\x02\x02\x02\xE6\xE8\x03\x02\x02\x02\xE7\xE9\x05:\x1E\x02" +
		"\xE8\xE7\x03\x02\x02\x02\xE8\xE9\x03\x02\x02\x02\xE9)\x03\x02\x02\x02" +
		"\xEA\xEE\x07\x16\x02\x02\xEB\xED\x05,\x17\x02\xEC\xEB\x03\x02\x02\x02" +
		"\xED\xF0\x03\x02\x02\x02\xEE\xEC\x03\x02\x02\x02\xEE\xEF\x03\x02\x02\x02" +
		"\xEF\xF1\x03\x02\x02\x02\xF0\xEE\x03\x02\x02\x02\xF1\xF2\x07\x17\x02\x02" +
		"\xF2+\x03\x02\x02\x02\xF3\xF5\x058\x1D\x02\xF4\xF3\x03\x02\x02\x02\xF4" +
		"\xF5\x03\x02\x02\x02\xF5\xF7\x03\x02\x02\x02\xF6\xF8\x07\x12\x02\x02\xF7" +
		"\xF6\x03\x02\x02\x02\xF7\xF8\x03\x02\x02\x02\xF8\xF9\x03\x02\x02\x02\xF9" +
		"\xFA\x050\x19\x02\xFA\xFF\x05.\x18\x02\xFB\xFC\x07\x1E\x02\x02\xFC\xFE" +
		"\x05.\x18\x02\xFD\xFB\x03\x02\x02\x02\xFE\u0101\x03\x02\x02\x02\xFF\xFD" +
		"\x03\x02\x02\x02\xFF\u0100\x03\x02\x02\x02\u0100\u0105\x03\x02\x02\x02" +
		"\u0101\xFF\x03\x02\x02\x02\u0102\u0104\x05\x1C\x0F\x02\u0103\u0102\x03" +
		"\x02\x02\x02\u0104\u0107\x03\x02\x02\x02\u0105\u0103\x03\x02\x02\x02\u0105" +
		"\u0106\x03\x02\x02\x02\u0106\u0108\x03\x02\x02\x02\u0107\u0105\x03\x02" +
		"\x02\x02\u0108\u010A\x07\x1D\x02\x02\u0109\u010B\x05:\x1E\x02\u010A\u0109" +
		"\x03\x02\x02\x02\u010A\u010B\x03\x02\x02\x02\u010B-\x03\x02\x02\x02\u010C" +
		"\u010D\x07$\x02\x02\u010D/\x03\x02\x02\x02\u010E\u0110\x052\x1A\x02\u010F" +
		"\u010E\x03\x02\x02\x02\u0110\u0111\x03\x02\x02\x02\u0111\u010F\x03\x02" +
		"\x02\x02\u0111\u0112\x03\x02\x02\x02\u01121\x03\x02\x02\x02\u0113\u0117" +
		"\t\x05\x02\x02\u0114\u0116\x07 \x02\x02\u0115\u0114\x03\x02\x02\x02\u0116" +
		"\u0119\x03\x02\x02\x02\u0117\u0115\x03\x02\x02\x02\u0117\u0118\x03\x02" +
		"\x02\x02\u01183\x03\x02\x02\x02\u0119\u0117\x03\x02\x02\x02\u011A\u0123" +
		"\x07\x18\x02\x02\u011B\u0120\x050\x19\x02\u011C\u011D\x07\x1E\x02\x02" +
		"\u011D\u011F\x050\x19\x02\u011E\u011C\x03\x02\x02\x02\u011F\u0122\x03" +
		"\x02\x02\x02\u0120\u011E\x03\x02\x02\x02\u0120\u0121\x03\x02\x02\x02\u0121" +
		"\u0124\x03\x02\x02\x02\u0122\u0120\x03\x02\x02\x02\u0123\u011B\x03\x02" +
		"\x02\x02\u0123\u0124\x03\x02\x02\x02\u0124\u0125\x03\x02\x02\x02\u0125" +
		"\u0126\x07\x19\x02\x02\u01265\x03\x02\x02\x02\u0127\u0128\x07\v\x02\x02" +
		"\u0128\u0129\x05\x1A\x0E\x02\u01297\x03\x02\x02\x02\u012A\u012B\t\x06" +
		"\x02\x02\u012B9\x03\x02\x02\x02\u012C\u012D\x07\x06\x02\x02\u012D;\x03" +
		"\x02\x02\x02%=BPX^chot{\x7F\x86\x90\x9A\xA8\xAE\xB7\xC0\xC9\xCD\xD7\xDD" +
		"\xE2\xE5\xE8\xEE\xF4\xF7\xFF\u0105\u010A\u0111\u0117\u0120\u0123";
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
	public enumDef(): EnumDefContext | undefined {
		return this.tryGetRuleContext(0, EnumDefContext);
	}
	public structDef(): StructDefContext | undefined {
		return this.tryGetRuleContext(0, StructDefContext);
	}
	public callbackDef(): CallbackDefContext | undefined {
		return this.tryGetRuleContext(0, CallbackDefContext);
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
	public EXTERN(): TerminalNode { return this.getToken(CHeaderParser.EXTERN, 0); }
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
	public VOID(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.VOID, 0); }
	public STATIC(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STATIC, 0); }
	public ENUM(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.ENUM, 0); }
	public STRUCT(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STRUCT, 0); }
	public UNION(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.UNION, 0); }
	public NUMBER(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.NUMBER, 0); }
	public STRING(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.STRING, 0); }
	public DEFINE(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.DEFINE, 0); }
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


