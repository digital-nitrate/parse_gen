#ifndef GENERATOR_H
#define GENERATOR_H 1

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gen_loc {
	int first_line;
	int last_line;
} gen_loc;

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
	char* name;
	char* type;
	gen_rarr rh;
	unsigned int id;
} gen_tok;

typedef struct gen_ntm {
	char* name;
	char* type;
	gen_rarr rh;
	gen_rarr lh;
} gen_ntm;

typedef struct gen_param {
	char* type;
	char* name;
} gen_param;

enum gen_code_type {
	GEN_CODE_COMP,
	GEN_CODE_RAW,
};

struct gen_code_unit {
	union {
		unsigned int index;
		char* data;
	};
	enum gen_code_type type;
};

typedef struct gen_act {
	struct gen_code_unit* acts;
	size_t len;
	size_t cap;
} gen_act;

typedef struct gen_slist {
	gen_sid* syms;
	size_t cnt;
	size_t cap;
} gen_slist;

typedef struct gen_rule {
	gen_act act;
	gen_loc act_loc;
	gen_slist rhs;
	gen_sid lhs;
} gen_rule;

typedef struct gen_type {
	gen_tok* tokens;
	gen_ntm* nterms;
	gen_param* pparams;
	gen_param* lparams;
	gen_rule* rules;
	char const* fname;
	gen_act top;
	gen_act req;
	gen_act prov;
	gen_act code;
	char* prefixlo;
	char* prefixhi;
	char* epilogue;
	gen_loc top_loc;
	gen_loc req_loc;
	gen_loc prov_loc;
	gen_loc code_loc;
	gen_loc epilogue_loc;
	size_t token_cnt;
	size_t token_cap;
	size_t nterm_cnt;
	size_t nterm_cap;
	size_t pparam_cnt;
	size_t pparam_cap;
	size_t lparam_cnt;
	size_t lparam_cap;
	size_t rule_cnt;
	size_t rule_cap;
	gen_sid start;
} gen_type;

extern int gen_bld(char const* restrict, gen_type* restrict);
extern unsigned int* gen_bld_ll1(gen_type const*);
extern int gen_wrt_ll1(gen_type const* restrict, unsigned int const* restrict, char const* restrict);
extern int gen_wrt_ll1_h(gen_type const* restrict, char const* restrict);
extern void gen_fini(gen_type*);

#ifdef __cplusplus
}
#endif

#endif
