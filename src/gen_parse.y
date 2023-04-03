%define api.pure full
%locations

%code requires {
	#include <stdlib.h>
	#include <limits.h>
	#include <string.h>
	#include "hash.h"
}

%code {
	#include "gen_lex.yy.h"
	static char const api_str[] = "api";
	static size_t const api_len = (sizeof api_str) - 1;
	static char const prefix_str[] = "prefix";
	static size_t const prefix_len = (sizeof prefix_str) - 1;
	#define HASH_LOAD 0.7
	void yyerror(YYLTYPE* locp, gen_type* out, struct hash_table* hash, yyscan_t scanner, char const* msg) {
		(void)out;
		(void)hash;
		(void)scanner;
		fprintf(stderr, "%d:%d->%d:%d$ %s\n", locp->first_line, locp->first_column, locp->last_line, locp->last_column, msg);
	}
	static uintmax_t compute_hash(struct string name) {
		uintmax_t sum = 1337;
		for (size_t i = 0; i < name.len; ++i) sum = 17 * sum + name.data[i];
		return sum;
	}
	static gen_sid ld_hash(gen_type* out, struct hash_table* hash, struct string name) {
		uintmax_t hs = compute_hash(name);
		size_t loc = hs % hash->bcnt;
		while (1) {
			if (hash->bins[loc].index.error) return (gen_sid){.error=1};
			struct string cname = hash->bins[loc].index.term ? out->tokens[hash->bins[loc].index.ind].name : out->nterms[hash->bins[loc].index.ind].name;
			if (cname.len == name.len && memcmp(name.data, cname.data, name.len) == 0) return hash->bins[loc].index;
			loc = loc + 1;
		}
	}
	static gen_sid add_hash(gen_type* out, struct hash_table* hash, struct string name, _Bool term) {
		if (hash->bcnt * HASH_LOAD <= hash->ecnt) {
			size_t nb = hash->bcnt * 2 - 1;
			struct hash_bin* nbins = malloc((sizeof *nbins) * nb);
			if (nbins == NULL) return (gen_sid){.error=1};
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
		if (term) {
			if (out->token_cap == out->token_cnt) {
				size_t nc = 2 * out->token_cap;
				gen_tok* np = realloc(out->tokens, (sizeof *np) * nc);
				if (np == NULL) return (gen_sid){.error=1};
				out->token_cap = nc;
				out->tokens = np;
			}
		} else {
			if (out->nterm_cap == out->nterm_cnt) {
				size_t nc = 2 * out->nterm_cap;
				gen_ntm* np = realloc(out->nterms, (sizeof *np) * nc);
				if (np == NULL) return (gen_sid){.error=1};
				out->nterm_cap = nc;
				out->nterms = np;
			}
		}
		uintmax_t hs = compute_hash(name);
		size_t loc = hs % hash->bcnt;
		while (1) {
			if (hash->bins[loc].index.error) break;
			struct string cname = hash->bins[loc].index.term ? out->tokens[hash->bins[loc].index.ind].name : out->nterms[hash->bins[loc].index.ind].name;
			if (cname.len == name.len && memcmp(name.data, cname.data, name.len) == 0) return (gen_sid){.error=1};
			loc = loc + 1;
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
}

%define api.value.type union

%parse-param {gen_type* out}
%parse-param {struct hash_table* hash}
%parse-param {void* scanner}
%lex-param {yyscan_t scanner}

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
		if (out->prologue_cap == out->prologue_cnt) {
			size_t nc = 2 * out->prologue_cap;
			struct string* np = realloc(out->prologue, (sizeof *np) * nc);
			if (np == NULL) {YYERROR;}
			out->prologue_cap = nc;
			out->prologue = np;
		}
		out->prologue[out->prologue_cnt] = $1;
		++(out->prologue_cnt);
	}
	| TOKENDECL typeopt IDEN UINT {
		gen_sid sym = add_hash(out, hash, $3, 1);
		if (sym.error) {YYERROR;}
		out->tokens[sym.ind].type = $2;
		out->tokens[sym.ind].id = $4;
		gen_ref* ar = malloc((sizeof *ar) * 16);
		if (ar == NULL) {YYERROR;}
		out->tokens[sym.ind].rh = (gen_rarr){.rules=ar,.cnt=0,.cap=16};
	}
	| NTERMDECL typeopt IDEN {
		gen_sid sym = add_hash(out, hash, $3, 0);
		if (sym.error) {YYERROR;}
		out->nterms[sym.ind].type = $2;
		gen_ref* ar = malloc((sizeof *ar) * 16);
		if (ar == NULL) {YYERROR;}
		out->nterms[sym.ind].rh = (gen_rarr){.rules=ar,.cnt=0,.cap=16};
		gen_ref* al = malloc((sizeof *al) * 16);
		if (al == NULL) {YYERROR;}
		out->nterms[sym.ind].lh = (gen_rarr){.rules=al,.cnt=0,.cap=16};
	}
	| STARTDECL IDEN {
		if (out->start.error == 0) {YYERROR;}
		gen_sid sym = ld_hash(out, hash, $2);
		if (sym.error || sym.term) {YYERROR;}
		out->start = sym;
	}
	| DEFINEDECL IDEN DOT IDEN IDEN {
		if (out->prefix.data != NULL || $2.len != api_len || memcmp(api_str, $2.data, api_len) != 0 || $4.len != prefix_len || memcmp(prefix_str, $4.data, prefix_len) != 0) {YYERROR;}
		out->prefix = $5;
	}
	| PARAMDECL TYPESPEC IDEN {
		if (out->param_cap == out->param_cnt) {
			size_t nc = 2 * out->param_cap;
			gen_param* np = realloc(out->params, (sizeof *np) * nc);
			if (np == NULL) {YYERROR;}
			out->param_cap = nc;
			out->params = np;
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
		gen_sid sym = ld_hash(out, hash, $2);
		if (sym.error || sym.term) {YYERROR;}
		gen_rarr* r = &(out->nterms[sym.ind].lh);
		for (size_t i = $4.low; i < $4.high; ++i) {
			if (r->cnt == r->cap) {
				size_t nc = 2 * r->cap;
				gen_ref* np = realloc(r->rules, (sizeof *np) * nc);
				if (np == NULL) {YYERROR;}
				r->cnt = nc;
				r->rules = np;				
			}
			r->rules[r->cnt] = (gen_ref){.rule=i,.loc=out->rules[i].rhs.cnt};
			++(r->cnt);
			out->rules[i].lhs = sym;
		}
	}

patterns:
	slist actopt {
		if (out->rule_cap == out->rule_cnt) {
			size_t nc = 2 * out->rule_cap;
			gen_rule* np = realloc(out->rules, (sizeof *np) * nc);
			if (np == NULL) {YYERROR;}
			out->rule_cap = nc;
			out->rules = np;
		}
		for (size_t i = 0; i < $1.cnt; ++i) {
			gen_rarr* r = $1.syms[i].term ? &(out->tokens[$1.syms[i].ind].rh) : &(out->nterms[$1.syms[i].ind].rh);
			if (r->cnt == r->cap) {
				size_t nc = 2 * r->cap;
				gen_ref* np = realloc(r->rules, (sizeof *np) * nc);
				if (np == NULL) {YYERROR;}
				r->cnt = nc;
				r->rules = np;				
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
		if (out->rule_cap == out->rule_cnt) {
			size_t nc = 2 * out->rule_cap;
			gen_rule* np = realloc(out->rules, (sizeof *np) * nc);
			if (np == NULL) {YYERROR;}
			out->rule_cap = nc;
			out->rules = np;
		}
		for (size_t i = 0; i < $3.cnt; ++i) {
			gen_rarr* r = $3.syms[i].term ? &(out->tokens[$3.syms[i].ind].rh) : &(out->nterms[$3.syms[i].ind].rh);
			if (r->cnt == r->cap) {
				size_t nc = 2 * r->cap;
				gen_ref* np = realloc(r->rules, (sizeof *np) * nc);
				if (np == NULL) {YYERROR;}
				r->cnt = nc;
				r->rules = np;				
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
	| slist IDEN {
		gen_sid sym = ld_hash(out, hash, $2);
		if (sym.error) {YYERROR;}
		if ($1.cap == $1.cnt) {
			size_t nc = 2 * $1.cap;
			gen_sid* np = realloc($1.syms, (sizeof *np) * nc);
			if (np == NULL) {YYERROR;}
			$1.cap = nc;
			$1.syms = np;
		}
		$1.syms[$1.cnt] = sym;
		++($1.cnt);
		$$ = $1;
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
