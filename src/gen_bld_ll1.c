#include <stdlib.h>
#include <stdio.h>

#include "generator.h"

#include "macros.h"

struct ref {
	gen_rind rule;
	gen_roff loc;
};

struct rarr {
	struct ref* refs;
	size_t cnt;
	size_t cap;
};

struct ntdata {
	unsigned int lambda:1, first:1, follow:1;
	struct rarr rh;
	struct rarr lh;
};

static int bld_met(gen_type const* restrict gen, struct rarr* tmet, struct ntdata* nmet) {
	struct rarr const* tmet_end = tmet + gen->token_cnt;
	for (struct rarr* curr = tmet; curr != tmet_end; ++curr) {
		curr->refs = malloc((sizeof *(curr->refs)) * 16);
		if (curr->refs == NULL) {
			for (struct rarr* tmp = tmet; tmp != curr; ++tmp) free(tmp->refs);
			goto Fail;
		}
		curr->cnt = 0;
		curr->cap = 16;
	}
	struct ntdata const* nmet_end = nmet + gen->nterm_cnt;
	for (struct ntdata* curr = nmet; curr != nmet_end; ++curr) {
		curr->rh.refs = malloc((sizeof *(curr->rh.refs)) * 16);
		curr->lh.refs = malloc((sizeof *(curr->lh.refs)) * 16);
		if (curr->rh.refs == NULL || curr->lh.refs == NULL) {
			free(curr->rh.refs);
			free(curr->lh.refs);
			for (struct ntdata* tmp = nmet; tmp != curr; ++tmp) {
				free(tmp->rh.refs);
				free(tmp->lh.refs);
			}
			goto CleanTmet;
		}
		curr->rh.cnt = 0;
		curr->rh.cap = 16;
		curr->lh.cnt = 0;
		curr->lh.cap = 16;
	}
	gen_rule const* rend = gen->rules + gen->rule_cnt;
	for (gen_rule const* curr = gen->rules; curr != rend; ++curr) {
		int res = 0;
		struct rarr* lh = &(nmet[curr->lhs].lh);
		DYNARR_CHK(lh->cnt, lh->cap, lh->refs, res);
		if (res != 0) goto CleanNmet;
		lh->refs[lh->cnt] = (struct ref){.rule = (gen_rind)(curr - gen->rules), .loc = curr->rhs.cnt};
		++(lh->cnt);
		gen_sid const* send = curr->rhs.syms + curr->rhs.cnt;
		for (gen_sid const* sc = curr->rhs.syms; sc != send; ++sc) {
			struct rarr* rh = sc->term ? &(tmet[sc->ind]) : &(nmet[sc->ind].rh);
			DYNARR_CHK(rh->cnt, rh->cap, rh->refs, res);
			if (res != 0) goto CleanNmet;
			rh->refs[rh->cnt] = (struct ref){.rule = (gen_rind)(curr - gen->rules), .loc = (gen_roff)(sc - curr->rhs.syms)};
			++(rh->cnt);
		}
	}
	return 0;
	CleanNmet: for (struct ntdata* curr = nmet; curr != nmet_end; ++curr) {
		free(curr->rh.refs);
		free(curr->lh.refs);
	}
	CleanTmet: for (struct rarr* curr = tmet; curr != tmet_end; ++curr) free(curr->refs);
	Fail: return 1;
}

static void bld_lam(gen_type const* restrict gen, gen_sind* restrict queue, struct ntdata* restrict nmet, gen_roff* restrict rsaves) {
	gen_sind* queue_top = queue;
	struct ntdata const* nmet_end = nmet + gen->nterm_cnt;
	for (struct ntdata* curr = nmet; curr != nmet_end; ++curr) curr->lambda = 0;
	gen_rule const* rend = gen->rules + gen->rule_cnt;
	for (gen_rule const* curr = gen->rules; curr != rend; ++curr) {
		rsaves[curr - gen->rules] = 0;
		if (curr->rhs.cnt == 0 && nmet[curr->lhs].lambda == 0) {
			nmet[curr->lhs].lambda = 1;
			*queue_top = curr->lhs;
			++queue_top;
		}
	}
	for (gen_sind* queue_curr = queue; queue_curr != queue_top; ++queue_curr) {
		struct rarr const* ls = &(nmet[*queue_curr].rh);
		struct ref const* rend = ls->refs + ls->cnt;
		for (struct ref const* curr = ls->refs; curr != rend; ++curr) {
			gen_rule* rule = &(gen->rules[curr->rule]);
			if (nmet[rule->lhs].lambda) continue;
			if (curr->loc > rsaves[curr->rule]) continue;
			gen_roff cntr = curr->loc;
			while (cntr < rule->rhs.cnt && rule->rhs.syms[cntr].term == 0 && nmet[rule->rhs.syms[cntr].ind].lambda == 1) ++cntr;
			if (cntr == rule->rhs.cnt) {
				nmet[rule->lhs].lambda = 1;
				*queue_top = rule->lhs;
				++queue_top;
			}
			rsaves[curr->rule] = cntr;
		}
	}
	for (gen_rind i = 0; i < gen->rule_cnt; ++i) {
		gen_slist rhs = gen->rules[i].rhs;
		gen_roff cntr = rsaves[i];
		while (cntr < rhs.cnt && rhs.syms[cntr].term == 0 && nmet[rhs.syms[cntr].ind].lambda) ++cntr;
		rsaves[i] = cntr;
	}
}

