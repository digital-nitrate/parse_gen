#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "generator.h"

#include "macros.h"

#define HASH_BASE (19)
#define HASH_LOAD (0.7)

struct rule_bul {
	gen_rind rule;
	gen_roff bul;
};

struct ker_hash_bin {
	struct rule_bul ref;
	_Bool active;
};

struct ntm_hash_bin {
	gen_sind sym;
	_Bool active;
};

struct ntm_hash {
	struct ntm_hash_bin* bins;
	size_t bcnt;
	size_t ecnt;
};

struct ker_hash {
	struct ker_hash_bin* bins;
	size_t bcnt;
	size_t ecnt;
};

struct item_set {
	struct ker_hash kernal;
	struct ntm_hash ntm_map;
};

struct item_graph {
	struct item_set* sets;
	gen_lrentry* trmtab;
	gen_lrentry* ntmtab;
	size_t cnt;
};

struct rarr {
	struct rule_bul* refs;
	size_t cnt;
	size_t cap;
};

struct ntdata {
	unsigned int lambda:1, first:1;
	struct rarr rh;
	struct rarr lh;
};

static int bld_met(gen_type const* restrict gen, struct rarr* restrict tmet, struct ntdata* restrict nmet) {
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
		lh->refs[lh->cnt] = (struct rule_bul){.rule = (gen_rind)(curr - gen->rules), .bul = curr->rhs.cnt};
		++(lh->cnt);
		gen_sid const* send = curr->rhs.syms + curr->rhs.cnt;
		for (gen_sid const* sc = curr->rhs.syms; sc != send; ++sc) {
			struct rarr* rh = sc->term ? &(tmet[sc->ind]) : &(nmet[sc->ind].rh);
			DYNARR_CHK(rh->cnt, rh->cap, rh->refs, res);
			if (res != 0) goto CleanNmet;
			rh->refs[rh->cnt] = (struct rule_bul){.rule = (gen_rind)(curr - gen->rules), .bul = (gen_roff)(sc - curr->rhs.syms)};
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
		struct rule_bul const* rend = ls->refs + ls->cnt;
		for (struct rule_bul const* curr = ls->refs; curr != rend; ++curr) {
			gen_rule* rule = &(gen->rules[curr->rule]);
			if (nmet[rule->lhs].lambda) continue;
			if (curr->bul > rsaves[curr->rule]) continue;
			gen_roff cntr = curr->bul;
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

#define TMP_MALLOC(_bins, _size, ...) do {\
	(_bins) = malloc((_size));\
} while (0)
#define TMP_FREE(_bins, ...) do {\
	free((_bins));\
} while (0)
#define TMP_NEW_SIZE(_nb, _bcnt, ...) do {\
	(_nb) = (_bcnt) * 2 - 1;\
} while (0)
#define TMP_INIT(_bin, ...) do {\
	(_bin).ref.rule = GEN_RIND_MAX;\
} while (0)
#define TMP_FILL(_chk, _hs, _key, _bin, ...) do {\
	(_bin).ref = (_key);\
	(_bin).active = 0;\
} while (0)
#define TMP_IS_EMPTY(_flg, _bin, ...) do {\
	(_flg) = ((_bin).ref.rule == GEN_RIND_MAX);\
} while (0)
#define TMP_IS_MATCH(_flg, _key, _bin, ...) do {\
	(_flg) = (((_bin).ref.rule == (_key).rule) && ((_bin).ref.bul == (_key).bul));\
} while (0)
#define TMP_PULL_HASH(_hs, _bin, ...) do {\
	(_hs) = (307) * (_bin).ref.rule + (301) * (_bin).ref.bul;\
} while (0)
#define TMP_HASH(_hs, _key, ...) do {\
	(_hs) = (307) * (_key).rule + (301) * (_key).bul;\
} while (0)
#define TMP_MEM(_out, ...) do {\
	(_out) = 2;\
} while (0)
#define TMP_EXIST(_out, ...) do {\
	(_out) = 1;\
} while (0)
#define TMP_RET(_out, _bin, ...) do {\
	(_out) = 0;\
} while (0)
#define TMP_ERR(_out, ...) do {\
	(_out) = 1;\
} while (0)

