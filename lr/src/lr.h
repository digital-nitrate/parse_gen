#ifndef LR_H
#define LR_H 1

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lr_entry {
	unsigned int end : 1, reduce : 1, loc : 30;
} lr_entry;

typedef struct lr_tab {
	lr_entry* data;
	unsigned int col_cnt;
	unsigned int row_cnt;
} lr_tab;

extern int read_lr(FILE* restrict, lr_tab* restrict);

#ifdef __cplusplus
}
#endif

#endif
