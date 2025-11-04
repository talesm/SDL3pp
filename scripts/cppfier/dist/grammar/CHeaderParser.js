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
exports.TrailingDocContext = exports.DocContext = exports.AttributeContext = exports.SignatureContext = exports.TypeElContext = exports.TypeContext = exports.IdContext = exports.StructItemContext = exports.StructBodyContext = exports.EnumItemContext = exports.EnumBodyContext = exports.PunctContext = exports.WordContext = exports.ExprContext = exports.StmContext = exports.IndexingContext = exports.GroupContext = exports.BlockContext = exports.InlineContext = exports.CompileTimeAssertContext = exports.CallbackDefContext = exports.StructDefContext = exports.EnumDefContext = exports.UnionDefContext = exports.AliasDefContext = exports.FunctionDefContext = exports.FunctionDeclContext = exports.DirectiveContext = exports.ExternCContext = exports.DeclContext = exports.ProgContext = exports.CHeaderParser = void 0;
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
                this.state = 63;
                this._errHandler.sync(this);
                switch (this.interpreter.adaptivePredict(this._input, 0, this._ctx)) {
                    case 1:
                        {
                            this.state = 62;
                            this.doc();
                        }
                        break;
                }
                this.state = 68;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || _la === CHeaderParser.ID) {
                    {
                        {
                            this.state = 65;
                            this.decl();
                        }
                    }
                    this.state = 70;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 71;
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
    decl() {
        let _localctx = new DeclContext(this._ctx, this.state);
        this.enterRule(_localctx, 2, CHeaderParser.RULE_decl);
        try {
            this.state = 84;
            this._errHandler.sync(this);
            switch (this.interpreter.adaptivePredict(this._input, 2, this._ctx)) {
                case 1:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 73;
                        this.directive();
                    }
                    break;
                case 2:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 74;
                        this.externC();
                    }
                    break;
                case 3:
                    this.enterOuterAlt(_localctx, 3);
                    {
                        this.state = 75;
                        this.functionDecl();
                    }
                    break;
                case 4:
                    this.enterOuterAlt(_localctx, 4);
                    {
                        this.state = 76;
                        this.functionDef();
                    }
                    break;
                case 5:
                    this.enterOuterAlt(_localctx, 5);
                    {
                        this.state = 77;
                        this.aliasDef();
                    }
                    break;
                case 6:
                    this.enterOuterAlt(_localctx, 6);
                    {
                        this.state = 78;
                        this.unionDef();
                    }
                    break;
                case 7:
                    this.enterOuterAlt(_localctx, 7);
                    {
                        this.state = 79;
                        this.enumDef();
                    }
                    break;
                case 8:
                    this.enterOuterAlt(_localctx, 8);
                    {
                        this.state = 80;
                        this.structDef();
                    }
                    break;
                case 9:
                    this.enterOuterAlt(_localctx, 9);
                    {
                        this.state = 81;
                        this.callbackDef();
                    }
                    break;
                case 10:
                    this.enterOuterAlt(_localctx, 10);
                    {
                        this.state = 82;
                        this.compileTimeAssert();
                    }
                    break;
                case 11:
                    this.enterOuterAlt(_localctx, 11);
                    {
                        this.state = 83;
                        this.doc();
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
                this.state = 86;
                this.match(CHeaderParser.EXTERN);
                this.state = 87;
                this.match(CHeaderParser.STRING);
                this.state = 88;
                this.match(CHeaderParser.CURLY_B);
                this.state = 92;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || _la === CHeaderParser.ID) {
                    {
                        {
                            this.state = 89;
                            this.decl();
                        }
                    }
                    this.state = 94;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 95;
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
                this.state = 98;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 97;
                        this.doc();
                    }
                }
                this.state = 100;
                this.match(CHeaderParser.DEFINE);
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
                this.state = 103;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 102;
                        this.doc();
                    }
                }
                this.state = 106;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.EXTERN) {
                    {
                        this.state = 105;
                        this.match(CHeaderParser.EXTERN);
                    }
                }
                this.state = 108;
                this.type();
                this.state = 110;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.ATTRIBUTE || _la === CHeaderParser.SDL_VARARG_ATTRIB) {
                    {
                        this.state = 109;
                        this.attribute();
                    }
                }
                this.state = 112;
                this.id();
                this.state = 113;
                this.signature();
                this.state = 114;
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
    functionDef() {
        let _localctx = new FunctionDefContext(this._ctx, this.state);
        this.enterRule(_localctx, 10, CHeaderParser.RULE_functionDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 117;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 116;
                        this.doc();
                    }
                }
                this.state = 119;
                this.inline();
                this.state = 120;
                this.type();
                this.state = 122;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.ATTRIBUTE || _la === CHeaderParser.SDL_VARARG_ATTRIB) {
                    {
                        this.state = 121;
                        this.attribute();
                    }
                }
                this.state = 124;
                this.id();
                this.state = 125;
                this.signature();
                this.state = 126;
                this.block();
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
    aliasDef() {
        let _localctx = new AliasDefContext(this._ctx, this.state);
        this.enterRule(_localctx, 12, CHeaderParser.RULE_aliasDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 129;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 128;
                        this.doc();
                    }
                }
                this.state = 131;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 133;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.STRUCT || _la === CHeaderParser.UNION) {
                    {
                        this.state = 132;
                        _la = this._input.LA(1);
                        if (!(_la === CHeaderParser.STRUCT || _la === CHeaderParser.UNION)) {
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
                this.state = 135;
                this.type();
                this.state = 136;
                this.id();
                this.state = 137;
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
    unionDef() {
        let _localctx = new UnionDefContext(this._ctx, this.state);
        this.enterRule(_localctx, 14, CHeaderParser.RULE_unionDef);
        let _la;
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
                this.state = 142;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 143;
                this.match(CHeaderParser.UNION);
                this.state = 144;
                this.id();
                this.state = 145;
                this.block();
                this.state = 146;
                this.id();
                this.state = 147;
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
    enumDef() {
        let _localctx = new EnumDefContext(this._ctx, this.state);
        this.enterRule(_localctx, 16, CHeaderParser.RULE_enumDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 150;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 149;
                        this.doc();
                    }
                }
                this.state = 152;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 153;
                this.match(CHeaderParser.ENUM);
                this.state = 154;
                this.id();
                this.state = 155;
                this.enumBody();
                this.state = 156;
                this.id();
                this.state = 157;
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
    structDef() {
        let _localctx = new StructDefContext(this._ctx, this.state);
        this.enterRule(_localctx, 18, CHeaderParser.RULE_structDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 160;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 159;
                        this.doc();
                    }
                }
                this.state = 162;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 163;
                this.match(CHeaderParser.STRUCT);
                this.state = 164;
                this.id();
                this.state = 165;
                this.structBody();
                this.state = 166;
                this.id();
                this.state = 167;
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
    callbackDef() {
        let _localctx = new CallbackDefContext(this._ctx, this.state);
        this.enterRule(_localctx, 20, CHeaderParser.RULE_callbackDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 170;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 169;
                        this.doc();
                    }
                }
                this.state = 172;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 173;
                this.type();
                this.state = 174;
                this.match(CHeaderParser.ROUND_B);
                this.state = 175;
                this.match(CHeaderParser.STAR);
                this.state = 176;
                this.id();
                this.state = 177;
                this.match(CHeaderParser.ROUND_E);
                this.state = 178;
                this.signature();
                this.state = 179;
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
    compileTimeAssert() {
        let _localctx = new CompileTimeAssertContext(this._ctx, this.state);
        this.enterRule(_localctx, 22, CHeaderParser.RULE_compileTimeAssert);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 181;
                this.match(CHeaderParser.SDL_COMPILE_TIME_ASSERT);
                this.state = 182;
                this.group();
                this.state = 183;
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
    inline() {
        let _localctx = new InlineContext(this._ctx, this.state);
        this.enterRule(_localctx, 24, CHeaderParser.RULE_inline);
        try {
            this.state = 188;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case CHeaderParser.SDL_INLINE:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 185;
                        this.match(CHeaderParser.SDL_INLINE);
                    }
                    break;
                case CHeaderParser.STATIC:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 186;
                        this.match(CHeaderParser.STATIC);
                        this.state = 187;
                        this.match(CHeaderParser.INLINE);
                    }
                    break;
                default:
                    throw new NoViableAltException_1.NoViableAltException(this);
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
    block() {
        let _localctx = new BlockContext(this._ctx, this.state);
        this.enterRule(_localctx, 26, CHeaderParser.RULE_block);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 190;
                this.match(CHeaderParser.CURLY_B);
                this.state = 194;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
                    {
                        {
                            this.state = 191;
                            this.stm();
                        }
                    }
                    this.state = 196;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 197;
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
    group() {
        let _localctx = new GroupContext(this._ctx, this.state);
        this.enterRule(_localctx, 28, CHeaderParser.RULE_group);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 199;
                this.match(CHeaderParser.ROUND_B);
                this.state = 203;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
                    {
                        {
                            this.state = 200;
                            this.stm();
                        }
                    }
                    this.state = 205;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 206;
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
    indexing() {
        let _localctx = new IndexingContext(this._ctx, this.state);
        this.enterRule(_localctx, 30, CHeaderParser.RULE_indexing);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 208;
                this.match(CHeaderParser.SQUARE_B);
                this.state = 212;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA))) !== 0) || ((((_la - 32)) & ~0x1F) === 0 && ((1 << (_la - 32)) & ((1 << (CHeaderParser.DOT - 32)) | (1 << (CHeaderParser.STAR - 32)) | (1 << (CHeaderParser.EQ - 32)) | (1 << (CHeaderParser.ELLIPSIS - 32)) | (1 << (CHeaderParser.PUNCT_EXTRA - 32)) | (1 << (CHeaderParser.STRING - 32)) | (1 << (CHeaderParser.ID - 32)) | (1 << (CHeaderParser.NUMBER - 32)))) !== 0)) {
                    {
                        {
                            this.state = 209;
                            this.stm();
                        }
                    }
                    this.state = 214;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 215;
                this.match(CHeaderParser.SQUARE_E);
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
        this.enterRule(_localctx, 32, CHeaderParser.RULE_stm);
        try {
            this.state = 223;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case CHeaderParser.CURLY_B:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 217;
                        this.block();
                    }
                    break;
                case CHeaderParser.SQUARE_B:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 218;
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
                        this.state = 219;
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
                        this.state = 220;
                        this.punct();
                    }
                    break;
                case CHeaderParser.LONG_DOC:
                case CHeaderParser.SHORT_DOC:
                    this.enterOuterAlt(_localctx, 5);
                    {
                        this.state = 221;
                        this.doc();
                    }
                    break;
                case CHeaderParser.TRAILING_DOC:
                    this.enterOuterAlt(_localctx, 6);
                    {
                        this.state = 222;
                        this.trailingDoc();
                    }
                    break;
                default:
                    throw new NoViableAltException_1.NoViableAltException(this);
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
    expr() {
        let _localctx = new ExprContext(this._ctx, this.state);
        this.enterRule(_localctx, 34, CHeaderParser.RULE_expr);
        try {
            this.state = 227;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case CHeaderParser.ROUND_B:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 225;
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
                        this.state = 226;
                        this.word();
                    }
                    break;
                default:
                    throw new NoViableAltException_1.NoViableAltException(this);
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
    word() {
        let _localctx = new WordContext(this._ctx, this.state);
        this.enterRule(_localctx, 36, CHeaderParser.RULE_word);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 229;
                _la = this._input.LA(1);
                if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || ((((_la - 37)) & ~0x1F) === 0 && ((1 << (_la - 37)) & ((1 << (CHeaderParser.STRING - 37)) | (1 << (CHeaderParser.ID - 37)) | (1 << (CHeaderParser.NUMBER - 37)))) !== 0))) {
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
    // @RuleVersion(0)
    punct() {
        let _localctx = new PunctContext(this._ctx, this.state);
        this.enterRule(_localctx, 38, CHeaderParser.RULE_punct);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 231;
                _la = this._input.LA(1);
                if (!(((((_la - 29)) & ~0x1F) === 0 && ((1 << (_la - 29)) & ((1 << (CHeaderParser.COLON - 29)) | (1 << (CHeaderParser.SEMI - 29)) | (1 << (CHeaderParser.COMMA - 29)) | (1 << (CHeaderParser.DOT - 29)) | (1 << (CHeaderParser.STAR - 29)) | (1 << (CHeaderParser.EQ - 29)) | (1 << (CHeaderParser.ELLIPSIS - 29)) | (1 << (CHeaderParser.PUNCT_EXTRA - 29)))) !== 0))) {
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
    // @RuleVersion(0)
    enumBody() {
        let _localctx = new EnumBodyContext(this._ctx, this.state);
        this.enterRule(_localctx, 40, CHeaderParser.RULE_enumBody);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 233;
                this.match(CHeaderParser.CURLY_B);
                this.state = 237;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC || _la === CHeaderParser.ID) {
                    {
                        {
                            this.state = 234;
                            this.enumItem();
                        }
                    }
                    this.state = 239;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 240;
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
    enumItem() {
        let _localctx = new EnumItemContext(this._ctx, this.state);
        this.enterRule(_localctx, 42, CHeaderParser.RULE_enumItem);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 243;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 242;
                        this.doc();
                    }
                }
                this.state = 245;
                this.id();
                this.state = 248;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.EQ) {
                    {
                        this.state = 246;
                        this.match(CHeaderParser.EQ);
                        this.state = 247;
                        this.expr();
                    }
                }
                this.state = 251;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.COMMA) {
                    {
                        this.state = 250;
                        this.match(CHeaderParser.COMMA);
                    }
                }
                this.state = 254;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.TRAILING_DOC) {
                    {
                        this.state = 253;
                        this.trailingDoc();
                    }
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
    structBody() {
        let _localctx = new StructBodyContext(this._ctx, this.state);
        this.enterRule(_localctx, 44, CHeaderParser.RULE_structBody);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 256;
                this.match(CHeaderParser.CURLY_B);
                this.state = 260;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.VOID))) !== 0) || _la === CHeaderParser.ID) {
                    {
                        {
                            this.state = 257;
                            this.structItem();
                        }
                    }
                    this.state = 262;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 263;
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
    structItem() {
        let _localctx = new StructItemContext(this._ctx, this.state);
        this.enterRule(_localctx, 46, CHeaderParser.RULE_structItem);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 266;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 265;
                        this.doc();
                    }
                }
                this.state = 272;
                this._errHandler.sync(this);
                switch (this.interpreter.adaptivePredict(this._input, 30, this._ctx)) {
                    case 1:
                        {
                            this.state = 269;
                            this._errHandler.sync(this);
                            _la = this._input.LA(1);
                            if (_la === CHeaderParser.CONST) {
                                {
                                    this.state = 268;
                                    this.match(CHeaderParser.CONST);
                                }
                            }
                            this.state = 271;
                            this.match(CHeaderParser.STRUCT);
                        }
                        break;
                }
                this.state = 274;
                this.type();
                this.state = 275;
                this.id();
                this.state = 280;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.COMMA) {
                    {
                        {
                            this.state = 276;
                            this.match(CHeaderParser.COMMA);
                            this.state = 277;
                            this.id();
                        }
                    }
                    this.state = 282;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 286;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.SQUARE_B) {
                    {
                        {
                            this.state = 283;
                            this.indexing();
                        }
                    }
                    this.state = 288;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 289;
                this.match(CHeaderParser.SEMI);
                this.state = 291;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.TRAILING_DOC) {
                    {
                        this.state = 290;
                        this.trailingDoc();
                    }
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
    id() {
        let _localctx = new IdContext(this._ctx, this.state);
        this.enterRule(_localctx, 48, CHeaderParser.RULE_id);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 293;
                this.match(CHeaderParser.ID);
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
        this.enterRule(_localctx, 50, CHeaderParser.RULE_type);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 296;
                this._errHandler.sync(this);
                _alt = 1;
                do {
                    switch (_alt) {
                        case 1:
                            {
                                {
                                    this.state = 295;
                                    this.typeEl();
                                }
                            }
                            break;
                        default:
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 298;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 34, this._ctx);
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
        this.enterRule(_localctx, 52, CHeaderParser.RULE_typeEl);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 300;
                _la = this._input.LA(1);
                if (!(((((_la - 10)) & ~0x1F) === 0 && ((1 << (_la - 10)) & ((1 << (CHeaderParser.CONST - 10)) | (1 << (CHeaderParser.VOID - 10)) | (1 << (CHeaderParser.ID - 10)))) !== 0))) {
                    this._errHandler.recoverInline(this);
                }
                else {
                    if (this._input.LA(1) === Token_1.Token.EOF) {
                        this.matchedEOF = true;
                    }
                    this._errHandler.reportMatch(this);
                    this.consume();
                }
                this.state = 304;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.STAR) {
                    {
                        {
                            this.state = 301;
                            this.match(CHeaderParser.STAR);
                        }
                    }
                    this.state = 306;
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
        this.enterRule(_localctx, 54, CHeaderParser.RULE_signature);
        let _la;
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 307;
                this.match(CHeaderParser.ROUND_B);
                this.state = 316;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (((((_la - 10)) & ~0x1F) === 0 && ((1 << (_la - 10)) & ((1 << (CHeaderParser.CONST - 10)) | (1 << (CHeaderParser.VOID - 10)) | (1 << (CHeaderParser.ID - 10)))) !== 0)) {
                    {
                        this.state = 308;
                        this.type();
                        this.state = 313;
                        this._errHandler.sync(this);
                        _alt = this.interpreter.adaptivePredict(this._input, 36, this._ctx);
                        while (_alt !== 2 && _alt !== ATN_1.ATN.INVALID_ALT_NUMBER) {
                            if (_alt === 1) {
                                {
                                    {
                                        this.state = 309;
                                        this.match(CHeaderParser.COMMA);
                                        this.state = 310;
                                        this.type();
                                    }
                                }
                            }
                            this.state = 315;
                            this._errHandler.sync(this);
                            _alt = this.interpreter.adaptivePredict(this._input, 36, this._ctx);
                        }
                    }
                }
                this.state = 320;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.COMMA) {
                    {
                        this.state = 318;
                        this.match(CHeaderParser.COMMA);
                        this.state = 319;
                        this.match(CHeaderParser.ELLIPSIS);
                    }
                }
                this.state = 322;
                this.match(CHeaderParser.ROUND_E);
                this.state = 324;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.ATTRIBUTE || _la === CHeaderParser.SDL_VARARG_ATTRIB) {
                    {
                        this.state = 323;
                        this.attribute();
                    }
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
    attribute() {
        let _localctx = new AttributeContext(this._ctx, this.state);
        this.enterRule(_localctx, 56, CHeaderParser.RULE_attribute);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 326;
                _la = this._input.LA(1);
                if (!(_la === CHeaderParser.ATTRIBUTE || _la === CHeaderParser.SDL_VARARG_ATTRIB)) {
                    this._errHandler.recoverInline(this);
                }
                else {
                    if (this._input.LA(1) === Token_1.Token.EOF) {
                        this.matchedEOF = true;
                    }
                    this._errHandler.reportMatch(this);
                    this.consume();
                }
                this.state = 327;
                this.group();
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
        this.enterRule(_localctx, 58, CHeaderParser.RULE_doc);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 329;
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
    // @RuleVersion(0)
    trailingDoc() {
        let _localctx = new TrailingDocContext(this._ctx, this.state);
        this.enterRule(_localctx, 60, CHeaderParser.RULE_trailingDoc);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 331;
                this.match(CHeaderParser.TRAILING_DOC);
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
CHeaderParser.TRAILING_DOC = 4;
CHeaderParser.LONG_DOC = 5;
CHeaderParser.SHORT_DOC = 6;
CHeaderParser.DEFINE = 7;
CHeaderParser.DIRECTIVE = 8;
CHeaderParser.ATTRIBUTE = 9;
CHeaderParser.CONST = 10;
CHeaderParser.ENUM = 11;
CHeaderParser.EXTERN = 12;
CHeaderParser.INLINE = 13;
CHeaderParser.SDL_NOISE = 14;
CHeaderParser.SDL_INLINE = 15;
CHeaderParser.STATIC = 16;
CHeaderParser.STRUCT = 17;
CHeaderParser.TYPEDEF = 18;
CHeaderParser.UNION = 19;
CHeaderParser.VOID = 20;
CHeaderParser.SDL_VARARG_ATTRIB = 21;
CHeaderParser.SDL_COMPILE_TIME_ASSERT = 22;
CHeaderParser.CURLY_B = 23;
CHeaderParser.CURLY_E = 24;
CHeaderParser.ROUND_B = 25;
CHeaderParser.ROUND_E = 26;
CHeaderParser.SQUARE_B = 27;
CHeaderParser.SQUARE_E = 28;
CHeaderParser.COLON = 29;
CHeaderParser.SEMI = 30;
CHeaderParser.COMMA = 31;
CHeaderParser.DOT = 32;
CHeaderParser.STAR = 33;
CHeaderParser.EQ = 34;
CHeaderParser.ELLIPSIS = 35;
CHeaderParser.PUNCT_EXTRA = 36;
CHeaderParser.STRING = 37;
CHeaderParser.ID = 38;
CHeaderParser.NUMBER = 39;
CHeaderParser.RULE_prog = 0;
CHeaderParser.RULE_decl = 1;
CHeaderParser.RULE_externC = 2;
CHeaderParser.RULE_directive = 3;
CHeaderParser.RULE_functionDecl = 4;
CHeaderParser.RULE_functionDef = 5;
CHeaderParser.RULE_aliasDef = 6;
CHeaderParser.RULE_unionDef = 7;
CHeaderParser.RULE_enumDef = 8;
CHeaderParser.RULE_structDef = 9;
CHeaderParser.RULE_callbackDef = 10;
CHeaderParser.RULE_compileTimeAssert = 11;
CHeaderParser.RULE_inline = 12;
CHeaderParser.RULE_block = 13;
CHeaderParser.RULE_group = 14;
CHeaderParser.RULE_indexing = 15;
CHeaderParser.RULE_stm = 16;
CHeaderParser.RULE_expr = 17;
CHeaderParser.RULE_word = 18;
CHeaderParser.RULE_punct = 19;
CHeaderParser.RULE_enumBody = 20;
CHeaderParser.RULE_enumItem = 21;
CHeaderParser.RULE_structBody = 22;
CHeaderParser.RULE_structItem = 23;
CHeaderParser.RULE_id = 24;
CHeaderParser.RULE_type = 25;
CHeaderParser.RULE_typeEl = 26;
CHeaderParser.RULE_signature = 27;
CHeaderParser.RULE_attribute = 28;
CHeaderParser.RULE_doc = 29;
CHeaderParser.RULE_trailingDoc = 30;
// tslint:disable:no-trailing-whitespace
CHeaderParser.ruleNames = [
    "prog", "decl", "externC", "directive", "functionDecl", "functionDef",
    "aliasDef", "unionDef", "enumDef", "structDef", "callbackDef", "compileTimeAssert",
    "inline", "block", "group", "indexing", "stm", "expr", "word", "punct",
    "enumBody", "enumItem", "structBody", "structItem", "id", "type", "typeEl",
    "signature", "attribute", "doc", "trailingDoc",
];
CHeaderParser._LITERAL_NAMES = [
    undefined, undefined, undefined, undefined, undefined, undefined, undefined,
    undefined, undefined, "'__attribute__'", "'const'", "'enum'", "'extern'",
    "'__inline__'", undefined, "'SDL_FORCE_INLINE'", "'static'", "'struct'",
    "'typedef'", "'union'", "'void'", undefined, "'SDL_COMPILE_TIME_ASSERT'",
    "'{'", "'}'", "'('", "')'", "'['", "']'", "':'", "';'", "','", "'.'",
    "'*'", "'='", "'...'",
];
CHeaderParser._SYMBOLIC_NAMES = [
    undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "TRAILING_DOC", "LONG_DOC",
    "SHORT_DOC", "DEFINE", "DIRECTIVE", "ATTRIBUTE", "CONST", "ENUM", "EXTERN",
    "INLINE", "SDL_NOISE", "SDL_INLINE", "STATIC", "STRUCT", "TYPEDEF", "UNION",
    "VOID", "SDL_VARARG_ATTRIB", "SDL_COMPILE_TIME_ASSERT", "CURLY_B", "CURLY_E",
    "ROUND_B", "ROUND_E", "SQUARE_B", "SQUARE_E", "COLON", "SEMI", "COMMA",
    "DOT", "STAR", "EQ", "ELLIPSIS", "PUNCT_EXTRA", "STRING", "ID", "NUMBER",
];
CHeaderParser.VOCABULARY = new VocabularyImpl_1.VocabularyImpl(CHeaderParser._LITERAL_NAMES, CHeaderParser._SYMBOLIC_NAMES, []);
CHeaderParser._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03)\u0150\x04\x02" +
    "\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
    "\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r\t\r\x04" +
    "\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x04\x11\t\x11\x04\x12\t\x12\x04" +
    "\x13\t\x13\x04\x14\t\x14\x04\x15\t\x15\x04\x16\t\x16\x04\x17\t\x17\x04" +
    "\x18\t\x18\x04\x19\t\x19\x04\x1A\t\x1A\x04\x1B\t\x1B\x04\x1C\t\x1C\x04" +
    "\x1D\t\x1D\x04\x1E\t\x1E\x04\x1F\t\x1F\x04 \t \x03\x02\x05\x02B\n\x02" +
    "\x03\x02\x07\x02E\n\x02\f\x02\x0E\x02H\v\x02\x03\x02\x03\x02\x03\x03\x03" +
    "\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03" +
    "\x03\x05\x03W\n\x03\x03\x04\x03\x04\x03\x04\x03\x04\x07\x04]\n\x04\f\x04" +
    "\x0E\x04`\v\x04\x03\x04\x03\x04\x03\x05\x05\x05e\n\x05\x03\x05\x03\x05" +
    "\x03\x06\x05\x06j\n\x06\x03\x06\x05\x06m\n\x06\x03\x06\x03\x06\x05\x06" +
    "q\n\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x07\x05\x07x\n\x07\x03\x07" +
    "\x03\x07\x03\x07\x05\x07}\n\x07\x03\x07\x03\x07\x03\x07\x03\x07\x03\b" +
    "\x05\b\x84\n\b\x03\b\x03\b\x05\b\x88\n\b\x03\b\x03\b\x03\b\x03\b\x03\t" +
    "\x05\t\x8F\n\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\n\x05\n\x99" +
    "\n\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\n\x03\v\x05\v\xA3\n\v\x03" +
    "\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\f\x05\f\xAD\n\f\x03\f\x03\f" +
    "\x03\f\x03\f\x03\f\x03\f\x03\f\x03\f\x03\f\x03\r\x03\r\x03\r\x03\r\x03" +
    "\x0E\x03\x0E\x03\x0E\x05\x0E\xBF\n\x0E\x03\x0F\x03\x0F\x07\x0F\xC3\n\x0F" +
    "\f\x0F\x0E\x0F\xC6\v\x0F\x03\x0F\x03\x0F\x03\x10\x03\x10\x07\x10\xCC\n" +
    "\x10\f\x10\x0E\x10\xCF\v\x10\x03\x10\x03\x10\x03\x11\x03\x11\x07\x11\xD5" +
    "\n\x11\f\x11\x0E\x11\xD8\v\x11\x03\x11\x03\x11\x03\x12\x03\x12\x03\x12" +
    "\x03\x12\x03\x12\x03\x12\x05\x12\xE2\n\x12\x03\x13\x03\x13\x05\x13\xE6" +
    "\n\x13\x03\x14\x03\x14\x03\x15\x03\x15\x03\x16\x03\x16\x07\x16\xEE\n\x16" +
    "\f\x16\x0E\x16\xF1\v\x16\x03\x16\x03\x16\x03\x17\x05\x17\xF6\n\x17\x03" +
    "\x17\x03\x17\x03\x17\x05\x17\xFB\n\x17\x03\x17\x05\x17\xFE\n\x17\x03\x17" +
    "\x05\x17\u0101\n\x17\x03\x18\x03\x18\x07\x18\u0105\n\x18\f\x18\x0E\x18" +
    "\u0108\v\x18\x03\x18\x03\x18\x03\x19\x05\x19\u010D\n\x19\x03\x19\x05\x19" +
    "\u0110\n\x19\x03\x19\x05\x19\u0113\n\x19\x03\x19\x03\x19\x03\x19\x03\x19" +
    "\x07\x19\u0119\n\x19\f\x19\x0E\x19\u011C\v\x19\x03\x19\x07\x19\u011F\n" +
    "\x19\f\x19\x0E\x19\u0122\v\x19\x03\x19\x03\x19\x05\x19\u0126\n\x19\x03" +
    "\x1A\x03\x1A\x03\x1B\x06\x1B\u012B\n\x1B\r\x1B\x0E\x1B\u012C\x03\x1C\x03" +
    "\x1C\x07\x1C\u0131\n\x1C\f\x1C\x0E\x1C\u0134\v\x1C\x03\x1D\x03\x1D\x03" +
    "\x1D\x03\x1D\x07\x1D\u013A\n\x1D\f\x1D\x0E\x1D\u013D\v\x1D\x05\x1D\u013F" +
    "\n\x1D\x03\x1D\x03\x1D\x05\x1D\u0143\n\x1D\x03\x1D\x03\x1D\x05\x1D\u0147" +
    "\n\x1D\x03\x1E\x03\x1E\x03\x1E\x03\x1F\x03\x1F\x03 \x03 \x03 \x02\x02" +
    "\x02!\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E\x02\x10\x02\x12\x02" +
    "\x14\x02\x16\x02\x18\x02\x1A\x02\x1C\x02\x1E\x02 \x02\"\x02$\x02&\x02" +
    "(\x02*\x02,\x02.\x020\x022\x024\x026\x028\x02:\x02<\x02>\x02\x02\b\x04" +
    "\x02\x13\x13\x15\x15\b\x02\t\t\f\r\x12\x13\x15\x16\x18\x18\')\x03\x02" +
    "\x1F&\x05\x02\f\f\x16\x16((\x04\x02\v\v\x17\x17\x03\x02\x07\b\x02\u0165" +
    "\x02A\x03\x02\x02\x02\x04V\x03\x02\x02\x02\x06X\x03\x02\x02\x02\bd\x03" +
    "\x02\x02\x02\ni\x03\x02\x02\x02\fw\x03\x02\x02\x02\x0E\x83\x03\x02\x02" +
    "\x02\x10\x8E\x03\x02\x02\x02\x12\x98\x03\x02\x02\x02\x14\xA2\x03\x02\x02" +
    "\x02\x16\xAC\x03\x02\x02\x02\x18\xB7\x03\x02\x02\x02\x1A\xBE\x03\x02\x02" +
    "\x02\x1C\xC0\x03\x02\x02\x02\x1E\xC9\x03\x02\x02\x02 \xD2\x03\x02\x02" +
    "\x02\"\xE1\x03\x02\x02\x02$\xE5\x03\x02\x02\x02&\xE7\x03\x02\x02\x02(" +
    "\xE9\x03\x02\x02\x02*\xEB\x03\x02\x02\x02,\xF5\x03\x02\x02\x02.\u0102" +
    "\x03\x02\x02\x020\u010C\x03\x02\x02\x022\u0127\x03\x02\x02\x024\u012A" +
    "\x03\x02\x02\x026\u012E\x03\x02\x02\x028\u0135\x03\x02\x02\x02:\u0148" +
    "\x03\x02\x02\x02<\u014B\x03\x02\x02\x02>\u014D\x03\x02\x02\x02@B\x05<" +
    "\x1F\x02A@\x03\x02\x02\x02AB\x03\x02\x02\x02BF\x03\x02\x02\x02CE\x05\x04" +
    "\x03\x02DC\x03\x02\x02\x02EH\x03\x02\x02\x02FD\x03\x02\x02\x02FG\x03\x02" +
    "\x02\x02GI\x03\x02\x02\x02HF\x03\x02\x02\x02IJ\x07\x02\x02\x03J\x03\x03" +
    "\x02\x02\x02KW\x05\b\x05\x02LW\x05\x06\x04\x02MW\x05\n\x06\x02NW\x05\f" +
    "\x07\x02OW\x05\x0E\b\x02PW\x05\x10\t\x02QW\x05\x12\n\x02RW\x05\x14\v\x02" +
    "SW\x05\x16\f\x02TW\x05\x18\r\x02UW\x05<\x1F\x02VK\x03\x02\x02\x02VL\x03" +
    "\x02\x02\x02VM\x03\x02\x02\x02VN\x03\x02\x02\x02VO\x03\x02\x02\x02VP\x03" +
    "\x02\x02\x02VQ\x03\x02\x02\x02VR\x03\x02\x02\x02VS\x03\x02\x02\x02VT\x03" +
    "\x02\x02\x02VU\x03\x02\x02\x02W\x05\x03\x02\x02\x02XY\x07\x0E\x02\x02" +
    "YZ\x07\'\x02\x02Z^\x07\x19\x02\x02[]\x05\x04\x03\x02\\[\x03\x02\x02\x02" +
    "]`\x03\x02\x02\x02^\\\x03\x02\x02\x02^_\x03\x02\x02\x02_a\x03\x02\x02" +
    "\x02`^\x03\x02\x02\x02ab\x07\x1A\x02\x02b\x07\x03\x02\x02\x02ce\x05<\x1F" +
    "\x02dc\x03\x02\x02\x02de\x03\x02\x02\x02ef\x03\x02\x02\x02fg\x07\t\x02" +
    "\x02g\t\x03\x02\x02\x02hj\x05<\x1F\x02ih\x03\x02\x02\x02ij\x03\x02\x02" +
    "\x02jl\x03\x02\x02\x02km\x07\x0E\x02\x02lk\x03\x02\x02\x02lm\x03\x02\x02" +
    "\x02mn\x03\x02\x02\x02np\x054\x1B\x02oq\x05:\x1E\x02po\x03\x02\x02\x02" +
    "pq\x03\x02\x02\x02qr\x03\x02\x02\x02rs\x052\x1A\x02st\x058\x1D\x02tu\x07" +
    " \x02\x02u\v\x03\x02\x02\x02vx\x05<\x1F\x02wv\x03\x02\x02\x02wx\x03\x02" +
    "\x02\x02xy\x03\x02\x02\x02yz\x05\x1A\x0E\x02z|\x054\x1B\x02{}\x05:\x1E" +
    "\x02|{\x03\x02\x02\x02|}\x03\x02\x02\x02}~\x03\x02\x02\x02~\x7F\x052\x1A" +
    "\x02\x7F\x80\x058\x1D\x02\x80\x81\x05\x1C\x0F\x02\x81\r\x03\x02\x02\x02" +
    "\x82\x84\x05<\x1F\x02\x83\x82\x03\x02\x02\x02\x83\x84\x03\x02\x02\x02" +
    "\x84\x85\x03\x02\x02\x02\x85\x87\x07\x14\x02\x02\x86\x88\t\x02\x02\x02" +
    "\x87\x86\x03\x02\x02\x02\x87\x88\x03\x02\x02\x02\x88\x89\x03\x02\x02\x02" +
    "\x89\x8A\x054\x1B\x02\x8A\x8B\x052\x1A\x02\x8B\x8C\x07 \x02\x02\x8C\x0F" +
    "\x03\x02\x02\x02\x8D\x8F\x05<\x1F\x02\x8E\x8D\x03\x02\x02\x02\x8E\x8F" +
    "\x03\x02\x02\x02\x8F\x90\x03\x02\x02\x02\x90\x91\x07\x14\x02\x02\x91\x92" +
    "\x07\x15\x02\x02\x92\x93\x052\x1A\x02\x93\x94\x05\x1C\x0F\x02\x94\x95" +
    "\x052\x1A\x02\x95\x96\x07 \x02\x02\x96\x11\x03\x02\x02\x02\x97\x99\x05" +
    "<\x1F\x02\x98\x97\x03\x02\x02\x02\x98\x99\x03\x02\x02\x02\x99\x9A\x03" +
    "\x02\x02\x02\x9A\x9B\x07\x14\x02\x02\x9B\x9C\x07\r\x02\x02\x9C\x9D\x05" +
    "2\x1A\x02\x9D\x9E\x05*\x16\x02\x9E\x9F\x052\x1A\x02\x9F\xA0\x07 \x02\x02" +
    "\xA0\x13\x03\x02\x02\x02\xA1\xA3\x05<\x1F\x02\xA2\xA1\x03\x02\x02\x02" +
    "\xA2\xA3\x03\x02\x02\x02\xA3\xA4\x03\x02\x02\x02\xA4\xA5\x07\x14\x02\x02" +
    "\xA5\xA6\x07\x13\x02\x02\xA6\xA7\x052\x1A\x02\xA7\xA8\x05.\x18\x02\xA8" +
    "\xA9\x052\x1A\x02\xA9\xAA\x07 \x02\x02\xAA\x15\x03\x02\x02\x02\xAB\xAD" +
    "\x05<\x1F\x02\xAC\xAB\x03\x02\x02\x02\xAC\xAD\x03\x02\x02\x02\xAD\xAE" +
    "\x03\x02\x02\x02\xAE\xAF\x07\x14\x02\x02\xAF\xB0\x054\x1B\x02\xB0\xB1" +
    "\x07\x1B\x02\x02\xB1\xB2\x07#\x02\x02\xB2\xB3\x052\x1A\x02\xB3\xB4\x07" +
    "\x1C\x02\x02\xB4\xB5\x058\x1D\x02\xB5\xB6\x07 \x02\x02\xB6\x17\x03\x02" +
    "\x02\x02\xB7\xB8\x07\x18\x02\x02\xB8\xB9\x05\x1E\x10\x02\xB9\xBA\x07 " +
    "\x02\x02\xBA\x19\x03\x02\x02\x02\xBB\xBF\x07\x11\x02\x02\xBC\xBD\x07\x12" +
    "\x02\x02\xBD\xBF\x07\x0F\x02\x02\xBE\xBB\x03\x02\x02\x02\xBE\xBC\x03\x02" +
    "\x02\x02\xBF\x1B\x03\x02\x02\x02\xC0\xC4\x07\x19\x02\x02\xC1\xC3\x05\"" +
    "\x12\x02\xC2\xC1\x03\x02\x02\x02\xC3\xC6\x03\x02\x02\x02\xC4\xC2\x03\x02" +
    "\x02\x02\xC4\xC5\x03\x02\x02\x02\xC5\xC7\x03\x02\x02\x02\xC6\xC4\x03\x02" +
    "\x02\x02\xC7\xC8\x07\x1A\x02\x02\xC8\x1D\x03\x02\x02\x02\xC9\xCD\x07\x1B" +
    "\x02\x02\xCA\xCC\x05\"\x12\x02\xCB\xCA\x03\x02\x02\x02\xCC\xCF\x03\x02" +
    "\x02\x02\xCD\xCB\x03\x02\x02\x02\xCD\xCE\x03\x02\x02\x02\xCE\xD0\x03\x02" +
    "\x02\x02\xCF\xCD\x03\x02\x02\x02\xD0\xD1\x07\x1C\x02\x02\xD1\x1F\x03\x02" +
    "\x02\x02\xD2\xD6\x07\x1D\x02\x02\xD3\xD5\x05\"\x12\x02\xD4\xD3\x03\x02" +
    "\x02\x02\xD5\xD8\x03\x02\x02\x02\xD6\xD4\x03\x02\x02\x02\xD6\xD7\x03\x02" +
    "\x02\x02\xD7\xD9\x03\x02\x02\x02\xD8\xD6\x03\x02\x02\x02\xD9\xDA\x07\x1E" +
    "\x02\x02\xDA!\x03\x02\x02\x02\xDB\xE2\x05\x1C\x0F\x02\xDC\xE2\x05 \x11" +
    "\x02\xDD\xE2\x05$\x13\x02\xDE\xE2\x05(\x15\x02\xDF\xE2\x05<\x1F\x02\xE0" +
    "\xE2\x05> \x02\xE1\xDB\x03\x02\x02\x02\xE1\xDC\x03\x02\x02\x02\xE1\xDD" +
    "\x03\x02\x02\x02\xE1\xDE\x03\x02\x02\x02\xE1\xDF\x03\x02\x02\x02\xE1\xE0" +
    "\x03\x02\x02\x02\xE2#\x03\x02\x02\x02\xE3\xE6\x05\x1E\x10\x02\xE4\xE6" +
    "\x05&\x14\x02\xE5\xE3\x03\x02\x02\x02\xE5\xE4\x03\x02\x02\x02\xE6%\x03" +
    "\x02\x02\x02\xE7\xE8\t\x03\x02\x02\xE8\'\x03\x02\x02\x02\xE9\xEA\t\x04" +
    "\x02\x02\xEA)\x03\x02\x02\x02\xEB\xEF\x07\x19\x02\x02\xEC\xEE\x05,\x17" +
    "\x02\xED\xEC\x03\x02\x02\x02\xEE\xF1\x03\x02\x02\x02\xEF\xED\x03\x02\x02" +
    "\x02\xEF\xF0\x03\x02\x02\x02\xF0\xF2\x03\x02\x02\x02\xF1\xEF\x03\x02\x02" +
    "\x02\xF2\xF3\x07\x1A\x02\x02\xF3+\x03\x02\x02\x02\xF4\xF6\x05<\x1F\x02" +
    "\xF5\xF4\x03\x02\x02\x02\xF5\xF6\x03\x02\x02\x02\xF6\xF7\x03\x02\x02\x02" +
    "\xF7\xFA\x052\x1A\x02\xF8\xF9\x07$\x02\x02\xF9\xFB\x05$\x13\x02\xFA\xF8" +
    "\x03\x02\x02\x02\xFA\xFB\x03\x02\x02\x02\xFB\xFD\x03\x02\x02\x02\xFC\xFE" +
    "\x07!\x02\x02\xFD\xFC\x03\x02\x02\x02\xFD\xFE\x03\x02\x02\x02\xFE\u0100" +
    "\x03\x02\x02\x02\xFF\u0101\x05> \x02\u0100\xFF\x03\x02\x02\x02\u0100\u0101" +
    "\x03\x02\x02\x02\u0101-\x03\x02\x02\x02\u0102\u0106\x07\x19\x02\x02\u0103" +
    "\u0105\x050\x19\x02\u0104\u0103\x03\x02\x02\x02\u0105\u0108\x03\x02\x02" +
    "\x02\u0106\u0104\x03\x02\x02\x02\u0106\u0107\x03\x02\x02\x02\u0107\u0109" +
    "\x03\x02\x02\x02\u0108\u0106\x03\x02\x02\x02\u0109\u010A\x07\x1A\x02\x02" +
    "\u010A/\x03\x02\x02\x02\u010B\u010D\x05<\x1F\x02\u010C\u010B\x03\x02\x02" +
    "\x02\u010C\u010D\x03\x02\x02\x02\u010D\u0112\x03\x02\x02\x02\u010E\u0110" +
    "\x07\f\x02\x02\u010F\u010E\x03\x02\x02\x02\u010F\u0110\x03\x02\x02\x02" +
    "\u0110\u0111\x03\x02\x02\x02\u0111\u0113\x07\x13\x02\x02\u0112\u010F\x03" +
    "\x02\x02\x02\u0112\u0113\x03\x02\x02\x02\u0113\u0114\x03\x02\x02\x02\u0114" +
    "\u0115\x054\x1B\x02\u0115\u011A\x052\x1A\x02\u0116\u0117\x07!\x02\x02" +
    "\u0117\u0119\x052\x1A\x02\u0118\u0116\x03\x02\x02\x02\u0119\u011C\x03" +
    "\x02\x02\x02\u011A\u0118\x03\x02\x02\x02\u011A\u011B\x03\x02\x02\x02\u011B" +
    "\u0120\x03\x02\x02\x02\u011C\u011A\x03\x02\x02\x02\u011D\u011F\x05 \x11" +
    "\x02\u011E\u011D\x03\x02\x02\x02\u011F\u0122\x03\x02\x02\x02\u0120\u011E" +
    "\x03\x02\x02\x02\u0120\u0121\x03\x02\x02\x02\u0121\u0123\x03\x02\x02\x02" +
    "\u0122\u0120\x03\x02\x02\x02\u0123\u0125\x07 \x02\x02\u0124\u0126\x05" +
    "> \x02\u0125\u0124\x03\x02\x02\x02\u0125\u0126\x03\x02\x02\x02\u01261" +
    "\x03\x02\x02\x02\u0127\u0128\x07(\x02\x02\u01283\x03\x02\x02\x02\u0129" +
    "\u012B\x056\x1C\x02\u012A\u0129\x03\x02\x02\x02\u012B\u012C\x03\x02\x02" +
    "\x02\u012C\u012A\x03\x02\x02\x02\u012C\u012D\x03\x02\x02\x02\u012D5\x03" +
    "\x02\x02\x02\u012E\u0132\t\x05\x02\x02\u012F\u0131\x07#\x02\x02\u0130" +
    "\u012F\x03\x02\x02\x02\u0131\u0134\x03\x02\x02\x02\u0132\u0130\x03\x02" +
    "\x02\x02\u0132\u0133\x03\x02\x02\x02\u01337\x03\x02\x02\x02\u0134\u0132" +
    "\x03\x02\x02\x02\u0135\u013E\x07\x1B\x02\x02\u0136\u013B\x054\x1B\x02" +
    "\u0137\u0138\x07!\x02\x02\u0138\u013A\x054\x1B\x02\u0139\u0137\x03\x02" +
    "\x02\x02\u013A\u013D\x03\x02\x02\x02\u013B\u0139\x03\x02\x02\x02\u013B" +
    "\u013C\x03\x02\x02\x02\u013C\u013F\x03\x02\x02\x02\u013D\u013B\x03\x02" +
    "\x02\x02\u013E\u0136\x03\x02\x02\x02\u013E\u013F\x03\x02\x02\x02\u013F" +
    "\u0142\x03\x02\x02\x02\u0140\u0141\x07!\x02\x02\u0141\u0143\x07%\x02\x02" +
    "\u0142\u0140\x03\x02\x02\x02\u0142\u0143\x03\x02\x02\x02\u0143\u0144\x03" +
    "\x02\x02\x02\u0144\u0146\x07\x1C\x02\x02\u0145\u0147\x05:\x1E\x02\u0146" +
    "\u0145\x03\x02\x02\x02\u0146\u0147\x03\x02\x02\x02\u01479\x03\x02\x02" +
    "\x02\u0148\u0149\t\x06\x02\x02\u0149\u014A\x05\x1E\x10\x02\u014A;\x03" +
    "\x02\x02\x02\u014B\u014C\t\x07\x02\x02\u014C=\x03\x02\x02\x02\u014D\u014E" +
    "\x07\x06\x02\x02\u014E?\x03\x02\x02\x02*AFV^dilpw|\x83\x87\x8E\x98\xA2" +
    "\xAC\xBE\xC4\xCD\xD6\xE1\xE5\xEF\xF5\xFA\xFD\u0100\u0106\u010C\u010F\u0112" +
    "\u011A\u0120\u0125\u012C\u0132\u013B\u013E\u0142\u0146";
class ProgContext extends ParserRuleContext_1.ParserRuleContext {
    EOF() { return this.getToken(CHeaderParser.EOF, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    decl(i) {
        if (i === undefined) {
            return this.getRuleContexts(DeclContext);
        }
        else {
            return this.getRuleContext(i, DeclContext);
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
class DeclContext extends ParserRuleContext_1.ParserRuleContext {
    directive() {
        return this.tryGetRuleContext(0, DirectiveContext);
    }
    externC() {
        return this.tryGetRuleContext(0, ExternCContext);
    }
    functionDecl() {
        return this.tryGetRuleContext(0, FunctionDeclContext);
    }
    functionDef() {
        return this.tryGetRuleContext(0, FunctionDefContext);
    }
    aliasDef() {
        return this.tryGetRuleContext(0, AliasDefContext);
    }
    unionDef() {
        return this.tryGetRuleContext(0, UnionDefContext);
    }
    enumDef() {
        return this.tryGetRuleContext(0, EnumDefContext);
    }
    structDef() {
        return this.tryGetRuleContext(0, StructDefContext);
    }
    callbackDef() {
        return this.tryGetRuleContext(0, CallbackDefContext);
    }
    compileTimeAssert() {
        return this.tryGetRuleContext(0, CompileTimeAssertContext);
    }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_decl; }
    // @Override
    enterRule(listener) {
        if (listener.enterDecl) {
            listener.enterDecl(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitDecl) {
            listener.exitDecl(this);
        }
    }
}
exports.DeclContext = DeclContext;
class ExternCContext extends ParserRuleContext_1.ParserRuleContext {
    EXTERN() { return this.getToken(CHeaderParser.EXTERN, 0); }
    STRING() { return this.getToken(CHeaderParser.STRING, 0); }
    CURLY_B() { return this.getToken(CHeaderParser.CURLY_B, 0); }
    CURLY_E() { return this.getToken(CHeaderParser.CURLY_E, 0); }
    decl(i) {
        if (i === undefined) {
            return this.getRuleContexts(DeclContext);
        }
        else {
            return this.getRuleContext(i, DeclContext);
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
    DEFINE() { return this.getToken(CHeaderParser.DEFINE, 0); }
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
    type() {
        return this.getRuleContext(0, TypeContext);
    }
    id() {
        return this.getRuleContext(0, IdContext);
    }
    signature() {
        return this.getRuleContext(0, SignatureContext);
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    EXTERN() { return this.tryGetToken(CHeaderParser.EXTERN, 0); }
    attribute() {
        return this.tryGetRuleContext(0, AttributeContext);
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
class FunctionDefContext extends ParserRuleContext_1.ParserRuleContext {
    inline() {
        return this.getRuleContext(0, InlineContext);
    }
    type() {
        return this.getRuleContext(0, TypeContext);
    }
    id() {
        return this.getRuleContext(0, IdContext);
    }
    signature() {
        return this.getRuleContext(0, SignatureContext);
    }
    block() {
        return this.getRuleContext(0, BlockContext);
    }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    attribute() {
        return this.tryGetRuleContext(0, AttributeContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_functionDef; }
    // @Override
    enterRule(listener) {
        if (listener.enterFunctionDef) {
            listener.enterFunctionDef(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitFunctionDef) {
            listener.exitFunctionDef(this);
        }
    }
}
exports.FunctionDefContext = FunctionDefContext;
class AliasDefContext extends ParserRuleContext_1.ParserRuleContext {
    TYPEDEF() { return this.getToken(CHeaderParser.TYPEDEF, 0); }
    type() {
        return this.getRuleContext(0, TypeContext);
    }
    id() {
        return this.getRuleContext(0, IdContext);
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    UNION() { return this.tryGetToken(CHeaderParser.UNION, 0); }
    STRUCT() { return this.tryGetToken(CHeaderParser.STRUCT, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_aliasDef; }
    // @Override
    enterRule(listener) {
        if (listener.enterAliasDef) {
            listener.enterAliasDef(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitAliasDef) {
            listener.exitAliasDef(this);
        }
    }
}
exports.AliasDefContext = AliasDefContext;
class UnionDefContext extends ParserRuleContext_1.ParserRuleContext {
    TYPEDEF() { return this.getToken(CHeaderParser.TYPEDEF, 0); }
    UNION() { return this.getToken(CHeaderParser.UNION, 0); }
    id(i) {
        if (i === undefined) {
            return this.getRuleContexts(IdContext);
        }
        else {
            return this.getRuleContext(i, IdContext);
        }
    }
    block() {
        return this.getRuleContext(0, BlockContext);
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_unionDef; }
    // @Override
    enterRule(listener) {
        if (listener.enterUnionDef) {
            listener.enterUnionDef(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitUnionDef) {
            listener.exitUnionDef(this);
        }
    }
}
exports.UnionDefContext = UnionDefContext;
class EnumDefContext extends ParserRuleContext_1.ParserRuleContext {
    TYPEDEF() { return this.getToken(CHeaderParser.TYPEDEF, 0); }
    ENUM() { return this.getToken(CHeaderParser.ENUM, 0); }
    id(i) {
        if (i === undefined) {
            return this.getRuleContexts(IdContext);
        }
        else {
            return this.getRuleContext(i, IdContext);
        }
    }
    enumBody() {
        return this.getRuleContext(0, EnumBodyContext);
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_enumDef; }
    // @Override
    enterRule(listener) {
        if (listener.enterEnumDef) {
            listener.enterEnumDef(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitEnumDef) {
            listener.exitEnumDef(this);
        }
    }
}
exports.EnumDefContext = EnumDefContext;
class StructDefContext extends ParserRuleContext_1.ParserRuleContext {
    TYPEDEF() { return this.getToken(CHeaderParser.TYPEDEF, 0); }
    STRUCT() { return this.getToken(CHeaderParser.STRUCT, 0); }
    id(i) {
        if (i === undefined) {
            return this.getRuleContexts(IdContext);
        }
        else {
            return this.getRuleContext(i, IdContext);
        }
    }
    structBody() {
        return this.getRuleContext(0, StructBodyContext);
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_structDef; }
    // @Override
    enterRule(listener) {
        if (listener.enterStructDef) {
            listener.enterStructDef(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitStructDef) {
            listener.exitStructDef(this);
        }
    }
}
exports.StructDefContext = StructDefContext;
class CallbackDefContext extends ParserRuleContext_1.ParserRuleContext {
    TYPEDEF() { return this.getToken(CHeaderParser.TYPEDEF, 0); }
    type() {
        return this.getRuleContext(0, TypeContext);
    }
    ROUND_B() { return this.getToken(CHeaderParser.ROUND_B, 0); }
    STAR() { return this.getToken(CHeaderParser.STAR, 0); }
    id() {
        return this.getRuleContext(0, IdContext);
    }
    ROUND_E() { return this.getToken(CHeaderParser.ROUND_E, 0); }
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
    get ruleIndex() { return CHeaderParser.RULE_callbackDef; }
    // @Override
    enterRule(listener) {
        if (listener.enterCallbackDef) {
            listener.enterCallbackDef(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitCallbackDef) {
            listener.exitCallbackDef(this);
        }
    }
}
exports.CallbackDefContext = CallbackDefContext;
class CompileTimeAssertContext extends ParserRuleContext_1.ParserRuleContext {
    SDL_COMPILE_TIME_ASSERT() { return this.getToken(CHeaderParser.SDL_COMPILE_TIME_ASSERT, 0); }
    group() {
        return this.getRuleContext(0, GroupContext);
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_compileTimeAssert; }
    // @Override
    enterRule(listener) {
        if (listener.enterCompileTimeAssert) {
            listener.enterCompileTimeAssert(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitCompileTimeAssert) {
            listener.exitCompileTimeAssert(this);
        }
    }
}
exports.CompileTimeAssertContext = CompileTimeAssertContext;
class InlineContext extends ParserRuleContext_1.ParserRuleContext {
    SDL_INLINE() { return this.tryGetToken(CHeaderParser.SDL_INLINE, 0); }
    STATIC() { return this.tryGetToken(CHeaderParser.STATIC, 0); }
    INLINE() { return this.tryGetToken(CHeaderParser.INLINE, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_inline; }
    // @Override
    enterRule(listener) {
        if (listener.enterInline) {
            listener.enterInline(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitInline) {
            listener.exitInline(this);
        }
    }
}
exports.InlineContext = InlineContext;
class BlockContext extends ParserRuleContext_1.ParserRuleContext {
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
    get ruleIndex() { return CHeaderParser.RULE_block; }
    // @Override
    enterRule(listener) {
        if (listener.enterBlock) {
            listener.enterBlock(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitBlock) {
            listener.exitBlock(this);
        }
    }
}
exports.BlockContext = BlockContext;
class GroupContext extends ParserRuleContext_1.ParserRuleContext {
    ROUND_B() { return this.getToken(CHeaderParser.ROUND_B, 0); }
    ROUND_E() { return this.getToken(CHeaderParser.ROUND_E, 0); }
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
    get ruleIndex() { return CHeaderParser.RULE_group; }
    // @Override
    enterRule(listener) {
        if (listener.enterGroup) {
            listener.enterGroup(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitGroup) {
            listener.exitGroup(this);
        }
    }
}
exports.GroupContext = GroupContext;
class IndexingContext extends ParserRuleContext_1.ParserRuleContext {
    SQUARE_B() { return this.getToken(CHeaderParser.SQUARE_B, 0); }
    SQUARE_E() { return this.getToken(CHeaderParser.SQUARE_E, 0); }
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
    get ruleIndex() { return CHeaderParser.RULE_indexing; }
    // @Override
    enterRule(listener) {
        if (listener.enterIndexing) {
            listener.enterIndexing(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitIndexing) {
            listener.exitIndexing(this);
        }
    }
}
exports.IndexingContext = IndexingContext;
class StmContext extends ParserRuleContext_1.ParserRuleContext {
    block() {
        return this.tryGetRuleContext(0, BlockContext);
    }
    indexing() {
        return this.tryGetRuleContext(0, IndexingContext);
    }
    expr() {
        return this.tryGetRuleContext(0, ExprContext);
    }
    punct() {
        return this.tryGetRuleContext(0, PunctContext);
    }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    trailingDoc() {
        return this.tryGetRuleContext(0, TrailingDocContext);
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
class ExprContext extends ParserRuleContext_1.ParserRuleContext {
    group() {
        return this.tryGetRuleContext(0, GroupContext);
    }
    word() {
        return this.tryGetRuleContext(0, WordContext);
    }
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
class WordContext extends ParserRuleContext_1.ParserRuleContext {
    ID() { return this.tryGetToken(CHeaderParser.ID, 0); }
    CONST() { return this.tryGetToken(CHeaderParser.CONST, 0); }
    VOID() { return this.tryGetToken(CHeaderParser.VOID, 0); }
    STATIC() { return this.tryGetToken(CHeaderParser.STATIC, 0); }
    ENUM() { return this.tryGetToken(CHeaderParser.ENUM, 0); }
    STRUCT() { return this.tryGetToken(CHeaderParser.STRUCT, 0); }
    UNION() { return this.tryGetToken(CHeaderParser.UNION, 0); }
    NUMBER() { return this.tryGetToken(CHeaderParser.NUMBER, 0); }
    STRING() { return this.tryGetToken(CHeaderParser.STRING, 0); }
    DEFINE() { return this.tryGetToken(CHeaderParser.DEFINE, 0); }
    SDL_COMPILE_TIME_ASSERT() { return this.tryGetToken(CHeaderParser.SDL_COMPILE_TIME_ASSERT, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_word; }
    // @Override
    enterRule(listener) {
        if (listener.enterWord) {
            listener.enterWord(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitWord) {
            listener.exitWord(this);
        }
    }
}
exports.WordContext = WordContext;
class PunctContext extends ParserRuleContext_1.ParserRuleContext {
    COLON() { return this.tryGetToken(CHeaderParser.COLON, 0); }
    SEMI() { return this.tryGetToken(CHeaderParser.SEMI, 0); }
    COMMA() { return this.tryGetToken(CHeaderParser.COMMA, 0); }
    DOT() { return this.tryGetToken(CHeaderParser.DOT, 0); }
    STAR() { return this.tryGetToken(CHeaderParser.STAR, 0); }
    EQ() { return this.tryGetToken(CHeaderParser.EQ, 0); }
    ELLIPSIS() { return this.tryGetToken(CHeaderParser.ELLIPSIS, 0); }
    PUNCT_EXTRA() { return this.tryGetToken(CHeaderParser.PUNCT_EXTRA, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_punct; }
    // @Override
    enterRule(listener) {
        if (listener.enterPunct) {
            listener.enterPunct(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitPunct) {
            listener.exitPunct(this);
        }
    }
}
exports.PunctContext = PunctContext;
class EnumBodyContext extends ParserRuleContext_1.ParserRuleContext {
    CURLY_B() { return this.getToken(CHeaderParser.CURLY_B, 0); }
    CURLY_E() { return this.getToken(CHeaderParser.CURLY_E, 0); }
    enumItem(i) {
        if (i === undefined) {
            return this.getRuleContexts(EnumItemContext);
        }
        else {
            return this.getRuleContext(i, EnumItemContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_enumBody; }
    // @Override
    enterRule(listener) {
        if (listener.enterEnumBody) {
            listener.enterEnumBody(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitEnumBody) {
            listener.exitEnumBody(this);
        }
    }
}
exports.EnumBodyContext = EnumBodyContext;
class EnumItemContext extends ParserRuleContext_1.ParserRuleContext {
    id() {
        return this.getRuleContext(0, IdContext);
    }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    EQ() { return this.tryGetToken(CHeaderParser.EQ, 0); }
    expr() {
        return this.tryGetRuleContext(0, ExprContext);
    }
    COMMA() { return this.tryGetToken(CHeaderParser.COMMA, 0); }
    trailingDoc() {
        return this.tryGetRuleContext(0, TrailingDocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_enumItem; }
    // @Override
    enterRule(listener) {
        if (listener.enterEnumItem) {
            listener.enterEnumItem(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitEnumItem) {
            listener.exitEnumItem(this);
        }
    }
}
exports.EnumItemContext = EnumItemContext;
class StructBodyContext extends ParserRuleContext_1.ParserRuleContext {
    CURLY_B() { return this.getToken(CHeaderParser.CURLY_B, 0); }
    CURLY_E() { return this.getToken(CHeaderParser.CURLY_E, 0); }
    structItem(i) {
        if (i === undefined) {
            return this.getRuleContexts(StructItemContext);
        }
        else {
            return this.getRuleContext(i, StructItemContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_structBody; }
    // @Override
    enterRule(listener) {
        if (listener.enterStructBody) {
            listener.enterStructBody(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitStructBody) {
            listener.exitStructBody(this);
        }
    }
}
exports.StructBodyContext = StructBodyContext;
class StructItemContext extends ParserRuleContext_1.ParserRuleContext {
    type() {
        return this.getRuleContext(0, TypeContext);
    }
    id(i) {
        if (i === undefined) {
            return this.getRuleContexts(IdContext);
        }
        else {
            return this.getRuleContext(i, IdContext);
        }
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    STRUCT() { return this.tryGetToken(CHeaderParser.STRUCT, 0); }
    COMMA(i) {
        if (i === undefined) {
            return this.getTokens(CHeaderParser.COMMA);
        }
        else {
            return this.getToken(CHeaderParser.COMMA, i);
        }
    }
    indexing(i) {
        if (i === undefined) {
            return this.getRuleContexts(IndexingContext);
        }
        else {
            return this.getRuleContext(i, IndexingContext);
        }
    }
    trailingDoc() {
        return this.tryGetRuleContext(0, TrailingDocContext);
    }
    CONST() { return this.tryGetToken(CHeaderParser.CONST, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_structItem; }
    // @Override
    enterRule(listener) {
        if (listener.enterStructItem) {
            listener.enterStructItem(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitStructItem) {
            listener.exitStructItem(this);
        }
    }
}
exports.StructItemContext = StructItemContext;
class IdContext extends ParserRuleContext_1.ParserRuleContext {
    ID() { return this.getToken(CHeaderParser.ID, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_id; }
    // @Override
    enterRule(listener) {
        if (listener.enterId) {
            listener.enterId(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitId) {
            listener.exitId(this);
        }
    }
}
exports.IdContext = IdContext;
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
    CONST() { return this.tryGetToken(CHeaderParser.CONST, 0); }
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
    ROUND_E() { return this.getToken(CHeaderParser.ROUND_E, 0); }
    type(i) {
        if (i === undefined) {
            return this.getRuleContexts(TypeContext);
        }
        else {
            return this.getRuleContext(i, TypeContext);
        }
    }
    COMMA(i) {
        if (i === undefined) {
            return this.getTokens(CHeaderParser.COMMA);
        }
        else {
            return this.getToken(CHeaderParser.COMMA, i);
        }
    }
    ELLIPSIS() { return this.tryGetToken(CHeaderParser.ELLIPSIS, 0); }
    attribute() {
        return this.tryGetRuleContext(0, AttributeContext);
    }
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
class AttributeContext extends ParserRuleContext_1.ParserRuleContext {
    group() {
        return this.getRuleContext(0, GroupContext);
    }
    ATTRIBUTE() { return this.tryGetToken(CHeaderParser.ATTRIBUTE, 0); }
    SDL_VARARG_ATTRIB() { return this.tryGetToken(CHeaderParser.SDL_VARARG_ATTRIB, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_attribute; }
    // @Override
    enterRule(listener) {
        if (listener.enterAttribute) {
            listener.enterAttribute(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitAttribute) {
            listener.exitAttribute(this);
        }
    }
}
exports.AttributeContext = AttributeContext;
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
class TrailingDocContext extends ParserRuleContext_1.ParserRuleContext {
    TRAILING_DOC() { return this.getToken(CHeaderParser.TRAILING_DOC, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_trailingDoc; }
    // @Override
    enterRule(listener) {
        if (listener.enterTrailingDoc) {
            listener.enterTrailingDoc(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitTrailingDoc) {
            listener.exitTrailingDoc(this);
        }
    }
}
exports.TrailingDocContext = TrailingDocContext;
