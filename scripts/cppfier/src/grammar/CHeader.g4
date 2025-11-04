grammar CHeader;
prog: doc? (stm)* EOF;
stm: directive | externC | functionDecl;

externC: EXTERN STRING CURLY_B (stm)* CURLY_E;
directive: doc? DIRECTIVE;
functionDecl: doc? EXTERN type ID signature SEMI;

type: (typeEl)+;
typeEl: (VOID | ID) STAR*;
signature: ROUND_B VOID ROUND_E;

doc: SHORT_DOC | LONG_DOC;

WS: [ \t\r\n]+ -> skip;
LONG_COMMENT: '/*' ~'*' .*? '*/' -> skip;
SHORT_COMMENT: '//' ~'/' .*? '\n' -> skip;

LONG_DOC: '/**' .*? '*/';
SHORT_DOC: '///' .*? '\n';

DIRECTIVE: '#' [0-9A-Za-z]+ (~'\n' | '\\\n')* '\n';

EXTERN: 'extern';
VOID: 'void';
SDL_NOISE: ('SDL_DECLSPEC' | 'SDLCALL') -> skip;

CURLY_B: '{';
CURLY_E: '}';
ROUND_B: '(';
ROUND_E: ')';
SEMI: ';';
STAR: '*';

STRING: '"' (~'"' | '\\"')* '"';
ID: [A-Za-z][A-Za-z0-9_]*;
