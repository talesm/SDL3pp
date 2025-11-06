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
exports.DoxyCommentLexer = void 0;
const ATNDeserializer_1 = require("antlr4ts/atn/ATNDeserializer");
const Lexer_1 = require("antlr4ts/Lexer");
const LexerATNSimulator_1 = require("antlr4ts/atn/LexerATNSimulator");
const VocabularyImpl_1 = require("antlr4ts/VocabularyImpl");
const Utils = __importStar(require("antlr4ts/misc/Utils"));
class DoxyCommentLexer extends Lexer_1.Lexer {
    // @Override
    // @NotNull
    get vocabulary() {
        return DoxyCommentLexer.VOCABULARY;
    }
    // tslint:enable:no-trailing-whitespace
    constructor(input) {
        super(input);
        this._interp = new LexerATNSimulator_1.LexerATNSimulator(DoxyCommentLexer._ATN, this);
    }
    // @Override
    get grammarFileName() { return "DoxyComment.g4"; }
    // @Override
    get ruleNames() { return DoxyCommentLexer.ruleNames; }
    // @Override
    get serializedATN() { return DoxyCommentLexer._serializedATN; }
    // @Override
    get channelNames() { return DoxyCommentLexer.channelNames; }
    // @Override
    get modeNames() { return DoxyCommentLexer.modeNames; }
    static get _ATN() {
        if (!DoxyCommentLexer.__ATN) {
            DoxyCommentLexer.__ATN = new ATNDeserializer_1.ATNDeserializer().deserialize(Utils.toCharArray(DoxyCommentLexer._serializedATN));
        }
        return DoxyCommentLexer.__ATN;
    }
}
exports.DoxyCommentLexer = DoxyCommentLexer;
DoxyCommentLexer.CODE_DELIM = 1;
DoxyCommentLexer.TABLE_DELIM = 2;
DoxyCommentLexer.LIST_DELIM = 3;
DoxyCommentLexer.TITLE_DELIM = 4;
DoxyCommentLexer.DEFGROUP_TAG = 5;
DoxyCommentLexer.RETURNS_TAG = 6;
DoxyCommentLexer.PARAM_TAG = 7;
DoxyCommentLexer.THREAD_SAFETY_TAG = 8;
DoxyCommentLexer.SA_TAG = 9;
DoxyCommentLexer.SINCE_TAG = 10;
DoxyCommentLexer.POST_TAG = 11;
DoxyCommentLexer.THROWS_TAG = 12;
DoxyCommentLexer.WORD = 13;
DoxyCommentLexer.WS = 14;
DoxyCommentLexer.NL = 15;
// tslint:disable:no-trailing-whitespace
DoxyCommentLexer.channelNames = [
    "DEFAULT_TOKEN_CHANNEL", "HIDDEN",
];
// tslint:disable:no-trailing-whitespace
DoxyCommentLexer.modeNames = [
    "DEFAULT_MODE",
];
DoxyCommentLexer.ruleNames = [
    "CODE_DELIM", "TABLE_DELIM", "LIST_DELIM", "TITLE_DELIM", "DEFGROUP_TAG",
    "RETURNS_TAG", "PARAM_TAG", "THREAD_SAFETY_TAG", "SA_TAG", "SINCE_TAG",
    "POST_TAG", "THROWS_TAG", "WORD", "WS", "NL",
];
DoxyCommentLexer._LITERAL_NAMES = [
    undefined, undefined, "'|'", "'-'",
];
DoxyCommentLexer._SYMBOLIC_NAMES = [
    undefined, "CODE_DELIM", "TABLE_DELIM", "LIST_DELIM", "TITLE_DELIM", "DEFGROUP_TAG",
    "RETURNS_TAG", "PARAM_TAG", "THREAD_SAFETY_TAG", "SA_TAG", "SINCE_TAG",
    "POST_TAG", "THROWS_TAG", "WORD", "WS", "NL",
];
DoxyCommentLexer.VOCABULARY = new VocabularyImpl_1.VocabularyImpl(DoxyCommentLexer._LITERAL_NAMES, DoxyCommentLexer._SYMBOLIC_NAMES, []);
DoxyCommentLexer._serializedATN = "\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x02\x11\x8B\b\x01" +
    "\x04\x02\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06" +
    "\x04\x07\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r" +
    "\t\r\x04\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x03\x02\x03\x02\x03\x02" +
    "\x03\x02\x03\x02\x07\x02\'\n\x02\f\x02\x0E\x02*\v\x02\x03\x03\x03\x03" +
    "\x03\x04\x03\x04\x03\x05\x06\x051\n\x05\r\x05\x0E\x052\x03\x06\x03\x06" +
    "\x03\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x07" +
    "\x03\x07\x03\x07\x03\x07\x03\x07\x03\x07\x03\x07\x03\x07\x03\x07\x03\b" +
    "\x03\b\x03\b\x03\b\x03\b\x03\b\x03\b\x03\t\x03\t\x03\t\x03\t\x03\t\x03" +
    "\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\t\x03\n\x03\n\x03\n\x03" +
    "\n\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\v\x03\f\x03\f\x03\f\x03\f\x03" +
    "\f\x03\f\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\r\x03\x0E\x03\x0E" +
    "\x07\x0Ex\n\x0E\f\x0E\x0E\x0E{\v\x0E\x03\x0F\x03\x0F\x03\x10\x07\x10\x80" +
    "\n\x10\f\x10\x0E\x10\x83\v\x10\x03\x10\x03\x10\x03\x10\x05\x10\x88\n\x10" +
    "\x05\x10\x8A\n\x10\x02\x02\x02\x11\x03\x02\x03\x05\x02\x04\x07\x02\x05" +
    "\t\x02\x06\v\x02\x07\r\x02\b\x0F\x02\t\x11\x02\n\x13\x02\v\x15\x02\f\x17" +
    "\x02\r\x19\x02\x0E\x1B\x02\x0F\x1D\x02\x10\x1F\x02\x11\x03\x02\x07\x05" +
    "\x022;C\\c|\x04\x02BB^^\b\x02\v\f\x0F\x0F\"\"%%//~~\x05\x02\v\f\x0F\x0F" +
    "\"\"\x04\x02\v\v\"\"\x02\x90\x02\x03\x03\x02\x02\x02\x02\x05\x03\x02\x02" +
    "\x02\x02\x07\x03\x02\x02\x02\x02\t\x03\x02\x02\x02\x02\v\x03\x02\x02\x02" +
    "\x02\r\x03\x02\x02\x02\x02\x0F\x03\x02\x02\x02\x02\x11\x03\x02\x02\x02" +
    "\x02\x13\x03\x02\x02\x02\x02\x15\x03\x02\x02\x02\x02\x17\x03\x02\x02\x02" +
    "\x02\x19\x03\x02\x02\x02\x02\x1B\x03\x02\x02\x02\x02\x1D\x03\x02\x02\x02" +
    "\x02\x1F\x03\x02\x02\x02\x03!\x03\x02\x02\x02\x05+\x03\x02\x02\x02\x07" +
    "-\x03\x02\x02\x02\t0\x03\x02\x02\x02\v4\x03\x02\x02\x02\r>\x03\x02\x02" +
    "\x02\x0FG\x03\x02\x02\x02\x11N\x03\x02\x02\x02\x13\\\x03\x02\x02\x02\x15" +
    "`\x03\x02\x02\x02\x17g\x03\x02\x02\x02\x19m\x03\x02\x02\x02\x1Bu\x03\x02" +
    "\x02\x02\x1D|\x03\x02\x02\x02\x1F\x81\x03\x02\x02\x02!\"\x07b\x02\x02" +
    "\"#\x07b\x02\x02#$\x07b\x02\x02$(\x03\x02\x02\x02%\'\t\x02\x02\x02&%\x03" +
    "\x02\x02\x02\'*\x03\x02\x02\x02(&\x03\x02\x02\x02()\x03\x02\x02\x02)\x04" +
    "\x03\x02\x02\x02*(\x03\x02\x02\x02+,\x07~\x02\x02,\x06\x03\x02\x02\x02" +
    "-.\x07/\x02\x02.\b\x03\x02\x02\x02/1\x07%\x02\x020/\x03\x02\x02\x0212" +
    "\x03\x02\x02\x0220\x03\x02\x02\x0223\x03\x02\x02\x023\n\x03\x02\x02\x02" +
    "45\t\x03\x02\x0256\x07f\x02\x0267\x07g\x02\x0278\x07h\x02\x0289\x07i\x02" +
    "\x029:\x07t\x02\x02:;\x07q\x02\x02;<\x07w\x02\x02<=\x07r\x02\x02=\f\x03" +
    "\x02\x02\x02>?\t\x03\x02\x02?@\x07t\x02\x02@A\x07g\x02\x02AB\x07v\x02" +
    "\x02BC\x07w\x02\x02CD\x07t\x02\x02DE\x07p\x02\x02EF\x07u\x02\x02F\x0E" +
    "\x03\x02\x02\x02GH\t\x03\x02\x02HI\x07r\x02\x02IJ\x07c\x02\x02JK\x07t" +
    "\x02\x02KL\x07c\x02\x02LM\x07o\x02\x02M\x10\x03\x02\x02\x02NO\t\x03\x02" +
    "\x02OP\x07v\x02\x02PQ\x07j\x02\x02QR\x07t\x02\x02RS\x07g\x02\x02ST\x07" +
    "c\x02\x02TU\x07f\x02\x02UV\x07u\x02\x02VW\x07c\x02\x02WX\x07h\x02\x02" +
    "XY\x07g\x02\x02YZ\x07v\x02\x02Z[\x07{\x02\x02[\x12\x03\x02\x02\x02\\]" +
    "\t\x03\x02\x02]^\x07u\x02\x02^_\x07c\x02\x02_\x14\x03\x02\x02\x02`a\t" +
    "\x03\x02\x02ab\x07u\x02\x02bc\x07k\x02\x02cd\x07p\x02\x02de\x07e\x02\x02" +
    "ef\x07g\x02\x02f\x16\x03\x02\x02\x02gh\t\x03\x02\x02hi\x07r\x02\x02ij" +
    "\x07q\x02\x02jk\x07u\x02\x02kl\x07v\x02\x02l\x18\x03\x02\x02\x02mn\t\x03" +
    "\x02\x02no\x07v\x02\x02op\x07j\x02\x02pq\x07t\x02\x02qr\x07q\x02\x02r" +
    "s\x07y\x02\x02st\x07u\x02\x02t\x1A\x03\x02\x02\x02uy\n\x04\x02\x02vx\n" +
    "\x05\x02\x02wv\x03\x02\x02\x02x{\x03\x02\x02\x02yw\x03\x02\x02\x02yz\x03" +
    "\x02\x02\x02z\x1C\x03\x02\x02\x02{y\x03\x02\x02\x02|}\t\x06\x02\x02}\x1E" +
    "\x03\x02\x02\x02~\x80\t\x06\x02\x02\x7F~\x03\x02\x02\x02\x80\x83\x03\x02" +
    "\x02\x02\x81\x7F\x03\x02\x02\x02\x81\x82\x03\x02\x02\x02\x82\x89\x03\x02" +
    "\x02\x02\x83\x81\x03\x02\x02\x02\x84\x8A\x07\f\x02\x02\x85\x87\x07\x0F" +
    "\x02\x02\x86\x88\x07\f\x02\x02\x87\x86\x03\x02\x02\x02\x87\x88\x03\x02" +
    "\x02\x02\x88\x8A\x03\x02\x02\x02\x89\x84\x03\x02\x02\x02\x89\x85\x03\x02" +
    "\x02\x02\x8A \x03\x02\x02\x02\t\x02(2y\x81\x87\x89\x02";
