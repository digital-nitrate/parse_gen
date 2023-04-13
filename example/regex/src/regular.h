#ifndef REGULAR_H
#define REGULAR_H 1

#include <limits.h>
#include <stddef.h>

#define DYNARR_CHK(cnt, cap, arr, res) do {\
	if ((cnt) == (cap)) {\
		size_t nc = 2 * (cap);\
		void* np = realloc((arr), (sizeof *(arr)) * nc);\
		if (np == NULL) {\
			(res) = 1;\
		} else {\
			(cap) = nc;\
			(arr) = np;\
		}\
	}\
} while (0)

typedef struct reg_nfa_lam {
	unsigned int* paths;
	size_t cnt;
	size_t cap;
} reg_nfa_lam;

typedef struct reg_nfa_node {
	reg_nfa_lam lambda;	
	unsigned int tran[1 << CHAR_BIT];
} reg_nfa_node;

typedef struct reg_nfa_table {
	reg_nfa_node* nodes;
	size_t node_cnt;
	size_t node_cap;
} reg_nfa_table;

typedef struct reg_in {
	unsigned int start;
	unsigned int end;
} reg_in;

typedef struct reg_nfa {
	reg_nfa_table table;
	reg_in specials;
} reg_nfa;

struct reg_link {
	struct reg_link* next;
	reg_in sub;
};

enum atommod_type {
	MOD_NONE,
	MOD_PLUS,
	MOD_KLEENE,
};

enum charrng_type {
	RNG_NONE,
	RNG_TOP,
};

struct charrng {
	enum charrng_type type;
	char top;
};

#endif
