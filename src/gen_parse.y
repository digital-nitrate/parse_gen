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
	#include <ctype.h>
	#include "gen_lex.yy.h"
	#include "generator.h"
	#include "macros.h"
	static void yyerror(YYLTYPE*, gen_type*, struct hash_table*, yyscan_t, char const*);
	static int dup_act(gen_act* restrict, gen_act const* restrict);
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

%token LEXERR
%token NL
%token SECSEP
%token TOKENDECL
%token NTERMDECL
%token STARTDECL
%token PREFIXDECL
%token PPARAMDECL
%token LPARAMDECL
%token CODETOPDECL
%token CODEREQDECL
%token CODEPROVDECL
%token DESTRUCTDECL
%token CODEDECL
%token CODEMERR
%token <char*> IDEN
%token <char*> TYPESPEC
%token <unsigned int> UINT
%token EMPTYDECL
%token COL
%token MID
%token SCL
%token LBR
%token RBR
%token <char*> RAW
%token <unsigned int> VALREF

%nterm program
%nterm declaration
%nterm decl
%nterm <char*> typeopt
%nterm rules
%nterm <struct {unsigned int low; unsigned int high;}> patterns
%nterm <gen_slist> slist
%nterm <gen_act> actopt
%nterm <struct {gen_act act; size_t cap;}> code
%nterm epilogue
%nterm <struct {gen_sid* ids; size_t len; size_t cap;}> idenlist

%destructor {
	free($$);
} IDEN TYPESPEC RAW typeopt

%destructor {
	free($$.syms);
} slist

%destructor {
	struct gen_code_unit const* end = $$.acts + $$.len;
	for (struct gen_code_unit* curr = $$.acts; curr != end; ++curr) {
		switch (curr->type) {
			case GEN_CODE_MERR:
			case GEN_CODE_COMP:
				break;
			case GEN_CODE_RAW:
				free(curr->data);
				break;
		}
	}
	free($$.acts);
} actopt

%destructor {
	struct gen_code_unit const* end = $$.act.acts + $$.act.len;
	for (struct gen_code_unit* curr = $$.act.acts; curr != end; ++curr) {
		switch (curr->type) {
			case GEN_CODE_MERR:
			case GEN_CODE_COMP:
				break;
			case GEN_CODE_RAW:
				free(curr->data);
				break;
		}
	}
	free($$.act.acts);
} code

%%

program:
	declaration
	| declaration SECSEP rules
	| declaration SECSEP rules SECSEP epilogue
	;

declaration:
	%empty
	| declaration decl NL
	;

