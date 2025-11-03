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
	public static readonly T__0 = 1;
	public static readonly T__1 = 2;
	public static readonly T__2 = 3;
	public static readonly T__3 = 4;
	public static readonly T__4 = 5;
	public static readonly T__5 = 6;
	public static readonly NEWLINE = 7;
	public static readonly INT = 8;
	public static readonly RULE_prog = 0;
	public static readonly RULE_expr = 1;
	// tslint:disable:no-trailing-whitespace
	public static readonly ruleNames: string[] = [
		"prog", "expr",
	];

	private static readonly _LITERAL_NAMES: Array<string | undefined> = [
		undefined, "'*'", "'/'", "'+'", "'-'", "'('", "')'",
	];
	private static readonly _SYMBOLIC_NAMES: Array<string | undefined> = [
		undefined, undefined, undefined, undefined, undefined, undefined, undefined, 
		"NEWLINE", "INT",
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
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 4;
			this.expr(0);
			this.state = 5;
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

	public expr(): ExprContext;
	public expr(_p: number): ExprContext;
	// @RuleVersion(0)
	public expr(_p?: number): ExprContext {
		if (_p === undefined) {
			_p = 0;
		}

		let _parentctx: ParserRuleContext = this._ctx;
		let _parentState: number = this.state;
		let _localctx: ExprContext = new ExprContext(this._ctx, _parentState);
		let _prevctx: ExprContext = _localctx;
		let _startState: number = 2;
		this.enterRecursionRule(_localctx, 2, CHeaderParser.RULE_expr, _p);
		let _la: number;
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 13;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case CHeaderParser.INT:
				{
				this.state = 8;
				this.match(CHeaderParser.INT);
				}
				break;
			case CHeaderParser.T__4:
				{
				this.state = 9;
				this.match(CHeaderParser.T__4);
				this.state = 10;
				this.expr(0);
				this.state = 11;
				this.match(CHeaderParser.T__5);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			this._ctx._stop = this._input.tryLT(-1);
			this.state = 23;
			this._errHandler.sync(this);
			_alt = this.interpreter.adaptivePredict(this._input, 2, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					if (this._parseListeners != null) {
						this.triggerExitRuleEvent();
					}
					_prevctx = _localctx;
					{
					this.state = 21;
					this._errHandler.sync(this);
					switch ( this.interpreter.adaptivePredict(this._input, 1, this._ctx) ) {
					case 1:
						{
						_localctx = new ExprContext(_parentctx, _parentState);
						this.pushNewRecursionContext(_localctx, _startState, CHeaderParser.RULE_expr);
						this.state = 15;
						if (!(this.precpred(this._ctx, 4))) {
							throw this.createFailedPredicateException("this.precpred(this._ctx, 4)");
						}
						this.state = 16;
						_la = this._input.LA(1);
						if (!(_la === CHeaderParser.T__0 || _la === CHeaderParser.T__1)) {
						this._errHandler.recoverInline(this);
						} else {
							if (this._input.LA(1) === Token.EOF) {
								this.matchedEOF = true;
							}

							this._errHandler.reportMatch(this);
							this.consume();
						}
						this.state = 17;
						this.expr(5);
						}
						break;

					case 2:
						{
						_localctx = new ExprContext(_parentctx, _parentState);
						this.pushNewRecursionContext(_localctx, _startState, CHeaderParser.RULE_expr);
						this.state = 18;
						if (!(this.precpred(this._ctx, 3))) {
							throw this.createFailedPredicateException("this.precpred(this._ctx, 3)");
						}
						this.state = 19;
						_la = this._input.LA(1);
						if (!(_la === CHeaderParser.T__2 || _la === CHeaderParser.T__3)) {
						this._errHandler.recoverInline(this);
						} else {
							if (this._input.LA(1) === Token.EOF) {
								this.matchedEOF = true;
							}

							this._errHandler.reportMatch(this);
							this.consume();
						}
						this.state = 20;
						this.expr(4);
						}
						break;
					}
					}
				}
				this.state = 25;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 2, this._ctx);
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
			this.unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public sempred(_localctx: RuleContext, ruleIndex: number, predIndex: number): boolean {
		switch (ruleIndex) {
		case 1:
			return this.expr_sempred(_localctx as ExprContext, predIndex);
		}
		return true;
	}
	private expr_sempred(_localctx: ExprContext, predIndex: number): boolean {
		switch (predIndex) {
		case 0:
			return this.precpred(this._ctx, 4);

		case 1:
			return this.precpred(this._ctx, 3);
		}
		return true;
	}

	public static readonly _serializedATN: string =
		"\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03\n\x1D\x04\x02" +
		"\t\x02\x04\x03\t\x03\x03\x02\x03\x02\x03\x02\x03\x03\x03\x03\x03\x03\x03" +
		"\x03\x03\x03\x03\x03\x05\x03\x10\n\x03\x03\x03\x03\x03\x03\x03\x03\x03" +
		"\x03\x03\x03\x03\x07\x03\x18\n\x03\f\x03\x0E\x03\x1B\v\x03\x03\x03\x02" +
		"\x02\x03\x04\x04\x02\x02\x04\x02\x02\x04\x03\x02\x03\x04\x03\x02\x05\x06" +
		"\x02\x1D\x02\x06\x03\x02\x02\x02\x04\x0F\x03\x02\x02\x02\x06\x07\x05\x04" +
		"\x03\x02\x07\b\x07\x02\x02\x03\b\x03\x03\x02\x02\x02\t\n\b\x03\x01\x02" +
		"\n\x10\x07\n\x02\x02\v\f\x07\x07\x02\x02\f\r\x05\x04\x03\x02\r\x0E\x07" +
		"\b\x02\x02\x0E\x10\x03\x02\x02\x02\x0F\t\x03\x02\x02\x02\x0F\v\x03\x02" +
		"\x02\x02\x10\x19\x03\x02\x02\x02\x11\x12\f\x06\x02\x02\x12\x13\t\x02\x02" +
		"\x02\x13\x18\x05\x04\x03\x07\x14\x15\f\x05\x02\x02\x15\x16\t\x03\x02\x02" +
		"\x16\x18\x05\x04\x03\x06\x17\x11\x03\x02\x02\x02\x17\x14\x03\x02\x02\x02" +
		"\x18\x1B\x03\x02\x02\x02\x19\x17\x03\x02\x02\x02\x19\x1A\x03\x02\x02\x02" +
		"\x1A\x05\x03\x02\x02\x02\x1B\x19\x03\x02\x02\x02\x05\x0F\x17\x19";
	public static __ATN: ATN;
	public static get _ATN(): ATN {
		if (!CHeaderParser.__ATN) {
			CHeaderParser.__ATN = new ATNDeserializer().deserialize(Utils.toCharArray(CHeaderParser._serializedATN));
		}

		return CHeaderParser.__ATN;
	}

}

export class ProgContext extends ParserRuleContext {
	public expr(): ExprContext {
		return this.getRuleContext(0, ExprContext);
	}
	public EOF(): TerminalNode { return this.getToken(CHeaderParser.EOF, 0); }
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


export class ExprContext extends ParserRuleContext {
	public expr(): ExprContext[];
	public expr(i: number): ExprContext;
	public expr(i?: number): ExprContext | ExprContext[] {
		if (i === undefined) {
			return this.getRuleContexts(ExprContext);
		} else {
			return this.getRuleContext(i, ExprContext);
		}
	}
	public INT(): TerminalNode | undefined { return this.tryGetToken(CHeaderParser.INT, 0); }
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


