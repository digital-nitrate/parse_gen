#include <stdlib.h>
#include <string.h>

#include "generator.h"
#include "hash.h"
#include "macros.h"

#define HASH_LOAD 0.7

static uintmax_t compute_hash(struct string name) {
	uintmax_t sum = 1337;
	for (size_t i = 0; i < name.len; ++i) sum = 17 * sum + name.data[i];
	return sum;
}

gen_sid hash_ld(gen_type* out, struct hash_table* hash, struct string name) {
	uintmax_t hs = compute_hash(name);
	size_t loc = hs % hash->bcnt;
	while (1) {
		if (hash->bins[loc].index.error) return (gen_sid){.error=1};
		struct string cname = hash->bins[loc].index.term ? out->tokens[hash->bins[loc].index.ind].name : out->nterms[hash->bins[loc].index.ind].name;
		if (cname.len == name.len && memcmp(name.data, cname.data, name.len) == 0) return hash->bins[loc].index;
		loc = (loc + 1) % hash->bcnt;
	}
}

gen_sid hash_add(gen_type* out, struct hash_table* hash, struct string name, _Bool term) {
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
	if (term) DYNARR_CHK(out->token_cnt, out->token_cap, out->tokens, res);
	else DYNARR_CHK(out->nterm_cnt, out->nterm_cap, out->nterms, res);
	if (res) return (gen_sid){.error=1,.term=1};
	uintmax_t hs = compute_hash(name);
	size_t loc = hs % hash->bcnt;
	while (1) {
		if (hash->bins[loc].index.error) break;
		struct string cname = hash->bins[loc].index.term ? out->tokens[hash->bins[loc].index.ind].name : out->nterms[hash->bins[loc].index.ind].name;
		if (cname.len == name.len && memcmp(name.data, cname.data, name.len) == 0) return (gen_sid){.error=1,.term=0};
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
