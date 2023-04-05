#include <stdlib.h>
#include <limits.h>

#include "generator.h"

unsigned int* gen_bld_ll1(gen_type const* gen) {
	unsigned int* table = malloc((sizeof *table) * (gen->token_cnt + 1) * gen->nterm_cnt);
	size_t* rsaves = malloc((sizeof *rsaves) * gen->rule_cnt);
	_Bool* lambda = malloc((sizeof *lambda) * gen->nterm_cnt);
	_Bool* first = malloc((sizeof *first) * gen->nterm_cnt);
	_Bool* follow = malloc((sizeof *follow) * gen->nterm_cnt);
	gen_sid* queue = malloc((sizeof *queue) * gen->nterm_cnt);
	size_t queue_curr = 0;
	size_t queue_top = 0;
	for (size_t i = 0; i < gen->nterm_cnt * (gen->token_cnt + 1); ++i) table[i] = UINT_MAX;
	for (size_t i = 0; i < gen->nterm_cnt; ++i) lambda[i] = 0;
	for (size_t i = 0; i < gen->rule_cnt; ++i) {
		rsaves[i] = 0;
		if (gen->rules[i].rhs.cnt == 0 && lambda[gen->rules[i].lhs.ind] == 0) {
			lambda[gen->rules[i].lhs.ind] = 1;
			queue[queue_top] = gen->rules[i].lhs;
			++queue_top;
		}
	}
	while (queue_curr != queue_top) {
		gen_rarr rs = gen->nterms[queue[queue_curr].ind].rh;
		for (size_t i = 0; i < rs.cnt; ++i) {
			gen_rule* rule = &(gen->rules[rs.rules[i].rule]);
			if (lambda[rule->lhs.ind]) continue;
			if (rs.rules[i].loc > rsaves[rs.rules[i].rule]) continue;
			size_t cntr = rs.rules[i].loc;
			while (cntr < rule->rhs.cnt && rule->rhs.syms[cntr].term == 0 && lambda[rule->rhs.syms[cntr].ind]) ++cntr;
			if (cntr == rule->rhs.cnt) {
				lambda[rule->lhs.ind] = 1;
				queue[queue_top] = rule->lhs;
				++queue_top;
			}
			rsaves[rs.rules[i].rule] = cntr;
		}
		++queue_curr;
	}
	for (size_t i = 0; i < gen->rule_cnt;++i) {
		gen_slist rhs = gen->rules[i].rhs;
		size_t cntr = rsaves[i];
		while (cntr < rhs.cnt && rhs.syms[cntr].term == 0 && lambda[rhs.syms[cntr].ind]) ++cntr;
		rsaves[i] = cntr;
	}
	for (size_t t = 0; t < gen->token_cnt; ++t) {
		queue_curr = 0;
		queue_top = 0;
		for (size_t i = 0; i < gen->nterm_cnt; ++i) {
			first[i] = 0;
			follow[i] = 0;
		}
		gen_rarr r = gen->tokens[t].rh;
		for (size_t i = 0; i < r.cnt; ++i) {
			if (rsaves[r.rules[i].rule] < r.rules[i].loc) continue;
			gen_sid lhs = gen->rules[r.rules[i].rule].lhs;
			if (first[lhs.ind] == 0) {
				first[lhs.ind] = 1;
				queue[queue_top] = lhs;
				++queue_top;
			}
			unsigned int tr = table[lhs.ind * (gen->token_cnt + 1) + t];
			if (tr != UINT_MAX && tr != r.rules[i].rule) {
				fprintf(stderr, "conflict for rules %u -- %u at (%u,%zu) {first set}\n", tr, r.rules[i].rule, lhs.ind, t);
			} else {
				table[lhs.ind * (gen->token_cnt + 1) + t] = r.rules[i].rule;
			}
		}
		while (queue_curr != queue_top) {
			gen_rarr in = gen->nterms[queue[queue_curr].ind].rh;
			for (size_t i = 0; i < in.cnt; ++i) {
				if (rsaves[in.rules[i].rule] < in.rules[i].loc) continue;
				gen_sid lhs = gen->rules[in.rules[i].rule].lhs;
				if (first[lhs.ind] == 0) {
					first[lhs.ind] = 1;
					queue[queue_top] = lhs;
					++queue_top;
				}
				unsigned int tr = table[lhs.ind * (gen->token_cnt + 1) + t];
				if (tr != UINT_MAX && tr != in.rules[i].rule) {
					fprintf(stderr, "conflict for rules %u -- %u at (%u,%zu) {first set}\n", tr, in.rules[i].rule, lhs.ind, t);
				} else {
					table[lhs.ind * (gen->token_cnt + 1) + t] = in.rules[i].rule;
				}
			}
			++queue_curr;
		}
		queue_curr = 0;
		queue_top = 0;
		for (size_t i = 0; i < r.cnt; ++i) {
			gen_rule* rule = &(gen->rules[r.rules[i].rule]);
			size_t cntr = r.rules[i].loc;
			while (1) {
				if (cntr == 0) {
					unsigned int tr = table[rule->lhs.ind * (gen->token_cnt + 1) + t];
					if (tr != UINT_MAX && tr != r.rules[i].rule) {
						fprintf(stderr, "conflict for rules %u -- %u at (%u,%zu) {follow set}\n", tr, r.rules[i].rule, rule->lhs.ind, t);
					} else {
						table[rule->lhs.ind * (gen->token_cnt + 1) + t] = r.rules[i].rule;
					}
					break;
				}
				--cntr;
				gen_sid sym = rule->rhs.syms[cntr];
				if (sym.term == 0) {
					if (follow[sym.ind] == 0) {
						follow[sym.ind] = 1;
						queue[queue_top] = sym;
						++queue_top;
					}
					if (lambda[sym.ind] == 0 || first[sym.ind] == 1) break;
				} else {
					break;
				}
			}
		}
		for (size_t i = 0; i < gen->nterm_cnt; ++i) {
			if (first[i] == 0) continue;
			gen_rarr in = gen->nterms[i].rh;
			for (size_t i = 0; i < in.cnt; ++i) {
				gen_rule* rule = &(gen->rules[in.rules[i].rule]);
				size_t cntr = in.rules[i].loc;
				while (1) {
					if (cntr == 0) {
						unsigned int tr = table[rule->lhs.ind * (gen->token_cnt + 1) + t];
						if (tr != UINT_MAX && tr != in.rules[i].rule) {
							fprintf(stderr, "conflict for rules %u -- %u at (%u,%zu) {follow set}\n", tr, in.rules[i].rule, rule->lhs.ind, t);
						} else {
							table[rule->lhs.ind * (gen->token_cnt + 1) + t] = in.rules[i].rule;
						}
						break;
					}
					--cntr;
					gen_sid sym = rule->rhs.syms[cntr];
					if (sym.term == 0) {
						if (follow[sym.ind] == 0) {
							follow[sym.ind] = 1;
							queue[queue_top] = sym;
							++queue_top;
						}
						if (lambda[sym.ind] == 0 || first[sym.ind] == 1) break;
					} else {
						break;
					}
				}
			}
		}
		while (queue_curr != queue_top) {
			gen_rarr in = gen->nterms[queue[queue_curr].ind].lh;
			for (size_t i = 0; i < in.cnt; ++i) {
				gen_rule* rule = &(gen->rules[in.rules[i].rule]);
				size_t cntr = in.rules[i].loc;
				while (1) {
					if (cntr == 0) {
						unsigned int tr = table[rule->lhs.ind * (gen->token_cnt + 1) + t];
						if (tr != UINT_MAX && tr != in.rules[i].rule) {
							fprintf(stderr, "conflict for rules %u -- %u at (%u,%zu) {follow set}\n", tr, in.rules[i].rule, rule->lhs.ind, t);
						} else {
							table[rule->lhs.ind * (gen->token_cnt + 1) + t] = in.rules[i].rule;
						}
						break;
					}
					--cntr;
					gen_sid sym = rule->rhs.syms[cntr];
					if (sym.term == 0) {
						if (follow[sym.ind] == 0) {
							follow[sym.ind] = 1;
							queue[queue_top] = sym;
							++queue_top;
						}
						if (lambda[sym.ind] == 0 || first[sym.ind] == 1) break;
					} else {
						break;
					}
				}
			}
			++queue_curr;
		}
	}
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		follow[i] = 0;
	}
	queue_curr = 0;
	queue_top = 1;
	queue[0] = gen->start;
	follow[gen->start.ind] = 1;
	while (queue_curr != queue_top) {
		gen_rarr in = gen->nterms[queue[queue_curr].ind].lh;
		for (size_t i = 0; i < in.cnt; ++i) {
			gen_rule* rule = &(gen->rules[in.rules[i].rule]);
			size_t cntr = in.rules[i].loc;
			while (1) {
				if (cntr == 0) {
					unsigned int tr = table[rule->lhs.ind * (gen->token_cnt + 1) + gen->token_cnt];
					if (tr != UINT_MAX && tr != in.rules[i].rule) {
						fprintf(stderr, "conflict for rules %u -- %u at (%u,%zu) {follow set}\n", tr, in.rules[i].rule, rule->lhs.ind, gen->token_cnt);
					} else {
						table[rule->lhs.ind * (gen->token_cnt + 1) + gen->token_cnt] = in.rules[i].rule;
					}
					break;
				}
				--cntr;
				gen_sid sym = rule->rhs.syms[cntr];
				if (sym.term == 0) {
					if (follow[sym.ind] == 0) {
						follow[sym.ind] = 1;
						queue[queue_top] = sym;
						++queue_top;
					}
					if (lambda[sym.ind] == 0) break;
				} else {
					break;
				}
			}
		}
		++queue_curr;		
	}
	free(queue);
	free(follow);
	free(first);
	free(lambda);
	free(rsaves);
	return table;
}
