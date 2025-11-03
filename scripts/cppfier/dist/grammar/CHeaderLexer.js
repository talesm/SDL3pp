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
exports.CHeaderLexer = void 0;
const ATNDeserializer_1 = require("antlr4ts/atn/ATNDeserializer");
const Lexer_1 = require("antlr4ts/Lexer");
const LexerATNSimulator_1 = require("antlr4ts/atn/LexerATNSimulator");
const VocabularyImpl_1 = require("antlr4ts/VocabularyImpl");
const Utils = __importStar(require("antlr4ts/misc/Utils"));
class CHeaderLexer extends Lexer_1.Lexer {
    // @Override
    // @NotNull
    get vocabulary() {
        return CHeaderLexer.VOCABULARY;
    }
    // tslint:enable:no-trailing-whitespace
    constructor(input) {
        super(input);
        this._interp = new LexerATNSimulator_1.LexerATNSimulator(CHeaderLexer._ATN, this);
    }
    // @Override
    get grammarFileName() { return "CHeader.g4"; }
    // @Override
    get ruleNames() { return CHeaderLexer.ruleNames; }
    // @Override
    get serializedATN() { return CHeaderLexer._serializedATN; }
    // @Override
    get channelNames() { return CHeaderLexer.channelNames; }
    // @Override
    get modeNames() { return CHeaderLexer.modeNames; }
    static get _ATN() {
        if (!CHeaderLexer.__ATN) {
            CHeaderLexer.__ATN = new ATNDeserializer_1.ATNDeserializer().deserialize(Utils.toCharArray(CHeaderLexer._serializedATN));
        }
        return CHeaderLexer.__ATN;
    }
}
exports.CHeaderLexer = CHeaderLexer;
CHeaderLexer.T__0 = 1;
CHeaderLexer.T__1 = 2;
CHeaderLexer.T__2 = 3;
CHeaderLexer.T__3 = 4;
CHeaderLexer.T__4 = 5;
CHeaderLexer.T__5 = 6;
CHeaderLexer.NEWLINE = 7;
CHeaderLexer.INT = 8;
// tslint:disable:no-trailing-whitespace
CHeaderLexer.channelNames = [
    "DEFAULT_TOKEN_CHANNEL", "HIDDEN",
];
// tslint:disable:no-trailing-whitespace
CHeaderLexer.modeNames = [
    "DEFAULT_MODE",
];
CHeaderLexer.ruleNames = [
    "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "NEWLINE", "INT",
];
CHeaderLexer._LITERAL_NAMES = [
    undefined, "'*'", "'/'", "'+'", "'-'", "'('", "')'",
];
CHeaderLexer._SYMBOLIC_NAMES = [
    undefined, undefined, undefined, undefined, undefined, undefined, undefined,
    "NEWLINE", "INT",
];
CHeaderLexer.VOCABULARY = new VocabularyImpl_1.VocabularyImpl(CHeaderLexer._LITERAL_NAMES, CHeaderLexer._SYMBOLIC_NAMES, []);
CHeaderLexer._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x02\n+\b\x01\x04" +
    "\x02\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04" +
    "\x07\t\x07\x04\b\t\b\x04\t\t\t\x03\x02\x03\x02\x03\x03\x03\x03\x03\x04" +
    "\x03\x04\x03\x05\x03\x05\x03\x06\x03\x06\x03\x07\x03\x07\x03\b\x06\b!" +
    "\n\b\r\b\x0E\b\"\x03\b\x03\b\x03\t\x06\t(\n\t\r\t\x0E\t)\x02\x02\x02\n" +
    "\x03\x02\x03\x05\x02\x04\x07\x02\x05\t\x02\x06\v\x02\x07\r\x02\b\x0F\x02" +
    "\t\x11\x02\n\x03\x02\x04\x04\x02\f\f\x0F\x0F\x03\x022;\x02,\x02\x03\x03" +
    "\x02\x02\x02\x02\x05\x03\x02\x02\x02\x02\x07\x03\x02\x02\x02\x02\t\x03" +
    "\x02\x02\x02\x02\v\x03\x02\x02\x02\x02\r\x03\x02\x02\x02\x02\x0F\x03\x02" +
    "\x02\x02\x02\x11\x03\x02\x02\x02\x03\x13\x03\x02\x02\x02\x05\x15\x03\x02" +
    "\x02\x02\x07\x17\x03\x02\x02\x02\t\x19\x03\x02\x02\x02\v\x1B\x03\x02\x02" +
    "\x02\r\x1D\x03\x02\x02\x02\x0F \x03\x02\x02\x02\x11\'\x03\x02\x02\x02" +
    "\x13\x14\x07,\x02\x02\x14\x04\x03\x02\x02\x02\x15\x16\x071\x02\x02\x16" +
    "\x06\x03\x02\x02\x02\x17\x18\x07-\x02\x02\x18\b\x03\x02\x02\x02\x19\x1A" +
    "\x07/\x02\x02\x1A\n\x03\x02\x02\x02\x1B\x1C\x07*\x02\x02\x1C\f\x03\x02" +
    "\x02\x02\x1D\x1E\x07+\x02\x02\x1E\x0E\x03\x02\x02\x02\x1F!\t\x02\x02\x02" +
    " \x1F\x03\x02\x02\x02!\"\x03\x02\x02\x02\" \x03\x02\x02\x02\"#\x03\x02" +
    "\x02\x02#$\x03\x02\x02\x02$%\b\b\x02\x02%\x10\x03\x02\x02\x02&(\t\x03" +
    "\x02\x02\'&\x03\x02\x02\x02()\x03\x02\x02\x02)\'\x03\x02\x02\x02)*\x03" +
    "\x02\x02\x02*\x12\x03\x02\x02\x02\x05\x02\")\x03\b\x02\x02";
