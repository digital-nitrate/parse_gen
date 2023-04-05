%define api.pure full
%define parse.error verbose
%locations

%code requires {
	#include "hash.h"
}

%code {
	#include <stdlib.h>
	#include <limits.h>
	#include <string.h>
	#include "gen_lex.yy.h"
	#include "generator.h"
	#include "macros.h"
	static char const api_str[] = "api";
	static size_t const api_len = (sizeof api_str) - 1;
	static char const prefix_str[] = "prefix";
	static size_t const prefix_len = (sizeof prefix_str) - 1;
	static void yyerror(YYLTYPE*, gen_type*, struct hash_table*, yyscan_t, char const*);
	static int init_gen(gen_type*);
}

%define api.value.type union

%parse-param {gen_type* out}
%parse-param {struct hash_table* hash}
%parse-param {void* scanner}
%lex-param {void* scanner}

%initial-action {
	init_gen(out);
	hash_init(hash);
}

%token <struct string> PROBLOCK
%token TOKENDECL
%token NTERMDECL
%token STARTDECL
%token DEFINEDECL
%token PARAMDECL
%token DOT
%token <struct string> IDEN
%token <struct string> TYPESPEC
%token <unsigned int> UINT
%token EMPTYDECL
%token COL
%token MID
%token SCL
%token LBR
%token RBR
%token <struct string> RAW
%token <unsigned int> SUBREF
%token SELFREF
%token SECSEP
%token LEXERR

%nterm program
%nterm declaration
%nterm decl
%nterm <struct string> typeopt
%nterm rules
%nterm <struct {unsigned int low; unsigned int high;}> patterns
%nterm <gen_slist> slist
%nterm <gen_act*> actopt
%nterm <gen_act*> code
%nterm epilogue

%destructor {
	free($$.data);
} PROBLOCK IDEN TYPESPEC RAW typeopt

%destructor {
	free($$.syms);
} slist

%destructor {
	gen_act* curr = $$;
	while (curr != NULL) {
		gen_act* tmp = curr;
		curr = tmp->next;
		if (tmp->type == GEN_CODE_RAW) free(tmp->data.data);
		free(tmp);
	}
} actopt code

%%

program:
	declaration
	| declaration SECSEP rules
	| declaration SECSEP rules SECSEP epilogue
	;

declaration:
	%empty
	| declaration decl
	;

decl:
	PROBLOCK {
		int res = 0;
		DYNARR_CHK(out->prologue_cnt, out->prologue_cap, out->prologue, res);
		if (res) {fprintf(stderr, "memory failure\n");YYERROR;}
		out->prologue[out->prologue_cnt] = $1;
		++(out->prologue_cnt);
	}
	| TOKENDECL typeopt IDEN UINT {
		gen_ref* ar = malloc((sizeof *ar) * 16);
		if (ar == NULL) {
			fputs("memory failure\n", stderr);
			YYERROR;
		}
		gen_sid sym = hash_add(out, hash, $3, 1);
		if (sym.error) {
			if (sym.term) {
				fputs("memory failure\n", stderr);
			} else {
				fprintf(stderr, "%d:%d->%d:%d$ symbol already defined\n", @3.first_line, @3.first_column, @3.last_line, @3.last_column);
			}
			free(ar);
			YYERROR;
		}
		out->tokens[sym.ind].type = $2;
		out->tokens[sym.ind].id = $4;
		out->tokens[sym.ind].rh = (gen_rarr){.rules=ar,.cnt=0,.cap=16};
	}
	| NTERMDECL typeopt IDEN {
		gen_ref* ar = malloc((sizeof *ar) * 16);
		if (ar == NULL) {
			fputs("memory failure\n", stderr);
			YYERROR;
		}
		gen_ref* al = malloc((sizeof *al) * 16);
		if (al == NULL) {
			fputs("memory failure\n", stderr);
			free(ar);
			YYERROR;
		}
		gen_sid sym = hash_add(out, hash, $3, 0);
		if (sym.error) {
			if (sym.term) {
				fputs("memory failure\n", stderr);
			} else {
				fprintf(stderr, "%d:%d->%d:%d$ symbol already defined\n", @3.first_line, @3.first_column, @3.last_line, @3.last_column);
			}
			free(al);
			free(ar);
			YYERROR;
		}
		out->nterms[sym.ind].type = $2;
		out->nterms[sym.ind].rh = (gen_rarr){.rules=ar,.cnt=0,.cap=16};
		out->nterms[sym.ind].lh = (gen_rarr){.rules=al,.cnt=0,.cap=16};
	}
	| STARTDECL IDEN {
		if (out->start.error == 0) {
			fprintf(stderr, "%d:%d->%d:%d$ start already set\n", @1.first_line, @1.first_column, @1.last_line, @1.last_column);
			YYERROR;
		}
		gen_sid sym = hash_ld(out, hash, $2);
		if (sym.error) {
			fprintf(stderr, "%d:%d->%d:%d$ no symbol found\n", @2.first_line, @2.first_column, @2.last_line, @2.last_column);
			YYERROR;
		}
		if (sym.term) {
			fprintf(stderr, "%d:%d->%d:%d$ expected nonterminal\n", @2.first_line, @2.first_column, @2.last_line, @2.last_column);
			YYERROR;
		}
		free($2.data);
		out->start = sym;
	}
	| DEFINEDECL IDEN DOT IDEN IDEN {
		if (out->prefix.data != NULL || $2.len != api_len || memcmp(api_str, $2.data, api_len) != 0 || $4.len != prefix_len || memcmp(prefix_str, $4.data, prefix_len) != 0) {
			fprintf(stderr, "%d:%d->%d:%d$ unknown name\n", @2.first_line, @2.first_column, @4.last_line, @4.last_column);
			YYERROR;
		}
		free($2.data);
		free($4.data);
		out->prefix = $5;
	}
	| PARAMDECL TYPESPEC IDEN {
		int res = 0;
		DYNARR_CHK(out->param_cnt, out->param_cap, out->params, res);
		if (res) {
			fprintf(stderr, "memory failure\n");
			YYERROR;
		}
		out->params[out->param_cnt].type = $2;
		out->params[out->param_cnt].name = $3;
		++(out->param_cnt);
	}
	;

