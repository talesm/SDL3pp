grammar CHeader;
prog: doc? (decl)* EOF;
decl:
	directive
	| externC
	| functionDecl
	| functionDef
	| aliasDef
	| doc;

externC: EXTERN STRING CURLY_B (decl)* CURLY_E;
directive: doc? DIRECTIVE;
functionDecl: doc? EXTERN type attribute? ID signature SEMI;
functionDef: doc? inline type attribute? ID signature block;
aliasDef: TYPEDEF type ID;

inline: SDL_INLINE | STATIC INLINE;
block: CURLY_B stm* CURLY_E;
group: ROUND_B stm* ROUND_E;
indexing: SQUARE_B stm* SQUARE_E;
stm: block | group | indexing | word | punct;
word: ID | VOID | STATIC | NUMBER | STRING | DIRECTIVE;
punct: COLON | SEMI | COMMA | DOT | STAR | PUNCT_EXTRA;

id: ID;
type: (typeEl)+;
typeEl: (VOID | ID) STAR*;
signature: ROUND_B (type (COMMA type)*)? ROUND_E;

attribute: ATTRIBUTE group;

doc: SHORT_DOC | LONG_DOC;

WS: [ \t\r\n]+ -> skip;
LONG_COMMENT: '/*' ~'*' .*? '*/' -> skip;
SHORT_COMMENT: '//' ~'/' .*? '\n' -> skip;

LONG_DOC: '/**' .*? '*/';
SHORT_DOC: '///' .*? '\n';

DIRECTIVE: '#' (~'\n' | '\\\n')* '\n';

ATTRIBUTE: '__attribute__';
EXTERN: 'extern';
INLINE: '__inline__';
SDL_NOISE: ('SDL_DECLSPEC' | 'SDLCALL') -> skip;
SDL_INLINE: 'SDL_FORCE_INLINE';
STATIC: 'static';
TYPEDEF: 'typedef';
VOID: 'void';

CURLY_B: '{';
CURLY_E: '}';
ROUND_B: '(';
ROUND_E: ')';
SQUARE_B: '[';
SQUARE_E: ']';
COLON: ':';
SEMI: ';';
COMMA: ',';
DOT: '.';
STAR: '*';
PUNCT_EXTRA: [<>&|!=%/+-];

STRING: '"' (~'"' | '\\"')* '"';
ID: [A-Za-z_][A-Za-z0-9_]*;
NUMBER: '0' | [1-9][0-9]*;
