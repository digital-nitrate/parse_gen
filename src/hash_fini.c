#include <stdlib.h>

#include "hash.h"

void hash_fini(struct hash_table* table) {
	free(table->bins);
}
