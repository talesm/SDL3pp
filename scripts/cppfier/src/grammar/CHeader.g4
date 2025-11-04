grammar CHeader;
prog: doc? (stm)* EOF;
stm: directive | externC | functionDecl | functionDef | doc;

externC: EXTERN STRING CURLY_B (stm)* CURLY_E;
directive: doc? DIRECTIVE;
functionDecl: doc? EXTERN type ID signature SEMI;
functionDef: doc? INLINE type ID signature CURLY_B;

type: (typeEl)+;
typeEl: (VOID | ID) STAR*;
signature: ROUND_B (VOID | signatureEl) ROUND_E;
signatureEl: type ID;

doc: SHORT_DOC | LONG_DOC;

WS: [ \t\r\n]+ -> skip;
LONG_COMMENT: '/*' ~'*' .*? '*/' -> skip;
SHORT_COMMENT: '//' ~'/' .*? '\n' -> skip;

LONG_DOC: '/**' .*? '*/';
SHORT_DOC: '///' .*? '\n';

DIRECTIVE: '#' [\t ]* [0-9A-Za-z]+ (~'\n' | '\\\n')* '\n';

EXTERN: 'extern';
VOID: 'void';
SDL_NOISE: ('SDL_DECLSPEC' | 'SDLCALL') -> skip;

CURLY_B: '{';
CURLY_E: '}';
ROUND_B: '(';
ROUND_E: ')';
COLON: ':';
SEMI: ';';
COMMA: ',';
DOT: '.';
STAR: '*';
INLINE: 'SDL_FORCE_INLINE';

STRING: '"' (~'"' | '\\"')* '"';
ID: [A-Za-z_][A-Za-z0-9_]*;
NUMBER: '0' | [1-9][0-9]*;
