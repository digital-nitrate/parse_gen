#include "gen_lex.yy.h"
#include "hash.h"
#include "generator.h"

int gen_bld(FILE* restrict src, gen_type* restrict gen) {
	gen->prologue = malloc((sizeof *(gen->prologue)) * 16);
	gen->prologue_cnt = 0;
	gen->prologue_cap = 16;
	gen->tokens = malloc((sizeof *(gen->tokens)) * 16);
	gen->token_cnt = 0;
	gen->token_cap = 16;
	gen->nterms = malloc((sizeof *(gen->nterms)) * 16);
	gen->nterm_cnt = 0;
	gen->nterm_cap = 16;
	gen->params = malloc((sizeof *(gen->params)) * 16);
	gen->param_cnt = 0;
	gen->param_cap = 16;
	gen->rules = malloc((sizeof *(gen->rules)) * 16);
	gen->rule_cnt = 0;
	gen->rule_cap = 16;
	gen->prefix.data = NULL;
	gen->start.error = 1;
	
	struct hash_bin* bins = malloc((sizeof *bins) * 19);
	if (bins == NULL) return 2;
	for (size_t i = 0; i < 19; ++i) bins[i].index.error = 1;
	struct hash_table table = {.bins=bins,.bcnt=19,.ecnt=0,};
	yyscan_t scanner;
	yylex_init(&scanner);
	yyset_in(src, scanner);
	int res = yyparse(gen, &table, scanner);
	yylex_destroy(scanner);
	return res;
}