decl:
	%empty
	| CODETOPDECL LBR code RBR {
		if (out->top.acts != NULL) {
			fputs("codetop already set\n", stderr);
			YYERROR;
		}
		out->top = $3.act;
	}
	| CODEREQDECL LBR code RBR {
		if (out->req.acts != NULL) {
			fputs("coderequires already set\n", stderr);
			YYERROR;
		}
		out->req = $3.act;
	}
	| CODEPROVDECL LBR code RBR {
		if (out->prov.acts != NULL) {
			fputs("codeprovides already set\n", stderr);
			YYERROR;
		}
		out->prov = $3.act;
	}
	| CODEDECL LBR code RBR {
		if (out->code.acts != NULL) {
			fputs("code already set\n", stderr);
			YYERROR;
		}
		out->code = $3.act;
	}
	| DESTRUCTDECL LBR code RBR idenlist {
		gen_sid const* end = $5.ids + $5.len;
		for (gen_sid const* curr = $5.ids; curr != end; ++curr) {
			gen_act* dest = curr->term ? &(out->tokens[curr->ind].des) : &(out->nterms[curr->ind].des);
			if (dest->acts != NULL) {YYERROR;}
			if (dup_act(dest, &($3.act))) {YYERROR;}
		}
		free($5.ids);
	}
	| TOKENDECL typeopt IDEN UINT {
		gen_ref* ar = malloc((sizeof *ar) * 16);
		if (ar == NULL) {
			fputs("memory failure\n", stderr);
			free($2);
			free($3);
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
			free($2);
			free($3);
			YYERROR;
		}
		out->tokens[sym.ind].type = $2;
		out->tokens[sym.ind].id = $4;
		out->tokens[sym.ind].rh = (gen_rarr){.rules=ar,.cnt=0,.cap=16};
		out->tokens[sym.ind].des.acts = NULL;
	}
	| NTERMDECL typeopt IDEN {
		gen_ref* ar = malloc((sizeof *ar) * 16);
		if (ar == NULL) {
			fputs("memory failure\n", stderr);
			free($2);
			free($3);
			YYERROR;
		}
		gen_ref* al = malloc((sizeof *al) * 16);
		if (al == NULL) {
			fputs("memory failure\n", stderr);
			free(ar);
			free($2);
			free($3);
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
			free($2);
			free($3);
			YYERROR;
		}
		out->nterms[sym.ind].type = $2;
		out->nterms[sym.ind].rh = (gen_rarr){.rules=ar,.cnt=0,.cap=16};
		out->nterms[sym.ind].lh = (gen_rarr){.rules=al,.cnt=0,.cap=16};
		out->nterms[sym.ind].des.acts = NULL;
	}
	| STARTDECL IDEN {
		if (out->start.error == 0) {
			fprintf(stderr, "%d:%d->%d:%d$ start already set\n", @1.first_line, @1.first_column, @1.last_line, @1.last_column);
			free($2);
			YYERROR;
		}
		gen_sid sym = hash_ld(out, hash, $2);
		free($2);
		if (sym.error) {
			fprintf(stderr, "%d:%d->%d:%d$ no symbol found\n", @2.first_line, @2.first_column, @2.last_line, @2.last_column);
			YYERROR;
		}
		if (sym.term) {
			fprintf(stderr, "%d:%d->%d:%d$ expected nonterminal\n", @2.first_line, @2.first_column, @2.last_line, @2.last_column);
			YYERROR;
		}
		out->start = sym;
	}
	| PREFIXDECL IDEN {
		if (out->prefixlo != NULL || out->prefixhi != NULL) {
			fprintf(stderr, "%d:%d->%d:%d$ prefix already set\n", @1.first_line, @1.first_column, @2.last_line, @2.last_column);
			free($2);
			YYERROR;
		}
		size_t len = strlen($2);
		char* lo = malloc(len + 1);
		if (lo == NULL) {
			fprintf(stderr, "memory failure\n");
			free($2);
			YYERROR;
		}
		char* hi = malloc(len + 1);
		if (hi == NULL) {
			fprintf(stderr, "memory failure\n");
			free(lo);
			free($2);
			YYERROR;
		}
		for (size_t i = 0; i < len; ++i) {
			lo[i] = tolower($2[i]);
			hi[i] = toupper($2[i]);
		}
		free($2);
		lo[len] = '\0';
		hi[len] = '\0';
		out->prefixlo = lo;
		out->prefixhi = hi;
	}
	| PPARAMDECL TYPESPEC IDEN {
		int res = 0;
		DYNARR_CHK(out->pparam_cnt, out->pparam_cap, out->pparams, res);
		if (res) {
			fprintf(stderr, "memory failure\n");
			free($2);
			free($3);
			YYERROR;
		}
		out->pparams[out->pparam_cnt].type = $2;
		out->pparams[out->pparam_cnt].name = $3;
		++(out->pparam_cnt);
	}
	| LPARAMDECL TYPESPEC IDEN {
		int res = 0;
		DYNARR_CHK(out->lparam_cnt, out->lparam_cap, out->lparams, res);
		if (res) {
			fprintf(stderr, "memory failure\n");
			free($2);
			free($3);
			YYERROR;
		}
		out->lparams[out->lparam_cnt].type = $2;
		out->lparams[out->lparam_cnt].name = $3;
		++(out->lparam_cnt);
	}
	;

idenlist:
	%empty {
		$$.ids = malloc((sizeof *($$.ids)) * 16);
		if ($$.ids == NULL) {YYERROR;}
		$$.len = 0;
		$$.cap = 16;
	}
	| idenlist IDEN {
		int res = 0;
		DYNARR_CHK($1.len, $1.cap, $1.ids, res);
		if (res) {YYERROR;}
		gen_sid id = hash_ld(out, hash, $2);
		if (id.error) {YYERROR;}
		$1.ids[$1.len] = id;
		++($1.len);
		$$.ids = $1.ids;
		$$.len = $1.len;
		$$.cap = $1.cap;
	}
	;

typeopt:
	%empty {$$ = NULL;}
	| TYPESPEC {$$ = $1;}
	;

rules:
	%empty
	| rules IDEN COL patterns SCL {
		gen_sid sym = hash_ld(out, hash, $2);
		free($2);
		if (sym.error) {
			fprintf(stderr, "%d:%d->%d:%d$ no symbol found\n", @2.first_line, @2.first_column, @2.last_line, @2.last_column);
			YYERROR;
		}
		if (sym.term) {
			fprintf(stderr, "%d:%d->%d:%d$ expected nonterminal\n", @2.first_line, @2.first_column, @2.last_line, @2.last_column);
			YYERROR;
		}
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
	}
	;

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
	;

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
		free($2);
		if (sym.error) {
			fprintf(stderr, "%d:%d->%d:%d$ no symbol found\n", @2.first_line, @2.first_column, @2.last_line, @2.last_column);
			YYERROR;
		}
		int res = 0;
		DYNARR_CHK($1.cnt, $1.cap, $1.syms, res);
		if (res) {
			fprintf(stderr, "memory failure\n");
			YYERROR;
		}
		$1.syms[$1.cnt] = sym;
		++($1.cnt);
		$$ = $1;
	}
	;

actopt:
	%empty {$$.acts = NULL;}
	| LBR code RBR {$$ = $2.act;}
	;

