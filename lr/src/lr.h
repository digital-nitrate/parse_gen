#ifndef LR_H
#define LR_H 1

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DYNARR_CHK(cnt,cap,arr,res) do {\
	if ((cnt) == (cap)) {\
		size_t nc = (cap) * 2;\
		void* np = realloc((arr), (sizeof *(arr)) * nc);\
		if (np == NULL) {\
			(res) = 1;\
		} else {\
			(cap) = nc;\
			(arr) = np;\
		}\
	}\
} while (0)

typedef struct gr_rl {
	unsigned int* rsids;
	size_t len;
	unsigned int lsid;
} gr_rl;

typedef struct gr_bin {
	char* name;
	uintmax_t hval;
	unsigned int id;
} gr_bin;

typedef struct gr_ty {
	gr_bin* bins;
	gr_rl* rules;
	size_t rule_cnt;
	size_t e_cnt;
	size_t b_cnt;
} gr_ty;

typedef struct lr_entry {
	unsigned int end : 1, reduce : 1, loc : 30;
} lr_entry;

typedef struct lr_tab {
	lr_entry* data;
	unsigned int row_cnt;
} lr_tab;

extern int read_gr(FILE* restrict, gr_ty* restrict);
extern int read_lr(FILE* restrict, lr_tab* restrict, gr_ty const* restrict);
extern void parse_lr(FILE* restrict, lr_tab const* restrict, gr_ty const* restrict);
extern unsigned int sym_ld(gr_ty const* restrict, char const* restrict);
extern unsigned int sym_mad(gr_ty* restrict, char const* restrict);

#ifdef __cplusplus
}
#endif

#endif
