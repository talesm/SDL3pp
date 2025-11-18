grammar DoxyComment;
doc: block* EOF;
block:
	title
	| paragraph
	| list
	| table
	| codeblock
	| taggedSection
	| empty;

title: TITLE_DELIM textLine;
paragraph: textLine+;
list: listItem+;
table: tableItem+ endLine;
codeblock: CODE_DELIM (~CODE_DELIM)* CODE_DELIM endLine;
taggedSection: taggedBlock+;
taggedBlock: blockTag textLine+;
empty: NL;

textLine: (WS | WORD) (~NL)* endLine;
listItem: WS* LIST_DELIM listItemContent;
listItemContent: textLine+;
tableItem:
	TABLE_DELIM (~TABLE_DELIM | TABLE_DELIM ~NL)* TABLE_DELIM endLine;
blockTag:
	PARAM_TAG WS+ WORD
	| DEFGROUP_TAG WS+ WORD
	| RETURNS_TAG
	| THREAD_SAFETY_TAG
	| SA_TAG
	| SINCE_TAG
	| POST_TAG
	| THROWS_TAG;
endLine: NL | EOF;

CODE_DELIM: '```' [A-Za-z0-9]*;
TABLE_DELIM: '|';
LIST_DELIM: '-';
TITLE_DELIM: '#'+;

DEFGROUP_TAG: [\\@]'defgroup';
RETURNS_TAG: [\\@]'returns';
PARAM_TAG: [\\@]'param';
THREAD_SAFETY_TAG: [\\@]'threadsafety';
SA_TAG: [\\@]'sa';
SINCE_TAG: [\\@]'since';
POST_TAG: [\\@]'post';
THROWS_TAG: [\\@]'throws';

WORD: ~[ \t\n\r|#-]~[ \t\n\r]*;
WS: [ \t];
NL: [ \t]* ('\n' | '\r' '\n'?);
