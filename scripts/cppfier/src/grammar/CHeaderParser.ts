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
	public static readonly EXTERN = 7;
	public static readonly VOID = 8;
	public static readonly SDL_NOISE = 9;
	public static readonly CURLY_B = 10;
	public static readonly CURLY_E = 11;
	public static readonly ROUND_B = 12;
	public static readonly ROUND_E = 13;
	public static readonly SEMI = 14;
	public static readonly STAR = 15;
	public static readonly STRING = 16;
	public static readonly ID = 17;
	public static readonly RULE_prog = 0;
	public static readonly RULE_stm = 1;
	public static readonly RULE_externC = 2;
	public static readonly RULE_directive = 3;
	public static readonly RULE_functionDecl = 4;
	public static readonly RULE_type = 5;
	public static readonly RULE_typeEl = 6;
	public static readonly RULE_signature = 7;
	public static readonly RULE_doc = 8;
	// tslint:disable:no-trailing-whitespace
	public static readonly ruleNames: string[] = [
		"prog", "stm", "externC", "directive", "functionDecl", "type", "typeEl", 
		"signature", "doc",
	];

	private static readonly _LITERAL_NAMES: Array<string | undefined> = [
		undefined, undefined, undefined, undefined, undefined, undefined, undefined, 
		"'extern'", "'void'", undefined, "'{'", "'}'", "'('", "')'", "';'", "'*'",
	];
	private static readonly _SYMBOLIC_NAMES: Array<string | undefined> = [
		undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "LONG_DOC", "SHORT_DOC", 
		"DIRECTIVE", "EXTERN", "VOID", "SDL_NOISE", "CURLY_B", "CURLY_E", "ROUND_B", 
		"ROUND_E", "SEMI", "STAR", "STRING", "ID",
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
			this.state = 19;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 0, this._ctx) ) {
			case 1:
				{
				this.state = 18;
				this.doc();
				}
				break;
			}
			this.state = 24;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.EXTERN))) !== 0)) {
				{
				{
				this.state = 21;
				this.stm();
				}
				}
				this.state = 26;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 27;
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
	public stm(): StmContext {
		let _localctx: StmContext = new StmContext(this._ctx, this.state);
		this.enterRule(_localctx, 2, CHeaderParser.RULE_stm);
		try {
			this.state = 32;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 2, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 29;
				this.directive();
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 30;
				this.externC();
				}
				break;

			case 3:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 31;
				this.functionDecl();
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
			this.state = 34;
			this.match(CHeaderParser.EXTERN);
			this.state = 35;
			this.match(CHeaderParser.STRING);
			this.state = 36;
			this.match(CHeaderParser.CURLY_B);
			this.state = 40;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.EXTERN))) !== 0)) {
				{
				{
				this.state = 37;
				this.stm();
				}
				}
				this.state = 42;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 43;
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
			this.state = 46;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 45;
				this.doc();
				}
			}

			this.state = 48;
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
			this.state = 51;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
				{
				this.state = 50;
				this.doc();
				}
			}

			this.state = 53;
			this.match(CHeaderParser.EXTERN);
			this.state = 54;
			this.type();
			this.state = 55;
			this.match(CHeaderParser.ID);
			this.state = 56;
			this.signature();
			this.state = 57;
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
	public type(): TypeContext {
		let _localctx: TypeContext = new TypeContext(this._ctx, this.state);
		this.enterRule(_localctx, 10, CHeaderParser.RULE_type);
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 60;
			this._errHandler.sync(this);
			_alt = 1;
			do {
				switch (_alt) {
				case 1:
					{
					{
					this.state = 59;
					this.typeEl();
					}
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				this.state = 62;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 6, this._ctx);
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
		this.enterRule(_localctx, 12, CHeaderParser.RULE_typeEl);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 64;
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
			this.state = 68;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === CHeaderParser.STAR) {
				{
				{
				this.state = 65;
				this.match(CHeaderParser.STAR);
				}
				}
				this.state = 70;
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
		this.enterRule(_localctx, 14, CHeaderParser.RULE_signature);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 71;
			this.match(CHeaderParser.ROUND_B);
			this.state = 72;
			this.match(CHeaderParser.VOID);
			this.state = 73;
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
	public doc(): DocContext {
		let _localctx: DocContext = new DocContext(this._ctx, this.state);
		this.enterRule(_localctx, 16, CHeaderParser.RULE_doc);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 75;
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
		"\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03\x13P\x04\x02" +
		"\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
		"\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x03\x02\x05\x02\x16\n\x02\x03\x02" +
		"\x07\x02\x19\n\x02\f\x02\x0E\x02\x1C\v\x02\x03\x02\x03\x02\x03\x03\x03" +
		"\x03\x03\x03\x05\x03#\n\x03\x03\x04\x03\x04\x03\x04\x03\x04\x07\x04)\n" +
		"\x04\f\x04\x0E\x04,\v\x04\x03\x04\x03\x04\x03\x05\x05\x051\n\x05\x03\x05" +
		"\x03\x05\x03\x06\x05\x066\n\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x06" +
		"\x03\x06\x03\x07\x06\x07?\n\x07\r\x07\x0E\x07@\x03\b\x03\b\x07\bE\n\b" +
		"\f\b\x0E\bH\v\b\x03\t\x03\t\x03\t\x03\t\x03\n\x03\n\x03\n\x02\x02\x02" +
		"\v\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E\x02\x10\x02\x12\x02\x02" +
		"\x04\x04\x02\n\n\x13\x13\x03\x02\x06\x07\x02O\x02\x15\x03\x02\x02\x02" +
		"\x04\"\x03\x02\x02\x02\x06$\x03\x02\x02\x02\b0\x03\x02\x02\x02\n5\x03" +
		"\x02\x02\x02\f>\x03\x02\x02\x02\x0EB\x03\x02\x02\x02\x10I\x03\x02\x02" +
		"\x02\x12M\x03\x02\x02\x02\x14\x16\x05\x12\n\x02\x15\x14\x03\x02\x02\x02" +
		"\x15\x16\x03\x02\x02\x02\x16\x1A\x03\x02\x02\x02\x17\x19\x05\x04\x03\x02" +
		"\x18\x17\x03\x02\x02\x02\x19\x1C\x03\x02\x02\x02\x1A\x18\x03\x02\x02\x02" +
		"\x1A\x1B\x03\x02\x02\x02\x1B\x1D\x03\x02\x02\x02\x1C\x1A\x03\x02\x02\x02" +
		"\x1D\x1E\x07\x02\x02\x03\x1E\x03\x03\x02\x02\x02\x1F#\x05\b\x05\x02 #" +
		"\x05\x06\x04\x02!#\x05\n\x06\x02\"\x1F\x03\x02\x02\x02\" \x03\x02\x02" +
		"\x02\"!\x03\x02\x02\x02#\x05\x03\x02\x02\x02$%\x07\t\x02\x02%&\x07\x12" +
		"\x02\x02&*\x07\f\x02\x02\')\x05\x04\x03\x02(\'\x03\x02\x02\x02),\x03\x02" +
		"\x02\x02*(\x03\x02\x02\x02*+\x03\x02\x02\x02+-\x03\x02\x02\x02,*\x03\x02" +
		"\x02\x02-.\x07\r\x02\x02.\x07\x03\x02\x02\x02/1\x05\x12\n\x020/\x03\x02" +
		"\x02\x0201\x03\x02\x02\x0212\x03\x02\x02\x0223\x07\b\x02\x023\t\x03\x02" +
		"\x02\x0246\x05\x12\n\x0254\x03\x02\x02\x0256\x03\x02\x02\x0267\x03\x02" +
		"\x02\x0278\x07\t\x02\x0289\x05\f\x07\x029:\x07\x13\x02\x02:;\x05\x10\t" +
		"\x02;<\x07\x10\x02\x02<\v\x03\x02\x02\x02=?\x05\x0E\b\x02>=\x03\x02\x02" +
		"\x02?@\x03\x02\x02\x02@>\x03\x02\x02\x02@A\x03\x02\x02\x02A\r\x03\x02" +
		"\x02\x02BF\t\x02\x02\x02CE\x07\x11\x02\x02DC\x03\x02\x02\x02EH\x03\x02" +
		"\x02\x02FD\x03\x02\x02\x02FG\x03\x02\x02\x02G\x0F\x03\x02\x02\x02HF\x03" +
		"\x02\x02\x02IJ\x07\x0E\x02\x02JK\x07\n\x02\x02KL\x07\x0F\x02\x02L\x11" +
		"\x03\x02\x02\x02MN\t\x03\x02\x02N\x13\x03\x02\x02\x02\n\x15\x1A\"*05@" +
		"F";
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


export class StmContext extends ParserRuleContext {
	public directive(): DirectiveContext | undefined {
		return this.tryGetRuleContext(0, DirectiveContext);
	}
	public externC(): ExternCContext | undefined {
		return this.tryGetRuleContext(0, ExternCContext);
	}
	public functionDecl(): FunctionDeclContext | undefined {
		return this.tryGetRuleContext(0, FunctionDeclContext);
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


export class ExternCContext extends ParserRuleContext {
	public EXTERN(): TerminalNode { return this.getToken(CHeaderParser.EXTERN, 0); }
	public STRING(): TerminalNode { return this.getToken(CHeaderParser.STRING, 0); }
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
	public VOID(): TerminalNode { return this.getToken(CHeaderParser.VOID, 0); }
	public ROUND_E(): TerminalNode { return this.getToken(CHeaderParser.ROUND_E, 0); }
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


