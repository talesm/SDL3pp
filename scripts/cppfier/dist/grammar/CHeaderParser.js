"use strict";
// Generated from src/grammar/CHeader.g4 by ANTLR 4.9.0-SNAPSHOT
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
exports.ExprContext = exports.ProgContext = exports.CHeaderParser = void 0;
const ATN_1 = require("antlr4ts/atn/ATN");
const ATNDeserializer_1 = require("antlr4ts/atn/ATNDeserializer");
const FailedPredicateException_1 = require("antlr4ts/FailedPredicateException");
const NoViableAltException_1 = require("antlr4ts/NoViableAltException");
const Parser_1 = require("antlr4ts/Parser");
const ParserRuleContext_1 = require("antlr4ts/ParserRuleContext");
const ParserATNSimulator_1 = require("antlr4ts/atn/ParserATNSimulator");
const RecognitionException_1 = require("antlr4ts/RecognitionException");
const Token_1 = require("antlr4ts/Token");
const VocabularyImpl_1 = require("antlr4ts/VocabularyImpl");
const Utils = __importStar(require("antlr4ts/misc/Utils"));
class CHeaderParser extends Parser_1.Parser {
    // @Override
    // @NotNull
    get vocabulary() {
        return CHeaderParser.VOCABULARY;
    }
    // tslint:enable:no-trailing-whitespace
    // @Override
    get grammarFileName() { return "CHeader.g4"; }
    // @Override
    get ruleNames() { return CHeaderParser.ruleNames; }
    // @Override
    get serializedATN() { return CHeaderParser._serializedATN; }
    createFailedPredicateException(predicate, message) {
        return new FailedPredicateException_1.FailedPredicateException(this, predicate, message);
    }
    constructor(input) {
        super(input);
        this._interp = new ParserATNSimulator_1.ParserATNSimulator(CHeaderParser._ATN, this);
    }
    // @RuleVersion(0)
    prog() {
        let _localctx = new ProgContext(this._ctx, this.state);
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
            if (re instanceof RecognitionException_1.RecognitionException) {
                _localctx.exception = re;
                this._errHandler.reportError(this, re);
                this._errHandler.recover(this, re);
            }
            else {
                throw re;
            }
        }
        finally {
            this.exitRule();
        }
        return _localctx;
    }
    // @RuleVersion(0)
    expr(_p) {
        if (_p === undefined) {
            _p = 0;
        }
        let _parentctx = this._ctx;
        let _parentState = this.state;
        let _localctx = new ExprContext(this._ctx, _parentState);
        let _prevctx = _localctx;
        let _startState = 2;
        this.enterRecursionRule(_localctx, 2, CHeaderParser.RULE_expr, _p);
        let _la;
        try {
            let _alt;
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
                        throw new NoViableAltException_1.NoViableAltException(this);
                }
                this._ctx._stop = this._input.tryLT(-1);
                this.state = 23;
                this._errHandler.sync(this);
                _alt = this.interpreter.adaptivePredict(this._input, 2, this._ctx);
                while (_alt !== 2 && _alt !== ATN_1.ATN.INVALID_ALT_NUMBER) {
                    if (_alt === 1) {
                        if (this._parseListeners != null) {
                            this.triggerExitRuleEvent();
                        }
                        _prevctx = _localctx;
                        {
                            this.state = 21;
                            this._errHandler.sync(this);
                            switch (this.interpreter.adaptivePredict(this._input, 1, this._ctx)) {
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
                                        }
                                        else {
                                            if (this._input.LA(1) === Token_1.Token.EOF) {
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
                                        }
                                        else {
                                            if (this._input.LA(1) === Token_1.Token.EOF) {
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
            if (re instanceof RecognitionException_1.RecognitionException) {
                _localctx.exception = re;
                this._errHandler.reportError(this, re);
                this._errHandler.recover(this, re);
            }
            else {
                throw re;
            }
        }
        finally {
            this.unrollRecursionContexts(_parentctx);
        }
        return _localctx;
    }
    sempred(_localctx, ruleIndex, predIndex) {
        switch (ruleIndex) {
            case 1:
                return this.expr_sempred(_localctx, predIndex);
        }
        return true;
    }
    expr_sempred(_localctx, predIndex) {
        switch (predIndex) {
            case 0:
                return this.precpred(this._ctx, 4);
            case 1:
                return this.precpred(this._ctx, 3);
        }
        return true;
    }
    static get _ATN() {
        if (!CHeaderParser.__ATN) {
            CHeaderParser.__ATN = new ATNDeserializer_1.ATNDeserializer().deserialize(Utils.toCharArray(CHeaderParser._serializedATN));
        }
        return CHeaderParser.__ATN;
    }
}
exports.CHeaderParser = CHeaderParser;
CHeaderParser.T__0 = 1;
CHeaderParser.T__1 = 2;
CHeaderParser.T__2 = 3;
CHeaderParser.T__3 = 4;
CHeaderParser.T__4 = 5;
CHeaderParser.T__5 = 6;
CHeaderParser.NEWLINE = 7;
CHeaderParser.INT = 8;
CHeaderParser.RULE_prog = 0;
CHeaderParser.RULE_expr = 1;
// tslint:disable:no-trailing-whitespace
CHeaderParser.ruleNames = [
    "prog", "expr",
];
CHeaderParser._LITERAL_NAMES = [
    undefined, "'*'", "'/'", "'+'", "'-'", "'('", "')'",
];
CHeaderParser._SYMBOLIC_NAMES = [
    undefined, undefined, undefined, undefined, undefined, undefined, undefined,
    "NEWLINE", "INT",
];
CHeaderParser.VOCABULARY = new VocabularyImpl_1.VocabularyImpl(CHeaderParser._LITERAL_NAMES, CHeaderParser._SYMBOLIC_NAMES, []);
CHeaderParser._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03\n\x1D\x04\x02" +
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
class ProgContext extends ParserRuleContext_1.ParserRuleContext {
    expr() {
        return this.getRuleContext(0, ExprContext);
    }
    EOF() { return this.getToken(CHeaderParser.EOF, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_prog; }
    // @Override
    enterRule(listener) {
        if (listener.enterProg) {
            listener.enterProg(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitProg) {
            listener.exitProg(this);
        }
    }
}
exports.ProgContext = ProgContext;
class ExprContext extends ParserRuleContext_1.ParserRuleContext {
    expr(i) {
        if (i === undefined) {
            return this.getRuleContexts(ExprContext);
        }
        else {
            return this.getRuleContext(i, ExprContext);
        }
    }
    INT() { return this.tryGetToken(CHeaderParser.INT, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_expr; }
    // @Override
    enterRule(listener) {
        if (listener.enterExpr) {
            listener.enterExpr(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitExpr) {
            listener.exitExpr(this);
        }
    }
}
exports.ExprContext = ExprContext;