static int ker_hash_init(struct ker_hash* table) {
	HASH_INIT(struct ker_hash_bin, TMP_INIT, TMP_MALLOC, HASH_BASE, table->bins, table->bcnt, table->ecnt);
	return (table->bins == NULL) ? 1 : 0;
}

static int ker_hash_ld(struct ker_hash const* table, struct rule_bul rule) {
	int res;
	HASH_LD(TMP_RET, TMP_IS_MATCH, TMP_ERR, TMP_IS_EMPTY, TMP_HASH, res, rule, table->bins, table->bcnt);
	return res;
}

static int ker_hash_add(struct ker_hash* table, struct rule_bul rule) {
	int res;
	HASH_ADD(struct ker_hash_bin, TMP_MALLOC, TMP_FREE, TMP_NEW_SIZE, TMP_INIT, TMP_FILL, TMP_IS_EMPTY, TMP_IS_MATCH, TMP_PULL_HASH, TMP_HASH, TMP_MEM, TMP_EXIST, TMP_RET, HASH_LOAD, res, rule, table->bins, table->bcnt, table->ecnt);
	return res;
}

#undef TMP_MALLOC
#undef TMP_FREE
#undef TMP_NEW_SIZE
#undef TMP_INIT
#undef TMP_FILL
#undef TMP_IS_EMPTY
#undef TMP_IS_MATCH
#undef TMP_PULL_HASH
#undef TMP_HASH
#undef TMP_MEM
#undef TMP_EXIST
#undef TMP_RET
#undef TMP_ERR

#define TMP_MALLOC(_bins, _size, ...) do {\
	(_bins) = malloc((_size));\
} while (0)
#define TMP_FREE(_bins, ...) do {\
	free((_bins));\
} while (0)
#define TMP_NEW_SIZE(_nb, _bcnt, ...) do {\
	(_nb) = (_bcnt) * 2 - 1;\
} while (0)
#define TMP_INIT(_bin, ...) do {\
	(_bin).sym = GEN_SIND_MAX;\
} while (0)
#define TMP_FILL(_chk, _hs, _key, _bin, _rque, _rque_top, _nmet, ...) do {\
	struct rarr const* refs = &((_nmet)[(_key)].lh);\
	(_bin).active = 0;\
	(_bin).sym = (_key);\
	struct rule_bul const* rend = refs->refs + refs->cnt;\
	for (struct rule_bul const* rcur = refs->refs; rcur != rend; ++rcur) {\
		(_rque)[(_rque_top)] = rcur->rule;\
		++(_rque_top);\
	}\
} while (0)
#define TMP_IS_EMPTY(_flg, _bin, ...) do {\
	(_flg) = ((_bin).sym == GEN_SIND_MAX);\
} while (0)
#define TMP_IS_MATCH(_flg, _key, _bin, ...) do {\
	(_flg) = ((_bin).sym == (_key));\
} while (0)
#define TMP_PULL_HASH(_hs, _bin, ...) do {\
	(_hs) = (307) * (_bin).sym;\
} while (0)
#define TMP_HASH(_hs, _key, ...) do {\
	(_hs) = (307) * (_key);\
} while (0)
#define TMP_MEM(_out, ...) do {\
	(_out) = 2;\
} while (0)
#define TMP_EXIST(_out, ...) do {\
	(_out) = 1;\
} while (0)
#define TMP_RET(_out, _bin, ...) do {\
	(_out) = 0;\
} while (0)
#define TMP_ERR(_out, ...) do {\
	(_out) = 1;\
} while (0)

