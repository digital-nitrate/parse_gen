#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "lr.h"

#define HASH_LOAD 0.7

static uintmax_t compute_hash(char const* name) {
	uintmax_t sum = 1337;
	while (*name != '\0') {
		sum = 17 * sum + *name;
		++name;
	}
	return sum;
}

unsigned int sym_ld(gr_ty const* restrict gram, char const* restrict name) {
	uintmax_t hs = compute_hash(name);
	size_t loc = hs % gram->b_cnt;
	while (1) {
		if (gram->bins[loc].id == UINT_MAX) return UINT_MAX;
		if (strcmp(name, gram->bins[loc].name) == 0) return gram->bins[loc].id;
		loc = (loc + 1) % gram->b_cnt;
	}
}

unsigned int sym_mad(gr_ty* restrict gram, char const* restrict name) {
	if (gram->b_cnt * HASH_LOAD <= gram->e_cnt) {
		size_t nb = gram->b_cnt * 2 - 1;
		gr_bin* nbins = malloc((sizeof *nbins) * nb);
		if (nbins == NULL) return UINT_MAX;
		for (size_t i = 0; i < nb; ++i) nbins[i].id = UINT_MAX;
		for (size_t i = 0; i < gram->b_cnt; ++i) {
			if (gram->bins[i].id != UINT_MAX) {
				size_t loc = gram->bins[i].hval % nb;
				while (nbins[loc].id != UINT_MAX) loc = (loc + 1) % nb;
				nbins[loc] = gram->bins[i];
			}
		}
		free(gram->bins);
		gram->b_cnt = nb;
		gram->bins = nbins;
	}
	uintmax_t hs = compute_hash(name);
	size_t loc = hs % gram->b_cnt;
	while (1) {
		if (gram->bins[loc].id == UINT_MAX) break;
		if (strcmp(name, gram->bins[loc].name) == 0) return gram->bins[loc].id;
		loc = (loc + 1) % gram->b_cnt;
	}
	gram->bins[loc].hval = hs;
	gram->bins[loc].id = gram->e_cnt;
	gram->bins[loc].name = malloc(strlen(name) + 1);
	strcpy(gram->bins[loc].name, name);
	++(gram->e_cnt);
	return gram->bins[loc].id;
}
