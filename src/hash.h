#ifndef HASH_H
#define HASH_H 1

#include <stdint.h>

#include "generator.h"

#ifdef __cplusplus
extern "C" {
#endif

struct hash_bin {
	uintmax_t hval;
	gen_sid index;
};

struct hash_table {
	struct hash_bin* bins;
	size_t bcnt;
	size_t ecnt;
};

extern int hash_init(struct hash_table*);
extern gen_sid hash_ld(gen_type*, struct hash_table*, char const*);
extern gen_sid hash_add(gen_type*, struct hash_table*, char*, _Bool);
extern void hash_fini(struct hash_table*);

#ifdef __cplusplus
}
#endif

#endif
