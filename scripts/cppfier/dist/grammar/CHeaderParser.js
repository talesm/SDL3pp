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
exports.DocContext = exports.AttributeContext = exports.SignatureContext = exports.TypeElContext = exports.TypeContext = exports.IdContext = exports.PunctContext = exports.WordContext = exports.StmContext = exports.IndexingContext = exports.GroupContext = exports.BlockContext = exports.InlineContext = exports.AliasDefContext = exports.FunctionDefContext = exports.FunctionDeclContext = exports.DirectiveContext = exports.ExternCContext = exports.DeclContext = exports.ProgContext = exports.CHeaderParser = void 0;
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
                this.state = 41;
                this._errHandler.sync(this);
                switch (this.interpreter.adaptivePredict(this._input, 0, this._ctx)) {
                    case 1:
                        {
                            this.state = 40;
                            this.doc();
                        }
                        break;
                }
                this.state = 46;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF))) !== 0)) {
                    {
                        {
                            this.state = 43;
                            this.decl();
                        }
                    }
                    this.state = 48;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 49;
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
            this.state = 57;
            this._errHandler.sync(this);
            switch (this.interpreter.adaptivePredict(this._input, 2, this._ctx)) {
                case 1:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 51;
                        this.directive();
                    }
                    break;
                case 2:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 52;
                        this.externC();
                    }
                    break;
                case 3:
                    this.enterOuterAlt(_localctx, 3);
                    {
                        this.state = 53;
                        this.functionDecl();
                    }
                    break;
                case 4:
                    this.enterOuterAlt(_localctx, 4);
                    {
                        this.state = 54;
                        this.functionDef();
                    }
                    break;
                case 5:
                    this.enterOuterAlt(_localctx, 5);
                    {
                        this.state = 55;
                        this.aliasDef();
                    }
                    break;
                case 6:
                    this.enterOuterAlt(_localctx, 6);
                    {
                        this.state = 56;
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
                this.state = 59;
                this.match(CHeaderParser.EXTERN);
                this.state = 60;
                this.match(CHeaderParser.STRING);
                this.state = 61;
                this.match(CHeaderParser.CURLY_B);
                this.state = 65;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.LONG_DOC) | (1 << CHeaderParser.SHORT_DOC) | (1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.EXTERN) | (1 << CHeaderParser.SDL_INLINE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.TYPEDEF))) !== 0)) {
                    {
                        {
                            this.state = 62;
                            this.decl();
                        }
                    }
                    this.state = 67;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 68;
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
                this.state = 71;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 70;
                        this.doc();
                    }
                }
                this.state = 73;
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
                this.state = 76;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 75;
                        this.doc();
                    }
                }
                this.state = 78;
                this.match(CHeaderParser.EXTERN);
                this.state = 79;
                this.type();
                this.state = 81;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.ATTRIBUTE) {
                    {
                        this.state = 80;
                        this.attribute();
                    }
                }
                this.state = 83;
                this.match(CHeaderParser.ID);
                this.state = 84;
                this.signature();
                this.state = 85;
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
                this.state = 88;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.LONG_DOC || _la === CHeaderParser.SHORT_DOC) {
                    {
                        this.state = 87;
                        this.doc();
                    }
                }
                this.state = 90;
                this.inline();
                this.state = 91;
                this.type();
                this.state = 93;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.ATTRIBUTE) {
                    {
                        this.state = 92;
                        this.attribute();
                    }
                }
                this.state = 95;
                this.match(CHeaderParser.ID);
                this.state = 96;
                this.signature();
                this.state = 97;
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
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 99;
                this.match(CHeaderParser.TYPEDEF);
                this.state = 100;
                this.type();
                this.state = 101;
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
    inline() {
        let _localctx = new InlineContext(this._ctx, this.state);
        this.enterRule(_localctx, 14, CHeaderParser.RULE_inline);
        try {
            this.state = 106;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case CHeaderParser.SDL_INLINE:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 103;
                        this.match(CHeaderParser.SDL_INLINE);
                    }
                    break;
                case CHeaderParser.STATIC:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 104;
                        this.match(CHeaderParser.STATIC);
                        this.state = 105;
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
        this.enterRule(_localctx, 16, CHeaderParser.RULE_block);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 108;
                this.match(CHeaderParser.CURLY_B);
                this.state = 112;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA) | (1 << CHeaderParser.DOT) | (1 << CHeaderParser.STAR) | (1 << CHeaderParser.PUNCT_EXTRA) | (1 << CHeaderParser.STRING) | (1 << CHeaderParser.ID) | (1 << CHeaderParser.NUMBER))) !== 0)) {
                    {
                        {
                            this.state = 109;
                            this.stm();
                        }
                    }
                    this.state = 114;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 115;
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
        this.enterRule(_localctx, 18, CHeaderParser.RULE_group);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 117;
                this.match(CHeaderParser.ROUND_B);
                this.state = 121;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA) | (1 << CHeaderParser.DOT) | (1 << CHeaderParser.STAR) | (1 << CHeaderParser.PUNCT_EXTRA) | (1 << CHeaderParser.STRING) | (1 << CHeaderParser.ID) | (1 << CHeaderParser.NUMBER))) !== 0)) {
                    {
                        {
                            this.state = 118;
                            this.stm();
                        }
                    }
                    this.state = 123;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 124;
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
        this.enterRule(_localctx, 20, CHeaderParser.RULE_indexing);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 126;
                this.match(CHeaderParser.SQUARE_B);
                this.state = 130;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.CURLY_B) | (1 << CHeaderParser.ROUND_B) | (1 << CHeaderParser.SQUARE_B) | (1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA) | (1 << CHeaderParser.DOT) | (1 << CHeaderParser.STAR) | (1 << CHeaderParser.PUNCT_EXTRA) | (1 << CHeaderParser.STRING) | (1 << CHeaderParser.ID) | (1 << CHeaderParser.NUMBER))) !== 0)) {
                    {
                        {
                            this.state = 127;
                            this.stm();
                        }
                    }
                    this.state = 132;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 133;
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
        this.enterRule(_localctx, 22, CHeaderParser.RULE_stm);
        try {
            this.state = 140;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case CHeaderParser.CURLY_B:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 135;
                        this.block();
                    }
                    break;
                case CHeaderParser.ROUND_B:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 136;
                        this.group();
                    }
                    break;
                case CHeaderParser.SQUARE_B:
                    this.enterOuterAlt(_localctx, 3);
                    {
                        this.state = 137;
                        this.indexing();
                    }
                    break;
                case CHeaderParser.DIRECTIVE:
                case CHeaderParser.STATIC:
                case CHeaderParser.VOID:
                case CHeaderParser.STRING:
                case CHeaderParser.ID:
                case CHeaderParser.NUMBER:
                    this.enterOuterAlt(_localctx, 4);
                    {
                        this.state = 138;
                        this.word();
                    }
                    break;
                case CHeaderParser.COLON:
                case CHeaderParser.SEMI:
                case CHeaderParser.COMMA:
                case CHeaderParser.DOT:
                case CHeaderParser.STAR:
                case CHeaderParser.PUNCT_EXTRA:
                    this.enterOuterAlt(_localctx, 5);
                    {
                        this.state = 139;
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
        this.enterRule(_localctx, 24, CHeaderParser.RULE_word);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 142;
                _la = this._input.LA(1);
                if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.DIRECTIVE) | (1 << CHeaderParser.STATIC) | (1 << CHeaderParser.VOID) | (1 << CHeaderParser.STRING) | (1 << CHeaderParser.ID) | (1 << CHeaderParser.NUMBER))) !== 0))) {
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
        this.enterRule(_localctx, 26, CHeaderParser.RULE_punct);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 144;
                _la = this._input.LA(1);
                if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << CHeaderParser.COLON) | (1 << CHeaderParser.SEMI) | (1 << CHeaderParser.COMMA) | (1 << CHeaderParser.DOT) | (1 << CHeaderParser.STAR) | (1 << CHeaderParser.PUNCT_EXTRA))) !== 0))) {
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
    id() {
        let _localctx = new IdContext(this._ctx, this.state);
        this.enterRule(_localctx, 28, CHeaderParser.RULE_id);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 146;
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
        this.enterRule(_localctx, 30, CHeaderParser.RULE_type);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 149;
                this._errHandler.sync(this);
                _alt = 1;
                do {
                    switch (_alt) {
                        case 1:
                            {
                                {
                                    this.state = 148;
                                    this.typeEl();
                                }
                            }
                            break;
                        default:
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 151;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 14, this._ctx);
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
        this.enterRule(_localctx, 32, CHeaderParser.RULE_typeEl);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 153;
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
                this.state = 157;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === CHeaderParser.STAR) {
                    {
                        {
                            this.state = 154;
                            this.match(CHeaderParser.STAR);
                        }
                    }
                    this.state = 159;
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
        this.enterRule(_localctx, 34, CHeaderParser.RULE_signature);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 160;
                this.match(CHeaderParser.ROUND_B);
                this.state = 169;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                if (_la === CHeaderParser.VOID || _la === CHeaderParser.ID) {
                    {
                        this.state = 161;
                        this.type();
                        this.state = 166;
                        this._errHandler.sync(this);
                        _la = this._input.LA(1);
                        while (_la === CHeaderParser.COMMA) {
                            {
                                {
                                    this.state = 162;
                                    this.match(CHeaderParser.COMMA);
                                    this.state = 163;
                                    this.type();
                                }
                            }
                            this.state = 168;
                            this._errHandler.sync(this);
                            _la = this._input.LA(1);
                        }
                    }
                }
                this.state = 171;
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
        this.enterRule(_localctx, 36, CHeaderParser.RULE_attribute);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 173;
                this.match(CHeaderParser.ATTRIBUTE);
                this.state = 174;
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
        this.enterRule(_localctx, 38, CHeaderParser.RULE_doc);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 176;
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
CHeaderParser.ATTRIBUTE = 7;
CHeaderParser.EXTERN = 8;
CHeaderParser.INLINE = 9;
CHeaderParser.SDL_NOISE = 10;
CHeaderParser.SDL_INLINE = 11;
CHeaderParser.STATIC = 12;
CHeaderParser.TYPEDEF = 13;
CHeaderParser.VOID = 14;
CHeaderParser.CURLY_B = 15;
CHeaderParser.CURLY_E = 16;
CHeaderParser.ROUND_B = 17;
CHeaderParser.ROUND_E = 18;
CHeaderParser.SQUARE_B = 19;
CHeaderParser.SQUARE_E = 20;
CHeaderParser.COLON = 21;
CHeaderParser.SEMI = 22;
CHeaderParser.COMMA = 23;
CHeaderParser.DOT = 24;
CHeaderParser.STAR = 25;
CHeaderParser.PUNCT_EXTRA = 26;
CHeaderParser.STRING = 27;
CHeaderParser.ID = 28;
CHeaderParser.NUMBER = 29;
CHeaderParser.RULE_prog = 0;
CHeaderParser.RULE_decl = 1;
CHeaderParser.RULE_externC = 2;
CHeaderParser.RULE_directive = 3;
CHeaderParser.RULE_functionDecl = 4;
CHeaderParser.RULE_functionDef = 5;
CHeaderParser.RULE_aliasDef = 6;
CHeaderParser.RULE_inline = 7;
CHeaderParser.RULE_block = 8;
CHeaderParser.RULE_group = 9;
CHeaderParser.RULE_indexing = 10;
CHeaderParser.RULE_stm = 11;
CHeaderParser.RULE_word = 12;
CHeaderParser.RULE_punct = 13;
CHeaderParser.RULE_id = 14;
CHeaderParser.RULE_type = 15;
CHeaderParser.RULE_typeEl = 16;
CHeaderParser.RULE_signature = 17;
CHeaderParser.RULE_attribute = 18;
CHeaderParser.RULE_doc = 19;
// tslint:disable:no-trailing-whitespace
CHeaderParser.ruleNames = [
    "prog", "decl", "externC", "directive", "functionDecl", "functionDef",
    "aliasDef", "inline", "block", "group", "indexing", "stm", "word", "punct",
    "id", "type", "typeEl", "signature", "attribute", "doc",
];
CHeaderParser._LITERAL_NAMES = [
    undefined, undefined, undefined, undefined, undefined, undefined, undefined,
    "'__attribute__'", "'extern'", "'__inline__'", undefined, "'SDL_FORCE_INLINE'",
    "'static'", "'typedef'", "'void'", "'{'", "'}'", "'('", "')'", "'['",
    "']'", "':'", "';'", "','", "'.'", "'*'",
];
CHeaderParser._SYMBOLIC_NAMES = [
    undefined, "WS", "LONG_COMMENT", "SHORT_COMMENT", "LONG_DOC", "SHORT_DOC",
    "DIRECTIVE", "ATTRIBUTE", "EXTERN", "INLINE", "SDL_NOISE", "SDL_INLINE",
    "STATIC", "TYPEDEF", "VOID", "CURLY_B", "CURLY_E", "ROUND_B", "ROUND_E",
    "SQUARE_B", "SQUARE_E", "COLON", "SEMI", "COMMA", "DOT", "STAR", "PUNCT_EXTRA",
    "STRING", "ID", "NUMBER",
];
CHeaderParser.VOCABULARY = new VocabularyImpl_1.VocabularyImpl(CHeaderParser._LITERAL_NAMES, CHeaderParser._SYMBOLIC_NAMES, []);
CHeaderParser._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03\x1F\xB5\x04\x02" +
    "\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
    "\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r\t\r\x04" +
    "\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x04\x11\t\x11\x04\x12\t\x12\x04" +
    "\x13\t\x13\x04\x14\t\x14\x04\x15\t\x15\x03\x02\x05\x02,\n\x02\x03\x02" +
    "\x07\x02/\n\x02\f\x02\x0E\x022\v\x02\x03\x02\x03\x02\x03\x03\x03\x03\x03" +
    "\x03\x03\x03\x03\x03\x03\x03\x05\x03<\n\x03\x03\x04\x03\x04\x03\x04\x03" +
    "\x04\x07\x04B\n\x04\f\x04\x0E\x04E\v\x04\x03\x04\x03\x04\x03\x05\x05\x05" +
    "J\n\x05\x03\x05\x03\x05\x03\x06\x05\x06O\n\x06\x03\x06\x03\x06\x03\x06" +
    "\x05\x06T\n\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x07\x05\x07[\n\x07" +
    "\x03\x07\x03\x07\x03\x07\x05\x07`\n\x07\x03\x07\x03\x07\x03\x07\x03\x07" +
    "\x03\b\x03\b\x03\b\x03\b\x03\t\x03\t\x03\t\x05\tm\n\t\x03\n\x03\n\x07" +
    "\nq\n\n\f\n\x0E\nt\v\n\x03\n\x03\n\x03\v\x03\v\x07\vz\n\v\f\v\x0E\v}\v" +
    "\v\x03\v\x03\v\x03\f\x03\f\x07\f\x83\n\f\f\f\x0E\f\x86\v\f\x03\f\x03\f" +
    "\x03\r\x03\r\x03\r\x03\r\x03\r\x05\r\x8F\n\r\x03\x0E\x03\x0E\x03\x0F\x03" +
    "\x0F\x03\x10\x03\x10\x03\x11\x06\x11\x98\n\x11\r\x11\x0E\x11\x99\x03\x12" +
    "\x03\x12\x07\x12\x9E\n\x12\f\x12\x0E\x12\xA1\v\x12\x03\x13\x03\x13\x03" +
    "\x13\x03\x13\x07\x13\xA7\n\x13\f\x13\x0E\x13\xAA\v\x13\x05\x13\xAC\n\x13" +
    "\x03\x13\x03\x13\x03\x14\x03\x14\x03\x14\x03\x15\x03\x15\x03\x15\x02\x02" +
    "\x02\x16\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E\x02\x10\x02\x12" +
    "\x02\x14\x02\x16\x02\x18\x02\x1A\x02\x1C\x02\x1E\x02 \x02\"\x02$\x02&" +
    "\x02(\x02\x02\x06\x06\x02\b\b\x0E\x0E\x10\x10\x1D\x1F\x03\x02\x17\x1C" +
    "\x04\x02\x10\x10\x1E\x1E\x03\x02\x06\x07\x02\xB9\x02+\x03\x02\x02\x02" +
    "\x04;\x03\x02\x02\x02\x06=\x03\x02\x02\x02\bI\x03\x02\x02\x02\nN\x03\x02" +
    "\x02\x02\fZ\x03\x02\x02\x02\x0Ee\x03\x02\x02\x02\x10l\x03\x02\x02\x02" +
    "\x12n\x03\x02\x02\x02\x14w\x03\x02\x02\x02\x16\x80\x03\x02\x02\x02\x18" +
    "\x8E\x03\x02\x02\x02\x1A\x90\x03\x02\x02\x02\x1C\x92\x03\x02\x02\x02\x1E" +
    "\x94\x03\x02\x02\x02 \x97\x03\x02\x02\x02\"\x9B\x03\x02\x02\x02$\xA2\x03" +
    "\x02\x02\x02&\xAF\x03\x02\x02\x02(\xB2\x03\x02\x02\x02*,\x05(\x15\x02" +
    "+*\x03\x02\x02\x02+,\x03\x02\x02\x02,0\x03\x02\x02\x02-/\x05\x04\x03\x02" +
    ".-\x03\x02\x02\x02/2\x03\x02\x02\x020.\x03\x02\x02\x0201\x03\x02\x02\x02" +
    "13\x03\x02\x02\x0220\x03\x02\x02\x0234\x07\x02\x02\x034\x03\x03\x02\x02" +
    "\x025<\x05\b\x05\x026<\x05\x06\x04\x027<\x05\n\x06\x028<\x05\f\x07\x02" +
    "9<\x05\x0E\b\x02:<\x05(\x15\x02;5\x03\x02\x02\x02;6\x03\x02\x02\x02;7" +
    "\x03\x02\x02\x02;8\x03\x02\x02\x02;9\x03\x02\x02\x02;:\x03\x02\x02\x02" +
    "<\x05\x03\x02\x02\x02=>\x07\n\x02\x02>?\x07\x1D\x02\x02?C\x07\x11\x02" +
    "\x02@B\x05\x04\x03\x02A@\x03\x02\x02\x02BE\x03\x02\x02\x02CA\x03\x02\x02" +
    "\x02CD\x03\x02\x02\x02DF\x03\x02\x02\x02EC\x03\x02\x02\x02FG\x07\x12\x02" +
    "\x02G\x07\x03\x02\x02\x02HJ\x05(\x15\x02IH\x03\x02\x02\x02IJ\x03\x02\x02" +
    "\x02JK\x03\x02\x02\x02KL\x07\b\x02\x02L\t\x03\x02\x02\x02MO\x05(\x15\x02" +
    "NM\x03\x02\x02\x02NO\x03\x02\x02\x02OP\x03\x02\x02\x02PQ\x07\n\x02\x02" +
    "QS\x05 \x11\x02RT\x05&\x14\x02SR\x03\x02\x02\x02ST\x03\x02\x02\x02TU\x03" +
    "\x02\x02\x02UV\x07\x1E\x02\x02VW\x05$\x13\x02WX\x07\x18\x02\x02X\v\x03" +
    "\x02\x02\x02Y[\x05(\x15\x02ZY\x03\x02\x02\x02Z[\x03\x02\x02\x02[\\\x03" +
    "\x02\x02\x02\\]\x05\x10\t\x02]_\x05 \x11\x02^`\x05&\x14\x02_^\x03\x02" +
    "\x02\x02_`\x03\x02\x02\x02`a\x03\x02\x02\x02ab\x07\x1E\x02\x02bc\x05$" +
    "\x13\x02cd\x05\x12\n\x02d\r\x03\x02\x02\x02ef\x07\x0F\x02\x02fg\x05 \x11" +
    "\x02gh\x07\x1E\x02\x02h\x0F\x03\x02\x02\x02im\x07\r\x02\x02jk\x07\x0E" +
    "\x02\x02km\x07\v\x02\x02li\x03\x02\x02\x02lj\x03\x02\x02\x02m\x11\x03" +
    "\x02\x02\x02nr\x07\x11\x02\x02oq\x05\x18\r\x02po\x03\x02\x02\x02qt\x03" +
    "\x02\x02\x02rp\x03\x02\x02\x02rs\x03\x02\x02\x02su\x03\x02\x02\x02tr\x03" +
    "\x02\x02\x02uv\x07\x12\x02\x02v\x13\x03\x02\x02\x02w{\x07\x13\x02\x02" +
    "xz\x05\x18\r\x02yx\x03\x02\x02\x02z}\x03\x02\x02\x02{y\x03\x02\x02\x02" +
    "{|\x03\x02\x02\x02|~\x03\x02\x02\x02}{\x03\x02\x02\x02~\x7F\x07\x14\x02" +
    "\x02\x7F\x15\x03\x02\x02\x02\x80\x84\x07\x15\x02\x02\x81\x83\x05\x18\r" +
    "\x02\x82\x81\x03\x02\x02\x02\x83\x86\x03\x02\x02\x02\x84\x82\x03\x02\x02" +
    "\x02\x84\x85\x03\x02\x02\x02\x85\x87\x03\x02\x02\x02\x86\x84\x03\x02\x02" +
    "\x02\x87\x88\x07\x16\x02\x02\x88\x17\x03\x02\x02\x02\x89\x8F\x05\x12\n" +
    "\x02\x8A\x8F\x05\x14\v\x02\x8B\x8F\x05\x16\f\x02\x8C\x8F\x05\x1A\x0E\x02" +
    "\x8D\x8F\x05\x1C\x0F\x02\x8E\x89\x03\x02\x02\x02\x8E\x8A\x03\x02\x02\x02" +
    "\x8E\x8B\x03\x02\x02\x02\x8E\x8C\x03\x02\x02\x02\x8E\x8D\x03\x02\x02\x02" +
    "\x8F\x19\x03\x02\x02\x02\x90\x91\t\x02\x02\x02\x91\x1B\x03\x02\x02\x02" +
    "\x92\x93\t\x03\x02\x02\x93\x1D\x03\x02\x02\x02\x94\x95\x07\x1E\x02\x02" +
    "\x95\x1F\x03\x02\x02\x02\x96\x98\x05\"\x12\x02\x97\x96\x03\x02\x02\x02" +
    "\x98\x99\x03\x02\x02\x02\x99\x97\x03\x02\x02\x02\x99\x9A\x03\x02\x02\x02" +
    "\x9A!\x03\x02\x02\x02\x9B\x9F\t\x04\x02\x02\x9C\x9E\x07\x1B\x02\x02\x9D" +
    "\x9C\x03\x02\x02\x02\x9E\xA1\x03\x02\x02\x02\x9F\x9D\x03\x02\x02\x02\x9F" +
    "\xA0\x03\x02\x02\x02\xA0#\x03\x02\x02\x02\xA1\x9F\x03\x02\x02\x02\xA2" +
    "\xAB\x07\x13\x02\x02\xA3\xA8\x05 \x11\x02\xA4\xA5\x07\x19\x02\x02\xA5" +
    "\xA7\x05 \x11\x02\xA6\xA4\x03\x02\x02\x02\xA7\xAA\x03\x02\x02\x02\xA8" +
    "\xA6\x03\x02\x02\x02\xA8\xA9\x03\x02\x02\x02\xA9\xAC\x03\x02\x02\x02\xAA" +
    "\xA8\x03\x02\x02\x02\xAB\xA3\x03\x02\x02\x02\xAB\xAC\x03\x02\x02\x02\xAC" +
    "\xAD\x03\x02\x02\x02\xAD\xAE\x07\x14\x02\x02\xAE%\x03\x02\x02\x02\xAF" +
    "\xB0\x07\t\x02\x02\xB0\xB1\x05\x14\v\x02\xB1\'\x03\x02\x02\x02\xB2\xB3" +
    "\t\x05\x02\x02\xB3)\x03\x02\x02\x02\x14+0;CINSZ_lr{\x84\x8E\x99\x9F\xA8" +
    "\xAB";
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
    ID() { return this.getToken(CHeaderParser.ID, 0); }
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
    ID() { return this.getToken(CHeaderParser.ID, 0); }
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
    ID() { return this.getToken(CHeaderParser.ID, 0); }
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
