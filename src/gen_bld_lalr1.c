#include <stdlib.h>
#include <stdio.h>

#include "generator.h"

#include "macros.h"

#define HASH_BASE (19)
#define HASH_LOAD (0.7)

struct ntm_hash_bin {
	size_t offset;
	gen_sind sym;
};

struct ntm_hash {
	struct ntm_hash_bin* bins;
	size_t bcnt;
	size_t ecnt;
};

struct item_set {
	//kernal
	//rulemap
	struct rule_bul* rules;
	size_t cnt;
};

struct item_graph {
	struct item_set* sets;
	size_t cnt;
};

int gen_bld_lalr1(gen_type const* restrict grm, gen_stat* restrict stat, gen_lrentry** restrict trmtab, gen_lrentry** restrict ntmtab) {
	//TODO
	(void)grm;
	(void)stat;
	(void)trmtab;
	(void)ntmtab;
	return 1;
}
