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
exports.TrailingDocContext = exports.DocContext = exports.AttributeContext = exports.SignatureContext = exports.TypeElContext = exports.TypeContext = exports.IdContext = exports.UnionInlineTypeContext = exports.StructCallbackContext = exports.StructVarContext = exports.StructItemContext = exports.StructBodyContext = exports.EnumItemContext = exports.EnumBodyContext = exports.PunctContext = exports.WordContext = exports.ExprContext = exports.StmContext = exports.IndexingContext = exports.GroupContext = exports.BlockContext = exports.InlineContext = exports.CompileTimeAssertContext = exports.CallbackDefContext = exports.StructDefContext = exports.EnumDefContext = exports.UnionDefContext = exports.AliasDefContext = exports.FunctionDefContext = exports.FunctionDeclContext = exports.GlobalVarContext = exports.ExternCContext = exports.DirectiveContext = exports.DeclContext = exports.ProgContext = exports.CHeaderParser = void 0;
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
                this.state = 71;
                this._errHandler.sync(this);
                switch (this.interpreter.adaptivePredict(this._input, 0, this._ctx)) {
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
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0) || _la === CHeaderParser.ID) {
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
            this.state = 93;
            this._errHandler.sync(this);
            switch (this.interpreter.adaptivePredict(this._input, 2, this._ctx)) {
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
        this.enterRule(_localctx, 4, CHeaderParser.RULE_directive);
        let _la;
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
        this.enterRule(_localctx, 6, CHeaderParser.RULE_externC);
        let _la;
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
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.TYPEDEF) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0) || _la === CHeaderParser.ID) {
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
    globalVar() {
        let _localctx = new GlobalVarContext(this._ctx, this.state);
        this.enterRule(_localctx, 8, CHeaderParser.RULE_globalVar);
        let _la;
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
        this.enterRule(_localctx, 10, CHeaderParser.RULE_functionDecl);
        let _la;
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
                this.state = 143;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0)) {
                    {
                        this.state = 142;
                        this.attribute();
                    }
                }
                this.state = 145;
                this.type();
                this.state = 147;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0)) {
                    {
                        this.state = 146;
                        this.attribute();
                    }
                }
                this.state = 149;
                this.id();
                this.state = 150;
                this.signature();
                this.state = 151;
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
        this.enterRule(_localctx, 12, CHeaderParser.RULE_functionDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 154;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 153;
                        this.doc();
                    }
                }
                this.state = 156;
                this.inline();
                this.state = 157;
                this.type();
                this.state = 159;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0)) {
                    {
                        this.state = 158;
                        this.attribute();
                    }
                }
                this.state = 161;
                this.id();
                this.state = 162;
                this.signature();
                this.state = 163;
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
        this.enterRule(_localctx, 14, CHeaderParser.RULE_aliasDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 166;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 165;
                        this.doc();
                    }
                }
                this.state = 168;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 170;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.STRUCT || _la === CHeaderParser.UNION) {
                    {
                        this.state = 169;
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
                this.state = 172;
                this.type();
                this.state = 173;
                this.id();
                this.state = 174;
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
        this.enterRule(_localctx, 16, CHeaderParser.RULE_unionDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 177;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 176;
                        this.doc();
                    }
                }
                this.state = 179;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 180;
                this.match(CHeaderParser.UNION);
                this.state = 181;
                this.id();
                this.state = 182;
                this.block();
                this.state = 183;
                _localctx._name = this.id();
                this.state = 184;
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
        this.enterRule(_localctx, 18, CHeaderParser.RULE_enumDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 187;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 186;
                        this.doc();
                    }
                }
                this.state = 189;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 190;
                this.match(CHeaderParser.ENUM);
                this.state = 191;
                this.id();
                this.state = 192;
                this.enumBody();
                this.state = 193;
                _localctx._name = this.id();
                this.state = 194;
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
        this.enterRule(_localctx, 20, CHeaderParser.RULE_structDef);
        let _la;
        try {
            this.state = 214;
            this._errHandler.sync(this);
            switch (this.interpreter.adaptivePredict(this._input, 22, this._ctx)) {
                case 1:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 197;
                        this._errHandler.sync(this);
                        _la = this._input.LA(1);
                        if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                            {
                                this.state = 196;
                                this.doc();
                            }
                        }
                        this.state = 199;
                        this.match(CHeaderParser.STRUCT);
                        this.state = 200;
                        _localctx._name = this.id();
                        this.state = 201;
                        this.structBody();
                        this.state = 202;
                        this.match(CHeaderParser.SEMI);
                    }
                    break;
                case 2:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 205;
                        this._errHandler.sync(this);
                        _la = this._input.LA(1);
                        if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                            {
                                this.state = 204;
                                this.doc();
                            }
                        }
                        this.state = 207;
                        this.match(CHeaderParser.TYPEDEF);
                        this.state = 208;
                        this.match(CHeaderParser.STRUCT);
                        this.state = 209;
                        this.id();
                        this.state = 210;
                        this.structBody();
                        this.state = 211;
                        _localctx._name = this.id();
                        this.state = 212;
                        this.match(CHeaderParser.SEMI);
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
    callbackDef() {
        let _localctx = new CallbackDefContext(this._ctx, this.state);
        this.enterRule(_localctx, 22, CHeaderParser.RULE_callbackDef);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 217;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 216;
                        this.doc();
                    }
                }
                this.state = 219;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 220;
                this.type();
                this.state = 221;
                this.match(CHeaderParser.ROUND_B);
                this.state = 222;
                this.match(CHeaderParser.STAR);
                this.state = 223;
                this.id();
                this.state = 224;
                this.match(CHeaderParser.ROUND_E);
                this.state = 225;
                this.signature();
                this.state = 226;
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
        this.enterRule(_localctx, 24, CHeaderParser.RULE_compileTimeAssert);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 228;
                this.match(CHeaderParser.SDL_COMPILE_TIME_ASSERT);
                this.state = 229;
                this.group();
                this.state = 230;
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
        this.enterRule(_localctx, 26, CHeaderParser.RULE_inline);
        try {
            this.state = 235;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case CHeaderParser.SDL_INLINE:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 232;
                        this.match(CHeaderParser.SDL_INLINE);
                    }
                    break;
                case CHeaderParser.STATIC:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 233;
                        this.match(CHeaderParser.STATIC);
                        this.state = 234;
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
        this.enterRule(_localctx, 28, CHeaderParser.RULE_block);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 237;
                this.match(CHeaderParser.CURLY_B);
                this.state = 241;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B))) !== 0) || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & ((1 << (CHeaderParser.COLON - 33)) | (1 << (CHeaderParser.SEMI - 33)) | (1 << (CHeaderParser.COMMA - 33)) | (1 << (CHeaderParser.DOT - 33)) | (1 << (CHeaderParser.STAR - 33)) | (1 << (CHeaderParser.EQ - 33)) | (1 << (CHeaderParser.ELLIPSIS - 33)) | (1 << (CHeaderParser.PUNCT_EXTRA - 33)) | (1 << (CHeaderParser.STRING - 33)) | (1 << (CHeaderParser.ID - 33)) | (1 << (CHeaderParser.NUMBER - 33)))) !== 0)) {
                    {
                        {
                            this.state = 238;
                            this.stm();
                        }
                    }
                    this.state = 243;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 244;
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
        this.enterRule(_localctx, 30, CHeaderParser.RULE_group);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 246;
                this.match(CHeaderParser.ROUND_B);
                this.state = 250;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B))) !== 0) || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & ((1 << (CHeaderParser.COLON - 33)) | (1 << (CHeaderParser.SEMI - 33)) | (1 << (CHeaderParser.COMMA - 33)) | (1 << (CHeaderParser.DOT - 33)) | (1 << (CHeaderParser.STAR - 33)) | (1 << (CHeaderParser.EQ - 33)) | (1 << (CHeaderParser.ELLIPSIS - 33)) | (1 << (CHeaderParser.PUNCT_EXTRA - 33)) | (1 << (CHeaderParser.STRING - 33)) | (1 << (CHeaderParser.ID - 33)) | (1 << (CHeaderParser.NUMBER - 33)))) !== 0)) {
                    {
                        {
                            this.state = 247;
                            this.stm();
                        }
                    }
                    this.state = 252;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 253;
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
        this.enterRule(_localctx, 32, CHeaderParser.RULE_indexing);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 255;
                this.match(CHeaderParser.SQUARE_B);
                this.state = 259;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.TRAILING_DOC) | (1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B))) !== 0) || ((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & ((1 << (CHeaderParser.COLON - 33)) | (1 << (CHeaderParser.SEMI - 33)) | (1 << (CHeaderParser.COMMA - 33)) | (1 << (CHeaderParser.DOT - 33)) | (1 << (CHeaderParser.STAR - 33)) | (1 << (CHeaderParser.EQ - 33)) | (1 << (CHeaderParser.ELLIPSIS - 33)) | (1 << (CHeaderParser.PUNCT_EXTRA - 33)) | (1 << (CHeaderParser.STRING - 33)) | (1 << (CHeaderParser.ID - 33)) | (1 << (CHeaderParser.NUMBER - 33)))) !== 0)) {
                    {
                        {
                            this.state = 256;
                            this.stm();
                        }
                    }
                    this.state = 261;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 262;
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
        this.enterRule(_localctx, 34, CHeaderParser.RULE_stm);
        try {
            this.state = 270;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case CHeaderParser.CURLY_B:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 264;
                        this.block();
                    }
                    break;
                case CHeaderParser.SQUARE_B:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 265;
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
                        this.state = 266;
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
                        this.state = 267;
                        this.punct();
                    }
                    break;
                case CHeaderParser.LONG_DOC:
                case CHeaderParser.SHORT_DOC:
                    this.enterOuterAlt(_localctx, 5);
                    {
                        this.state = 268;
                        this.doc();
                    }
                    break;
                case CHeaderParser.TRAILING_DOC:
                    this.enterOuterAlt(_localctx, 6);
                    {
                        this.state = 269;
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
        this.enterRule(_localctx, 36, CHeaderParser.RULE_expr);
        try {
            this.state = 274;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case CHeaderParser.ROUND_B:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 272;
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
                        this.state = 273;
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
        this.enterRule(_localctx, 38, CHeaderParser.RULE_word);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 276;
                _la = this._input.LA(1);
                if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DEFINE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.ENUM) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_COMPILE_TIME_ASSERT))) !== 0) || ((((_la - 41)) & ~0x1F) === 0 && ((1 << (_la - 41)) & ((1 << (CHeaderParser.STRING - 41)) | (1 << (CHeaderParser.ID - 41)) | (1 << (CHeaderParser.NUMBER - 41)))) !== 0))) {
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
        this.enterRule(_localctx, 40, CHeaderParser.RULE_punct);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 278;
                _la = this._input.LA(1);
                if (!(((((_la - 33)) & ~0x1F) === 0 && ((1 << (_la - 33)) & ((1 << (CHeaderParser.COLON - 33)) | (1 << (CHeaderParser.SEMI - 33)) | (1 << (CHeaderParser.COMMA - 33)) | (1 << (CHeaderParser.DOT - 33)) | (1 << (CHeaderParser.STAR - 33)) | (1 << (CHeaderParser.EQ - 33)) | (1 << (CHeaderParser.ELLIPSIS - 33)) | (1 << (CHeaderParser.PUNCT_EXTRA - 33)))) !== 0))) {
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
        this.enterRule(_localctx, 42, CHeaderParser.RULE_enumBody);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 280;
                this.match(CHeaderParser.CURLY_B);
                this.state = 284;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC || _la === CHeaderParser.ID) {
                    {
                        {
                            this.state = 281;
                            this.enumItem();
                        }
                    }
                    this.state = 286;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 287;
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
        this.enterRule(_localctx, 44, CHeaderParser.RULE_enumItem);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 290;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 289;
                        this.doc();
                    }
                }
                this.state = 292;
                this.id();
                this.state = 295;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.EQ) {
                    {
                        this.state = 293;
                        this.match(CHeaderParser.EQ);
                        this.state = 294;
                        this.expr();
                    }
                }
                this.state = 298;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.COMMA) {
                    {
                        this.state = 297;
                        this.match(CHeaderParser.COMMA);
                    }
                }
                this.state = 301;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.TRAILING_DOC) {
                    {
                        this.state = 300;
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
        this.enterRule(_localctx, 46, CHeaderParser.RULE_structBody);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 303;
                this.match(CHeaderParser.CURLY_B);
                this.state = 308;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID))) !== 0) || _la === CHeaderParser.ID) {
                    {
                        this.state = 306;
                        this._errHandler.sync(this);
                        switch (this.interpreter.adaptivePredict(this._input, 35, this._ctx)) {
                            case 1:
                                {
                                    this.state = 304;
                                    this.structItem();
                                }
                                break;
                            case 2:
                                {
                                    this.state = 305;
                                    this.unionInlineType();
                                }
                                break;
                        }
                    }
                    this.state = 310;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 311;
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
        this.enterRule(_localctx, 48, CHeaderParser.RULE_structItem);
        try {
            this.state = 315;
            this._errHandler.sync(this);
            switch (this.interpreter.adaptivePredict(this._input, 37, this._ctx)) {
                case 1:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 313;
                        this.structVar();
                    }
                    break;
                case 2:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 314;
                        this.structCallback();
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
    structVar() {
        let _localctx = new StructVarContext(this._ctx, this.state);
        this.enterRule(_localctx, 50, CHeaderParser.RULE_structVar);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 318;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 317;
                        this.doc();
                    }
                }
                this.state = 324;
                this._errHandler.sync(this);
                switch (this.interpreter.adaptivePredict(this._input, 40, this._ctx)) {
                    case 1:
                        {
                            this.state = 321;
                            this._errHandler.sync(this);
                            _la = this._input.LA(1);
                            if (_la === CHeaderParser.CONST) {
                                {
                                    this.state = 320;
                                    this.match(CHeaderParser.CONST);
                                }
                            }
                            this.state = 323;
                            this.match(CHeaderParser.STRUCT);
                        }
                        break;
                }
                this.state = 326;
                this.type();
                this.state = 327;
                this.id();
                this.state = 332;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.COMMA) {
                    {
                        {
                            this.state = 328;
                            this.match(CHeaderParser.COMMA);
                            this.state = 329;
                            this.id();
                        }
                    }
                    this.state = 334;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 338;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.SQUARE_B) {
                    {
                        {
                            this.state = 335;
                            this.indexing();
                        }
                    }
                    this.state = 340;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 341;
                this.match(CHeaderParser.SEMI);
                this.state = 343;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.TRAILING_DOC) {
                    {
                        this.state = 342;
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
    structCallback() {
        let _localctx = new StructCallbackContext(this._ctx, this.state);
        this.enterRule(_localctx, 52, CHeaderParser.RULE_structCallback);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 346;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 345;
                        this.doc();
                    }
                }
                this.state = 348;
                this.type();
                this.state = 349;
                this.match(CHeaderParser.ROUND_B);
                this.state = 350;
                this.match(CHeaderParser.STAR);
                this.state = 351;
                this.id();
                this.state = 352;
                this.match(CHeaderParser.ROUND_E);
                this.state = 353;
                this.signature();
                this.state = 354;
                this.match(CHeaderParser.SEMI);
                this.state = 356;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.TRAILING_DOC) {
                    {
                        this.state = 355;
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
    unionInlineType() {
        let _localctx = new UnionInlineTypeContext(this._ctx, this.state);
        this.enterRule(_localctx, 54, CHeaderParser.RULE_unionInlineType);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 359;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 358;
                        this.doc();
                    }
                }
                this.state = 361;
                this.match(CHeaderParser.UNION);
                this.state = 362;
                this.block();
                this.state = 363;
                this.id();
                this.state = 364;
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
    id() {
        let _localctx = new IdContext(this._ctx, this.state);
        this.enterRule(_localctx, 56, CHeaderParser.RULE_id);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 366;
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
        this.enterRule(_localctx, 58, CHeaderParser.RULE_type);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 369;
                this._errHandler.sync(this);
                _alt = 1;
                do {
                    switch (_alt) {
                        case 1:
                            {
                                {
                                    this.state = 368;
                                    this.typeEl();
                                }
                            }
                            break;
                        default:
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 371;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 47, this._ctx);
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
        this.enterRule(_localctx, 60, CHeaderParser.RULE_typeEl);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 373;
                _la = this._input.LA(1);
                if (!(_la === CHeaderParser.CONST || _la === CHeaderParser.VOID || _la === CHeaderParser.ID)) {
                    this._errHandler.recoverInline(this);
                }
                else {
                    if (this._input.LA(1) === Token_1.Token.EOF) {
                        this.matchedEOF = true;
                    }
                    this._errHandler.reportMatch(this);
                    this.consume();
                }
                this.state = 378;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.SQUARE_B || _la === CHeaderParser.STAR) {
                    {
                        this.state = 376;
                        this._errHandler.sync(this);
                        switch (this._input.LA(1)) {
                            case CHeaderParser.STAR:
                                {
                                    this.state = 374;
                                    this.match(CHeaderParser.STAR);
                                }
                                break;
                            case CHeaderParser.SQUARE_B:
                                {
                                    this.state = 375;
                                    this.indexing();
                                }
                                break;
                            default:
                                throw new NoViableAltException_1.NoViableAltException(this);
                        }
                    }
                    this.state = 380;
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
        this.enterRule(_localctx, 62, CHeaderParser.RULE_signature);
        let _la;
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 381;
                this.match(CHeaderParser.ROUND_B);
                this.state = 396;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0) || _la === CHeaderParser.ID) {
                    {
                        this.state = 383;
                        this._errHandler.sync(this);
                        _la = this._input.LA(1);
                        if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0)) {
                            {
                                this.state = 382;
                                this.attribute();
                            }
                        }
                        this.state = 385;
                        this.type();
                        this.state = 393;
                        this._errHandler.sync(this);
                        _alt = this.interpreter.adaptivePredict(this._input, 52, this._ctx);
                        while (_alt !== 2 && _alt !== ATN_1.ATN.INVALID_ALT_NUMBER) {
                            if (_alt === 1) {
                                {
                                    {
                                        this.state = 386;
                                        this.match(CHeaderParser.COMMA);
                                        this.state = 388;
                                        this._errHandler.sync(this);
                                        _la = this._input.LA(1);
                                        if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0)) {
                                            {
                                                this.state = 387;
                                                this.attribute();
                                            }
                                        }
                                        this.state = 390;
                                        this.type();
                                    }
                                }
                            }
                            this.state = 395;
                            this._errHandler.sync(this);
                            _alt = this.interpreter.adaptivePredict(this._input, 52, this._ctx);
                        }
                    }
                }
                this.state = 400;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.COMMA) {
                    {
                        this.state = 398;
                        this.match(CHeaderParser.COMMA);
                        this.state = 399;
                        this.match(CHeaderParser.ELLIPSIS);
                    }
                }
                this.state = 402;
                this.match(CHeaderParser.ROUND_E);
                this.state = 404;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0)) {
                    {
                        this.state = 403;
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
        this.enterRule(_localctx, 64, CHeaderParser.RULE_attribute);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 406;
                _la = this._input.LA(1);
                if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.ATTRIBUTE) | (1 << CHeaderParser.SDL_VARARG_ATTRIB) | (1 << CHeaderParser.SDL_ACQUIRE) | (1 << CHeaderParser.SDL_RELEASE) | (1 << CHeaderParser.SDL_ALLOC_SIZE) | (1 << CHeaderParser.SDL_INOUT))) !== 0))) {
                    this._errHandler.recoverInline(this);
                }
                else {
                    if (this._input.LA(1) === Token_1.Token.EOF) {
                        this.matchedEOF = true;
                    }
                    this._errHandler.reportMatch(this);
                    this.consume();
                }
                this.state = 407;
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
        this.enterRule(_localctx, 66, CHeaderParser.RULE_doc);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 409;
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
        this.enterRule(_localctx, 68, CHeaderParser.RULE_trailingDoc);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 411;
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
CHeaderParser.SDL_ACQUIRE = 22;
CHeaderParser.SDL_RELEASE = 23;
CHeaderParser.SDL_COMPILE_TIME_ASSERT = 24;
CHeaderParser.SDL_ALLOC_SIZE = 25;
CHeaderParser.SDL_INOUT = 26;
CHeaderParser.CURLY_B = 27;
CHeaderParser.CURLY_E = 28;
CHeaderParser.ROUND_B = 29;
CHeaderParser.ROUND_E = 30;
CHeaderParser.SQUARE_B = 31;
CHeaderParser.SQUARE_E = 32;
CHeaderParser.COLON = 33;
CHeaderParser.SEMI = 34;
CHeaderParser.COMMA = 35;
CHeaderParser.DOT = 36;
CHeaderParser.STAR = 37;
CHeaderParser.EQ = 38;
CHeaderParser.ELLIPSIS = 39;
CHeaderParser.PUNCT_EXTRA = 40;
CHeaderParser.STRING = 41;
CHeaderParser.ID = 42;
CHeaderParser.NUMBER = 43;
CHeaderParser.RULE_prog = 0;
CHeaderParser.RULE_decl = 1;
CHeaderParser.RULE_directive = 2;
CHeaderParser.RULE_externC = 3;
CHeaderParser.RULE_globalVar = 4;
CHeaderParser.RULE_functionDecl = 5;
CHeaderParser.RULE_functionDef = 6;
CHeaderParser.RULE_aliasDef = 7;
CHeaderParser.RULE_unionDef = 8;
CHeaderParser.RULE_enumDef = 9;
CHeaderParser.RULE_structDef = 10;
CHeaderParser.RULE_callbackDef = 11;
CHeaderParser.RULE_compileTimeAssert = 12;
CHeaderParser.RULE_inline = 13;
CHeaderParser.RULE_block = 14;
CHeaderParser.RULE_group = 15;
CHeaderParser.RULE_indexing = 16;
CHeaderParser.RULE_stm = 17;
CHeaderParser.RULE_expr = 18;
CHeaderParser.RULE_word = 19;
CHeaderParser.RULE_punct = 20;
CHeaderParser.RULE_enumBody = 21;
CHeaderParser.RULE_enumItem = 22;
CHeaderParser.RULE_structBody = 23;
CHeaderParser.RULE_structItem = 24;
CHeaderParser.RULE_structVar = 25;
CHeaderParser.RULE_structCallback = 26;
CHeaderParser.RULE_unionInlineType = 27;
CHeaderParser.RULE_id = 28;
CHeaderParser.RULE_type = 29;
CHeaderParser.RULE_typeEl = 30;
CHeaderParser.RULE_signature = 31;
CHeaderParser.RULE_attribute = 32;
CHeaderParser.RULE_doc = 33;
CHeaderParser.RULE_trailingDoc = 34;
// tslint:disable:no-trailing-whitespace
CHeaderParser.ruleNames = [
    "prog", "decl", "directive", "externC", "globalVar", "functionDecl", "functionDef",
    "aliasDef", "unionDef", "enumDef", "structDef", "callbackDef", "compileTimeAssert",
    "inline", "block", "group", "indexing", "stm", "expr", "word", "punct",
    "enumBody", "enumItem", "structBody", "structItem", "structVar", "structCallback",
    "unionInlineType", "id", "type", "typeEl", "signature", "attribute", "doc",
    "trailingDoc",
];
CHeaderParser._LITERAL_NAMES = [
    undefined, undefined, undefined, undefined, undefined, undefined, undefined,
    undefined, undefined, "'__attribute__'", "'const'", "'enum'", "'extern'",
    "'__inline__'", undefined, "'SDL_FORCE_INLINE'", "'static'", "'struct'",
    "'typedef'", "'union'", "'void'", undefined, undefined, undefined, "'SDL_COMPILE_TIME_ASSERT'",
    undefined, undefined, "'{'", "'}'", "'('", "')'", "'['", "']'", "':'",
    "';'", "','", "'.'", "'*'", "'='", "'...'",
];
CHeaderParser._SYMBOLIC_NAMES = [
    undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "TRAILING_DOC", "LONG_DOC",
    "SHORT_DOC", "DEFINE", "DIRECTIVE", "ATTRIBUTE", "CONST", "ENUM", "EXTERN",
    "INLINE", "SDL_NOISE", "SDL_INLINE", "STATIC", "STRUCT", "TYPEDEF", "UNION",
    "VOID", "SDL_VARARG_ATTRIB", "SDL_ACQUIRE", "SDL_RELEASE", "SDL_COMPILE_TIME_ASSERT",
    "SDL_ALLOC_SIZE", "SDL_INOUT", "CURLY_B", "CURLY_E", "ROUND_B", "ROUND_E",
    "SQUARE_B", "SQUARE_E", "COLON", "SEMI", "COMMA", "DOT", "STAR", "EQ",
    "ELLIPSIS", "PUNCT_EXTRA", "STRING", "ID", "NUMBER",
];
CHeaderParser.VOCABULARY = new VocabularyImpl_1.VocabularyImpl(CHeaderParser._LITERAL_NAMES, CHeaderParser._SYMBOLIC_NAMES, []);
CHeaderParser._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03-\u01A0\x04\x02" +
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
    "\x8F\n\x07\x03\x07\x05\x07\x92\n\x07\x03\x07\x03\x07\x05\x07\x96\n\x07" +
    "\x03\x07\x03\x07\x03\x07\x03\x07\x03\b\x05\b\x9D\n\b\x03\b\x03\b\x03\b" +
    "\x05\b\xA2\n\b\x03\b\x03\b\x03\b\x03\b\x03\t\x05\t\xA9\n\t\x03\t\x03\t" +
    "\x05\t\xAD\n\t\x03\t\x03\t\x03\t\x03\t\x03\n\x05\n\xB4\n\n\x03\n\x03\n" +
    "\x03\n\x03\n\x03\n\x03\n\x03\n\x03\v\x05\v\xBE\n\v\x03\v\x03\v\x03\v\x03" +
    "\v\x03\v\x03\v\x03\v\x03\f\x05\f\xC8\n\f\x03\f\x03\f\x03\f\x03\f\x03\f" +
    "\x03\f\x05\f\xD0\n\f\x03\f\x03\f\x03\f\x03\f\x03\f\x03\f\x03\f\x05\f\xD9" +
    "\n\f\x03\r\x05\r\xDC\n\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03" +
    "\r\x03\r\x03\x0E\x03\x0E\x03\x0E\x03\x0E\x03\x0F\x03\x0F\x03\x0F\x05\x0F" +
    "\xEE\n\x0F\x03\x10\x03\x10\x07\x10\xF2\n\x10\f\x10\x0E\x10\xF5\v\x10\x03" +
    "\x10\x03\x10\x03\x11\x03\x11\x07\x11\xFB\n\x11\f\x11\x0E\x11\xFE\v\x11" +
    "\x03\x11\x03\x11\x03\x12\x03\x12\x07\x12\u0104\n\x12\f\x12\x0E\x12\u0107" +
    "\v\x12\x03\x12\x03\x12\x03\x13\x03\x13\x03\x13\x03\x13\x03\x13\x03\x13" +
    "\x05\x13\u0111\n\x13\x03\x14\x03\x14\x05\x14\u0115\n\x14\x03\x15\x03\x15" +
    "\x03\x16\x03\x16\x03\x17\x03\x17\x07\x17\u011D\n\x17\f\x17\x0E\x17\u0120" +
    "\v\x17\x03\x17\x03\x17\x03\x18\x05\x18\u0125\n\x18\x03\x18\x03\x18\x03" +
    "\x18\x05\x18\u012A\n\x18\x03\x18\x05\x18\u012D\n\x18\x03\x18\x05\x18\u0130" +
    "\n\x18\x03\x19\x03\x19\x03\x19\x07\x19\u0135\n\x19\f\x19\x0E\x19\u0138" +
    "\v\x19\x03\x19\x03\x19\x03\x1A\x03\x1A\x05\x1A\u013E\n\x1A\x03\x1B\x05" +
    "\x1B\u0141\n\x1B\x03\x1B\x05\x1B\u0144\n\x1B\x03\x1B\x05\x1B\u0147\n\x1B" +
    "\x03\x1B\x03\x1B\x03\x1B\x03\x1B\x07\x1B\u014D\n\x1B\f\x1B\x0E\x1B\u0150" +
    "\v\x1B\x03\x1B\x07\x1B\u0153\n\x1B\f\x1B\x0E\x1B\u0156\v\x1B\x03\x1B\x03" +
    "\x1B\x05\x1B\u015A\n\x1B\x03\x1C\x05\x1C\u015D\n\x1C\x03\x1C\x03\x1C\x03" +
    "\x1C\x03\x1C\x03\x1C\x03\x1C\x03\x1C\x03\x1C\x05\x1C\u0167\n\x1C\x03\x1D" +
    "\x05\x1D\u016A\n\x1D\x03\x1D\x03\x1D\x03\x1D\x03\x1D\x03\x1D\x03\x1E\x03" +
    "\x1E\x03\x1F\x06\x1F\u0174\n\x1F\r\x1F\x0E\x1F\u0175\x03 \x03 \x03 \x07" +
    " \u017B\n \f \x0E \u017E\v \x03!\x03!\x05!\u0182\n!\x03!\x03!\x03!\x05" +
    "!\u0187\n!\x03!\x07!\u018A\n!\f!\x0E!\u018D\v!\x05!\u018F\n!\x03!\x03" +
    "!\x05!\u0193\n!\x03!\x03!\x05!\u0197\n!\x03\"\x03\"\x03\"\x03#\x03#\x03" +
    "$\x03$\x03$\x02\x02\x02%\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E" +
    "\x02\x10\x02\x12\x02\x14\x02\x16\x02\x18\x02\x1A\x02\x1C\x02\x1E\x02 " +
    "\x02\"\x02$\x02&\x02(\x02*\x02,\x02.\x020\x022\x024\x026\x028\x02:\x02" +
    "<\x02>\x02@\x02B\x02D\x02F\x02\x02\b\x04\x02\x13\x13\x15\x15\b\x02\t\t" +
    "\f\r\x12\x13\x15\x16\x1A\x1A+-\x03\x02#*\x05\x02\f\f\x16\x16,,\x05\x02" +
    "\v\v\x17\x19\x1B\x1C\x03\x02\x07\b\x02\u01C2\x02I\x03\x02\x02\x02\x04" +
    "_\x03\x02\x02\x02\x06b\x03\x02\x02\x02\bf\x03\x02\x02\x02\nr\x03\x02\x02" +
    "\x02\f\x8B\x03\x02\x02\x02\x0E\x9C\x03\x02\x02\x02\x10\xA8\x03\x02\x02" +
    "\x02\x12\xB3\x03\x02\x02\x02\x14\xBD\x03\x02\x02\x02\x16\xD8\x03\x02\x02" +
    "\x02\x18\xDB\x03\x02\x02\x02\x1A\xE6\x03\x02\x02\x02\x1C\xED\x03\x02\x02" +
    "\x02\x1E\xEF\x03\x02\x02\x02 \xF8\x03\x02\x02\x02\"\u0101\x03\x02\x02" +
    "\x02$\u0110\x03\x02\x02\x02&\u0114\x03\x02\x02\x02(\u0116\x03\x02\x02" +
    "\x02*\u0118\x03\x02\x02\x02,\u011A\x03\x02\x02\x02.\u0124\x03\x02\x02" +
    "\x020\u0131\x03\x02\x02\x022\u013D\x03\x02\x02\x024\u0140\x03\x02\x02" +
    "\x026\u015C\x03\x02\x02\x028\u0169\x03\x02\x02\x02:\u0170\x03\x02\x02" +
    "\x02<\u0173\x03\x02\x02\x02>\u0177\x03\x02\x02\x02@\u017F\x03\x02\x02" +
    "\x02B\u0198\x03\x02\x02\x02D\u019B\x03\x02\x02\x02F\u019D\x03\x02\x02" +
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
    "\x0E\x02\x02gh\x07+\x02\x02hl\x07\x1D\x02\x02ik\x05\x04\x03\x02ji\x03" +
    "\x02\x02\x02kn\x03\x02\x02\x02lj\x03\x02\x02\x02lm\x03\x02\x02\x02mo\x03" +
    "\x02\x02\x02nl\x03\x02\x02\x02op\x07\x1E\x02\x02p\t\x03\x02\x02\x02qs" +
    "\x05D#\x02rq\x03\x02\x02\x02rs\x03\x02\x02\x02su\x03\x02\x02\x02tv\x07" +
    "\x0E\x02\x02ut\x03\x02\x02\x02uv\x03\x02\x02\x02vw\x03\x02\x02\x02wx\x05" +
    "<\x1F\x02x}\x05:\x1E\x02yz\x07%\x02\x02z|\x05:\x1E\x02{y\x03\x02\x02\x02" +
    "|\x7F\x03\x02\x02\x02}{\x03\x02\x02\x02}~\x03\x02\x02\x02~\x83\x03\x02" +
    "\x02\x02\x7F}\x03\x02\x02\x02\x80\x82\x05\"\x12\x02\x81\x80\x03\x02\x02" +
    "\x02\x82\x85\x03\x02\x02\x02\x83\x81\x03\x02\x02\x02\x83\x84\x03\x02\x02" +
    "\x02\x84\x86\x03\x02\x02\x02\x85\x83\x03\x02\x02\x02\x86\x88\x07$\x02" +
    "\x02\x87\x89\x05F$\x02\x88\x87\x03\x02\x02\x02\x88\x89\x03\x02\x02\x02" +
    "\x89\v\x03\x02\x02\x02\x8A\x8C\x05D#\x02\x8B\x8A\x03\x02\x02\x02\x8B\x8C" +
    "\x03\x02\x02\x02\x8C\x8E\x03\x02\x02\x02\x8D\x8F\x07\x0E\x02\x02\x8E\x8D" +
    "\x03\x02\x02\x02\x8E\x8F\x03\x02\x02\x02\x8F\x91\x03\x02\x02\x02\x90\x92" +
    "\x05B\"\x02\x91\x90\x03\x02\x02\x02\x91\x92\x03\x02\x02\x02\x92\x93\x03" +
    "\x02\x02\x02\x93\x95\x05<\x1F\x02\x94\x96\x05B\"\x02\x95\x94\x03\x02\x02" +
    "\x02\x95\x96\x03\x02\x02\x02\x96\x97\x03\x02\x02\x02\x97\x98\x05:\x1E" +
    "\x02\x98\x99\x05@!\x02\x99\x9A\x07$\x02\x02\x9A\r\x03\x02\x02\x02\x9B" +
    "\x9D\x05D#\x02\x9C\x9B\x03\x02\x02\x02\x9C\x9D\x03\x02\x02\x02\x9D\x9E" +
    "\x03\x02\x02\x02\x9E\x9F\x05\x1C\x0F\x02\x9F\xA1\x05<\x1F\x02\xA0\xA2" +
    "\x05B\"\x02\xA1\xA0\x03\x02\x02\x02\xA1\xA2\x03\x02\x02\x02\xA2\xA3\x03" +
    "\x02\x02\x02\xA3\xA4\x05:\x1E\x02\xA4\xA5\x05@!\x02\xA5\xA6\x05\x1E\x10" +
    "\x02\xA6\x0F\x03\x02\x02\x02\xA7\xA9\x05D#\x02\xA8\xA7\x03\x02\x02\x02" +
    "\xA8\xA9\x03\x02\x02\x02\xA9\xAA\x03\x02\x02\x02\xAA\xAC\x07\x14\x02\x02" +
    "\xAB\xAD\t\x02\x02\x02\xAC\xAB\x03\x02\x02\x02\xAC\xAD\x03\x02\x02\x02" +
    "\xAD\xAE\x03\x02\x02\x02\xAE\xAF\x05<\x1F\x02\xAF\xB0\x05:\x1E\x02\xB0" +
    "\xB1\x07$\x02\x02\xB1\x11\x03\x02\x02\x02\xB2\xB4\x05D#\x02\xB3\xB2\x03" +
    "\x02\x02\x02\xB3\xB4\x03\x02\x02\x02\xB4\xB5\x03\x02\x02\x02\xB5\xB6\x07" +
    "\x14\x02\x02\xB6\xB7\x07\x15\x02\x02\xB7\xB8\x05:\x1E\x02\xB8\xB9\x05" +
    "\x1E\x10\x02\xB9\xBA\x05:\x1E\x02\xBA\xBB\x07$\x02\x02\xBB\x13\x03\x02" +
    "\x02\x02\xBC\xBE\x05D#\x02\xBD\xBC\x03\x02\x02\x02\xBD\xBE\x03\x02\x02" +
    "\x02\xBE\xBF\x03\x02\x02\x02\xBF\xC0\x07\x14\x02\x02\xC0\xC1\x07\r\x02" +
    "\x02\xC1\xC2\x05:\x1E\x02\xC2\xC3\x05,\x17\x02\xC3\xC4\x05:\x1E\x02\xC4" +
    "\xC5\x07$\x02\x02\xC5\x15\x03\x02\x02\x02\xC6\xC8\x05D#\x02\xC7\xC6\x03" +
    "\x02\x02\x02\xC7\xC8\x03\x02\x02\x02\xC8\xC9\x03\x02\x02\x02\xC9\xCA\x07" +
    "\x13\x02\x02\xCA\xCB\x05:\x1E\x02\xCB\xCC\x050\x19\x02\xCC\xCD\x07$\x02" +
    "\x02\xCD\xD9\x03\x02\x02\x02\xCE\xD0\x05D#\x02\xCF\xCE\x03\x02\x02\x02" +
    "\xCF\xD0\x03\x02\x02\x02\xD0\xD1\x03\x02\x02\x02\xD1\xD2\x07\x14\x02\x02" +
    "\xD2\xD3\x07\x13\x02\x02\xD3\xD4\x05:\x1E\x02\xD4\xD5\x050\x19\x02\xD5" +
    "\xD6\x05:\x1E\x02\xD6\xD7\x07$\x02\x02\xD7\xD9\x03\x02\x02\x02\xD8\xC7" +
    "\x03\x02\x02\x02\xD8\xCF\x03\x02\x02\x02\xD9\x17\x03\x02\x02\x02\xDA\xDC" +
    "\x05D#\x02\xDB\xDA\x03\x02\x02\x02\xDB\xDC\x03\x02\x02\x02\xDC\xDD\x03" +
    "\x02\x02\x02\xDD\xDE\x07\x14\x02\x02\xDE\xDF\x05<\x1F\x02\xDF\xE0\x07" +
    "\x1F\x02\x02\xE0\xE1\x07\'\x02\x02\xE1\xE2\x05:\x1E\x02\xE2\xE3\x07 \x02" +
    "\x02\xE3\xE4\x05@!\x02\xE4\xE5\x07$\x02\x02\xE5\x19\x03\x02\x02\x02\xE6" +
    "\xE7\x07\x1A\x02\x02\xE7\xE8\x05 \x11\x02\xE8\xE9\x07$\x02\x02\xE9\x1B" +
    "\x03\x02\x02\x02\xEA\xEE\x07\x11\x02\x02\xEB\xEC\x07\x12\x02\x02\xEC\xEE" +
    "\x07\x0F\x02\x02\xED\xEA\x03\x02\x02\x02\xED\xEB\x03\x02\x02\x02\xEE\x1D" +
    "\x03\x02\x02\x02\xEF\xF3\x07\x1D\x02\x02\xF0\xF2\x05$\x13\x02\xF1\xF0" +
    "\x03\x02\x02\x02\xF2\xF5\x03\x02\x02\x02\xF3\xF1\x03\x02\x02\x02\xF3\xF4" +
    "\x03\x02\x02\x02\xF4\xF6\x03\x02\x02\x02\xF5\xF3\x03\x02\x02\x02\xF6\xF7" +
    "\x07\x1E\x02\x02\xF7\x1F\x03\x02\x02\x02\xF8\xFC\x07\x1F\x02\x02\xF9\xFB" +
    "\x05$\x13\x02\xFA\xF9\x03\x02\x02\x02\xFB\xFE\x03\x02\x02\x02\xFC\xFA" +
    "\x03\x02\x02\x02\xFC\xFD\x03\x02\x02\x02\xFD\xFF\x03\x02\x02\x02\xFE\xFC" +
    "\x03\x02\x02\x02\xFF\u0100\x07 \x02\x02\u0100!\x03\x02\x02\x02\u0101\u0105" +
    "\x07!\x02\x02\u0102\u0104\x05$\x13\x02\u0103\u0102\x03\x02\x02\x02\u0104" +
    "\u0107\x03\x02\x02\x02\u0105\u0103\x03\x02\x02\x02\u0105\u0106\x03\x02" +
    "\x02\x02\u0106\u0108\x03\x02\x02\x02\u0107\u0105\x03\x02\x02\x02\u0108" +
    "\u0109\x07\"\x02\x02\u0109#\x03\x02\x02\x02\u010A\u0111\x05\x1E\x10\x02" +
    "\u010B\u0111\x05\"\x12\x02\u010C\u0111\x05&\x14\x02\u010D\u0111\x05*\x16" +
    "\x02\u010E\u0111\x05D#\x02\u010F\u0111\x05F$\x02\u0110\u010A\x03\x02\x02" +
    "\x02\u0110\u010B\x03\x02\x02\x02\u0110\u010C\x03\x02\x02\x02\u0110\u010D" +
    "\x03\x02\x02\x02\u0110\u010E\x03\x02\x02\x02\u0110\u010F\x03\x02\x02\x02" +
    "\u0111%\x03\x02\x02\x02\u0112\u0115\x05 \x11\x02\u0113\u0115\x05(\x15" +
    "\x02\u0114\u0112\x03\x02\x02\x02\u0114\u0113\x03\x02\x02\x02\u0115\'\x03" +
    "\x02\x02\x02\u0116\u0117\t\x03\x02\x02\u0117)\x03\x02\x02\x02\u0118\u0119" +
    "\t\x04\x02\x02\u0119+\x03\x02\x02\x02\u011A\u011E\x07\x1D\x02\x02\u011B" +
    "\u011D\x05.\x18\x02\u011C\u011B\x03\x02\x02\x02\u011D\u0120\x03\x02\x02" +
    "\x02\u011E\u011C\x03\x02\x02\x02\u011E\u011F\x03\x02\x02\x02\u011F\u0121" +
    "\x03\x02\x02\x02\u0120\u011E\x03\x02\x02\x02\u0121\u0122\x07\x1E\x02\x02" +
    "\u0122-\x03\x02\x02\x02\u0123\u0125\x05D#\x02\u0124\u0123\x03\x02\x02" +
    "\x02\u0124\u0125\x03\x02\x02\x02\u0125\u0126\x03\x02\x02\x02\u0126\u0129" +
    "\x05:\x1E\x02\u0127\u0128\x07(\x02\x02\u0128\u012A\x05&\x14\x02\u0129" +
    "\u0127\x03\x02\x02\x02\u0129\u012A\x03\x02\x02\x02\u012A\u012C\x03\x02" +
    "\x02\x02\u012B\u012D\x07%\x02\x02\u012C\u012B\x03\x02\x02\x02\u012C\u012D" +
    "\x03\x02\x02\x02\u012D\u012F\x03\x02\x02\x02\u012E\u0130\x05F$\x02\u012F" +
    "\u012E\x03\x02\x02\x02\u012F\u0130\x03\x02\x02\x02\u0130/\x03\x02\x02" +
    "\x02\u0131\u0136\x07\x1D\x02\x02\u0132\u0135\x052\x1A\x02\u0133\u0135" +
    "\x058\x1D\x02\u0134\u0132\x03\x02\x02\x02\u0134\u0133\x03\x02\x02\x02" +
    "\u0135\u0138\x03\x02\x02\x02\u0136\u0134\x03\x02\x02\x02\u0136\u0137\x03" +
    "\x02\x02\x02\u0137\u0139\x03\x02\x02\x02\u0138\u0136\x03\x02\x02\x02\u0139" +
    "\u013A\x07\x1E\x02\x02\u013A1\x03\x02\x02\x02\u013B\u013E\x054\x1B\x02" +
    "\u013C\u013E\x056\x1C\x02\u013D\u013B\x03\x02\x02\x02\u013D\u013C\x03" +
    "\x02\x02\x02\u013E3\x03\x02\x02\x02\u013F\u0141\x05D#\x02\u0140\u013F" +
    "\x03\x02\x02\x02\u0140\u0141\x03\x02\x02\x02\u0141\u0146\x03\x02\x02\x02" +
    "\u0142\u0144\x07\f\x02\x02\u0143\u0142\x03\x02\x02\x02\u0143\u0144\x03" +
    "\x02\x02\x02\u0144\u0145\x03\x02\x02\x02\u0145\u0147\x07\x13\x02\x02\u0146" +
    "\u0143\x03\x02\x02\x02\u0146\u0147\x03\x02\x02\x02\u0147\u0148\x03\x02" +
    "\x02\x02\u0148\u0149\x05<\x1F\x02\u0149\u014E\x05:\x1E\x02\u014A\u014B" +
    "\x07%\x02\x02\u014B\u014D\x05:\x1E\x02\u014C\u014A\x03\x02\x02\x02\u014D" +
    "\u0150\x03\x02\x02\x02\u014E\u014C\x03\x02\x02\x02\u014E\u014F\x03\x02" +
    "\x02\x02\u014F\u0154\x03\x02\x02\x02\u0150\u014E\x03\x02\x02\x02\u0151" +
    "\u0153\x05\"\x12\x02\u0152\u0151\x03\x02\x02\x02\u0153\u0156\x03\x02\x02" +
    "\x02\u0154\u0152\x03\x02\x02\x02\u0154\u0155\x03\x02\x02\x02\u0155\u0157" +
    "\x03\x02\x02\x02\u0156\u0154\x03\x02\x02\x02\u0157\u0159\x07$\x02\x02" +
    "\u0158\u015A\x05F$\x02\u0159\u0158\x03\x02\x02\x02\u0159\u015A\x03\x02" +
    "\x02\x02\u015A5\x03\x02\x02\x02\u015B\u015D\x05D#\x02\u015C\u015B\x03" +
    "\x02\x02\x02\u015C\u015D\x03\x02\x02\x02\u015D\u015E\x03\x02\x02\x02\u015E" +
    "\u015F\x05<\x1F\x02\u015F\u0160\x07\x1F\x02\x02\u0160\u0161\x07\'\x02" +
    "\x02\u0161\u0162\x05:\x1E\x02\u0162\u0163\x07 \x02\x02\u0163\u0164\x05" +
    "@!\x02\u0164\u0166\x07$\x02\x02\u0165\u0167\x05F$\x02\u0166\u0165\x03" +
    "\x02\x02\x02\u0166\u0167\x03\x02\x02\x02\u01677\x03\x02\x02\x02\u0168" +
    "\u016A\x05D#\x02\u0169\u0168\x03\x02\x02\x02\u0169\u016A\x03\x02\x02\x02" +
    "\u016A\u016B\x03\x02\x02\x02\u016B\u016C\x07\x15\x02\x02\u016C\u016D\x05" +
    "\x1E\x10\x02\u016D\u016E\x05:\x1E\x02\u016E\u016F\x07$\x02\x02\u016F9" +
    "\x03\x02\x02\x02\u0170\u0171\x07,\x02\x02\u0171;\x03\x02\x02\x02\u0172" +
    "\u0174\x05> \x02\u0173\u0172\x03\x02\x02\x02\u0174\u0175\x03\x02\x02\x02" +
    "\u0175\u0173\x03\x02\x02\x02\u0175\u0176\x03\x02\x02\x02\u0176=\x03\x02" +
    "\x02\x02\u0177\u017C\t\x05\x02\x02\u0178\u017B\x07\'\x02\x02\u0179\u017B" +
    "\x05\"\x12\x02\u017A\u0178\x03\x02\x02\x02\u017A\u0179\x03\x02\x02\x02" +
    "\u017B\u017E\x03\x02\x02\x02\u017C\u017A\x03\x02\x02\x02\u017C\u017D\x03" +
    "\x02\x02\x02\u017D?\x03\x02\x02\x02\u017E\u017C\x03\x02\x02\x02\u017F" +
    "\u018E\x07\x1F\x02\x02\u0180\u0182\x05B\"\x02\u0181\u0180\x03\x02\x02" +
    "\x02\u0181\u0182\x03\x02\x02\x02\u0182\u0183\x03\x02\x02\x02\u0183\u018B" +
    "\x05<\x1F\x02\u0184\u0186\x07%\x02\x02\u0185\u0187\x05B\"\x02\u0186\u0185" +
    "\x03\x02\x02\x02\u0186\u0187\x03\x02\x02\x02\u0187\u0188\x03\x02\x02\x02" +
    "\u0188\u018A\x05<\x1F\x02\u0189\u0184\x03\x02\x02\x02\u018A\u018D\x03" +
    "\x02\x02\x02\u018B\u0189\x03\x02\x02\x02\u018B\u018C\x03\x02\x02\x02\u018C" +
    "\u018F\x03\x02\x02\x02\u018D\u018B\x03\x02\x02\x02\u018E\u0181\x03\x02" +
    "\x02\x02\u018E\u018F\x03\x02\x02\x02\u018F\u0192\x03\x02\x02\x02\u0190" +
    "\u0191\x07%\x02\x02\u0191\u0193\x07)\x02\x02\u0192\u0190\x03\x02\x02\x02" +
    "\u0192\u0193\x03\x02\x02\x02\u0193\u0194\x03\x02\x02\x02\u0194\u0196\x07" +
    " \x02\x02\u0195\u0197\x05B\"\x02\u0196\u0195\x03\x02\x02\x02\u0196\u0197" +
    "\x03\x02\x02\x02\u0197A\x03\x02\x02\x02\u0198\u0199\t\x06\x02\x02\u0199" +
    "\u019A\x05 \x11\x02\u019AC\x03\x02\x02\x02\u019B\u019C\t\x07\x02\x02\u019C" +
    "E\x03\x02\x02\x02\u019D\u019E\x07\x06\x02\x02\u019EG\x03\x02\x02\x02:" +
    "IN_blru}\x83\x88\x8B\x8E\x91\x95\x9C\xA1\xA8\xAC\xB3\xBD\xC7\xCF\xD8\xDB" +
    "\xED\xF3\xFC\u0105\u0110\u0114\u011E\u0124\u0129\u012C\u012F\u0134\u0136" +
    "\u013D\u0140\u0143\u0146\u014E\u0154\u0159\u015C\u0166\u0169\u0175\u017A" +
    "\u017C\u0181\u0186\u018B\u018E\u0192\u0196";
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
    globalVar() {
        return this.tryGetRuleContext(0, GlobalVarContext);
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
class GlobalVarContext extends ParserRuleContext_1.ParserRuleContext {
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
    EXTERN() { return this.tryGetToken(CHeaderParser.EXTERN, 0); }
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
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_globalVar; }
    // @Override
    enterRule(listener) {
        if (listener.enterGlobalVar) {
            listener.enterGlobalVar(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitGlobalVar) {
            listener.exitGlobalVar(this);
        }
    }
}
exports.GlobalVarContext = GlobalVarContext;
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
    attribute(i) {
        if (i === undefined) {
            return this.getRuleContexts(AttributeContext);
        }
        else {
            return this.getRuleContext(i, AttributeContext);
        }
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
    STRUCT() { return this.getToken(CHeaderParser.STRUCT, 0); }
    structBody() {
        return this.getRuleContext(0, StructBodyContext);
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    id(i) {
        if (i === undefined) {
            return this.getRuleContexts(IdContext);
        }
        else {
            return this.getRuleContext(i, IdContext);
        }
    }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    TYPEDEF() { return this.tryGetToken(CHeaderParser.TYPEDEF, 0); }
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
    unionInlineType(i) {
        if (i === undefined) {
            return this.getRuleContexts(UnionInlineTypeContext);
        }
        else {
            return this.getRuleContext(i, UnionInlineTypeContext);
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
    structVar() {
        return this.tryGetRuleContext(0, StructVarContext);
    }
    structCallback() {
        return this.tryGetRuleContext(0, StructCallbackContext);
    }
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
class StructVarContext extends ParserRuleContext_1.ParserRuleContext {
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
    get ruleIndex() { return CHeaderParser.RULE_structVar; }
    // @Override
    enterRule(listener) {
        if (listener.enterStructVar) {
            listener.enterStructVar(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitStructVar) {
            listener.exitStructVar(this);
        }
    }
}
exports.StructVarContext = StructVarContext;
class StructCallbackContext extends ParserRuleContext_1.ParserRuleContext {
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
    trailingDoc() {
        return this.tryGetRuleContext(0, TrailingDocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_structCallback; }
    // @Override
    enterRule(listener) {
        if (listener.enterStructCallback) {
            listener.enterStructCallback(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitStructCallback) {
            listener.exitStructCallback(this);
        }
    }
}
exports.StructCallbackContext = StructCallbackContext;
class UnionInlineTypeContext extends ParserRuleContext_1.ParserRuleContext {
    UNION() { return this.getToken(CHeaderParser.UNION, 0); }
    block() {
        return this.getRuleContext(0, BlockContext);
    }
    id() {
        return this.getRuleContext(0, IdContext);
    }
    SEMI() { return this.getToken(CHeaderParser.SEMI, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_unionInlineType; }
    // @Override
    enterRule(listener) {
        if (listener.enterUnionInlineType) {
            listener.enterUnionInlineType(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitUnionInlineType) {
            listener.exitUnionInlineType(this);
        }
    }
}
exports.UnionInlineTypeContext = UnionInlineTypeContext;
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
    indexing(i) {
        if (i === undefined) {
            return this.getRuleContexts(IndexingContext);
        }
        else {
            return this.getRuleContext(i, IndexingContext);
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
    attribute(i) {
        if (i === undefined) {
            return this.getRuleContexts(AttributeContext);
        }
        else {
            return this.getRuleContext(i, AttributeContext);
        }
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
    SDL_ACQUIRE() { return this.tryGetToken(CHeaderParser.SDL_ACQUIRE, 0); }
    SDL_RELEASE() { return this.tryGetToken(CHeaderParser.SDL_RELEASE, 0); }
    SDL_ALLOC_SIZE() { return this.tryGetToken(CHeaderParser.SDL_ALLOC_SIZE, 0); }
    SDL_INOUT() { return this.tryGetToken(CHeaderParser.SDL_INOUT, 0); }
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
