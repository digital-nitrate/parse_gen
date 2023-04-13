%option 7bit reentrant

%option nounput noinput nodefault noyywrap

%top {
	#include "regular.h"
	#include "regex_parse.h"
	#define YY_DECL int yylex(YYSTYPE* yylvalp, void* yyscanner)
	extern YY_DECL;
}

%%

\| {return tokpipe;}
\* {return tokkleene;}
\+ {return tokplus;}
\( {return tokopen;}
\) {return tokclose;}
[a-zA-Z0-9] {yylvalp->tokchar = *yytext; return tokchar;}
\. {return tokdot;}
\- {return tokdash;}
<<EOF>> {return YYEOF;}
\n {return YYEOF;}
. {return YYUNDEF;}

%%

static void write_reg(FILE* restrict out, reg_nfa const* restrict r) {
	_Bool* table = malloc((sizeof *table) * r->table.node_cnt);
	fputs("+----++", out);
	for (size_t i = 0; i < (1 << CHAR_BIT); ++i) fputs("-----+", out);
	fputs("\n|  T ||", out);
	for (size_t i = 0; i < (1 << CHAR_BIT); ++i) {
		if (i >= 32 && i <= 126) fprintf(out, "  %c  |", (char)i);
		else fprintf(out, "<%3zu>|", i);
	}
	fputs("\n+----++", out);
	for (size_t i = 0; i < (1 << CHAR_BIT); ++i) fputs("-----+", out);
	fputs("\n+----++", out);
	for (size_t i = 0; i < (1 << CHAR_BIT); ++i) fputs("-----+", out);
	for (size_t i = 0; i < r->table.node_cnt; ++i) {
		fprintf(out, "\n|%c%3zu||", r->specials.end == i ? '+' : (r->specials.start == i ? 'S' : ' '), i);
		for (size_t j = 0; j < (1 << CHAR_BIT); ++j) {
			if (r->table.nodes[i].tran[j] == UINT_MAX) {
				fprintf(out, "  E  |");
			} else {
				fprintf(out, " %3u |", r->table.nodes[i].tran[j]);
			}
		}
		fputs("\n+----++", out);
		for (size_t i = 0; i < (1 << CHAR_BIT); ++i) fputs("-----+", out);
	}
	fputs("\n\n+----++", out);
	for (size_t i = 0; i < r->table.node_cnt; ++i) fputs("----+", out);
	fputs("\n|  L ||", out);
	for (size_t i = 0; i < r->table.node_cnt; ++i) {
		fprintf(out, "%c%3zu|", r->specials.end == i ? '+' : (r->specials.start == i ? 'S' : ' '), i);
	}
	fputs("\n+----++", out);
	for (size_t i = 0; i < r->table.node_cnt; ++i) fputs("----+", out);
	fputs("\n+----++", out);
	for (size_t i = 0; i < r->table.node_cnt; ++i) fputs("----+", out);
	for (size_t i = 0; i < r->table.node_cnt; ++i) {
		for (size_t j = 0; j < r->table.node_cnt; ++j) table[j] = 0;
		for (size_t j = 0; j < r->table.nodes[i].lambda.cnt; ++j) table[r->table.nodes[i].lambda.paths[j]] = 1;
		fprintf(out, "\n|%c%3zu||", r->specials.end == i ? '+' : (r->specials.start == i ? 'S' : ' '), i);
		for (size_t j = 0; j < r->table.node_cnt; ++j) {
			fprintf(out, "  %c |", table[j] ? '*' : ' ');
		}
		fputs("\n+----++", out);
		for (size_t i = 0; i < r->table.node_cnt; ++i) fputs("----+", out);
	}
	fputc('\n', out);
}

extern int yyparse(reg_nfa* out, yyscan_t scanner);

int main(void) {
	yyscan_t scanner;
	yylex_init(&scanner);
	yyset_in(stdin, scanner);
	reg_nfa out;
	out.table.nodes = malloc((sizeof *(out.table.nodes)) * 16);
	out.table.node_cnt = 0;
	out.table.node_cap = 16;
	int res = yyparse(&out, scanner);
	yylex_destroy(scanner);
	if (res == 0) {
		write_reg(stdout, &out);
		fputc('\n', stdout);
	}
	return res;
}
