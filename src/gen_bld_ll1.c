#include <stdlib.h>
#include <limits.h>

#include "generator.h"

struct ntdata {
	unsigned int lambda:1, first:1, follow:1;
};

static void bld_lam(gen_type const* restrict gen, gen_sind* restrict queue, struct ntdata* restrict nmet, gen_roff* restrict rsaves) {
	gen_sind* queue_top = queue;
	struct ntdata const* nmet_end = nmet + gen->nterm_cnt;
	for (struct ntdata* curr = nmet; curr != nmet_end; ++curr) curr->lambda = 0;
	for (gen_rind i = 0; i < gen->rule_cnt; ++i) {
		rsaves[i] = 0;
		if (gen->rules[i].rhs.cnt == 0 && nmet[gen->rules[i].lhs.ind].lambda == 0) {
			nmet[gen->rules[i].lhs.ind].lambda = 1;
			*queue_top = gen->rules[i].lhs.ind;
			++queue_top;
		}
	}
	for (gen_sind* queue_curr = queue; queue_curr != queue_top; ++queue_curr) {
		gen_rarr rs = gen->nterms[*queue_curr].rh;
		for (gen_rind i = 0; i < rs.cnt; ++i) {
			gen_rule* rule = &(gen->rules[rs.rules[i].rule]);
			if (nmet[rule->lhs.ind].lambda) continue;
			if (rs.rules[i].loc > rsaves[rs.rules[i].rule]) continue;
			gen_roff cntr = rs.rules[i].loc;
			while (cntr < rule->rhs.cnt && rule->rhs.syms[cntr].term == 0 && nmet[rule->rhs.syms[cntr].ind].lambda == 1) ++cntr;
			if (cntr == rule->rhs.cnt) {
				nmet[rule->lhs.ind].lambda = 1;
				*queue_top = rule->lhs.ind;
				++queue_top;
			}
			rsaves[rs.rules[i].rule] = cntr;
		}
	}
	for (gen_rind i = 0; i < gen->rule_cnt; ++i) {
		gen_slist rhs = gen->rules[i].rhs;
		gen_roff cntr = rsaves[i];
		while (cntr < rhs.cnt && rhs.syms[cntr].term == 0 && nmet[rhs.syms[cntr].ind].lambda) ++cntr;
		rsaves[i] = cntr;
	}
}

static void trav_fst(gen_type const* restrict gen, struct ntdata* restrict nmet, gen_roff const* restrict rsaves, gen_rind* restrict table, gen_rarr const* restrict in, gen_sind** restrict queue_top, gen_sind t) {
	for (size_t i = 0; i < in->cnt; ++i) {
		if (rsaves[in->rules[i].rule] < in->rules[i].loc) continue;
		gen_sid lhs = gen->rules[in->rules[i].rule].lhs;
		if (nmet[lhs.ind].first == 0) {
			nmet[lhs.ind].first = 1;
			**queue_top = lhs.ind;
			++*queue_top;
		}
		unsigned int tr = table[lhs.ind * (gen->token_cnt + 1) + t];
		if (tr != GEN_RIND_MAX && tr != in->rules[i].rule) {
			unsigned int lo = tr < in->rules[i].rule ? tr : in->rules[i].rule;
			fprintf(stderr, "warning: conflict for rules %u -- %u at (%u,%u) {first set}; using %u\n", tr, in->rules[i].rule, lhs.ind, t, lo);
			table[lhs.ind * (gen->token_cnt + 1) + t] = lo;
		} else {
			table[lhs.ind * (gen->token_cnt + 1) + t] = in->rules[i].rule;
		}
	}
}

static void bld_fst(gen_type const* restrict gen, gen_sind* restrict queue, struct ntdata* restrict nmet, gen_roff const* restrict rsaves, gen_rind* restrict table, gen_sind t) {
	gen_sind* queue_top = queue;
	trav_fst(gen, nmet, rsaves, table, &(gen->tokens[t].rh), &queue_top, t);
	for (gen_sind* queue_curr = queue; queue_curr != queue_top; ++queue_curr) {
		trav_fst(gen, nmet, rsaves, table, &(gen->nterms[*queue_curr].rh), &queue_top, t);
	}
}

