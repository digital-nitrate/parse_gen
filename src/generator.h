#ifndef GENERATOR_H
#define GENERATOR_H 1

#include <stdio.h>

#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gen_sid {
	unsigned int error:1, term:1, ind:30;
} gen_sid;

typedef struct gen_ref {
	unsigned int rule;
	unsigned int loc;
} gen_ref;

typedef struct gen_rarr {
	gen_ref* rules;
	size_t cnt;
	size_t cap;
} gen_rarr;

typedef struct gen_tok {
	struct string name;
	struct string type;
	gen_rarr rh;
	unsigned int id;
} gen_tok;

typedef struct gen_ntm {
	struct string name;
	struct string type;
	gen_rarr rh;
	gen_rarr lh;
} gen_ntm;

typedef struct gen_param {
	struct string type;
	struct string name;
} gen_param;

enum gen_code_type {
	GEN_CODE_SELF,
	GEN_CODE_COMP,
	GEN_CODE_RAW,
};

typedef struct gen_act {
	struct gen_act* next;
	union {
		unsigned int index;
		struct string data;
	};
	enum gen_code_type type;
} gen_act;

typedef struct gen_slist {
	gen_sid* syms;
	size_t cnt;
	size_t cap;
} gen_slist;

typedef struct gen_rule {
	gen_act* act;
	gen_slist rhs;
	gen_sid lhs;
} gen_rule;

typedef struct gen_type {
	struct string* prologue;
	gen_tok* tokens;
	gen_ntm* nterms;
	gen_param* params;
	gen_rule* rules;
	struct string prefix;
	struct string epilogue;
	size_t prologue_cnt;
	size_t prologue_cap;
	size_t token_cnt;
	size_t token_cap;
	size_t nterm_cnt;
	size_t nterm_cap;
	size_t param_cnt;
	size_t param_cap;
	size_t rule_cnt;
	size_t rule_cap;
	gen_sid start;
} gen_type;

extern int gen_bld(FILE* restrict, gen_type* restrict);
extern unsigned int* gen_bld_ll1(gen_type const*);
extern void gen_wrt_ll1(gen_type const* restrict, unsigned int const* restrict, FILE* restrict);
extern void gen_fini(gen_type*);

#ifdef __cplusplus
}
#endif

#endif
