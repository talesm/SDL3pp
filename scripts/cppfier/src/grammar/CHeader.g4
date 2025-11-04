grammar CHeader;
prog: doc? (decl)* EOF;
decl:
	directive
	| externC
	| functionDecl
	| functionDef
	| aliasDef
	| unionDef
	| enumDef
	| structDef
	| callbackDef
	| compileTimeAssert
	| doc;

externC: EXTERN STRING CURLY_B (decl)* CURLY_E;
directive: doc? DEFINE;
functionDecl: doc? EXTERN? type attribute? id signature SEMI;
functionDef: doc? inline type attribute? id signature block;
aliasDef: doc? TYPEDEF (UNION | STRUCT)? type id SEMI;
unionDef: doc? TYPEDEF UNION id block id SEMI;
enumDef: doc? TYPEDEF ENUM id enumBody id SEMI;
structDef: doc? TYPEDEF STRUCT id structBody id SEMI;
callbackDef:
	doc? TYPEDEF type ROUND_B STAR id ROUND_E signature SEMI;
compileTimeAssert: SDL_COMPILE_TIME_ASSERT group SEMI;

inline: SDL_INLINE | STATIC INLINE;
block: CURLY_B stm* CURLY_E;
group: ROUND_B stm* ROUND_E;
indexing: SQUARE_B stm* SQUARE_E;
stm: block | indexing | expr | punct | doc | trailingDoc;
expr: group | word;
word:
	ID
	| CONST
	| VOID
	| STATIC
	| ENUM
	| STRUCT
	| UNION
	| NUMBER
	| STRING
	| DEFINE
	| SDL_COMPILE_TIME_ASSERT;
punct:
	COLON
	| SEMI
	| COMMA
	| DOT
	| STAR
	| EQ
	| ELLIPSIS
	| PUNCT_EXTRA;

enumBody: CURLY_B enumItem* CURLY_E;
enumItem: doc? id (EQ expr)? COMMA? trailingDoc?;

structBody: CURLY_B (structItem | unionInlineType)* CURLY_E;
structItem:
	doc? (CONST? STRUCT)? type id (COMMA id)* indexing* SEMI trailingDoc?;
unionInlineType: doc? UNION block id SEMI;

id: ID;
type: (typeEl)+;
typeEl: (VOID | ID | CONST) STAR*;
signature:
	ROUND_B (type (COMMA type)*)? (COMMA ELLIPSIS)? ROUND_E attribute?;

attribute: (ATTRIBUTE | SDL_VARARG_ATTRIB) group;

doc: SHORT_DOC | LONG_DOC;
trailingDoc: TRAILING_DOC;

WS: [ \t\r\n]+ -> skip;
LONG_COMMENT: '/*' ~'*' .*? '*/' -> skip;
SHORT_COMMENT: '//' ~'/' .*? '\n' -> skip;

TRAILING_DOC: '/**<' .*? '*/';
LONG_DOC: '/**' .*? '*/';
SHORT_DOC: '///' .*? '\n';

DEFINE: '#' ' '* 'define' (~'\n' | '\\\n')* '\n';
DIRECTIVE:
	(
		'#' ' '* (
			'elif'
			| 'else'
			| 'endif'
			| 'error'
			| 'if'
			| 'ifdef'
			| 'ifndef'
			| 'include'
			| 'pragma'
			| 'undef'
		) (~'\n' | '\\\n')* '\n'
	) -> skip;

ATTRIBUTE: '__attribute__';
CONST: 'const';
ENUM: 'enum';
EXTERN: 'extern';
INLINE: '__inline__';
SDL_NOISE: ('SDL_DECLSPEC' | 'SDLCALL' | 'SDL_ANALYZER_NORETURN') -> skip;
SDL_INLINE: 'SDL_FORCE_INLINE';
STATIC: 'static';
STRUCT: 'struct';
TYPEDEF: 'typedef';
UNION: 'union';
VOID: 'void';
SDL_VARARG_ATTRIB: 'SDL_' [A-Z0-9_]+ '_VARARG_FUNC' 'V'?;
SDL_COMPILE_TIME_ASSERT: 'SDL_COMPILE_TIME_ASSERT';

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
ELLIPSIS: '...';
PUNCT_EXTRA: [<>&|!=%/+-];

STRING: '"' (~'"' | '\\"')* '"';
ID: [A-Za-z_][A-Za-z0-9_]*;
NUMBER: [+-]? ('0' | [1-9][0-9]* | '0x' [0-9A-Fa-f]+) [ul]*;