static void trav_fol(gen_type const* restrict gen, struct ntdata* restrict nmet, gen_rind* restrict table, gen_rarr const* restrict in, gen_sind** restrict queue_top, gen_sind t) {
	for (size_t i = 0; i < in->cnt; ++i) {
		gen_rule* rule = &(gen->rules[in->rules[i].rule]);
		size_t cntr = in->rules[i].loc;
		while (cntr != 0) {
			gen_sid sym = rule->rhs.syms[cntr - 1];
			if (sym.term == 1) break;
			if (nmet[sym.ind].follow == 0) {
				nmet[sym.ind].follow = 1;
				**queue_top = sym.ind;
				++*queue_top;
			}
			if (nmet[sym.ind].lambda == 0 || nmet[sym.ind].first == 1) break;
			--cntr;
		}
		if (cntr != 0) continue;
		gen_rind tr = table[rule->lhs.ind * (gen->token_cnt + 1) + t];
		if (tr != GEN_RIND_MAX && tr != in->rules[i].rule) {
			gen_rind lo = tr < in->rules[i].rule ? tr : in->rules[i].rule;
			fprintf(stderr, "warning: conflict for rules %u -- %u at (%u,%u) {follow set}; using %u\n", tr, in->rules[i].rule, rule->lhs.ind, t, lo);
			table[rule->lhs.ind * (gen->token_cnt + 1) + t] = lo;
		} else {
			table[rule->lhs.ind * (gen->token_cnt + 1) + t] = in->rules[i].rule;
		}
	}
}

gen_rind* gen_bld_ll1(gen_type const* gen) {
	gen_rind* table = malloc((sizeof *table) * (gen->token_cnt + 1) * gen->nterm_cnt);
	gen_roff* rsaves = malloc((sizeof *rsaves) * gen->rule_cnt);
	struct ntdata* nmet = malloc((sizeof *nmet) * gen->nterm_cnt);
	gen_sind* queue = malloc((sizeof *queue) * gen->nterm_cnt);
	gen_rind const* table_end = table + (size_t)(gen->token_cnt + 1) * gen->nterm_cnt;
	for (gen_rind* curr = table; curr != table_end; ++curr) *curr = GEN_RIND_MAX;
	bld_lam(gen, queue, nmet, rsaves);
	struct ntdata const* nmet_end = nmet + gen->nterm_cnt;
	for (gen_sind t = 0; t < gen->token_cnt; ++t) {
		for (struct ntdata* curr = nmet; curr != nmet_end; ++curr) {
			curr->first = 0;
			curr->follow = 0;
		}
		bld_fst(gen, queue, nmet, rsaves, table, t);
		gen_sind* queue_top = queue;
		trav_fol(gen, nmet, table, &(gen->tokens[t].rh), &queue_top, t);
		for (gen_sind i = 0; i < gen->nterm_cnt; ++i) {
			if (nmet[i].first == 0) continue;
			trav_fol(gen, nmet, table, &(gen->nterms[i].rh), &queue_top, t);
		}
		for (gen_sind* queue_curr = queue; queue_curr != queue_top; ++queue_curr) {
			trav_fol(gen, nmet, table, &(gen->nterms[*queue_curr].lh), &queue_top, t);
		}
	}
	for (struct ntdata* curr = nmet; curr != nmet_end; ++curr) {
		curr->first = 0;
		curr->follow = 0;
	}
	nmet[gen->start.ind].follow = 1;
	gen_sind* queue_top = queue;
	*queue_top = gen->start.ind;
	++queue_top;
	for (gen_sind* queue_curr = queue; queue_curr != queue_top; ++queue_curr) {
		trav_fol(gen, nmet, table, &(gen->nterms[*queue_curr].lh), &queue_top, gen->token_cnt);
	}
	free(queue);
	free(nmet);
	free(rsaves);
	return table;
}
