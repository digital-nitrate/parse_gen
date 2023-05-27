#ifndef GENERATOR_H
#define GENERATOR_H 1

#include <stdio.h>

#define GEN_SIND_PRI "u"
#define GEN_SIND_BIT (30)
#define GEN_SIND_MAX ((gen_sind)((1U << 30) - 1))
#define GEN_RIND_PRI "u"
#define GEN_RIND_BIT (31)
#define GEN_RIND_MAX ((gen_rind)((1U << 31) - 1))
#define GEN_ROFF_PRI "hhu"
#define GEN_ROFF_BIT (7)
#define GEN_ROFF_MAX ((gen_roff)((1U << 7) - 1))

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int gen_sind;
typedef unsigned int gen_rind;
typedef unsigned char gen_roff;

typedef struct gen_sid {
	unsigned int ind:GEN_SIND_BIT, term:1, error:1;
} gen_sid;


typedef struct gen_loc {
	int first_line;
	int last_line;
} gen_loc;

enum gen_code_type {
	GEN_CODE_COMP,
	GEN_CODE_MERR,
	GEN_CODE_RAW,
};

struct gen_code_unit {
	union {
		char* data;
		gen_roff index;
	};
	enum gen_code_type type;
};

typedef struct gen_act {
	struct gen_code_unit* acts;
	gen_loc loc;
	gen_roff len;
} gen_act;

typedef struct gen_tok {
	char* name;
	char* type;
	gen_act des;
	unsigned int id;
} gen_tok;

typedef struct gen_ntm {
	char* name;
	char* type;
	gen_act des;
} gen_ntm;

typedef struct gen_param {
	char* type;
	char* name;
} gen_param;

typedef struct gen_slist {
	gen_sid* syms;
	gen_roff cnt;
} gen_slist;

typedef struct gen_rule {
	gen_act act;
	gen_slist rhs;
	gen_sind lhs;
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
	size_t pparam_cnt;
	size_t lparam_cnt;
	gen_loc epilogue_loc;
	gen_sind token_cnt;
	gen_sind nterm_cnt;
	gen_rind rule_cnt;
	gen_sid start;
} gen_type;

extern int gen_bld(char const* restrict, gen_type* restrict);
extern gen_rind* gen_bld_ll1(gen_type const*);
extern int gen_wrt_ll1(gen_type const* restrict, gen_rind const* restrict, char const* restrict);
extern int gen_wrt_ll1_h(gen_type const* restrict, char const* restrict);
extern void gen_fini(gen_type*);
extern void gen_act_fini(gen_act*);

#ifdef __cplusplus
}
#endif

#endif