code:
	%empty {
		struct gen_code_unit* acts = malloc((sizeof *acts) * 16);
		if (acts == NULL) {YYERROR;}
		$$.act.acts = acts;
		$$.act.len = 0;
		$$.act.loc.first_line = @$.first_line;
		$$.act.loc.last_line = @$.last_line;
		$$.cap = 16;
	}
	| code RAW {
		int res = 0;
		DYNARR_CHK($1.act.len, $1.cap, $1.act.acts, res);
		if (res) {YYERROR;}
		$1.act.acts[$1.act.len].type = GEN_CODE_RAW;
		$1.act.acts[$1.act.len].data = $2;
		++($1.act.len);
		$1.act.loc.first_line = @$.first_line;
		$1.act.loc.last_line = @$.last_line;
		$$.act = $1.act;
		$$.cap = $1.cap;
	}
	| code VALREF {
		int res = 0;
		DYNARR_CHK($1.act.len, $1.cap, $1.act.acts, res);
		if (res) {YYERROR;}
		$1.act.acts[$1.act.len].type = GEN_CODE_COMP;
		$1.act.acts[$1.act.len].index = $2;
		++($1.act.len);
		$1.act.loc.first_line = @$.first_line;
		$1.act.loc.last_line = @$.last_line;
		$$.act = $1.act;
		$$.cap = $1.cap;
	}
	| code CODEMERR {
		int res = 0;
		DYNARR_CHK($1.act.len, $1.cap, $1.act.acts, res);
		if (res) {YYERROR;}
		$1.act.acts[$1.act.len].type = GEN_CODE_MERR;
		++($1.act.len);
		$1.act.loc.first_line = @$.first_line;
		$1.act.loc.last_line = @$.last_line;
		$$.act = $1.act;
		$$.cap = $1.cap;
	}
	;

epilogue:
	%empty
	| RAW {out->epilogue = $1;out->epilogue_loc.first_line = @1.first_line;out->epilogue_loc.last_line = @1.last_line;}
	;

%%

int dup_act(gen_act* restrict dest, gen_act const* restrict src) {
	struct gen_code_unit* cact = malloc((sizeof *cact) * src->len);
	if (cact == NULL) return 1;
	for (size_t i = 0; i < src->len; ++i) {
		switch (src->acts[i].type) {
			case GEN_CODE_COMP:
				cact[i].type = GEN_CODE_COMP;
				cact[i].index = src->acts[i].index;
				break;
			case GEN_CODE_MERR:
				cact[i].type = GEN_CODE_MERR;
				break;
			case GEN_CODE_RAW: {
				size_t ln = strlen(src->acts[i].data);
				char* cdat = malloc(ln + 1);
				if (cdat == NULL) return 1;
				memcpy(cdat, src->acts[i].data, ln + 1);
				cact[i].type = GEN_CODE_RAW;
				cact[i].data = cdat;
				break;
			}
		}
	}
	dest->acts = cact;
	dest->len = src->len;
	dest->loc = src->loc;
	return 0;
}

void yyerror(YYLTYPE* locp, gen_type* out, struct hash_table* hash, yyscan_t scanner, char const* msg) {
	(void)out;
	(void)hash;
	(void)scanner;
	fprintf(stderr, "%d:%d->%d:%d$ %s\n", locp->first_line, locp->first_column, locp->last_line, locp->last_column, msg);
}

int init_gen(gen_type* gen) {
	gen->tokens = malloc((sizeof *(gen->tokens)) * 16);
	if (gen->tokens == NULL) goto Error;
	gen->nterms = malloc((sizeof *(gen->nterms)) * 16);
	if (gen->nterms == NULL) goto CleanTokens;
	gen->pparams = malloc((sizeof *(gen->pparams)) * 16);
	if (gen->pparams == NULL) goto CleanNterms;
	gen->lparams = malloc((sizeof *(gen->lparams)) * 16);
	if (gen->lparams == NULL) goto CleanPParams;
	gen->rules = malloc((sizeof *(gen->rules)) * 16);
	if (gen->rules == NULL) goto CleanParams;
	gen->top.acts = NULL;
	gen->req.acts = NULL;
	gen->prov.acts = NULL;
	gen->code.acts = NULL;
	gen->token_cnt = 0;
	gen->token_cap = 16;
	gen->nterm_cnt = 0;
	gen->nterm_cap = 16;
	gen->pparam_cnt = 0;
	gen->pparam_cap = 16;
	gen->lparam_cnt = 0;
	gen->lparam_cap = 16;
	gen->rule_cnt = 0;
	gen->rule_cap = 16;
	gen->prefixlo = NULL;
	gen->prefixhi = NULL;
	gen->epilogue = NULL;
	gen->start.error = 1;
	return 0;
	CleanParams: free(gen->lparams);
	CleanPParams: free(gen->pparams);
	CleanNterms: free(gen->nterms);
	CleanTokens: free(gen->tokens);
	Error: return 1;
}

int gen_bld(char const* restrict srcpath, gen_type* restrict gen) {
	FILE* src = fopen(srcpath, "rb");
	if (src == NULL) return 1;
	gen->fname = srcpath;
	struct hash_table table;
	yyscan_t scanner;
	yylex_init(&scanner);
	yyset_in(src, scanner);
	int res = yyparse(gen, &table, scanner);
	hash_fini(&table);
	yylex_destroy(scanner);
	fclose(src);
	return res;
}