//TODO Add reference tracking
static int clo_item_set(gen_type const* restrict grm, struct ntdata* restrict nmet, gen_rind* restrict rque, struct ker_hash* restrict tarr, struct ker_hash* restrict narr, struct item_set* restrict set) {
	HASH_INIT(struct ntm_hash_bin, TMP_INIT, TMP_MALLOC, HASH_BASE, set->ntm_map.bins, set->ntm_map.bcnt, set->ntm_map.ecnt);
	if (set->ntm_map.bins == NULL) goto Fail;
	size_t rque_top = 0;
	struct ker_hash_bin const* ker_end = set->kernal.bins + set->kernal.bcnt;
	for (struct ker_hash_bin const* curr = set->kernal.bins; curr != ker_end; ++curr) {
		if (curr->ref.rule == GEN_RIND_MAX) continue;
		gen_slist const* rhs = &(grm->rules[curr->ref.rule].rhs);
		if (rhs->cnt == curr->ref.bul) continue;
		gen_sid sym = rhs->syms[curr->ref.bul];
		if (ker_hash_add(sym.ind + (sym.term ? tarr : narr), (struct rule_bul){.rule=curr->ref.rule,.bul=curr->ref.bul+1})) goto CleanHash;
		if (sym.term) continue;
		int res;
		HASH_ADD(struct ntm_hash_bin, TMP_MALLOC, TMP_FREE, TMP_NEW_SIZE, TMP_INIT, TMP_FILL, TMP_IS_EMPTY, TMP_IS_MATCH, TMP_PULL_HASH, TMP_HASH, TMP_MEM, TMP_EXIST, TMP_RET, HASH_LOAD, res, sym.ind, set->ntm_map.bins, set->ntm_map.bcnt, set->ntm_map.ecnt, rque, rque_top, nmet);
		if (res == 2) goto CleanHash;
	}
	for (size_t rque_curr = 0; rque_curr != rque_top; ++rque_curr) {
		gen_rind rule = rque[rque_curr];
		gen_slist const* rhs = &(grm->rules[rule].rhs);
		if (rhs->cnt == 0) continue;
		gen_sid sym = rhs->syms[0];
		if (ker_hash_add(sym.ind + (sym.term ? tarr : narr), (struct rule_bul){.rule=rule,.bul=1})) goto CleanHash;
		if (sym.term) continue;
		int res;
		HASH_ADD(struct ntm_hash_bin, TMP_MALLOC, TMP_FREE, TMP_NEW_SIZE, TMP_INIT, TMP_FILL, TMP_IS_EMPTY, TMP_IS_MATCH, TMP_PULL_HASH, TMP_HASH, TMP_MEM, TMP_EXIST, TMP_RET, HASH_LOAD, res, sym.ind, set->ntm_map.bins, set->ntm_map.bcnt, set->ntm_map.ecnt, rque, rque_top, nmet);
		if (res == 2) goto CleanHash;
	}
	return 0;
	CleanHash: free(set->ntm_map.bins);
	Fail: return 1;
}

#undef TMP_MALLOC
#undef TMP_FREE
#undef TMP_NEW_SIZE
#undef TMP_INIT
#undef TMP_FILL
#undef TMP_IS_EMPTY
#undef TMP_IS_MATCH
#undef TMP_PULL_HASH
#undef TMP_HASH
#undef TMP_MEM
#undef TMP_EXIST
#undef TMP_RET
#undef TMP_ERR

static int con_graph(struct item_graph* restrict target, gen_type const* restrict grm) {
	//TODO
	(void)target;
	(void)grm;
	return 1;
}

int gen_bld_lalr1(gen_type const* restrict grm, gen_stat* restrict stat, gen_lrentry** restrict trmtab, gen_lrentry** restrict ntmtab) {
	//TODO
	(void)grm;
	(void)stat;
	(void)trmtab;
	(void)ntmtab;
	return 1;
}
