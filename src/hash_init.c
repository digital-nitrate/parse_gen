#include <stdlib.h>

#include "hash.h"

#define HASH_BASE 19

int hash_init(struct hash_table* table) {
	table->bins = malloc((sizeof *(table->bins)) * HASH_BASE);
	if (table->bins == NULL) return 1;
	for (size_t i = 0; i < HASH_BASE; ++i) table->bins[i].index.error = 1;
	table->bcnt = HASH_BASE;
	table->ecnt = 0;
	return 0;
}
