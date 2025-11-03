import { CharStreams, CommonTokenStream } from 'antlr4ts';
import { CHeaderListener } from './grammar/CHeaderListener';
import { CHeaderParser, ProgContext } from './grammar/CHeaderParser';
import { CHeaderLexer } from './grammar/CHeaderLexer';
import { ParseTreeWalker } from 'antlr4ts/tree/ParseTreeWalker';
import { ParseTreeListener } from 'antlr4ts/tree/ParseTreeListener';
import { TerminalNode } from 'antlr4ts/tree/TerminalNode';

// Create the lexer and parser
let lexer = new CHeaderLexer(CharStreams.fromString('1+1\n'));
let tokenStream = new CommonTokenStream(lexer);
let parser = new CHeaderParser(tokenStream);

// Parse the input, where `compilationUnit` is whatever entry point you defined
let tree = parser.prog();

class ProgListener implements CHeaderListener {
  // Assuming a parser rule with name: `functionDeclaration`
  enterProg(context: ProgContext) {
    console.log(`Function start line number ${context._start.line}`);
    // ...
  }

  exitProg(context: ProgContext) { }

  // other enterX functions...
  visitTerminal(/*@NotNull*/ node: TerminalNode) { }
  // visitErrorNode(/*@NotNull*/ node: ErrorNode) {};
  // enterEveryRule(/*@NotNull*/ ctx: ParserRuleContext) {};
  // exitEveryRule(/*@NotNull*/ ctx: ParserRuleContext) {};
}

// Create the listener
const listener: ParseTreeListener = new ProgListener();
// Use the entry point for listeners
ParseTreeWalker.DEFAULT.walk(listener, tree);
