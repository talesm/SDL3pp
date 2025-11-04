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
exports.DocContext = exports.SignatureContext = exports.TypeElContext = exports.TypeContext = exports.FunctionDeclContext = exports.DirectiveContext = exports.ExternCContext = exports.StmContext = exports.ProgContext = exports.CHeaderParser = void 0;
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
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 19;
                this._errHandler.sync(this);
                switch (this.interpreter.adaptivePredict(this._input, 0, this._ctx)) {
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
    stm() {
        let _localctx = new StmContext(this._ctx, this.state);
        this.enterRule(_localctx, 2, CHeaderParser.RULE_stm);
        try {
            this.state = 32;
            this._errHandler.sync(this);
            switch (this.interpreter.adaptivePredict(this._input, 2, this._ctx)) {
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
    externC() {
        let _localctx = new ExternCContext(this._ctx, this.state);
        this.enterRule(_localctx, 4, CHeaderParser.RULE_externC);
        let _la;
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
    directive() {
        let _localctx = new DirectiveContext(this._ctx, this.state);
        this.enterRule(_localctx, 6, CHeaderParser.RULE_directive);
        let _la;
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
    functionDecl() {
        let _localctx = new FunctionDeclContext(this._ctx, this.state);
        this.enterRule(_localctx, 8, CHeaderParser.RULE_functionDecl);
        let _la;
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
    type() {
        let _localctx = new TypeContext(this._ctx, this.state);
        this.enterRule(_localctx, 10, CHeaderParser.RULE_type);
        try {
            let _alt;
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
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 62;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 6, this._ctx);
                } while (_alt !== 2 && _alt !== ATN_1.ATN.INVALID_ALT_NUMBER);
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
    typeEl() {
        let _localctx = new TypeElContext(this._ctx, this.state);
        this.enterRule(_localctx, 12, CHeaderParser.RULE_typeEl);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 64;
                _la = this._input.LA(1);
                if (!(_la === CHeaderParser.VOID || _la === CHeaderParser.ID)) {
                    this._errHandler.recoverInline(this);
                }
                else {
                    if (this._input.LA(1) === Token_1.Token.EOF) {
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
    signature() {
        let _localctx = new SignatureContext(this._ctx, this.state);
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
    doc() {
        let _localctx = new DocContext(this._ctx, this.state);
        this.enterRule(_localctx, 16, CHeaderParser.RULE_doc);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 75;
                _la = this._input.LA(1);
                if (!(_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC)) {
                    this._errHandler.recoverInline(this);
                }
                else {
                    if (this._input.LA(1) === Token_1.Token.EOF) {
                        this.matchedEOF = true;
                    }
                    this._errHandler.reportMatch(this);
                    this.consume();
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
            this.exitRule();
        }
        return _localctx;
    }
    static get _ATN() {
        if (!CHeaderParser.__ATN) {
            CHeaderParser.__ATN = new ATNDeserializer_1.ATNDeserializer().deserialize(Utils.toCharArray(CHeaderParser._serializedATN));
        }
        return CHeaderParser.__ATN;
    }
}
exports.CHeaderParser = CHeaderParser;
CHeaderParser.WS = 1;
CHeaderParser.LONG_COMMENT = 2;
CHeaderParser.SHORT_COMMENT = 3;
CHeaderParser.LONG_DOC = 4;
CHeaderParser.SHORT_DOC = 5;
CHeaderParser.DIRECTIVE = 6;
CHeaderParser.EXTERN = 7;
CHeaderParser.VOID = 8;
CHeaderParser.SDL_NOISE = 9;
CHeaderParser.CURLY_B = 10;
CHeaderParser.CURLY_E = 11;
CHeaderParser.ROUND_B = 12;
CHeaderParser.ROUND_E = 13;
CHeaderParser.SEMI = 14;
CHeaderParser.STAR = 15;
CHeaderParser.STRING = 16;
CHeaderParser.ID = 17;
CHeaderParser.RULE_prog = 0;
CHeaderParser.RULE_stm = 1;
CHeaderParser.RULE_externC = 2;
CHeaderParser.RULE_directive = 3;
CHeaderParser.RULE_functionDecl = 4;
CHeaderParser.RULE_type = 5;
CHeaderParser.RULE_typeEl = 6;
CHeaderParser.RULE_signature = 7;
CHeaderParser.RULE_doc = 8;
// tslint:disable:no-trailing-whitespace
CHeaderParser.ruleNames = [
    "prog", "stm", "externC", "directive", "functionDecl", "type", "typeEl",
    "signature", "doc",
];
CHeaderParser._LITERAL_NAMES = [
    undefined, undefined, undefined, undefined, undefined, undefined, undefined,
    "'extern'", "'void'", undefined, "'{'", "'}'", "'('", "')'", "';'", "'*'",
];
CHeaderParser._SYMBOLIC_NAMES = [
    undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "LONG_DOC", "SHORT_DOC",
    "DIRECTIVE", "EXTERN", "VOID", "SDL_NOISE", "CURLY_B", "CURLY_E", "ROUND_B",
    "ROUND_E", "SEMI", "STAR", "STRING", "ID",
];
CHeaderParser.VOCABULARY = new VocabularyImpl_1.VocabularyImpl(CHeaderParser._LITERAL_NAMES, CHeaderParser._SYMBOLIC_NAMES, []);
CHeaderParser._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03\x13P\x04\x02" +
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
class ProgContext extends ParserRuleContext_1.ParserRuleContext {
    EOF() { return this.getToken(CHeaderParser.EOF, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    stm(i) {
        if (i === undefined) {
            return this.getRuleContexts(StmContext);
        }
        else {
            return this.getRuleContext(i, StmContext);
        }
    }
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
class StmContext extends ParserRuleContext_1.ParserRuleContext {
    directive() {
        return this.tryGetRuleContext(0, DirectiveContext);
    }
    externC() {
        return this.tryGetRuleContext(0, ExternCContext);
    }
    functionDecl() {
        return this.tryGetRuleContext(0, FunctionDeclContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_stm; }
    // @Override
    enterRule(listener) {
        if (listener.enterStm) {
            listener.enterStm(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitStm) {
            listener.exitStm(this);
        }
    }
}
exports.StmContext = StmContext;
class ExternCContext extends ParserRuleContext_1.ParserRuleContext {
    EXTERN() { return this.getToken(CHeaderParser.EXTERN, 0); }
    STRING() { return this.getToken(CHeaderParser.STRING, 0); }
    CURLY_B() { return this.getToken(CHeaderParser.CURLY_B, 0); }
    CURLY_E() { return this.getToken(CHeaderParser.CURLY_E, 0); }
    stm(i) {
        if (i === undefined) {
            return this.getRuleContexts(StmContext);
        }
        else {
            return this.getRuleContext(i, StmContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_externC; }
    // @Override
    enterRule(listener) {
        if (listener.enterExternC) {
            listener.enterExternC(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitExternC) {
            listener.exitExternC(this);
        }
    }
}
exports.ExternCContext = ExternCContext;
class DirectiveContext extends ParserRuleContext_1.ParserRuleContext {
    DIRECTIVE() { return this.getToken(CHeaderParser.DIRECTIVE, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_directive; }
    // @Override
    enterRule(listener) {
        if (listener.enterDirective) {
            listener.enterDirective(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitDirective) {
            listener.exitDirective(this);
        }
    }
}
exports.DirectiveContext = DirectiveContext;
class FunctionDeclContext extends ParserRuleContext_1.ParserRuleContext {
    EXTERN() { return this.getToken(CHeaderParser.EXTERN, 0); }
    type() {
        return this.getRuleContext(0, TypeContext);
    }
    ID() { return this.getToken(CHeaderParser.ID, 0); }
    signature() {
        return this.getRuleContext(0, SignatureContext);
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_functionDecl; }
    // @Override
    enterRule(listener) {
        if (listener.enterFunctionDecl) {
            listener.enterFunctionDecl(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitFunctionDecl) {
            listener.exitFunctionDecl(this);
        }
    }
}
exports.FunctionDeclContext = FunctionDeclContext;
class TypeContext extends ParserRuleContext_1.ParserRuleContext {
    typeEl(i) {
        if (i === undefined) {
            return this.getRuleContexts(TypeElContext);
        }
        else {
            return this.getRuleContext(i, TypeElContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_type; }
    // @Override
    enterRule(listener) {
        if (listener.enterType) {
            listener.enterType(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitType) {
            listener.exitType(this);
        }
    }
}
exports.TypeContext = TypeContext;
class TypeElContext extends ParserRuleContext_1.ParserRuleContext {
    VOID() { return this.tryGetToken(CHeaderParser.VOID, 0); }
    ID() { return this.tryGetToken(CHeaderParser.ID, 0); }
    STAR(i) {
        if (i === undefined) {
            return this.getTokens(CHeaderParser.STAR);
        }
        else {
            return this.getToken(CHeaderParser.STAR, i);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_typeEl; }
    // @Override
    enterRule(listener) {
        if (listener.enterTypeEl) {
            listener.enterTypeEl(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitTypeEl) {
            listener.exitTypeEl(this);
        }
    }
}
exports.TypeElContext = TypeElContext;
class SignatureContext extends ParserRuleContext_1.ParserRuleContext {
    ROUND_B() { return this.getToken(CHeaderParser.ROUND_B, 0); }
    VOID() { return this.getToken(CHeaderParser.VOID, 0); }
    ROUND_E() { return this.getToken(CHeaderParser.ROUND_E, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_signature; }
    // @Override
    enterRule(listener) {
        if (listener.enterSignature) {
            listener.enterSignature(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitSignature) {
            listener.exitSignature(this);
        }
    }
}
exports.SignatureContext = SignatureContext;
class DocContext extends ParserRuleContext_1.ParserRuleContext {
    SHORT_DOC() { return this.tryGetToken(CHeaderParser.SHORT_DOC, 0); }
    LONG_DOC() { return this.tryGetToken(CHeaderParser.LONG_DOC, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_doc; }
    // @Override
    enterRule(listener) {
        if (listener.enterDoc) {
            listener.enterDoc(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitDoc) {
            listener.exitDoc(this);
        }
    }
}
exports.DocContext = DocContext;
