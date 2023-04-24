%define api.pure full
%define parse.error verbose
%locations

%code requires {
	#include <stdint.h>
	#include "generator.h"

	struct hash_bin {
		uintmax_t hval;
		gen_sid index;
	};

	struct hash_table {
		struct hash_bin* bins;
		size_t bcnt;
		size_t ecnt;
	};

	struct cap_track {
		size_t token_cap;
		size_t nterm_cap;
		size_t pparam_cap;
		size_t lparam_cap;
		size_t rule_cap;
	};
}

%code {
	#include <stdlib.h>
	#include <limits.h>
	#include <string.h>
	#include <ctype.h>
	#include "gen_lex.yy.h"
	#include "macros.h"
	#define ERR_PRINT(loc, format, ...) fprintf(stderr, "%d:%d->%d:%d$"format"\n", (loc).first_line, (loc).first_column, (loc).last_line, (loc).last_column, ##__VA_ARGS__)
	#define ERR_MEM fputs("memory failure\n", stderr)
	static void yyerror(YYLTYPE*, gen_type*, struct hash_table*, struct cap_track*, yyscan_t, char const*);
	static int dup_act(gen_act* restrict, gen_act const* restrict);
	static int init_gen(gen_type* restrict, struct cap_track* restrict);
	static int hash_init(struct hash_table*);
	static gen_sid hash_ld(gen_type const* restrict, struct hash_table const* restrict, char const* restrict);
	static gen_sid hash_add(gen_type* restrict, struct hash_table* restrict, struct cap_track* restrict, char* restrict, _Bool);
}

%define api.value.type union

%parse-param {gen_type* out}
%parse-param {struct hash_table* hash}
%parse-param {struct cap_track* caps}
%parse-param {void* scanner}
%lex-param {void* scanner}

%initial-action {
	init_gen(out, caps);
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
%nterm <struct {gen_rind low; gen_rind high;}> patterns
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
	gen_act_fini(&($$));
} actopt

