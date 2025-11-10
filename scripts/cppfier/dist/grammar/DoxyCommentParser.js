"use strict";
// Generated from src/grammar/DoxyComment.g4 by ANTLR 4.9.0-SNAPSHOT
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
exports.EndLineContext = exports.BlockTagContext = exports.TableItemContext = exports.ListItemContentContext = exports.ListItemContext = exports.TextLineContext = exports.EmptyContext = exports.TaggedBlockContext = exports.TaggedSectionContext = exports.CodeblockContext = exports.TableContext = exports.ListContext = exports.ParagraphContext = exports.TitleContext = exports.BlockContext = exports.DocContext = exports.DoxyCommentParser = void 0;
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
class DoxyCommentParser extends Parser_1.Parser {
    // @Override
    // @NotNull
    get vocabulary() {
        return DoxyCommentParser.VOCABULARY;
    }
    // tslint:enable:no-trailing-whitespace
    // @Override
    get grammarFileName() { return "DoxyComment.g4"; }
    // @Override
    get ruleNames() { return DoxyCommentParser.ruleNames; }
    // @Override
    get serializedATN() { return DoxyCommentParser._serializedATN; }
    createFailedPredicateException(predicate, message) {
        return new FailedPredicateException_1.FailedPredicateException(this, predicate, message);
    }
    constructor(input) {
        super(input);
        this._interp = new ParserATNSimulator_1.ParserATNSimulator(DoxyCommentParser._ATN, this);
    }
    // @RuleVersion(0)
    doc() {
        let _localctx = new DocContext(this._ctx, this.state);
        this.enterRule(_localctx, 0, DoxyCommentParser.RULE_doc);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 35;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << DoxyCommentParser.CODE_DELIM) | (1 << DoxyCommentParser.TABLE_DELIM) | (1 << DoxyCommentParser.LIST_DELIM) | (1 << DoxyCommentParser.TITLE_DELIM) | (1 << DoxyCommentParser.DEFGROUP_TAG) | (1 << DoxyCommentParser.RETURNS_TAG) | (1 << DoxyCommentParser.PARAM_TAG) | (1 << DoxyCommentParser.THREAD_SAFETY_TAG) | (1 << DoxyCommentParser.SA_TAG) | (1 << DoxyCommentParser.SINCE_TAG) | (1 << DoxyCommentParser.POST_TAG) | (1 << DoxyCommentParser.THROWS_TAG) | (1 << DoxyCommentParser.WORD) | (1 << DoxyCommentParser.WS) | (1 << DoxyCommentParser.NL))) !== 0)) {
                    {
                        {
                            this.state = 32;
                            this.block();
                        }
                    }
                    this.state = 37;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 38;
                this.match(DoxyCommentParser.EOF);
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
        this.enterRule(_localctx, 2, DoxyCommentParser.RULE_block);
        try {
            this.state = 47;
            this._errHandler.sync(this);
            switch (this.interpreter.adaptivePredict(this._input, 1, this._ctx)) {
                case 1:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 40;
                        this.title();
                    }
                    break;
                case 2:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 41;
                        this.paragraph();
                    }
                    break;
                case 3:
                    this.enterOuterAlt(_localctx, 3);
                    {
                        this.state = 42;
                        this.list();
                    }
                    break;
                case 4:
                    this.enterOuterAlt(_localctx, 4);
                    {
                        this.state = 43;
                        this.table();
                    }
                    break;
                case 5:
                    this.enterOuterAlt(_localctx, 5);
                    {
                        this.state = 44;
                        this.codeblock();
                    }
                    break;
                case 6:
                    this.enterOuterAlt(_localctx, 6);
                    {
                        this.state = 45;
                        this.taggedSection();
                    }
                    break;
                case 7:
                    this.enterOuterAlt(_localctx, 7);
                    {
                        this.state = 46;
                        this.empty();
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
    title() {
        let _localctx = new TitleContext(this._ctx, this.state);
        this.enterRule(_localctx, 4, DoxyCommentParser.RULE_title);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 49;
                this.match(DoxyCommentParser.TITLE_DELIM);
                this.state = 50;
                this.textLine();
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
    paragraph() {
        let _localctx = new ParagraphContext(this._ctx, this.state);
        this.enterRule(_localctx, 6, DoxyCommentParser.RULE_paragraph);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 53;
                this._errHandler.sync(this);
                _alt = 1;
                do {
                    switch (_alt) {
                        case 1:
                            {
                                {
                                    this.state = 52;
                                    this.textLine();
                                }
                            }
                            break;
                        default:
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 55;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 2, this._ctx);
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
    list() {
        let _localctx = new ListContext(this._ctx, this.state);
        this.enterRule(_localctx, 8, DoxyCommentParser.RULE_list);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 58;
                this._errHandler.sync(this);
                _alt = 1;
                do {
                    switch (_alt) {
                        case 1:
                            {
                                {
                                    this.state = 57;
                                    this.listItem();
                                }
                            }
                            break;
                        default:
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 60;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 3, this._ctx);
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
    table() {
        let _localctx = new TableContext(this._ctx, this.state);
        this.enterRule(_localctx, 10, DoxyCommentParser.RULE_table);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 63;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                do {
                    {
                        {
                            this.state = 62;
                            this.tableItem();
                        }
                    }
                    this.state = 65;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                } while (_la === DoxyCommentParser.TABLE_DELIM);
                this.state = 67;
                this.endLine();
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
    codeblock() {
        let _localctx = new CodeblockContext(this._ctx, this.state);
        this.enterRule(_localctx, 12, DoxyCommentParser.RULE_codeblock);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 69;
                this.match(DoxyCommentParser.CODE_DELIM);
                this.state = 73;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << DoxyCommentParser.TABLE_DELIM) | (1 << DoxyCommentParser.LIST_DELIM) | (1 << DoxyCommentParser.TITLE_DELIM) | (1 << DoxyCommentParser.DEFGROUP_TAG) | (1 << DoxyCommentParser.RETURNS_TAG) | (1 << DoxyCommentParser.PARAM_TAG) | (1 << DoxyCommentParser.THREAD_SAFETY_TAG) | (1 << DoxyCommentParser.SA_TAG) | (1 << DoxyCommentParser.SINCE_TAG) | (1 << DoxyCommentParser.POST_TAG) | (1 << DoxyCommentParser.THROWS_TAG) | (1 << DoxyCommentParser.WORD) | (1 << DoxyCommentParser.WS) | (1 << DoxyCommentParser.NL))) !== 0)) {
                    {
                        {
                            this.state = 70;
                            _la = this._input.LA(1);
                            if (_la <= 0 || (_la === DoxyCommentParser.CODE_DELIM)) {
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
                    this.state = 75;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 76;
                this.match(DoxyCommentParser.CODE_DELIM);
                this.state = 77;
                this.endLine();
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
    taggedSection() {
        let _localctx = new TaggedSectionContext(this._ctx, this.state);
        this.enterRule(_localctx, 14, DoxyCommentParser.RULE_taggedSection);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 80;
                this._errHandler.sync(this);
                _alt = 1;
                do {
                    switch (_alt) {
                        case 1:
                            {
                                {
                                    this.state = 79;
                                    this.taggedBlock();
                                }
                            }
                            break;
                        default:
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 82;
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
    taggedBlock() {
        let _localctx = new TaggedBlockContext(this._ctx, this.state);
        this.enterRule(_localctx, 16, DoxyCommentParser.RULE_taggedBlock);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 84;
                this.blockTag();
                this.state = 86;
                this._errHandler.sync(this);
                _alt = 1;
                do {
                    switch (_alt) {
                        case 1:
                            {
                                {
                                    this.state = 85;
                                    this.textLine();
                                }
                            }
                            break;
                        default:
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 88;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 7, this._ctx);
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
    empty() {
        let _localctx = new EmptyContext(this._ctx, this.state);
        this.enterRule(_localctx, 18, DoxyCommentParser.RULE_empty);
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 90;
                this.match(DoxyCommentParser.NL);
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
    textLine() {
        let _localctx = new TextLineContext(this._ctx, this.state);
        this.enterRule(_localctx, 20, DoxyCommentParser.RULE_textLine);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 92;
                _la = this._input.LA(1);
                if (!(_la === DoxyCommentParser.WORD || _la === DoxyCommentParser.WS)) {
                    this._errHandler.recoverInline(this);
                }
                else {
                    if (this._input.LA(1) === Token_1.Token.EOF) {
                        this.matchedEOF = true;
                    }
                    this._errHandler.reportMatch(this);
                    this.consume();
                }
                this.state = 96;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << DoxyCommentParser.CODE_DELIM) | (1 << DoxyCommentParser.TABLE_DELIM) | (1 << DoxyCommentParser.LIST_DELIM) | (1 << DoxyCommentParser.TITLE_DELIM) | (1 << DoxyCommentParser.DEFGROUP_TAG) | (1 << DoxyCommentParser.RETURNS_TAG) | (1 << DoxyCommentParser.PARAM_TAG) | (1 << DoxyCommentParser.THREAD_SAFETY_TAG) | (1 << DoxyCommentParser.SA_TAG) | (1 << DoxyCommentParser.SINCE_TAG) | (1 << DoxyCommentParser.POST_TAG) | (1 << DoxyCommentParser.THROWS_TAG) | (1 << DoxyCommentParser.WORD) | (1 << DoxyCommentParser.WS))) !== 0)) {
                    {
                        {
                            this.state = 93;
                            _la = this._input.LA(1);
                            if (_la <= 0 || (_la === DoxyCommentParser.NL)) {
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
                    this.state = 98;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 99;
                this.endLine();
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
    listItem() {
        let _localctx = new ListItemContext(this._ctx, this.state);
        this.enterRule(_localctx, 22, DoxyCommentParser.RULE_listItem);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 104;
                this._errHandler.sync(this);
                _la = this._input.LA(1);
                while (_la === DoxyCommentParser.WS) {
                    {
                        {
                            this.state = 101;
                            this.match(DoxyCommentParser.WS);
                        }
                    }
                    this.state = 106;
                    this._errHandler.sync(this);
                    _la = this._input.LA(1);
                }
                this.state = 107;
                this.match(DoxyCommentParser.LIST_DELIM);
                this.state = 108;
                this.listItemContent();
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
    listItemContent() {
        let _localctx = new ListItemContentContext(this._ctx, this.state);
        this.enterRule(_localctx, 24, DoxyCommentParser.RULE_listItemContent);
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 111;
                this._errHandler.sync(this);
                _alt = 1;
                do {
                    switch (_alt) {
                        case 1:
                            {
                                {
                                    this.state = 110;
                                    this.textLine();
                                }
                            }
                            break;
                        default:
                            throw new NoViableAltException_1.NoViableAltException(this);
                    }
                    this.state = 113;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 10, this._ctx);
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
    tableItem() {
        let _localctx = new TableItemContext(this._ctx, this.state);
        this.enterRule(_localctx, 26, DoxyCommentParser.RULE_tableItem);
        let _la;
        try {
            let _alt;
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 115;
                this.match(DoxyCommentParser.TABLE_DELIM);
                this.state = 121;
                this._errHandler.sync(this);
                _alt = this.interpreter.adaptivePredict(this._input, 12, this._ctx);
                while (_alt !== 2 && _alt !== ATN_1.ATN.INVALID_ALT_NUMBER) {
                    if (_alt === 1) {
                        {
                            this.state = 119;
                            this._errHandler.sync(this);
                            switch (this._input.LA(1)) {
                                case DoxyCommentParser.CODE_DELIM:
                                case DoxyCommentParser.LIST_DELIM:
                                case DoxyCommentParser.TITLE_DELIM:
                                case DoxyCommentParser.DEFGROUP_TAG:
                                case DoxyCommentParser.RETURNS_TAG:
                                case DoxyCommentParser.PARAM_TAG:
                                case DoxyCommentParser.THREAD_SAFETY_TAG:
                                case DoxyCommentParser.SA_TAG:
                                case DoxyCommentParser.SINCE_TAG:
                                case DoxyCommentParser.POST_TAG:
                                case DoxyCommentParser.THROWS_TAG:
                                case DoxyCommentParser.WORD:
                                case DoxyCommentParser.WS:
                                case DoxyCommentParser.NL:
                                    {
                                        this.state = 116;
                                        _la = this._input.LA(1);
                                        if (_la <= 0 || (_la === DoxyCommentParser.TABLE_DELIM)) {
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
                                    break;
                                case DoxyCommentParser.TABLE_DELIM:
                                    {
                                        this.state = 117;
                                        this.match(DoxyCommentParser.TABLE_DELIM);
                                        this.state = 118;
                                        _la = this._input.LA(1);
                                        if (_la <= 0 || (_la === DoxyCommentParser.NL)) {
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
                                    break;
                                default:
                                    throw new NoViableAltException_1.NoViableAltException(this);
                            }
                        }
                    }
                    this.state = 123;
                    this._errHandler.sync(this);
                    _alt = this.interpreter.adaptivePredict(this._input, 12, this._ctx);
                }
                this.state = 124;
                this.match(DoxyCommentParser.TABLE_DELIM);
                this.state = 125;
                this.endLine();
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
    blockTag() {
        let _localctx = new BlockTagContext(this._ctx, this.state);
        this.enterRule(_localctx, 28, DoxyCommentParser.RULE_blockTag);
        let _la;
        try {
            this.state = 147;
            this._errHandler.sync(this);
            switch (this._input.LA(1)) {
                case DoxyCommentParser.PARAM_TAG:
                    this.enterOuterAlt(_localctx, 1);
                    {
                        this.state = 127;
                        this.match(DoxyCommentParser.PARAM_TAG);
                        this.state = 129;
                        this._errHandler.sync(this);
                        _la = this._input.LA(1);
                        do {
                            {
                                {
                                    this.state = 128;
                                    this.match(DoxyCommentParser.WS);
                                }
                            }
                            this.state = 131;
                            this._errHandler.sync(this);
                            _la = this._input.LA(1);
                        } while (_la === DoxyCommentParser.WS);
                        this.state = 133;
                        this.match(DoxyCommentParser.WORD);
                    }
                    break;
                case DoxyCommentParser.DEFGROUP_TAG:
                    this.enterOuterAlt(_localctx, 2);
                    {
                        this.state = 134;
                        this.match(DoxyCommentParser.DEFGROUP_TAG);
                        this.state = 136;
                        this._errHandler.sync(this);
                        _la = this._input.LA(1);
                        do {
                            {
                                {
                                    this.state = 135;
                                    this.match(DoxyCommentParser.WS);
                                }
                            }
                            this.state = 138;
                            this._errHandler.sync(this);
                            _la = this._input.LA(1);
                        } while (_la === DoxyCommentParser.WS);
                        this.state = 140;
                        this.match(DoxyCommentParser.WORD);
                    }
                    break;
                case DoxyCommentParser.RETURNS_TAG:
                    this.enterOuterAlt(_localctx, 3);
                    {
                        this.state = 141;
                        this.match(DoxyCommentParser.RETURNS_TAG);
                    }
                    break;
                case DoxyCommentParser.THREAD_SAFETY_TAG:
                    this.enterOuterAlt(_localctx, 4);
                    {
                        this.state = 142;
                        this.match(DoxyCommentParser.THREAD_SAFETY_TAG);
                    }
                    break;
                case DoxyCommentParser.SA_TAG:
                    this.enterOuterAlt(_localctx, 5);
                    {
                        this.state = 143;
                        this.match(DoxyCommentParser.SA_TAG);
                    }
                    break;
                case DoxyCommentParser.SINCE_TAG:
                    this.enterOuterAlt(_localctx, 6);
                    {
                        this.state = 144;
                        this.match(DoxyCommentParser.SINCE_TAG);
                    }
                    break;
                case DoxyCommentParser.POST_TAG:
                    this.enterOuterAlt(_localctx, 7);
                    {
                        this.state = 145;
                        this.match(DoxyCommentParser.POST_TAG);
                    }
                    break;
                case DoxyCommentParser.THROWS_TAG:
                    this.enterOuterAlt(_localctx, 8);
                    {
                        this.state = 146;
                        this.match(DoxyCommentParser.THROWS_TAG);
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
    endLine() {
        let _localctx = new EndLineContext(this._ctx, this.state);
        this.enterRule(_localctx, 30, DoxyCommentParser.RULE_endLine);
        let _la;
        try {
            this.enterOuterAlt(_localctx, 1);
            {
                this.state = 149;
                _la = this._input.LA(1);
                if (!(_la === DoxyCommentParser.EOF || _la === DoxyCommentParser.NL)) {
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
        if (!DoxyCommentParser.__ATN) {
            DoxyCommentParser.__ATN = new ATNDeserializer_1.ATNDeserializer().deserialize(Utils.toCharArray(DoxyCommentParser._serializedATN));
        }
        return DoxyCommentParser.__ATN;
    }
}
exports.DoxyCommentParser = DoxyCommentParser;
DoxyCommentParser.CODE_DELIM = 1;
DoxyCommentParser.TABLE_DELIM = 2;
DoxyCommentParser.LIST_DELIM = 3;
DoxyCommentParser.TITLE_DELIM = 4;
DoxyCommentParser.DEFGROUP_TAG = 5;
DoxyCommentParser.RETURNS_TAG = 6;
DoxyCommentParser.PARAM_TAG = 7;
DoxyCommentParser.THREAD_SAFETY_TAG = 8;
DoxyCommentParser.SA_TAG = 9;
DoxyCommentParser.SINCE_TAG = 10;
DoxyCommentParser.POST_TAG = 11;
DoxyCommentParser.THROWS_TAG = 12;
DoxyCommentParser.WORD = 13;
DoxyCommentParser.WS = 14;
DoxyCommentParser.NL = 15;
DoxyCommentParser.RULE_doc = 0;
DoxyCommentParser.RULE_block = 1;
DoxyCommentParser.RULE_title = 2;
DoxyCommentParser.RULE_paragraph = 3;
DoxyCommentParser.RULE_list = 4;
DoxyCommentParser.RULE_table = 5;
DoxyCommentParser.RULE_codeblock = 6;
DoxyCommentParser.RULE_taggedSection = 7;
DoxyCommentParser.RULE_taggedBlock = 8;
DoxyCommentParser.RULE_empty = 9;
DoxyCommentParser.RULE_textLine = 10;
DoxyCommentParser.RULE_listItem = 11;
DoxyCommentParser.RULE_listItemContent = 12;
DoxyCommentParser.RULE_tableItem = 13;
DoxyCommentParser.RULE_blockTag = 14;
DoxyCommentParser.RULE_endLine = 15;
// tslint:disable:no-trailing-whitespace
DoxyCommentParser.ruleNames = [
    "doc", "block", "title", "paragraph", "list", "table", "codeblock", "taggedSection",
    "taggedBlock", "empty", "textLine", "listItem", "listItemContent", "tableItem",
    "blockTag", "endLine",
];
DoxyCommentParser._LITERAL_NAMES = [
    undefined, undefined, "'|'", "'-'",
];
DoxyCommentParser._SYMBOLIC_NAMES = [
    undefined, "CODE_DELIM", "TABLE_DELIM", "LIST_DELIM", "TITLE_DELIM", "DEFGROUP_TAG",
    "RETURNS_TAG", "PARAM_TAG", "THREAD_SAFETY_TAG", "SA_TAG", "SINCE_TAG",
    "POST_TAG", "THROWS_TAG", "WORD", "WS", "NL",
];
DoxyCommentParser.VOCABULARY = new VocabularyImpl_1.VocabularyImpl(DoxyCommentParser._LITERAL_NAMES, DoxyCommentParser._SYMBOLIC_NAMES, []);
DoxyCommentParser._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03\x11\x9A\x04\x02" +
    "\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
    "\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r\t\r\x04" +
    "\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x04\x11\t\x11\x03\x02\x07\x02$" +
    "\n\x02\f\x02\x0E\x02\'\v\x02\x03\x02\x03\x02\x03\x03\x03\x03\x03\x03\x03" +
    "\x03\x03\x03\x03\x03\x03\x03\x05\x032\n\x03\x03\x04\x03\x04\x03\x04\x03" +
    "\x05\x06\x058\n\x05\r\x05\x0E\x059\x03\x06\x06\x06=\n\x06\r\x06\x0E\x06" +
    ">\x03\x07\x06\x07B\n\x07\r\x07\x0E\x07C\x03\x07\x03\x07\x03\b\x03\b\x07" +
    "\bJ\n\b\f\b\x0E\bM\v\b\x03\b\x03\b\x03\b\x03\t\x06\tS\n\t\r\t\x0E\tT\x03" +
    "\n\x03\n\x06\nY\n\n\r\n\x0E\nZ\x03\v\x03\v\x03\f\x03\f\x07\fa\n\f\f\f" +
    "\x0E\fd\v\f\x03\f\x03\f\x03\r\x07\ri\n\r\f\r\x0E\rl\v\r\x03\r\x03\r\x03" +
    "\r\x03\x0E\x06\x0Er\n\x0E\r\x0E\x0E\x0Es\x03\x0F\x03\x0F\x03\x0F\x03\x0F" +
    "\x07\x0Fz\n\x0F\f\x0F\x0E\x0F}\v\x0F\x03\x0F\x03\x0F\x03\x0F\x03\x10\x03" +
    "\x10\x06\x10\x84\n\x10\r\x10\x0E\x10\x85\x03\x10\x03\x10\x03\x10\x06\x10" +
    "\x8B\n\x10\r\x10\x0E\x10\x8C\x03\x10\x03\x10\x03\x10\x03\x10\x03\x10\x03" +
    "\x10\x03\x10\x05\x10\x96\n\x10\x03\x11\x03\x11\x03\x11\x02\x02\x02\x12" +
    "\x02\x02\x04\x02\x06\x02\b\x02\n\x02\f\x02\x0E\x02\x10\x02\x12\x02\x14" +
    "\x02\x16\x02\x18\x02\x1A\x02\x1C\x02\x1E\x02 \x02\x02\x07\x03\x02\x03" +
    "\x03\x03\x02\x0F\x10\x03\x02\x11\x11\x03\x02\x04\x04\x03\x03\x11\x11\x02" +
    "\xA4\x02%\x03\x02\x02\x02\x041\x03\x02\x02\x02\x063\x03\x02\x02\x02\b" +
    "7\x03\x02\x02\x02\n<\x03\x02\x02\x02\fA\x03\x02\x02\x02\x0EG\x03\x02\x02" +
    "\x02\x10R\x03\x02\x02\x02\x12V\x03\x02\x02\x02\x14\\\x03\x02\x02\x02\x16" +
    "^\x03\x02\x02\x02\x18j\x03\x02\x02\x02\x1Aq\x03\x02\x02\x02\x1Cu\x03\x02" +
    "\x02\x02\x1E\x95\x03\x02\x02\x02 \x97\x03\x02\x02\x02\"$\x05\x04\x03\x02" +
    "#\"\x03\x02\x02\x02$\'\x03\x02\x02\x02%#\x03\x02\x02\x02%&\x03\x02\x02" +
    "\x02&(\x03\x02\x02\x02\'%\x03\x02\x02\x02()\x07\x02\x02\x03)\x03\x03\x02" +
    "\x02\x02*2\x05\x06\x04\x02+2\x05\b\x05\x02,2\x05\n\x06\x02-2\x05\f\x07" +
    "\x02.2\x05\x0E\b\x02/2\x05\x10\t\x0202\x05\x14\v\x021*\x03\x02\x02\x02" +
    "1+\x03\x02\x02\x021,\x03\x02\x02\x021-\x03\x02\x02\x021.\x03\x02\x02\x02" +
    "1/\x03\x02\x02\x0210\x03\x02\x02\x022\x05\x03\x02\x02\x0234\x07\x06\x02" +
    "\x0245\x05\x16\f\x025\x07\x03\x02\x02\x0268\x05\x16\f\x0276\x03\x02\x02" +
    "\x0289\x03\x02\x02\x0297\x03\x02\x02\x029:\x03\x02\x02\x02:\t\x03\x02" +
    "\x02\x02;=\x05\x18\r\x02<;\x03\x02\x02\x02=>\x03\x02\x02\x02><\x03\x02" +
    "\x02\x02>?\x03\x02\x02\x02?\v\x03\x02\x02\x02@B\x05\x1C\x0F\x02A@\x03" +
    "\x02\x02\x02BC\x03\x02\x02\x02CA\x03\x02\x02\x02CD\x03\x02\x02\x02DE\x03" +
    "\x02\x02\x02EF\x05 \x11\x02F\r\x03\x02\x02\x02GK\x07\x03\x02\x02HJ\n\x02" +
    "\x02\x02IH\x03\x02\x02\x02JM\x03\x02\x02\x02KI\x03\x02\x02\x02KL\x03\x02" +
    "\x02\x02LN\x03\x02\x02\x02MK\x03\x02\x02\x02NO\x07\x03\x02\x02OP\x05 " +
    "\x11\x02P\x0F\x03\x02\x02\x02QS\x05\x12\n\x02RQ\x03\x02\x02\x02ST\x03" +
    "\x02\x02\x02TR\x03\x02\x02\x02TU\x03\x02\x02\x02U\x11\x03\x02\x02\x02" +
    "VX\x05\x1E\x10\x02WY\x05\x16\f\x02XW\x03\x02\x02\x02YZ\x03\x02\x02\x02" +
    "ZX\x03\x02\x02\x02Z[\x03\x02\x02\x02[\x13\x03\x02\x02\x02\\]\x07\x11\x02" +
    "\x02]\x15\x03\x02\x02\x02^b\t\x03\x02\x02_a\n\x04\x02\x02`_\x03\x02\x02" +
    "\x02ad\x03\x02\x02\x02b`\x03\x02\x02\x02bc\x03\x02\x02\x02ce\x03\x02\x02" +
    "\x02db\x03\x02\x02\x02ef\x05 \x11\x02f\x17\x03\x02\x02\x02gi\x07\x10\x02" +
    "\x02hg\x03\x02\x02\x02il\x03\x02\x02\x02jh\x03\x02\x02\x02jk\x03\x02\x02" +
    "\x02km\x03\x02\x02\x02lj\x03\x02\x02\x02mn\x07\x05\x02\x02no\x05\x1A\x0E" +
    "\x02o\x19\x03\x02\x02\x02pr\x05\x16\f\x02qp\x03\x02\x02\x02rs\x03\x02" +
    "\x02\x02sq\x03\x02\x02\x02st\x03\x02\x02\x02t\x1B\x03\x02\x02\x02u{\x07" +
    "\x04\x02\x02vz\n\x05\x02\x02wx\x07\x04\x02\x02xz\n\x04\x02\x02yv\x03\x02" +
    "\x02\x02yw\x03\x02\x02\x02z}\x03\x02\x02\x02{y\x03\x02\x02\x02{|\x03\x02" +
    "\x02\x02|~\x03\x02\x02\x02}{\x03\x02\x02\x02~\x7F\x07\x04\x02\x02\x7F" +
    "\x80\x05 \x11\x02\x80\x1D\x03\x02\x02\x02\x81\x83\x07\t\x02\x02\x82\x84" +
    "\x07\x10\x02\x02\x83\x82\x03\x02\x02\x02\x84\x85\x03\x02\x02\x02\x85\x83" +
    "\x03\x02\x02\x02\x85\x86\x03\x02\x02\x02\x86\x87\x03\x02\x02\x02\x87\x96" +
    "\x07\x0F\x02\x02\x88\x8A\x07\x07\x02\x02\x89\x8B\x07\x10\x02\x02\x8A\x89" +
    "\x03\x02\x02\x02\x8B\x8C\x03\x02\x02\x02\x8C\x8A\x03\x02\x02\x02\x8C\x8D" +
    "\x03\x02\x02\x02\x8D\x8E\x03\x02\x02\x02\x8E\x96\x07\x0F\x02\x02\x8F\x96" +
    "\x07\b\x02\x02\x90\x96\x07\n\x02\x02\x91\x96\x07\v\x02\x02\x92\x96\x07" +
    "\f\x02\x02\x93\x96\x07\r\x02\x02\x94\x96\x07\x0E\x02\x02\x95\x81\x03\x02" +
    "\x02\x02\x95\x88\x03\x02\x02\x02\x95\x8F\x03\x02\x02\x02\x95\x90\x03\x02" +
    "\x02\x02\x95\x91\x03\x02\x02\x02\x95\x92\x03\x02\x02\x02\x95\x93\x03\x02" +
    "\x02\x02\x95\x94\x03\x02\x02\x02\x96\x1F\x03\x02\x02\x02\x97\x98\t\x06" +
    "\x02\x02\x98!\x03\x02\x02\x02\x12%19>CKTZbjsy{\x85\x8C\x95";
class DocContext extends ParserRuleContext_1.ParserRuleContext {
    EOF() { return this.getToken(DoxyCommentParser.EOF, 0); }
    block(i) {
        if (i === undefined) {
            return this.getRuleContexts(BlockContext);
        }
        else {
            return this.getRuleContext(i, BlockContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_doc; }
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
class BlockContext extends ParserRuleContext_1.ParserRuleContext {
    title() {
        return this.tryGetRuleContext(0, TitleContext);
    }
    paragraph() {
        return this.tryGetRuleContext(0, ParagraphContext);
    }
    list() {
        return this.tryGetRuleContext(0, ListContext);
    }
    table() {
        return this.tryGetRuleContext(0, TableContext);
    }
    codeblock() {
        return this.tryGetRuleContext(0, CodeblockContext);
    }
    taggedSection() {
        return this.tryGetRuleContext(0, TaggedSectionContext);
    }
    empty() {
        return this.tryGetRuleContext(0, EmptyContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_block; }
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
class TitleContext extends ParserRuleContext_1.ParserRuleContext {
    TITLE_DELIM() { return this.getToken(DoxyCommentParser.TITLE_DELIM, 0); }
    textLine() {
        return this.getRuleContext(0, TextLineContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_title; }
    // @Override
    enterRule(listener) {
        if (listener.enterTitle) {
            listener.enterTitle(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitTitle) {
            listener.exitTitle(this);
        }
    }
}
exports.TitleContext = TitleContext;
class ParagraphContext extends ParserRuleContext_1.ParserRuleContext {
    textLine(i) {
        if (i === undefined) {
            return this.getRuleContexts(TextLineContext);
        }
        else {
            return this.getRuleContext(i, TextLineContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_paragraph; }
    // @Override
    enterRule(listener) {
        if (listener.enterParagraph) {
            listener.enterParagraph(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitParagraph) {
            listener.exitParagraph(this);
        }
    }
}
exports.ParagraphContext = ParagraphContext;
class ListContext extends ParserRuleContext_1.ParserRuleContext {
    listItem(i) {
        if (i === undefined) {
            return this.getRuleContexts(ListItemContext);
        }
        else {
            return this.getRuleContext(i, ListItemContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_list; }
    // @Override
    enterRule(listener) {
        if (listener.enterList) {
            listener.enterList(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitList) {
            listener.exitList(this);
        }
    }
}
exports.ListContext = ListContext;
class TableContext extends ParserRuleContext_1.ParserRuleContext {
    endLine() {
        return this.getRuleContext(0, EndLineContext);
    }
    tableItem(i) {
        if (i === undefined) {
            return this.getRuleContexts(TableItemContext);
        }
        else {
            return this.getRuleContext(i, TableItemContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_table; }
    // @Override
    enterRule(listener) {
        if (listener.enterTable) {
            listener.enterTable(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitTable) {
            listener.exitTable(this);
        }
    }
}
exports.TableContext = TableContext;
class CodeblockContext extends ParserRuleContext_1.ParserRuleContext {
    CODE_DELIM(i) {
        if (i === undefined) {
            return this.getTokens(DoxyCommentParser.CODE_DELIM);
        }
        else {
            return this.getToken(DoxyCommentParser.CODE_DELIM, i);
        }
    }
    endLine() {
        return this.getRuleContext(0, EndLineContext);
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_codeblock; }
    // @Override
    enterRule(listener) {
        if (listener.enterCodeblock) {
            listener.enterCodeblock(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitCodeblock) {
            listener.exitCodeblock(this);
        }
    }
}
exports.CodeblockContext = CodeblockContext;
class TaggedSectionContext extends ParserRuleContext_1.ParserRuleContext {
    taggedBlock(i) {
        if (i === undefined) {
            return this.getRuleContexts(TaggedBlockContext);
        }
        else {
            return this.getRuleContext(i, TaggedBlockContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_taggedSection; }
    // @Override
    enterRule(listener) {
        if (listener.enterTaggedSection) {
            listener.enterTaggedSection(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitTaggedSection) {
            listener.exitTaggedSection(this);
        }
    }
}
exports.TaggedSectionContext = TaggedSectionContext;
class TaggedBlockContext extends ParserRuleContext_1.ParserRuleContext {
    blockTag() {
        return this.getRuleContext(0, BlockTagContext);
    }
    textLine(i) {
        if (i === undefined) {
            return this.getRuleContexts(TextLineContext);
        }
        else {
            return this.getRuleContext(i, TextLineContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_taggedBlock; }
    // @Override
    enterRule(listener) {
        if (listener.enterTaggedBlock) {
            listener.enterTaggedBlock(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitTaggedBlock) {
            listener.exitTaggedBlock(this);
        }
    }
}
exports.TaggedBlockContext = TaggedBlockContext;
class EmptyContext extends ParserRuleContext_1.ParserRuleContext {
    NL() { return this.getToken(DoxyCommentParser.NL, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_empty; }
    // @Override
    enterRule(listener) {
        if (listener.enterEmpty) {
            listener.enterEmpty(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitEmpty) {
            listener.exitEmpty(this);
        }
    }
}
exports.EmptyContext = EmptyContext;
class TextLineContext extends ParserRuleContext_1.ParserRuleContext {
    endLine() {
        return this.getRuleContext(0, EndLineContext);
    }
    WS() { return this.tryGetToken(DoxyCommentParser.WS, 0); }
    WORD() { return this.tryGetToken(DoxyCommentParser.WORD, 0); }
    NL(i) {
        if (i === undefined) {
            return this.getTokens(DoxyCommentParser.NL);
        }
        else {
            return this.getToken(DoxyCommentParser.NL, i);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_textLine; }
    // @Override
    enterRule(listener) {
        if (listener.enterTextLine) {
            listener.enterTextLine(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitTextLine) {
            listener.exitTextLine(this);
        }
    }
}
exports.TextLineContext = TextLineContext;
class ListItemContext extends ParserRuleContext_1.ParserRuleContext {
    LIST_DELIM() { return this.getToken(DoxyCommentParser.LIST_DELIM, 0); }
    listItemContent() {
        return this.getRuleContext(0, ListItemContentContext);
    }
    WS(i) {
        if (i === undefined) {
            return this.getTokens(DoxyCommentParser.WS);
        }
        else {
            return this.getToken(DoxyCommentParser.WS, i);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_listItem; }
    // @Override
    enterRule(listener) {
        if (listener.enterListItem) {
            listener.enterListItem(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitListItem) {
            listener.exitListItem(this);
        }
    }
}
exports.ListItemContext = ListItemContext;
class ListItemContentContext extends ParserRuleContext_1.ParserRuleContext {
    textLine(i) {
        if (i === undefined) {
            return this.getRuleContexts(TextLineContext);
        }
        else {
            return this.getRuleContext(i, TextLineContext);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_listItemContent; }
    // @Override
    enterRule(listener) {
        if (listener.enterListItemContent) {
            listener.enterListItemContent(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitListItemContent) {
            listener.exitListItemContent(this);
        }
    }
}
exports.ListItemContentContext = ListItemContentContext;
class TableItemContext extends ParserRuleContext_1.ParserRuleContext {
    TABLE_DELIM(i) {
        if (i === undefined) {
            return this.getTokens(DoxyCommentParser.TABLE_DELIM);
        }
        else {
            return this.getToken(DoxyCommentParser.TABLE_DELIM, i);
        }
    }
    endLine() {
        return this.getRuleContext(0, EndLineContext);
    }
    NL(i) {
        if (i === undefined) {
            return this.getTokens(DoxyCommentParser.NL);
        }
        else {
            return this.getToken(DoxyCommentParser.NL, i);
        }
    }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_tableItem; }
    // @Override
    enterRule(listener) {
        if (listener.enterTableItem) {
            listener.enterTableItem(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitTableItem) {
            listener.exitTableItem(this);
        }
    }
}
exports.TableItemContext = TableItemContext;
class BlockTagContext extends ParserRuleContext_1.ParserRuleContext {
    PARAM_TAG() { return this.tryGetToken(DoxyCommentParser.PARAM_TAG, 0); }
    WORD() { return this.tryGetToken(DoxyCommentParser.WORD, 0); }
    WS(i) {
        if (i === undefined) {
            return this.getTokens(DoxyCommentParser.WS);
        }
        else {
            return this.getToken(DoxyCommentParser.WS, i);
        }
    }
    DEFGROUP_TAG() { return this.tryGetToken(DoxyCommentParser.DEFGROUP_TAG, 0); }
    RETURNS_TAG() { return this.tryGetToken(DoxyCommentParser.RETURNS_TAG, 0); }
    THREAD_SAFETY_TAG() { return this.tryGetToken(DoxyCommentParser.THREAD_SAFETY_TAG, 0); }
    SA_TAG() { return this.tryGetToken(DoxyCommentParser.SA_TAG, 0); }
    SINCE_TAG() { return this.tryGetToken(DoxyCommentParser.SINCE_TAG, 0); }
    POST_TAG() { return this.tryGetToken(DoxyCommentParser.POST_TAG, 0); }
    THROWS_TAG() { return this.tryGetToken(DoxyCommentParser.THROWS_TAG, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_blockTag; }
    // @Override
    enterRule(listener) {
        if (listener.enterBlockTag) {
            listener.enterBlockTag(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitBlockTag) {
            listener.exitBlockTag(this);
        }
    }
}
exports.BlockTagContext = BlockTagContext;
class EndLineContext extends ParserRuleContext_1.ParserRuleContext {
    NL() { return this.tryGetToken(DoxyCommentParser.NL, 0); }
    EOF() { return this.tryGetToken(DoxyCommentParser.EOF, 0); }
    constructor(parent, invokingState) {
        super(parent, invokingState);
    }
    // @Override
    get ruleIndex() { return DoxyCommentParser.RULE_endLine; }
    // @Override
    enterRule(listener) {
        if (listener.enterEndLine) {
            listener.enterEndLine(this);
        }
    }
    // @Override
    exitRule(listener) {
        if (listener.exitEndLine) {
            listener.exitEndLine(this);
        }
    }
}
exports.EndLineContext = EndLineContext;
