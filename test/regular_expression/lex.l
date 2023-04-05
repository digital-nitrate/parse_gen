%option 7bit reentrant

%option nounput noinput nodefault noyywrap

%top {
	
	#include "regular.h"

	typedef union YYSTYPE {
		char chara;
		reg ALT;
		struct reg_link* ALTLIST;
		reg SEQ;
		struct reg_link* SEQLIST;
		reg ATOM;
		enum atommod_type ATOMMOD;
		reg NUCLEUS;
		struct charrng CHARRNG;
	} YYSTYPE;

	#define YY_DECL int yylex(YYSTYPE* yylvalp, reg* out, yyscan_t yyscanner)
}

%%

\| {return 1;}
\* {return 2;}
\+ {return 3;}
\( {return 4;}
\) {return 5;}
[a-zA-Z0-9] {yylvalp->chara = *yytext; return 6;}
\. {return 7;}
\- {return 8;}
<<EOF>> {return -1;}
\n {return -1;}
. {return -2;}

%%

static void write_reg(FILE* restrict out, reg const* restrict r) {
	switch (r->type) {
		case REG_LAMBDA:
			break;
		case REG_CHAR:
			fputc(r->c, out);
			break;
		case REG_RANGE:
			fprintf(out, "%c-%c", r->c1, r->c2);
			break;
		case REG_DOT:
			fputc('.', out);
			break;
		case REG_KLEENE:
			fputc('(', out);
			write_reg(out, r->kleene);
			fputs(")*", out);
			break;
		case REG_PLUS:
			fputc('(', out);
			write_reg(out, r->plus);
			fputs(")*", out);
			break;
		case REG_SEQ: {
			struct reg_link const* curr = r->seq;
			while (curr != NULL) {
				fputc('(', out);
				write_reg(out, &(curr->sub));
				fputc(')', out);
				curr = curr->next;
			}
			break;
		}
		case REG_ALT: {
			struct reg_link const* curr = r->alt;
			fputc('(', out);
			write_reg(out, &(curr->sub));
			fputc(')', out);
			curr = curr->next;
			while (curr != NULL) {
				fputs("|(", out);
				write_reg(out, &(curr->sub));
				fputc(')', out);
				curr = curr->next;
			}
			break;
		}
	}
}

extern int yyparse(reg* out, yyscan_t scanner);

int main(void) {
	yyscan_t scanner;
	yylex_init(&scanner);
	yyset_in(stdin, scanner);
	reg out;
	int res = yyparse(&out, scanner);
	yylex_destroy(scanner);
	if (res == 0) {
		write_reg(stdout, &out);
		fputc('\n', stdout);
	}
	return res;
}
