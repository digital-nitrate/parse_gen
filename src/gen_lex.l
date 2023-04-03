%option 7bit reentrant bison-bridge bison-locations

%option yylineno
%option noyywrap nounput noinput nodefault

%s rules end code

%top{
	#include "gen_parse.tab.h"
	#define YY_USER_ACTION \
		yylloc_param->first_column = yylloc_param->last_column;\
		if (yylloc_param->last_line == yylineno) {\
			yylloc_param->last_column += yyleng;\
		} else {\
			yylloc_param->last_column = yytext + yyleng - strrchr(yytext, '\n');\
		}\
		yylloc_param->first_line = yylloc_param->last_line;\
		yylloc_param->last_line = yylineno;\
}

%%

<INITIAL,rules>[ \t\r\n]+ {}
<INITIAL,rules>\/\*([^\*]|\*[^\/])*\*\/ {}
<INITIAL>%\{(\}|%*([^%\/\"\'\}]|\/\*([^*]|\*[^\/])*\*\/|\"([^\"\\]|\\(.|\n))*\"|\'([^\'\\]|\\(.|\n))*\'))*%\} {
	char* data = malloc(yyleng - 4);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext + 2, yyleng - 4);
	yylval_param->PROBLOCK = (struct string){.data=data,.len=yyleng-4,.cap=yyleng-4};
	return PROBLOCK;
}
<INITIAL,rules>[a-zA-Z][a-zA-Z0-9_]* {
	char* data = malloc(yyleng);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext, yyleng);
	yylval_param->IDEN = (struct string){.data=data,.len=yyleng,.cap=yyleng};
	return IDEN;
}
<INITIAL>\<[^>]*\> {
	char* data = malloc(yyleng - 2);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext + 1, yyleng - 2);
	yylval_param->TYPESPEC = (struct string){.data=data,.len=yyleng-2,.cap=yyleng-2};
	return TYPESPEC;
}
<INITIAL>[0-9]+ {
	unsigned int value = 0;
	for (int i = 0; i < yyleng; ++i) value = 10 * value + (yytext[i] - '0');
	yylval_param->UINT = value;
	return UINT;
}
<INITIAL>\. {return DOT;}
<INITIAL>%token {return TOKENDECL;}
<INITIAL>%nterm {return NTERMDECL;}
<INITIAL>%start {return STARTDECL;}
<INITIAL>%define {return DEFINEDECL;}
<INITIAL>%param {return PARAMDECL;}
<INITIAL>%% {
	BEGIN(rules);	
	return SECSEP;
}
<rules>%empty {return EMPTYDECL;}
<rules>: {return COL;}
<rules>\| {return MID;}
<rules>; {return SCL;}
<rules>%\{ {
	BEGIN(code);
	return LBR;
}
<rules>%% {
	BEGIN(end);
	return SECSEP;
}
<code>\$\$ {
	return SELFREF;
}
<code>\$[1-9][0-9]* {
	unsigned int value = 0;
	for (int i = 1; i < yyleng; ++i) value = 10 * value + (yytext[i] - '0');
	yylval_param->SUBREF = value - 1;
	return SUBREF;
}
<code>(\}|(\/|%)*([^%\/\"\'\\}\$]|\/\*([^*]|\*[^\/])*\*\/|\"([^\"\\]|\\(.|\n))*\"|\'([^\'\\]|\\(.|\n))*\'))+ {
	char* data = malloc(yyleng);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext, yyleng);
	yylval_param->RAW = (struct string){.data=data,.len=yyleng,.cap=yyleng,};
	return RAW;
}
<code>%\} {
	BEGIN(rules);
	return RBR;
}
<end>(.|\n)+ {
	char* data = malloc(yyleng);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext, yyleng);
	yylval_param->RAW = (struct string){.data=data,.len=yyleng,.cap=yyleng,};
	return RAW;
}
. {return LEXERR;}