typeopt:
	%empty {$$ = (struct string){.data = NULL, .len = 0};}
	| TYPESPEC {$$ = $1;}
	;

rules:
	%empty
	| rules IDEN COL patterns SCL {
		gen_sid sym = hash_ld(out, hash, $2);
		if (sym.error || sym.term) {YYERROR;}
		gen_rarr* r = &(out->nterms[sym.ind].lh);
		for (size_t i = $4.low; i < $4.high; ++i) {
			int res = 0;
			DYNARR_CHK(r->cnt, r->cap, r->rules, res);
			if (res) {
				fprintf(stderr, "memory failure\n");
				YYERROR;
			}
			r->rules[r->cnt] = (gen_ref){.rule=i,.loc=out->rules[i].rhs.cnt};
			++(r->cnt);
			out->rules[i].lhs = sym;
		}
		free($2.data);
	}

patterns:
	slist actopt {
		int res = 0;
		DYNARR_CHK(out->rule_cnt, out->rule_cap, out->rules, res);
		if (res) {
			fprintf(stderr, "memory failure\n");
			YYERROR;
		}
		for (size_t i = 0; i < $1.cnt; ++i) {
			gen_rarr* r = $1.syms[i].term ? &(out->tokens[$1.syms[i].ind].rh) : &(out->nterms[$1.syms[i].ind].rh);
			DYNARR_CHK(r->cnt, r->cap, r->rules, res);
			if (res) {
				fprintf(stderr, "memory failure\n");
				YYERROR;
			}
			r->rules[r->cnt] = (gen_ref){.rule=out->rule_cnt,.loc=i};
			++(r->cnt);
		}
		out->rules[out->rule_cnt].act = $2;
		out->rules[out->rule_cnt].rhs = $1;
		$$.low = out->rule_cnt;
		$$.high = $$.low + 1;
		++(out->rule_cnt);
	}
	| patterns MID slist actopt {
		int res = 0;
		DYNARR_CHK(out->rule_cnt, out->rule_cap, out->rules, res);
		if (res) {
			fprintf(stderr, "memory failure\n");
			YYERROR;
		}
		for (size_t i = 0; i < $3.cnt; ++i) {
			gen_rarr* r = $3.syms[i].term ? &(out->tokens[$3.syms[i].ind].rh) : &(out->nterms[$3.syms[i].ind].rh);
			DYNARR_CHK(r->cnt, r->cap, r->rules, res);
			if (res) {
				fprintf(stderr, "memory failure\n");
				YYERROR;
			}
			r->rules[r->cnt] = (gen_ref){.rule=out->rule_cnt,.loc=i};
			++(r->cnt);
		}
		out->rules[out->rule_cnt].act = $4;
		out->rules[out->rule_cnt].rhs = $3;
		$$.low = $1.low;
		$$.high = $1.high + 1;
		++(out->rule_cnt);
	}

