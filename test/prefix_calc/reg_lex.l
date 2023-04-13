%option 7bit reentrant

%option nounput noinput nodefault noyywrap

%top{
	#include "reg_par.h"
	#define YY_DECL int yylex(YYSTYPE* yylvalp, void* yyscanner)
	extern YY_DECL;
}

%%

[ \t\r]+ {}
[0-9]+\.[0-9]* {
	yylvalp->NUM = atof(yytext);
	return NUM;
}
\+ {return ADD;}
- {return SUB;}
\* {return MUL;}
\/ {return DIV;}
\^ {return POW;}
n {return NEG;}
\n {return NL;}
<<EOF>> {return YYEOF;}
. {return YYUNDEF;}

%%

extern int yyparse(yyscan_t scanner);

int main(void) {
	yyscan_t scanner;
	yylex_init(&scanner);
	yyset_in(stdin, scanner);
	int res = yyparse(scanner);
	yylex_destroy(scanner);
	return res;
}
