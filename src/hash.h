#ifndef HASH_H
#define HASH_H 1

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


#ifdef __cplusplus
}
#endif

#endif
