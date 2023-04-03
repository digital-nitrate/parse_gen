%option 7bit reentrant

%option nounput noinput nodefault noyywrap

%top{
	typedef union YYSTYPE {
		double NUM;
	} YYSTYPE;
	#define YY_DECL int yylex(YYSTYPE* yylvalp, yyscan_t yyscanner)
}

%%

[ \t\r]+ {}
[0-9]+\.[0-9]* {
	yylvalp->NUM = atof(yytext);
	return 257;
}
\+ {return 258;}
- {return 259;}
\* {return 260;}
\/ {return 261;}
\^ {return 262;}
n {return 263;}
\n {return 264;}
<<EOF>> {return -1;}
. {return -2;}

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