slist:
	%empty {
		gen_sid* syms = malloc((sizeof *syms) * 16);
		if (syms == NULL) {YYERROR;}
		$$.syms = syms;
		$$.cnt = 0;
		$$.cap = 16;
	}
	| EMPTYDECL {
		gen_sid* syms = malloc((sizeof *syms) * 16);
		if (syms == NULL) {YYERROR;}
		$$.syms = syms;
		$$.cnt = 0;
		$$.cap = 16;
	}
	| slist IDEN {
		gen_sid sym = hash_ld(out, hash, $2);
		if (sym.error) {YYERROR;}
		int res = 0;
		DYNARR_CHK($1.cnt, $1.cap, $1.syms, res);
		if (res) {
			fprintf(stderr, "memory failure\n");
			YYERROR;
		}
		$1.syms[$1.cnt] = sym;
		++($1.cnt);
		$$ = $1;
		free($2.data);
	}

actopt:
	%empty {$$ = NULL;}
	| LBR code RBR {$$ = $2;}

code:
	%empty {$$ = NULL;}
	| RAW code {
		gen_act* node = malloc(sizeof *node);
		if (node == NULL) {YYERROR;}
		node->next = $2;
		node->data = $1;
		node->type = GEN_CODE_RAW;
		$$ = node;
	}
	| SUBREF code {
		gen_act* node = malloc(sizeof *node);
		if (node == NULL){YYERROR;}
		node->next = $2;
		node->index = $1;
		node->type = GEN_CODE_COMP;
		$$ = node;
	}
	| SELFREF code {
		gen_act* node = malloc(sizeof *node);
		if (node == NULL) {YYERROR;}
		node->next = $2;
		node->type = GEN_CODE_SELF;
		$$ = node;
	}

epilogue:
	%empty
	| RAW {out->epilogue = $1;}

%%

void yyerror(YYLTYPE* locp, gen_type* out, struct hash_table* hash, yyscan_t scanner, char const* msg) {
	(void)out;
	(void)hash;
	(void)scanner;
	fprintf(stderr, "%d:%d->%d:%d$ %s\n", locp->first_line, locp->first_column, locp->last_line, locp->last_column, msg);
}

int init_gen(gen_type* gen) {
	gen->prologue = malloc((sizeof *(gen->prologue)) * 16);
	if (gen->prologue == NULL) goto Error;
	gen->tokens = malloc((sizeof *(gen->tokens)) * 16);
	if (gen->tokens == NULL) goto CleanPrologue;
	gen->nterms = malloc((sizeof *(gen->nterms)) * 16);
	if (gen->nterms == NULL) goto CleanTokens;
	gen->params = malloc((sizeof *(gen->params)) * 16);
	if (gen->params == NULL) goto CleanNterms;
	gen->rules = malloc((sizeof *(gen->rules)) * 16);
	if (gen->rules == NULL) goto CleanParams;
	gen->prologue_cnt = 0;
	gen->prologue_cap = 16;
	gen->token_cnt = 0;
	gen->token_cap = 16;
	gen->nterm_cnt = 0;
	gen->nterm_cap = 16;
	gen->param_cnt = 0;
	gen->param_cap = 16;
	gen->rule_cnt = 0;
	gen->rule_cap = 16;
	gen->prefix.data = NULL;
	gen->epilogue.data = NULL;
	gen->start.error = 1;
	return 0;
	CleanParams: free(gen->params);
	CleanNterms: free(gen->nterms);
	CleanTokens: free(gen->tokens);
	CleanPrologue: free(gen->prologue);
	Error: return 1;
}

int gen_bld(FILE* restrict src, gen_type* restrict gen) {
	struct hash_table table;
	yyscan_t scanner;
	yylex_init(&scanner);
	yyset_in(src, scanner);
	int res = yyparse(gen, &table, scanner);
	hash_fini(&table);
	yylex_destroy(scanner);
	return res;
}
