grammar CHeader;
prog: doc? (decl)* EOF;
decl:
	directive
	| externC
	| functionDecl
	| functionDef
	| aliasDef
	| enumDef
	| callbackDef
	| doc;

externC: EXTERN STRING CURLY_B (decl)* CURLY_E;
directive: doc? DIRECTIVE;
functionDecl: doc? EXTERN type attribute? id signature SEMI;
functionDef: doc? inline type attribute? id signature block;
aliasDef: doc? TYPEDEF type id SEMI;
enumDef: doc? TYPEDEF ENUM id enumBody id SEMI;
callbackDef:
	doc? TYPEDEF type ROUND_B STAR id ROUND_E signature SEMI;

inline: SDL_INLINE | STATIC INLINE;
block: CURLY_B stm* CURLY_E;
group: ROUND_B stm* ROUND_E;
indexing: SQUARE_B stm* SQUARE_E;
stm: block | group | indexing | word | punct;
word: ID | VOID | STATIC | NUMBER | STRING | DIRECTIVE;
punct: COLON | SEMI | COMMA | DOT | STAR | EQ | PUNCT_EXTRA;

enumBody: CURLY_B (enumItem)* enumItemLast CURLY_E;
enumItem: doc? id (EQ NUMBER)? COMMA trailingDoc?;
enumItemLast: doc? id (EQ NUMBER)? COMMA? trailingDoc?;

id: ID;
type: (typeEl)+;
typeEl: (VOID | ID) STAR*;
signature: ROUND_B (type (COMMA type)*)? ROUND_E;

attribute: ATTRIBUTE group;

doc: SHORT_DOC | LONG_DOC;
trailingDoc: TRAILING_DOC;

WS: [ \t\r\n]+ -> skip;
LONG_COMMENT: '/*' ~'*' .*? '*/' -> skip;
SHORT_COMMENT: '//' ~'/' .*? '\n' -> skip;

TRAILING_DOC: '/**<' .*? '*/';
LONG_DOC: '/**' .*? '*/';
SHORT_DOC: '///' .*? '\n';

DIRECTIVE: '#' (~'\n' | '\\\n')* '\n';

ATTRIBUTE: '__attribute__';
ENUM: 'enum';
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
EQ: '=';
PUNCT_EXTRA: [<>&|!=%/+-];

STRING: '"' (~'"' | '\\"')* '"';
ID: [A-Za-z_][A-Za-z0-9_]*;
NUMBER: '0' | [1-9][0-9]* | '0x' [0-9A-Fa-f]+;
