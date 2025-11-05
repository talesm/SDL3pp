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
exports.TrailingDocContext = exports.DocContext = exports.AttributeContext = exports.SignatureContext = exports.TypeElContext = exports.TypeContext = exports.IdContext = exports.UnionInlineTypeContext = exports.StructItemContext = exports.StructBodyContext = exports.EnumItemContext = exports.EnumBodyContext = exports.PunctContext = exports.WordContext = exports.ExprContext = exports.StmContext = exports.IndexingContext = exports.GroupContext = exports.BlockContext = exports.InlineContext = exports.CompileTimeAssertContext = exports.CallbackDefContext = exports.StructDefContext = exports.EnumDefContext = exports.UnionDefContext = exports.AliasDefContext = exports.FunctionDefContext = exports.FunctionDeclContext = exports.DirectiveContext = exports.ExternCContext = exports.DeclContext = exports.ProgContext = exports.CHeaderParser = void 0;
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
                this.state = 65;
                this._errHandler.sync(this);
                switch (this.interpreter.adaptivePredict(this._input, 0, this._ctx)) {
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
            this.state = 86;
            this._errHandler.sync(this);
            switch (this.interpreter.adaptivePredict(this._input, 2, this._ctx)) {
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
                this.state = 137;
                this.type();
                this.state = 138;
                this.id();
                this.state = 139;
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
                this.state = 183;
                this.match(CHeaderParser.SDL_COMPILE_TIME_ASSERT);
                this.state = 184;
                this.group();
                this.state = 185;
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
                this.state = 231;
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
                this.state = 233;
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
                this.state = 258;
                this.match(CHeaderParser.CURLY_B);
                this.state = 263;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.CONST) | (1 << CHeaderParser.STRUCT) | (1 << CHeaderParser.UNION) | (1 << CHeaderParser.VOID))) !== 0) || _la === CHeaderParser.ID) {
                    {
                        this.state = 261;
                        this._errHandler.sync(this);
                        switch (this.interpreter.adaptivePredict(this._input, 27, this._ctx)) {
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
                switch (this.interpreter.adaptivePredict(this._input, 31, this._ctx)) {
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
        this.enterRule(_localctx, 48, CHeaderParser.RULE_unionInlineType);
        let _la;
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
        this.enterRule(_localctx, 50, CHeaderParser.RULE_id);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 304;
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
        this.enterRule(_localctx, 52, CHeaderParser.RULE_type);
        try {
            let _alt;
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
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 309;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 36, this._ctx);
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
        this.enterRule(_localctx, 54, CHeaderParser.RULE_typeEl);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 311;
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
                                throw new NoViableAltException_1.NoViableAltException(this);
                        }
                    }
                    this.state = 318;
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
        this.enterRule(_localctx, 56, CHeaderParser.RULE_signature);
        let _la;
        try {
            let _alt;
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
                        while (_alt !== 2 && _alt !== ATN_1.ATN.INVALID_ALT_NUMBER) {
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
        this.enterRule(_localctx, 58, CHeaderParser.RULE_attribute);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 338;
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
                this.state = 339;
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
        this.enterRule(_localctx, 60, CHeaderParser.RULE_doc);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 341;
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
        this.enterRule(_localctx, 62, CHeaderParser.RULE_trailingDoc);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 343;
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
CHeaderParser.RULE_unionInlineType = 24;
CHeaderParser.RULE_id = 25;
CHeaderParser.RULE_type = 26;
CHeaderParser.RULE_typeEl = 27;
CHeaderParser.RULE_signature = 28;
CHeaderParser.RULE_attribute = 29;
CHeaderParser.RULE_doc = 30;
CHeaderParser.RULE_trailingDoc = 31;
// tslint:disable:no-trailing-whitespace
CHeaderParser.ruleNames = [
    "prog", "decl", "externC", "directive", "functionDecl", "functionDef",
    "aliasDef", "unionDef", "enumDef", "structDef", "callbackDef", "compileTimeAssert",
    "inline", "block", "group", "indexing", "stm", "expr", "word", "punct",
    "enumBody", "enumItem", "structBody", "structItem", "unionInlineType",
    "id", "type", "typeEl", "signature", "attribute", "doc", "trailingDoc",
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
CHeaderParser._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03)\u015C\x04\x02" +
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