static void trav_fst(gen_type const* restrict gen, struct ntdata* restrict nmet, gen_roff const* restrict rsaves, gen_rind* restrict table, struct rarr const* restrict in, gen_sind** restrict queue_top, gen_sind t) {
	struct ref const* in_end = in->refs + in->cnt;
	for (struct ref const* in_curr = in->refs; in_curr != in_end; ++in_curr) {
		if (rsaves[in_curr->rule] < in_curr->loc) continue;
		gen_sind lhs = gen->rules[in_curr->rule].lhs;
		if (nmet[lhs].first == 0) {
			nmet[lhs].first = 1;
			**queue_top = lhs;
			++*queue_top;
		}
		gen_rind* tr = &(table[(size_t)lhs * (gen->token_cnt + 1) + t]);
		if (*tr != GEN_RIND_MAX && *tr != in_curr->rule) {
			gen_rind lo = *tr < in_curr->rule ? *tr : in_curr->rule;
			fprintf(stderr, "warning: conflict for rules %"GEN_RIND_PRI" -- %"GEN_RIND_PRI" at (%"GEN_SIND_PRI",%"GEN_SIND_PRI") {first set}; using %"GEN_RIND_PRI"\n", *tr, in_curr->rule, lhs, t, lo);
			*tr = lo;
		} else {
			*tr = in_curr->rule;
		}
	}
}

static void bld_fst(gen_type const* restrict gen, gen_sind* restrict queue, struct rarr const* restrict tmet, struct ntdata* restrict nmet, gen_roff const* restrict rsaves, gen_rind* restrict table, gen_sind t) {
	gen_sind* queue_top = queue;
	trav_fst(gen, nmet, rsaves, table, &(tmet[t]), &queue_top, t);
	for (gen_sind* queue_curr = queue; queue_curr != queue_top; ++queue_curr) {
		trav_fst(gen, nmet, rsaves, table, &(nmet[*queue_curr].rh), &queue_top, t);
	}
}

static void trav_fol(gen_type const* restrict gen, struct ntdata* restrict nmet, gen_rind* restrict table, struct rarr const* restrict in, gen_sind** restrict queue_top, gen_sind t) {
	struct ref const* in_end = in->refs + in->cnt;
	for (struct ref const* in_curr = in->refs; in_curr != in_end; ++in_curr) {
		gen_rule* rule = &(gen->rules[in_curr->rule]);
		gen_roff cntr = in_curr->loc;
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
		gen_rind* tr = &(table[(size_t)(rule->lhs) * (gen->token_cnt + 1) + t]);
		if (*tr != GEN_RIND_MAX && *tr != in_curr->rule) {
			gen_rind lo = *tr < in_curr->rule ? *tr : in_curr->rule;
			fprintf(stderr, "warning: conflict for rules %"GEN_RIND_PRI" -- %"GEN_RIND_PRI" at (%"GEN_SIND_PRI",%"GEN_SIND_PRI") {follow set}; using %"GEN_RIND_PRI"\n", *tr, in_curr->rule, rule->lhs, t, lo);
			*tr = lo;
		} else {
			*tr = in_curr->rule;
		}
	}
}

gen_rind* gen_bld_ll1(gen_type const* gen) {
	gen_rind* table = malloc((sizeof *table) * (gen->token_cnt + 1) * gen->nterm_cnt);
	gen_roff* rsaves = malloc((sizeof *rsaves) * gen->rule_cnt);
	struct rarr* tmet = malloc((sizeof *tmet) * gen->token_cnt);
	struct ntdata* nmet = malloc((sizeof *nmet) * gen->nterm_cnt);
	gen_sind* queue = malloc((sizeof *queue) * gen->nterm_cnt);
	if (table == NULL || rsaves == NULL || tmet == NULL || nmet == NULL || queue == NULL || bld_met(gen, tmet, nmet)) goto Error;
	gen_rind const* table_end = table + (size_t)(gen->token_cnt + 1) * gen->nterm_cnt;
	for (gen_rind* curr = table; curr != table_end; ++curr) *curr = GEN_RIND_MAX;
	bld_lam(gen, queue, nmet, rsaves);
	struct ntdata const* nmet_end = nmet + gen->nterm_cnt;
	for (gen_sind t = 0; t < gen->token_cnt; ++t) {
		for (struct ntdata* curr = nmet; curr != nmet_end; ++curr) {
			curr->first = 0;
			curr->follow = 0;
		}
		bld_fst(gen, queue, tmet, nmet, rsaves, table, t);
		gen_sind* queue_top = queue;
		trav_fol(gen, nmet, table, &(tmet[t]), &queue_top, t);
		for (gen_sind i = 0; i < gen->nterm_cnt; ++i) {
			if (nmet[i].first == 0) continue;
			trav_fol(gen, nmet, table, &(nmet[i].rh), &queue_top, t);
		}
		for (gen_sind* queue_curr = queue; queue_curr != queue_top; ++queue_curr) {
			trav_fol(gen, nmet, table, &(nmet[*queue_curr].lh), &queue_top, t);
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
		trav_fol(gen, nmet, table, &(nmet[*queue_curr].lh), &queue_top, gen->token_cnt);
	}
	free(queue);
	struct rarr const* tmet_end = tmet + gen->token_cnt;
	for (struct rarr* curr = tmet; curr != tmet_end; ++curr) free(curr->refs);
	free(tmet);
	for (struct ntdata* curr = nmet; curr != nmet_end; ++curr) {
		free(curr->rh.refs);
		free(curr->lh.refs);
	}
	free(nmet);
	free(rsaves);
	return table;
	Error: free(queue);
	free(nmet);
	free(tmet);
	free(rsaves);
	free(table);
	return NULL;
}
