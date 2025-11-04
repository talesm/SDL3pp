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
exports.TrailingDocContext = exports.DocContext = exports.AttributeContext = exports.SignatureContext = exports.TypeElContext = exports.TypeContext = exports.IdContext = exports.StructItemContext = exports.StructBodyContext = exports.EnumItemLastContext = exports.EnumItemContext = exports.EnumBodyContext = exports.PunctContext = exports.WordContext = exports.StmContext = exports.IndexingContext = exports.GroupContext = exports.BlockContext = exports.InlineContext = exports.CallbackDefContext = exports.StructDefContext = exports.EnumDefContext = exports.AliasDefContext = exports.FunctionDefContext = exports.FunctionDeclContext = exports.DirectiveContext = exports.ExternCContext = exports.DeclContext = exports.ProgContext = exports.CHeaderParser = void 0;
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
                this.state = 59;
                this._errHandler.sync(this);
                switch (this.interpreter.adaptivePredict(this._input, 0, this._ctx)) {
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
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF))) !== 0)) {
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
            this.state = 78;
            this._errHandler.sync(this);
            switch (this.interpreter.adaptivePredict(this._input, 2, this._ctx)) {
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
                this.state = 80;
                this.match(CHeaderParser.EXTERN);
                this.state = 81;
                this.match(CHeaderParser.STRING);
                this.state = 82;
                this.match(CHeaderParser.CURLY_B);
                this.state = 86;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF))) !== 0)) {
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
                this.state = 127;
                this.type();
                this.state = 128;
                this.id();
                this.state = 129;
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
        this.enterRule(_localctx, 14, CHeaderParser.RULE_enumDef);
        let _la;
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
        this.enterRule(_localctx, 16, CHeaderParser.RULE_structDef);
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
        this.enterRule(_localctx, 18, CHeaderParser.RULE_callbackDef);
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
        this.enterRule(_localctx, 22, CHeaderParser.RULE_block);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 168;
                this.match(CHeaderParser.CURLY_B);
                this.state = 172;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (((((_la - 7)) & ~0x1F) === 0 && ((1 << (_la - 7)) & ((1 << (CHeaderParser.DIRECTIVE - 7)) | (1 << (CHeaderParser.ENUM - 7)) | (1 << (CHeaderParser.STATIC - 7)) | (1 << (CHeaderParser.STRUCT - 7)) | (1 << (CHeaderParser.UNION - 7)) | (1 << (CHeaderParser.VOID - 7)) | (1 << (CHeaderParser.CURLY_B - 7)) | (1 << (CHeaderParser.ROUND_B - 7)) | (1 << (CHeaderParser.SQUARE_B - 7)) | (1 << (CHeaderParser.COLON - 7)) | (1 << (CHeaderParser.SEMI - 7)) | (1 << (CHeaderParser.COMMA - 7)) | (1 << (CHeaderParser.DOT - 7)) | (1 << (CHeaderParser.STAR - 7)) | (1 << (CHeaderParser.EQ - 7)) | (1 << (CHeaderParser.PUNCT_EXTRA - 7)) | (1 << (CHeaderParser.STRING - 7)) | (1 << (CHeaderParser.ID - 7)) | (1 << (CHeaderParser.NUMBER - 7)))) !== 0)) {
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
        this.enterRule(_localctx, 24, CHeaderParser.RULE_group);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 177;
                this.match(CHeaderParser.ROUND_B);
                this.state = 181;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (((((_la - 7)) & ~0x1F) === 0 && ((1 << (_la - 7)) & ((1 << (CHeaderParser.DIRECTIVE - 7)) | (1 << (CHeaderParser.ENUM - 7)) | (1 << (CHeaderParser.STATIC - 7)) | (1 << (CHeaderParser.STRUCT - 7)) | (1 << (CHeaderParser.UNION - 7)) | (1 << (CHeaderParser.VOID - 7)) | (1 << (CHeaderParser.CURLY_B - 7)) | (1 << (CHeaderParser.ROUND_B - 7)) | (1 << (CHeaderParser.SQUARE_B - 7)) | (1 << (CHeaderParser.COLON - 7)) | (1 << (CHeaderParser.SEMI - 7)) | (1 << (CHeaderParser.COMMA - 7)) | (1 << (CHeaderParser.DOT - 7)) | (1 << (CHeaderParser.STAR - 7)) | (1 << (CHeaderParser.EQ - 7)) | (1 << (CHeaderParser.PUNCT_EXTRA - 7)) | (1 << (CHeaderParser.STRING - 7)) | (1 << (CHeaderParser.ID - 7)) | (1 << (CHeaderParser.NUMBER - 7)))) !== 0)) {
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
        this.enterRule(_localctx, 26, CHeaderParser.RULE_indexing);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 186;
                this.match(CHeaderParser.SQUARE_B);
                this.state = 190;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (((((_la - 7)) & ~0x1F) === 0 && ((1 << (_la - 7)) & ((1 << (CHeaderParser.DIRECTIVE - 7)) | (1 << (CHeaderParser.ENUM - 7)) | (1 << (CHeaderParser.STATIC - 7)) | (1 << (CHeaderParser.STRUCT - 7)) | (1 << (CHeaderParser.UNION - 7)) | (1 << (CHeaderParser.VOID - 7)) | (1 << (CHeaderParser.CURLY_B - 7)) | (1 << (CHeaderParser.ROUND_B - 7)) | (1 << (CHeaderParser.SQUARE_B - 7)) | (1 << (CHeaderParser.COLON - 7)) | (1 << (CHeaderParser.SEMI - 7)) | (1 << (CHeaderParser.COMMA - 7)) | (1 << (CHeaderParser.DOT - 7)) | (1 << (CHeaderParser.STAR - 7)) | (1 << (CHeaderParser.EQ - 7)) | (1 << (CHeaderParser.PUNCT_EXTRA - 7)) | (1 << (CHeaderParser.STRING - 7)) | (1 << (CHeaderParser.ID - 7)) | (1 << (CHeaderParser.NUMBER - 7)))) !== 0)) {
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
        this.enterRule(_localctx, 28, CHeaderParser.RULE_stm);
        try {
            this.state = 200;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case CHeaderParser.CURLY_B:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 195;
                        this.block();
                    }
                    break;
                case CHeaderParser.ROUND_B:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 196;
                        this.group();
                    }
                    break;
                case CHeaderParser.SQUARE_B:
                    this.enterOuterAlt(_localctx, 3);
                    {
                        this.state = 197;
                        this.indexing();
                    }
                    break;
                case CHeaderParser.DIRECTIVE:
                case CHeaderParser.ENUM:
                case CHeaderParser.STATIC:
                case CHeaderParser.STRUCT:
                case CHeaderParser.UNION:
                case CHeaderParser.VOID:
                case CHeaderParser.STRING:
                case CHeaderParser.ID:
                case CHeaderParser.NUMBER:
                    this.enterOuterAlt(_localctx, 4);
                    {
                        this.state = 198;
                        this.word();
                    }
                    break;
                case CHeaderParser.COLON:
                case CHeaderParser.SEMI:
                case CHeaderParser.COMMA:
                case CHeaderParser.DOT:
                case CHeaderParser.STAR:
                case CHeaderParser.EQ:
                case CHeaderParser.PUNCT_EXTRA:
                    this.enterOuterAlt(_localctx, 5);
                    {
                        this.state = 199;
                        this.punct();
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
        this.enterRule(_localctx, 30, CHeaderParser.RULE_word);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 202;
                _la = this._input.LA(1);
                if (!(((((_la - 7)) & ~0x1F) === 0 && ((1 << (_la - 7)) & ((1 << (CHeaderParser.DIRECTIVE - 7)) | (1 << (CHeaderParser.ENUM - 7)) | (1 << (CHeaderParser.STATIC - 7)) | (1 << (CHeaderParser.STRUCT - 7)) | (1 << (CHeaderParser.UNION - 7)) | (1 << (CHeaderParser.VOID - 7)) | (1 << (CHeaderParser.STRING - 7)) | (1 << (CHeaderParser.ID - 7)) | (1 << (CHeaderParser.NUMBER - 7)))) !== 0))) {
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
        this.enterRule(_localctx, 32, CHeaderParser.RULE_punct);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 204;
                _la = this._input.LA(1);
                if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA) | (1 << CHeaderParser.DOT) | (1 << CHeaderParser.STAR) | (1 << CHeaderParser.EQ) | (1 << CHeaderParser.PUNCT_EXTRA))) !== 0))) {
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
        this.enterRule(_localctx, 34, CHeaderParser.RULE_enumBody);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 206;
                this.match(CHeaderParser.CURLY_B);
                this.state = 210;
                this._errHandler.sync(this);
                _alt = this.interpreter.adaptivePredict(this._input, 19, this._ctx);
                while (_alt !== 2 && _alt !== ATN_1.ATN.INVALID_ALT_NUMBER) {
                    if (_alt === 1) {
                        {
                            {
                                this.state = 207;
                                this.enumItem();
                            }
                        }
                    }
                    this.state = 212;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 19, this._ctx);
                }
                this.state = 213;
                this.enumItemLast();
                this.state = 214;
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
        this.enterRule(_localctx, 36, CHeaderParser.RULE_enumItem);
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
                this.id();
                this.state = 222;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.EQ) {
                    {
                        this.state = 220;
                        this.match(CHeaderParser.EQ);
                        this.state = 221;
                        this.match(CHeaderParser.NUMBER);
                    }
                }
                this.state = 224;
                this.match(CHeaderParser.COMMA);
                this.state = 226;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.TRAILING_DOC) {
                    {
                        this.state = 225;
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
    enumItemLast() {
        let _localctx = new EnumItemLastContext(this._ctx, this.state);
        this.enterRule(_localctx, 38, CHeaderParser.RULE_enumItemLast);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 229;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 228;
                        this.doc();
                    }
                }
                this.state = 231;
                this.id();
                this.state = 234;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.EQ) {
                    {
                        this.state = 232;
                        this.match(CHeaderParser.EQ);
                        this.state = 233;
                        this.match(CHeaderParser.NUMBER);
                    }
                }
                this.state = 237;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.COMMA) {
                    {
                        this.state = 236;
                        this.match(CHeaderParser.COMMA);
                    }
                }
                this.state = 240;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.TRAILING_DOC) {
                    {
                        this.state = 239;
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
        this.enterRule(_localctx, 40, CHeaderParser.RULE_structBody);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 242;
                this.match(CHeaderParser.CURLY_B);
                this.state = 246;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (((((_la - 5)) & ~0x1F) === 0 && ((1 << (_la - 5)) & ((1 << (CHeaderParser.LONG_DOC - 5)) | (1 << (CHeaderParser.SHORT_DOC - 5)) | (1 << (CHeaderParser.VOID - 5)) | (1 << (CHeaderParser.ID - 5)))) !== 0)) {
                    {
                        {
                            this.state = 243;
                            this.structItem();
                        }
                    }
                    this.state = 248;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 249;
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
        this.enterRule(_localctx, 42, CHeaderParser.RULE_structItem);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 252;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 251;
                        this.doc();
                    }
                }
                this.state = 254;
                this.type();
                this.state = 255;
                this.id();
                this.state = 260;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.COMMA) {
                    {
                        {
                            this.state = 256;
                            this.match(CHeaderParser.COMMA);
                            this.state = 257;
                            this.id();
                        }
                    }
                    this.state = 262;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 266;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.SQUARE_B) {
                    {
                        {
                            this.state = 263;
                            this.indexing();
                        }
                    }
                    this.state = 268;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 269;
                this.match(CHeaderParser.SEMI);
                this.state = 271;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.TRAILING_DOC) {
                    {
                        this.state = 270;
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
        this.enterRule(_localctx, 44, CHeaderParser.RULE_id);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 273;
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
        this.enterRule(_localctx, 46, CHeaderParser.RULE_type);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 276;
                this._errHandler.sync(this);
                _alt = 1;
                do {
                    switch (_alt) {
                        case 1:
                            {
                                {
                                    this.state = 275;
                                    this.typeEl();
                                }
                            }
                            break;
                        default:
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 278;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 32, this._ctx);
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
        this.enterRule(_localctx, 48, CHeaderParser.RULE_typeEl);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 280;
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
                this.state = 284;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.STAR) {
                    {
                        {
                            this.state = 281;
                            this.match(CHeaderParser.STAR);
                        }
                    }
                    this.state = 286;
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
        this.enterRule(_localctx, 50, CHeaderParser.RULE_signature);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 287;
                this.match(CHeaderParser.ROUND_B);
                this.state = 296;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.VOID || _la === CHeaderParser.ID) {
                    {
                        this.state = 288;
                        this.type();
                        this.state = 293;
                        this._errHandler.sync(this);
                        _la = this._input.LA(1);
                        while (_la === CHeaderParser.COMMA) {
                            {
                                {
                                    this.state = 289;
                                    this.match(CHeaderParser.COMMA);
                                    this.state = 290;
                                    this.type();
                                }
                            }
                            this.state = 295;
                            this._errHandler.sync(this);
                            _la = this._input.LA(1);
                        }
                    }
                }
                this.state = 298;
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
    attribute() {
        let _localctx = new AttributeContext(this._ctx, this.state);
        this.enterRule(_localctx, 52, CHeaderParser.RULE_attribute);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 300;
                this.match(CHeaderParser.ATTRIBUTE);
                this.state = 301;
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
        this.enterRule(_localctx, 54, CHeaderParser.RULE_doc);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 303;
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
        this.enterRule(_localctx, 56, CHeaderParser.RULE_trailingDoc);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 305;
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
CHeaderParser.DIRECTIVE = 7;
CHeaderParser.ATTRIBUTE = 8;
CHeaderParser.ENUM = 9;
CHeaderParser.EXTERN = 10;
CHeaderParser.INLINE = 11;
CHeaderParser.SDL_NOISE = 12;
CHeaderParser.SDL_INLINE = 13;
CHeaderParser.STATIC = 14;
CHeaderParser.STRUCT = 15;
CHeaderParser.TYPEDEF = 16;
CHeaderParser.UNION = 17;
CHeaderParser.VOID = 18;
CHeaderParser.CURLY_B = 19;
CHeaderParser.CURLY_E = 20;
CHeaderParser.ROUND_B = 21;
CHeaderParser.ROUND_E = 22;
CHeaderParser.SQUARE_B = 23;
CHeaderParser.SQUARE_E = 24;
CHeaderParser.COLON = 25;
CHeaderParser.SEMI = 26;
CHeaderParser.COMMA = 27;
CHeaderParser.DOT = 28;
CHeaderParser.STAR = 29;
CHeaderParser.EQ = 30;
CHeaderParser.PUNCT_EXTRA = 31;
CHeaderParser.STRING = 32;
CHeaderParser.ID = 33;
CHeaderParser.NUMBER = 34;
CHeaderParser.RULE_prog = 0;
CHeaderParser.RULE_decl = 1;
CHeaderParser.RULE_externC = 2;
CHeaderParser.RULE_directive = 3;
CHeaderParser.RULE_functionDecl = 4;
CHeaderParser.RULE_functionDef = 5;
CHeaderParser.RULE_aliasDef = 6;
CHeaderParser.RULE_enumDef = 7;
CHeaderParser.RULE_structDef = 8;
CHeaderParser.RULE_callbackDef = 9;
CHeaderParser.RULE_inline = 10;
CHeaderParser.RULE_block = 11;
CHeaderParser.RULE_group = 12;
CHeaderParser.RULE_indexing = 13;
CHeaderParser.RULE_stm = 14;
CHeaderParser.RULE_word = 15;
CHeaderParser.RULE_punct = 16;
CHeaderParser.RULE_enumBody = 17;
CHeaderParser.RULE_enumItem = 18;
CHeaderParser.RULE_enumItemLast = 19;
CHeaderParser.RULE_structBody = 20;
CHeaderParser.RULE_structItem = 21;
CHeaderParser.RULE_id = 22;
CHeaderParser.RULE_type = 23;
CHeaderParser.RULE_typeEl = 24;
CHeaderParser.RULE_signature = 25;
CHeaderParser.RULE_attribute = 26;
CHeaderParser.RULE_doc = 27;
CHeaderParser.RULE_trailingDoc = 28;
// tslint:disable:no-trailing-whitespace
CHeaderParser.ruleNames = [
    "prog", "decl", "externC", "directive", "functionDecl", "functionDef",
    "aliasDef", "enumDef", "structDef", "callbackDef", "inline", "block",
    "group", "indexing", "stm", "word", "punct", "enumBody", "enumItem", "enumItemLast",
    "structBody", "structItem", "id", "type", "typeEl", "signature", "attribute",
    "doc", "trailingDoc",
];
CHeaderParser._LITERAL_NAMES = [
    undefined, undefined, undefined, undefined, undefined, undefined, undefined,
    undefined, "'__attribute__'", "'enum'", "'extern'", "'__inline__'", undefined,
    "'SDL_FORCE_INLINE'", "'static'", "'struct'", "'typedef'", "'union'",
    "'void'", "'{'", "'}'", "'('", "')'", "'['", "']'", "':'", "';'", "','",
    "'.'", "'*'", "'='",
];
CHeaderParser._SYMBOLIC_NAMES = [
    undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "TRAILING_DOC", "LONG_DOC",
    "SHORT_DOC", "DIRECTIVE", "ATTRIBUTE", "ENUM", "EXTERN", "INLINE", "SDL_NOISE",
    "SDL_INLINE", "STATIC", "STRUCT", "TYPEDEF", "UNION", "VOID", "CURLY_B",
    "CURLY_E", "ROUND_B", "ROUND_E", "SQUARE_B", "SQUARE_E", "COLON", "SEMI",
    "COMMA", "DOT", "STAR", "EQ", "PUNCT_EXTRA", "STRING", "ID", "NUMBER",
];
CHeaderParser.VOCABULARY = new VocabularyImpl_1.VocabularyImpl(CHeaderParser._LITERAL_NAMES, CHeaderParser._SYMBOLIC_NAMES, []);
CHeaderParser._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03$\u0136\x04\x02" +
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
    "\x0F\x03\x0F\x03\x0F\x03\x10\x03\x10\x03\x10\x03\x10\x03\x10\x05\x10\xCB" +
    "\n\x10\x03\x11\x03\x11\x03\x12\x03\x12\x03\x13\x03\x13\x07\x13\xD3\n\x13" +
    "\f\x13\x0E\x13\xD6\v\x13\x03\x13\x03\x13\x03\x13\x03\x14\x05\x14\xDC\n" +
    "\x14\x03\x14\x03\x14\x03\x14\x05\x14\xE1\n\x14\x03\x14\x03\x14\x05\x14" +
    "\xE5\n\x14\x03\x15\x05\x15\xE8\n\x15\x03\x15\x03\x15\x03\x15\x05\x15\xED" +
    "\n\x15\x03\x15\x05\x15\xF0\n\x15\x03\x15\x05\x15\xF3\n\x15\x03\x16\x03" +
    "\x16\x07\x16\xF7\n\x16\f\x16\x0E\x16\xFA\v\x16\x03\x16\x03\x16\x03\x17" +
    "\x05\x17\xFF\n\x17\x03\x17\x03\x17\x03\x17\x03\x17\x07\x17\u0105\n\x17" +
    "\f\x17\x0E\x17\u0108\v\x17\x03\x17\x07\x17\u010B\n\x17\f\x17\x0E\x17\u010E" +
    "\v\x17\x03\x17\x03\x17\x05\x17\u0112\n\x17\x03\x18\x03\x18\x03\x19\x06" +
    "\x19\u0117\n\x19\r\x19\x0E\x19\u0118\x03\x1A\x03\x1A\x07\x1A\u011D\n\x1A" +
    "\f\x1A\x0E\x1A\u0120\v\x1A\x03\x1B\x03\x1B\x03\x1B\x03\x1B\x07\x1B\u0126" +
    "\n\x1B\f\x1B\x0E\x1B\u0129\v\x1B\x05\x1B\u012B\n\x1B\x03\x1B\x03\x1B\x03" +
    "\x1C\x03\x1C\x03\x1C\x03\x1D\x03\x1D\x03\x1E\x03\x1E\x03\x1E\x02\x02\x02" +
    "\x1F\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E\x02\x10\x02\x12\x02" +
    "\x14\x02\x16\x02\x18\x02\x1A\x02\x1C\x02\x1E\x02 \x02\"\x02$\x02&\x02" +
    "(\x02*\x02,\x02.\x020\x022\x024\x026\x028\x02:\x02\x02\x07\x04\x02\x11" +
    "\x11\x13\x13\x07\x02\t\t\v\v\x10\x11\x13\x14\"$\x03\x02\x1B!\x04\x02\x14" +
    "\x14##\x03\x02\x07\b\x02\u0146\x02=\x03\x02\x02\x02\x04P\x03\x02\x02\x02" +
    "\x06R\x03\x02\x02\x02\b^\x03\x02\x02\x02\nc\x03\x02\x02\x02\fo\x03\x02" +
    "\x02\x02\x0E{\x03\x02\x02\x02\x10\x86\x03\x02\x02\x02\x12\x90\x03\x02" +
    "\x02\x02\x14\x9A\x03\x02\x02\x02\x16\xA8\x03\x02\x02\x02\x18\xAA\x03\x02" +
    "\x02\x02\x1A\xB3\x03\x02\x02\x02\x1C\xBC\x03\x02\x02\x02\x1E\xCA\x03\x02" +
    "\x02\x02 \xCC\x03\x02\x02\x02\"\xCE\x03\x02\x02\x02$\xD0\x03\x02\x02\x02" +
    "&\xDB\x03\x02\x02\x02(\xE7\x03\x02\x02\x02*\xF4\x03\x02\x02\x02,\xFE\x03" +
    "\x02\x02\x02.\u0113\x03\x02\x02\x020\u0116\x03\x02\x02\x022\u011A\x03" +
    "\x02\x02\x024\u0121\x03\x02\x02\x026\u012E\x03\x02\x02\x028\u0131\x03" +
    "\x02\x02\x02:\u0133\x03\x02\x02\x02<>\x058\x1D\x02=<\x03\x02\x02\x02=" +
    ">\x03\x02\x02\x02>B\x03\x02\x02\x02?A\x05\x04\x03\x02@?\x03\x02\x02\x02" +
    "AD\x03\x02\x02\x02B@\x03\x02\x02\x02BC\x03\x02\x02\x02CE\x03\x02\x02\x02" +
    "DB\x03\x02\x02\x02EF\x07\x02\x02\x03F\x03\x03\x02\x02\x02GQ\x05\b\x05" +
    "\x02HQ\x05\x06\x04\x02IQ\x05\n\x06\x02JQ\x05\f\x07\x02KQ\x05\x0E\b\x02" +
    "LQ\x05\x10\t\x02MQ\x05\x12\n\x02NQ\x05\x14\v\x02OQ\x058\x1D\x02PG\x03" +
    "\x02\x02\x02PH\x03\x02\x02\x02PI\x03\x02\x02\x02PJ\x03\x02\x02\x02PK\x03" +
    "\x02\x02\x02PL\x03\x02\x02\x02PM\x03\x02\x02\x02PN\x03\x02\x02\x02PO\x03" +
    "\x02\x02\x02Q\x05\x03\x02\x02\x02RS\x07\f\x02\x02ST\x07\"\x02\x02TX\x07" +
    "\x15\x02\x02UW\x05\x04\x03\x02VU\x03\x02\x02\x02WZ\x03\x02\x02\x02XV\x03" +
    "\x02\x02\x02XY\x03\x02\x02\x02Y[\x03\x02\x02\x02ZX\x03\x02\x02\x02[\\" +
    "\x07\x16\x02\x02\\\x07\x03\x02\x02\x02]_\x058\x1D\x02^]\x03\x02\x02\x02" +
    "^_\x03\x02\x02\x02_`\x03\x02\x02\x02`a\x07\t\x02\x02a\t\x03\x02\x02\x02" +
    "bd\x058\x1D\x02cb\x03\x02\x02\x02cd\x03\x02\x02\x02de\x03\x02\x02\x02" +
    "ef\x07\f\x02\x02fh\x050\x19\x02gi\x056\x1C\x02hg\x03\x02\x02\x02hi\x03" +
    "\x02\x02\x02ij\x03\x02\x02\x02jk\x05.\x18\x02kl\x054\x1B\x02lm\x07\x1C" +
    "\x02\x02m\v\x03\x02\x02\x02np\x058\x1D\x02on\x03\x02\x02\x02op\x03\x02" +
    "\x02\x02pq\x03\x02\x02\x02qr\x05\x16\f\x02rt\x050\x19\x02su\x056\x1C\x02" +
    "ts\x03\x02\x02\x02tu\x03\x02\x02\x02uv\x03\x02\x02\x02vw\x05.\x18\x02" +
    "wx\x054\x1B\x02xy\x05\x18\r\x02y\r\x03\x02\x02\x02z|\x058\x1D\x02{z\x03" +
    "\x02\x02\x02{|\x03\x02\x02\x02|}\x03\x02\x02\x02}\x7F\x07\x12\x02\x02" +
    "~\x80\t\x02\x02\x02\x7F~\x03\x02\x02\x02\x7F\x80\x03\x02\x02\x02\x80\x81" +
    "\x03\x02\x02\x02\x81\x82\x050\x19\x02\x82\x83\x05.\x18\x02\x83\x84\x07" +
    "\x1C\x02\x02\x84\x0F\x03\x02\x02\x02\x85\x87\x058\x1D\x02\x86\x85\x03" +
    "\x02\x02\x02\x86\x87\x03\x02\x02\x02\x87\x88\x03\x02\x02\x02\x88\x89\x07" +
    "\x12\x02\x02\x89\x8A\x07\v\x02\x02\x8A\x8B\x05.\x18\x02\x8B\x8C\x05$\x13" +
    "\x02\x8C\x8D\x05.\x18\x02\x8D\x8E\x07\x1C\x02\x02\x8E\x11\x03\x02\x02" +
    "\x02\x8F\x91\x058\x1D\x02\x90\x8F\x03\x02\x02\x02\x90\x91\x03\x02\x02" +
    "\x02\x91\x92\x03\x02\x02\x02\x92\x93\x07\x12\x02\x02\x93\x94\x07\x11\x02" +
    "\x02\x94\x95\x05.\x18\x02\x95\x96\x05*\x16\x02\x96\x97\x05.\x18\x02\x97" +
    "\x98\x07\x1C\x02\x02\x98\x13\x03\x02\x02\x02\x99\x9B\x058\x1D\x02\x9A" +
    "\x99\x03\x02\x02\x02\x9A\x9B\x03\x02\x02\x02\x9B\x9C\x03\x02\x02\x02\x9C" +
    "\x9D\x07\x12\x02\x02\x9D\x9E\x050\x19\x02\x9E\x9F\x07\x17\x02\x02\x9F" +
    "\xA0\x07\x1F\x02\x02\xA0\xA1\x05.\x18\x02\xA1\xA2\x07\x18\x02\x02\xA2" +
    "\xA3\x054\x1B\x02\xA3\xA4\x07\x1C\x02\x02\xA4\x15\x03\x02\x02\x02\xA5" +
    "\xA9\x07\x0F\x02\x02\xA6\xA7\x07\x10\x02\x02\xA7\xA9\x07\r\x02\x02\xA8" +
    "\xA5\x03\x02\x02\x02\xA8\xA6\x03\x02\x02\x02\xA9\x17\x03\x02\x02\x02\xAA" +
    "\xAE\x07\x15\x02\x02\xAB\xAD\x05\x1E\x10\x02\xAC\xAB\x03\x02\x02\x02\xAD" +
    "\xB0\x03\x02\x02\x02\xAE\xAC\x03\x02\x02\x02\xAE\xAF\x03\x02\x02\x02\xAF" +
    "\xB1\x03\x02\x02\x02\xB0\xAE\x03\x02\x02\x02\xB1\xB2\x07\x16\x02\x02\xB2" +
    "\x19\x03\x02\x02\x02\xB3\xB7\x07\x17\x02\x02\xB4\xB6\x05\x1E\x10\x02\xB5" +
    "\xB4\x03\x02\x02\x02\xB6\xB9\x03\x02\x02\x02\xB7\xB5\x03\x02\x02\x02\xB7" +
    "\xB8\x03\x02\x02\x02\xB8\xBA\x03\x02\x02\x02\xB9\xB7\x03\x02\x02\x02\xBA" +
    "\xBB\x07\x18\x02\x02\xBB\x1B\x03\x02\x02\x02\xBC\xC0\x07\x19\x02\x02\xBD" +
    "\xBF\x05\x1E\x10\x02\xBE\xBD\x03\x02\x02\x02\xBF\xC2\x03\x02\x02\x02\xC0" +
    "\xBE\x03\x02\x02\x02\xC0\xC1\x03\x02\x02\x02\xC1\xC3\x03\x02\x02\x02\xC2" +
    "\xC0\x03\x02\x02\x02\xC3\xC4\x07\x1A\x02\x02\xC4\x1D\x03\x02\x02\x02\xC5" +
    "\xCB\x05\x18\r\x02\xC6\xCB\x05\x1A\x0E\x02\xC7\xCB\x05\x1C\x0F\x02\xC8" +
    "\xCB\x05 \x11\x02\xC9\xCB\x05\"\x12\x02\xCA\xC5\x03\x02\x02\x02\xCA\xC6" +
    "\x03\x02\x02\x02\xCA\xC7\x03\x02\x02\x02\xCA\xC8\x03\x02\x02\x02\xCA\xC9" +
    "\x03\x02\x02\x02\xCB\x1F\x03\x02\x02\x02\xCC\xCD\t\x03\x02\x02\xCD!\x03" +
    "\x02\x02\x02\xCE\xCF\t\x04\x02\x02\xCF#\x03\x02\x02\x02\xD0\xD4\x07\x15" +
    "\x02\x02\xD1\xD3\x05&\x14\x02\xD2\xD1\x03\x02\x02\x02\xD3\xD6\x03\x02" +
    "\x02\x02\xD4\xD2\x03\x02\x02\x02\xD4\xD5\x03\x02\x02\x02\xD5\xD7\x03\x02" +
    "\x02\x02\xD6\xD4\x03\x02\x02\x02\xD7\xD8\x05(\x15\x02\xD8\xD9\x07\x16" +
    "\x02\x02\xD9%\x03\x02\x02\x02\xDA\xDC\x058\x1D\x02\xDB\xDA\x03\x02\x02" +
    "\x02\xDB\xDC\x03\x02\x02\x02\xDC\xDD\x03\x02\x02\x02\xDD\xE0\x05.\x18" +
    "\x02\xDE\xDF\x07 \x02\x02\xDF\xE1\x07$\x02\x02\xE0\xDE\x03\x02\x02\x02" +
    "\xE0\xE1\x03\x02\x02\x02\xE1\xE2\x03\x02\x02\x02\xE2\xE4\x07\x1D\x02\x02" +
    "\xE3\xE5\x05:\x1E\x02\xE4\xE3\x03\x02\x02\x02\xE4\xE5\x03\x02\x02\x02" +
    "\xE5\'\x03\x02\x02\x02\xE6\xE8\x058\x1D\x02\xE7\xE6\x03\x02\x02\x02\xE7" +
    "\xE8\x03\x02\x02\x02\xE8\xE9\x03\x02\x02\x02\xE9\xEC\x05.\x18\x02\xEA" +
    "\xEB\x07 \x02\x02\xEB\xED\x07$\x02\x02\xEC\xEA\x03\x02\x02\x02\xEC\xED" +
    "\x03\x02\x02\x02\xED\xEF\x03\x02\x02\x02\xEE\xF0\x07\x1D\x02\x02\xEF\xEE" +
    "\x03\x02\x02\x02\xEF\xF0\x03\x02\x02\x02\xF0\xF2\x03\x02\x02\x02\xF1\xF3" +
    "\x05:\x1E\x02\xF2\xF1\x03\x02\x02\x02\xF2\xF3\x03\x02\x02\x02\xF3)\x03" +
    "\x02\x02\x02\xF4\xF8\x07\x15\x02\x02\xF5\xF7\x05,\x17\x02\xF6\xF5\x03" +
    "\x02\x02\x02\xF7\xFA\x03\x02\x02\x02\xF8\xF6\x03\x02\x02\x02\xF8\xF9\x03" +
    "\x02\x02\x02\xF9\xFB\x03\x02\x02\x02\xFA\xF8\x03\x02\x02\x02\xFB\xFC\x07" +
    "\x16\x02\x02\xFC+\x03\x02\x02\x02\xFD\xFF\x058\x1D\x02\xFE\xFD\x03\x02" +
    "\x02\x02\xFE\xFF\x03\x02\x02\x02\xFF\u0100\x03\x02\x02\x02\u0100\u0101" +
    "\x050\x19\x02\u0101\u0106\x05.\x18\x02\u0102\u0103\x07\x1D\x02\x02\u0103" +
    "\u0105\x05.\x18\x02\u0104\u0102\x03\x02\x02\x02\u0105\u0108\x03\x02\x02" +
    "\x02\u0106\u0104\x03\x02\x02\x02\u0106\u0107\x03\x02\x02\x02\u0107\u010C" +
    "\x03\x02\x02\x02\u0108\u0106\x03\x02\x02\x02\u0109\u010B\x05\x1C\x0F\x02" +
    "\u010A\u0109\x03\x02\x02\x02\u010B\u010E\x03\x02\x02\x02\u010C\u010A\x03" +
    "\x02\x02\x02\u010C\u010D\x03\x02\x02\x02\u010D\u010F\x03\x02\x02\x02\u010E" +
    "\u010C\x03\x02\x02\x02\u010F\u0111\x07\x1C\x02\x02\u0110\u0112\x05:\x1E" +
    "\x02\u0111\u0110\x03\x02\x02\x02\u0111\u0112\x03\x02\x02\x02\u0112-\x03" +
    "\x02\x02\x02\u0113\u0114\x07#\x02\x02\u0114/\x03\x02\x02\x02\u0115\u0117" +
    "\x052\x1A\x02\u0116\u0115\x03\x02\x02\x02\u0117\u0118\x03\x02\x02\x02" +
    "\u0118\u0116\x03\x02\x02\x02\u0118\u0119\x03\x02\x02\x02\u01191\x03\x02" +
    "\x02\x02\u011A\u011E\t\x05\x02\x02\u011B\u011D\x07\x1F\x02\x02\u011C\u011B" +
    "\x03\x02\x02\x02\u011D\u0120\x03\x02\x02\x02\u011E\u011C\x03\x02\x02\x02" +
    "\u011E\u011F\x03\x02\x02\x02\u011F3\x03\x02\x02\x02\u0120\u011E\x03\x02" +
    "\x02\x02\u0121\u012A\x07\x17\x02\x02\u0122\u0127\x050\x19\x02\u0123\u0124" +
    "\x07\x1D\x02\x02\u0124\u0126\x050\x19\x02\u0125\u0123\x03\x02\x02\x02" +
    "\u0126\u0129\x03\x02\x02\x02\u0127\u0125\x03\x02\x02\x02\u0127\u0128\x03" +
    "\x02\x02\x02\u0128\u012B\x03\x02\x02\x02\u0129\u0127\x03\x02\x02\x02\u012A" +
    "\u0122\x03\x02\x02\x02\u012A\u012B\x03\x02\x02\x02\u012B\u012C\x03\x02" +
    "\x02\x02\u012C\u012D\x07\x18\x02\x02\u012D5\x03\x02\x02\x02\u012E\u012F" +
    "\x07\n\x02\x02\u012F\u0130\x05\x1A\x0E\x02\u01307\x03\x02\x02\x02\u0131" +
    "\u0132\t\x06\x02\x02\u01329\x03\x02\x02\x02\u0133\u0134\x07\x06\x02\x02" +
    "\u0134;\x03\x02\x02\x02&=BPX^chot{\x7F\x86\x90\x9A\xA8\xAE\xB7\xC0\xCA" +
    "\xD4\xDB\xE0\xE4\xE7\xEC\xEF\xF2\xF8\xFE\u0106\u010C\u0111\u0118\u011E" +
    "\u0127\u012A";
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
    enumDef() {
        return this.tryGetRuleContext(0, EnumDefContext);
    }
    structDef() {
        return this.tryGetRuleContext(0, StructDefContext);
    }
    callbackDef() {
        return this.tryGetRuleContext(0, CallbackDefContext);
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
    group() {
        return this.tryGetRuleContext(0, GroupContext);
    }
    indexing() {
        return this.tryGetRuleContext(0, IndexingContext);
    }
    word() {
        return this.tryGetRuleContext(0, WordContext);
    }
    punct() {
        return this.tryGetRuleContext(0, PunctContext);
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
class WordContext extends ParserRuleContext_1.ParserRuleContext {
    ID() { return this.tryGetToken(CHeaderParser.ID, 0); }
    VOID() { return this.tryGetToken(CHeaderParser.VOID, 0); }
    STATIC() { return this.tryGetToken(CHeaderParser.STATIC, 0); }
    ENUM() { return this.tryGetToken(CHeaderParser.ENUM, 0); }
    STRUCT() { return this.tryGetToken(CHeaderParser.STRUCT, 0); }
    UNION() { return this.tryGetToken(CHeaderParser.UNION, 0); }
    NUMBER() { return this.tryGetToken(CHeaderParser.NUMBER, 0); }
    STRING() { return this.tryGetToken(CHeaderParser.STRING, 0); }
    DIRECTIVE() { return this.tryGetToken(CHeaderParser.DIRECTIVE, 0); }
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
    enumItemLast() {
        return this.getRuleContext(0, EnumItemLastContext);
    }
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
    COMMA() { return this.getToken(CHeaderParser.COMMA, 0); }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    EQ() { return this.tryGetToken(CHeaderParser.EQ, 0); }
    NUMBER() { return this.tryGetToken(CHeaderParser.NUMBER, 0); }
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
class EnumItemLastContext extends ParserRuleContext_1.ParserRuleContext {
    id() {
        return this.getRuleContext(0, IdContext);
    }
    doc() {
        return this.tryGetRuleContext(0, DocContext);
    }
    EQ() { return this.tryGetToken(CHeaderParser.EQ, 0); }
    NUMBER() { return this.tryGetToken(CHeaderParser.NUMBER, 0); }
    COMMA() { return this.tryGetToken(CHeaderParser.COMMA, 0); }
    trailingDoc() {
        return this.tryGetRuleContext(0, TrailingDocContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return CHeaderParser.RULE_enumItemLast; }
    // @Override
    enterRule(listener) {
        if (listener.enterEnumItemLast) {
            listener.enterEnumItemLast(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitEnumItemLast) {
            listener.exitEnumItemLast(this);
        }
    }
}
exports.EnumItemLastContext = EnumItemLastContext;
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
    ATTRIBUTE() { return this.getToken(CHeaderParser.ATTRIBUTE, 0); }
    group() {
        return this.getRuleContext(0, GroupContext);
    }
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