%destructor {
	gen_act_fini(&($$.act));
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
			gen_act_fini(&($3.act));
			ERR_PRINT(@1, "codetop already set");
			YYERROR;
		}
		out->top = $3.act;
	}
	| CODEREQDECL LBR code RBR {
		if (out->req.acts != NULL) {
			gen_act_fini(&($3.act));
			ERR_PRINT(@1, "coderequires already set");
			YYERROR;
		}
		out->req = $3.act;
	}
	| CODEPROVDECL LBR code RBR {
		if (out->prov.acts != NULL) {
			gen_act_fini(&($3.act));
			ERR_PRINT(@1, "codeprovides already set");
			YYERROR;
		}
		out->prov = $3.act;
	}
	| CODEDECL LBR code RBR {
		if (out->code.acts != NULL) {
			gen_act_fini(&($3.act));
			ERR_PRINT(@1, "code already set");
			YYERROR;
		}
		out->code = $3.act;
	}
	| DESTRUCTDECL LBR code RBR idenlist {
		gen_sid const* end = $5.ids + $5.len;
		for (gen_sid const* curr = $5.ids; curr != end; ++curr) {
			gen_act* dest = curr->term ? &(out->tokens[curr->ind].des) : &(out->nterms[curr->ind].des);
			if (dest->acts != NULL) {
				free($5.ids);
				ERR_PRINT(@5, "symbol \"%s\" destructor already set", curr->term ? out->tokens[curr->ind].name : out->nterms[curr->ind].name);
				YYERROR;
			}
			if (dup_act(dest, &($3.act))) {
				free($5.ids);
				ERR_MEM;
				YYERROR;
			}
		}
		free($5.ids);
	}
	| TOKENDECL typeopt IDEN UINT {
		gen_ref* ar = malloc((sizeof *ar) * 16);
		if (ar == NULL) {
			free($2);
			free($3);
			ERR_MEM;
			YYERROR;
		}
		gen_sid sym = hash_add(out, hash, caps, $3, 1);
		if (sym.error) {
			free(ar);
			free($2);
			free($3);
			if (sym.term) {
				ERR_MEM;
			} else {
				ERR_PRINT(@3, "symbol already declared");
			}
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
			free($2);
			free($3);
			ERR_MEM;
			YYERROR;
		}
		gen_ref* al = malloc((sizeof *al) * 16);
		if (al == NULL) {
			free(ar);
			free($2);
			free($3);
			ERR_MEM;
			YYERROR;
		}
		gen_sid sym = hash_add(out, hash, caps, $3, 0);
		if (sym.error) {
			free(al);
			free(ar);
			free($2);
			free($3);
			if (sym.term) {
				ERR_MEM;
			} else {
				ERR_PRINT(@3, "symbol already declared");
			}
			YYERROR;
		}
		out->nterms[sym.ind].type = $2;
		out->nterms[sym.ind].rh = (gen_rarr){.rules=ar,.cnt=0,.cap=16};
		out->nterms[sym.ind].lh = (gen_rarr){.rules=al,.cnt=0,.cap=16};
		out->nterms[sym.ind].des.acts = NULL;
	}
	| STARTDECL IDEN {
		if (out->start.error == 0) {
			free($2);
			ERR_PRINT(@1, "start already set");
			YYERROR;
		}
		gen_sid sym = hash_ld(out, hash, $2);
		free($2);
		if (sym.error) {
			ERR_PRINT(@2, "symbol not found");
			YYERROR;
		}
		if (sym.term) {
			ERR_PRINT(@2, "expected nonterminal");
			YYERROR;
		}
		out->start = sym;
	}
	| PREFIXDECL IDEN {
		if (out->prefixlo != NULL || out->prefixhi != NULL) {
			free($2);
			ERR_PRINT(@1, "prefix already set");
			YYERROR;
		}
		size_t len = strlen($2);
		char* lo = malloc(len + 1);
		if (lo == NULL) {
			free($2);
			ERR_MEM;
			YYERROR;
		}
		char* hi = malloc(len + 1);
		if (hi == NULL) {
			free(lo);
			free($2);
			ERR_MEM;
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
		DYNARR_CHK(out->pparam_cnt, caps->pparam_cap, out->pparams, res);
		if (res) {
			free($2);
			free($3);
			ERR_MEM;
			YYERROR;
		}
		out->pparams[out->pparam_cnt].type = $2;
		out->pparams[out->pparam_cnt].name = $3;
		++(out->pparam_cnt);
	}
	| LPARAMDECL TYPESPEC IDEN {
		int res = 0;
		DYNARR_CHK(out->lparam_cnt, caps->lparam_cap, out->lparams, res);
		if (res) {
			free($2);
			free($3);
			ERR_MEM;
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
		if ($$.ids == NULL) {
			ERR_MEM;
			YYERROR;
		}
		$$.len = 0;
		$$.cap = 16;
	}
	| idenlist IDEN {
		int res = 0;
		DYNARR_CHK($1.len, $1.cap, $1.ids, res);
		if (res) {
			free($2);
			ERR_MEM;
			YYERROR;
		}
		gen_sid id = hash_ld(out, hash, $2);
		free($2);
		if (id.error) {
			ERR_PRINT(@2, "symbol not found");
			YYERROR;
		}
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
			ERR_PRINT(@2, "symbol not found");
			YYERROR;
		}
		if (sym.term) {
			ERR_PRINT(@2, "expected nonterminal");
			YYERROR;
		}
		gen_rarr* r = &(out->nterms[sym.ind].lh);
		for (size_t i = $4.low; i < $4.high; ++i) {
			int res = 0;
			DYNARR_CHK(r->cnt, r->cap, r->rules, res);
			if (res) {
				ERR_MEM;
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
		DYNARR_CHK(out->rule_cnt, caps->rule_cap, out->rules, res);
		if (res) {
			free($1.syms);
			gen_act_fini(&($2));
			ERR_MEM;
			YYERROR;
		}
		for (size_t i = 0; i < $1.cnt; ++i) {
			gen_rarr* r = $1.syms[i].term ? &(out->tokens[$1.syms[i].ind].rh) : &(out->nterms[$1.syms[i].ind].rh);
			DYNARR_CHK(r->cnt, r->cap, r->rules, res);
			if (res) {
				free($1.syms);
				gen_act_fini(&($2));
				ERR_MEM;
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
		DYNARR_CHK(out->rule_cnt, caps->rule_cap, out->rules, res);
		if (res) {
			free($3.syms);
			gen_act_fini(&($4));
			ERR_MEM;
			YYERROR;
		}
		for (size_t i = 0; i < $3.cnt; ++i) {
			gen_rarr* r = $3.syms[i].term ? &(out->tokens[$3.syms[i].ind].rh) : &(out->nterms[$3.syms[i].ind].rh);
			DYNARR_CHK(r->cnt, r->cap, r->rules, res);
			if (res) {
				free($3.syms);
				gen_act_fini(&($4));
				ERR_MEM;
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
		if (syms == NULL) {
			ERR_MEM;
			YYERROR;
		}
		$$.syms = syms;
		$$.cnt = 0;
		$$.cap = 16;
	}
	| EMPTYDECL {
		gen_sid* syms = malloc((sizeof *syms) * 16);
		if (syms == NULL) {
			ERR_MEM;
			YYERROR;
		}
		$$.syms = syms;
		$$.cnt = 0;
		$$.cap = 16;
	}
	| slist IDEN {
		gen_sid sym = hash_ld(out, hash, $2);
		free($2);
		if (sym.error) {
			free($1.syms);
			ERR_PRINT(@2, "symbol not found");
			YYERROR;
		}
		int res = 0;
		DYNARR_CHK($1.cnt, $1.cap, $1.syms, res);
		if (res) {
			free($1.syms);
			ERR_MEM;
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
		if (acts == NULL) {
			ERR_MEM;
			YYERROR;
		}
		$$.act.acts = acts;
		$$.act.len = 0;
		$$.act.loc.first_line = @$.first_line;
		$$.act.loc.last_line = @$.last_line;
		$$.cap = 16;
	}
	| code RAW {
		int res = 0;
		DYNARR_CHK($1.act.len, $1.cap, $1.act.acts, res);
		if (res) {
			gen_act_fini(&($1.act));
			ERR_MEM;
			YYERROR;
		}
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
		if (res) {
			gen_act_fini(&($1.act));
			ERR_MEM;
			YYERROR;
		}
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
		if (res) {
			gen_act_fini(&($1.act));
			ERR_MEM;
			YYERROR;
		}
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

#define HASH_BASE 19

#define HASH_LOAD 0.7

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

void yyerror(YYLTYPE* locp, gen_type* out, struct hash_table* hash, struct cap_track* caps, yyscan_t scanner, char const* msg) {
	(void)out;
	(void)hash;
	(void)caps;
	(void)scanner;
	fprintf(stderr, "%d:%d->%d:%d$ %s\n", locp->first_line, locp->first_column, locp->last_line, locp->last_column, msg);
}

int init_gen(gen_type* restrict gen, struct cap_track* restrict caps) {
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
	gen->nterm_cnt = 0;
	gen->pparam_cnt = 0;
	gen->lparam_cnt = 0;
	gen->rule_cnt = 0;
	gen->prefixlo = NULL;
	gen->prefixhi = NULL;
	gen->epilogue = NULL;
	gen->start.error = 1;
	caps->token_cap = 16;
	caps->nterm_cap = 16;
	caps->pparam_cap = 16;
	caps->lparam_cap = 16;
	caps->rule_cap = 16;
	return 0;
	CleanParams: free(gen->lparams);
	CleanPParams: free(gen->pparams);
	CleanNterms: free(gen->nterms);
	CleanTokens: free(gen->tokens);
	Error: return 1;
}

int hash_init(struct hash_table* table) {
	table->bins = malloc((sizeof *(table->bins)) * HASH_BASE);
	if (table->bins == NULL) return 1;
	for (size_t i = 0; i < HASH_BASE; ++i) table->bins[i].index.error = 1;
	table->bcnt = HASH_BASE;
	table->ecnt = 0;
	return 0;
}

static uintmax_t compute_hash(char const* name) {
	uintmax_t sum = 1337;
	while (*name != '\0') {
		sum = 17 * sum + *name;
		++name;
	}
	return sum;
}

gen_sid hash_ld(gen_type const* restrict out, struct hash_table const* restrict hash, char const* restrict name) {
	uintmax_t hs = compute_hash(name);
	size_t loc = hs % hash->bcnt;
	while (1) {
		if (hash->bins[loc].index.error) return (gen_sid){.error=1};
		char const* cname = hash->bins[loc].index.term ? out->tokens[hash->bins[loc].index.ind].name : out->nterms[hash->bins[loc].index.ind].name;
		if (strcmp(name, cname) == 0) return hash->bins[loc].index;
		loc = (loc + 1) % hash->bcnt;
	}
}

gen_sid hash_add(gen_type* restrict out, struct hash_table* restrict hash, struct cap_track* restrict caps, char* restrict name, _Bool term) {
	if (hash->bcnt * HASH_LOAD <= hash->ecnt) {
		size_t nb = hash->bcnt * 2 - 1;
		struct hash_bin* nbins = malloc((sizeof *nbins) * nb);
		if (nbins == NULL) return (gen_sid){.error=1,.term=1};
		for (size_t i = 0; i < nb; ++i) nbins[i].index.error = 1;
		for (size_t i = 0; i < hash->bcnt; ++i) {
			if (hash->bins[i].index.error == 0) {
				size_t loc = hash->bins[i].hval % nb;
				while (nbins[loc].index.error == 0) loc = (loc + 1) % nb;
				nbins[loc] = hash->bins[i];
			}
		}
		free(hash->bins);
		hash->bcnt = nb;
		hash->bins = nbins;
	}
	int res = 0;
	if (term) DYNARR_CHK(out->token_cnt, caps->token_cap, out->tokens, res);
	else DYNARR_CHK(out->nterm_cnt, caps->nterm_cap, out->nterms, res);
	if (res) return (gen_sid){.error=1,.term=1};
	uintmax_t hs = compute_hash(name);
	size_t loc = hs % hash->bcnt;
	while (1) {
		if (hash->bins[loc].index.error) break;
		char const* cname = hash->bins[loc].index.term ? out->tokens[hash->bins[loc].index.ind].name : out->nterms[hash->bins[loc].index.ind].name;
		if (strcmp(name, cname) == 0) return (gen_sid){.error=1,.term=0};
		loc = (loc + 1) % hash->bcnt;
	}
	hash->bins[loc].hval = hs;
	hash->bins[loc].index.error = 0;
	hash->bins[loc].index.term = term;
	++(hash->ecnt);
	if (term) {
		hash->bins[loc].index.ind = out->token_cnt;
		out->tokens[out->token_cnt].name = name;
		++(out->token_cnt);
	} else {
		hash->bins[loc].index.ind = out->nterm_cnt;
		out->nterms[out->nterm_cnt].name = name;
		++(out->nterm_cnt);
	}
	return hash->bins[loc].index;
}

int gen_bld(char const* restrict srcpath, gen_type* restrict gen) {
	FILE* src = fopen(srcpath, "rb");
	if (src == NULL) return 1;
	gen->fname = srcpath;
	struct hash_table table;
	struct cap_track caps;
	yyscan_t scanner;
	yylex_init(&scanner);
	yyset_in(src, scanner);
	int res = yyparse(gen, &table, &caps, scanner);
	free(table.bins);
	yylex_destroy(scanner);
	fclose(src);
	return res;
}
