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

<INITIAL>\n {return NL;}
<rules>: {return COL;}
<rules>\| {return MID;}
<rules>; {return SCL;}
<INITIAL>\/token {return TOKENDECL;}
<INITIAL>\/nterm {return NTERMDECL;}
<INITIAL>\/start {return STARTDECL;}
<INITIAL>\/prefix {return PREFIXDECL;}
<INITIAL>\/parseparam {return PPARAMDECL;}
<INITIAL>\/lexparam {return LPARAMDECL;}
<INITIAL>\/codetop {return CODETOPDECL;}
<INITIAL>\/coderequires {return CODEREQDECL;}
<INITIAL>\/codeprovides {return CODEPROVDECL;}
<INITIAL>\/code {return CODEDECL;}
<rules>\/empty {return EMPTYDECL;}
<INITIAL>%% {BEGIN(rules);return SECSEP;}
<rules>%% {BEGIN(end);return SECSEP;}
<rules>\/\{ {BEGIN(code);return LBR;}
<code>\/\} {BEGIN(rules);return RBR;}
<INITIAL>[0-9]+ {
	unsigned int value = 0;
	for (int i = 0; i < yyleng; ++i) value = 10 * value + (yytext[i] - '0');
	yylval_param->UINT = value;
	return UINT;
}
<INITIAL,rules>[a-zA-Z][a-zA-Z0-9_]* {
	char* data = malloc(yyleng + 1);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext, yyleng);
	data[yyleng] = '\0';
	yylval_param->IDEN = data;
	return IDEN;
}
<INITIAL>\<[^\>\n]*\> {
	char* data = malloc(yyleng - 1);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext + 1, yyleng - 2);
	data[yyleng - 1] = '\0';
	yylval_param->TYPESPEC = data;
	return TYPESPEC;
}
<INITIAL>\/\{(\*|\}|\/*([^\*\/\"\'\}]|\/\*([^\*]|\*+[^\*\/])*\*+\/|\"([^\"\\]|\\(.|\n))*\"|\'([^\'\\]|\\(.|\n))*\'))*\/\} {
	char* data = malloc(yyleng - 3);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext + 2, yyleng - 4);
	data[yyleng - 3] = '\0';
	yylval_param->CODEBLOCK = data;
	return CODEBLOCK;
}
<code>\$[0-9]+ {
	unsigned int value = 0;
	for (int i = 1; i < yyleng; ++i) value = 10 * value + (yytext[i] - '0');
	yylval_param->VALREF = value;
	return VALREF;
}
<end>(.|\n)+ {
	char* data = malloc(yyleng + 1);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext, yyleng);
	data[yyleng] = '\0';
	yylval_param->RAW = data;
	return RAW;
}
<code>(\*|\}|\/*([^\*\/\"\'\$\}]|\/\*([^\*]|\*+[^\*\/])*\*+\/|\"([^\"\\]|\\(.|\n))*\"|\'([^\'\\]|\\(.|\n))*\'))* {
	char* data = malloc(yyleng + 1);
	if (data == NULL) return LEXERR;
	memcpy(data, yytext, yyleng);
	data[yyleng] = '\0';
	yylval_param->RAW = data;
	return RAW;
}
<INITIAL>[ \t]+ {}
<rules>[ \t\n]+ {}
<INITIAL,rules>\/\*([^\*]|\*+[^\*\/])*\*+\/ {}
<INITIAL>\/[a-zA-Z][a-zA-Z0-9_]* {return LEXERR;}
. {return LEXERR;}
