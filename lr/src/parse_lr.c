#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "lr.h"

struct node {
	struct node** subs;
	size_t sub_cnt;
	unsigned int type;
};

struct ann_node {
	struct node* dat;
	unsigned int state;
};

static void print_node(struct node* node, gr_ty const* restrict gram) {
	for (size_t i = 0; i < gram->b_cnt; ++i) {
		if (gram->bins[i].id == node->type) fputs(gram->bins[i].name, stdout);
	}
	if (node->sub_cnt != 0) {
		fputc('(', stdout);
		print_node(node->subs[0], gram);
		for (size_t i = 1; i < node->sub_cnt; ++i) {
			fputc(',', stdout);
			print_node(node->subs[i], gram);
		}
		fputc(')', stdout);
	}
}

static unsigned int ntk(FILE* restrict src, gr_ty const* restrict gram) {
	char* buffer = malloc(16);
	size_t curr = 0;
	size_t cap = 16;
	int res = 0;
	int c = fgetc(src);
	while (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
		DYNARR_CHK(curr,cap,buffer,res);
		if (res) return UINT_MAX;
		buffer[curr] = c;
		++curr;
		c = fgetc(src);
	}
	while (c != '\n' && c != EOF) c = fgetc(src);
	DYNARR_CHK(curr,cap,buffer,res);
	if (res) return UINT_MAX;
	buffer[curr] = '\0';
	unsigned int i = sym_ld(gram, buffer);
	free(buffer);
	return i;
}

void parse_lr(FILE* restrict src, lr_tab const* restrict tab, gr_ty const* restrict gram) {
	struct ann_node* ar = malloc((sizeof *ar) * 16);
	size_t ar_curr = 1;
	size_t ar_cap = 16;
	struct node** rl = malloc((sizeof *rl) * 16);
	size_t rl_curr = 0;
	size_t rl_cap = 16;
	struct node* t_top = NULL;
	ar[0].dat = NULL;
	ar[0].state = 0;
	int res = 0;
	while (1) {
		struct node* look;
		if (rl_curr == 0) {
			if (t_top == NULL) {
				t_top = malloc(sizeof *t_top);
				t_top->subs = NULL;
				t_top->sub_cnt = 0;
				t_top->type = ntk(src, gram);
			}
			look = t_top;
		} else {
			look = rl[rl_curr - 1];
		}
		lr_entry act = tab->data[ar[ar_curr - 1].state * (gram->e_cnt) + look->type];
		if (act.end) {
			if (act.reduce) {
				t_top = malloc(sizeof *t_top);
				t_top->subs = malloc((sizeof *(t_top->subs)) * gram->rules[act.loc].len);
				t_top->sub_cnt = gram->rules[act.loc].len;
				t_top->type = gram->rules[act.loc].lsid;
				for (size_t i = 0; i < gram->rules[act.loc].len; ++i) {
					t_top->subs[i] = ar[ar_curr - gram->rules[act.loc].len + i].dat;
				}
				break;
			} else {
				fprintf(stdout, "syntax error\n");
				return;
			}
		} else {
			if (act.reduce) {
				DYNARR_CHK(rl_curr, rl_cap, rl,res);
				if (res) return;
				rl[rl_curr] = malloc(sizeof *(rl[rl_curr]));
				rl[rl_curr]->subs = malloc((sizeof *(rl[rl_curr]->subs)) * gram->rules[act.loc].len);
				rl[rl_curr]->sub_cnt = gram->rules[act.loc].len;
				rl[rl_curr]->type = gram->rules[act.loc].lsid;
				for (size_t i = 0; i < gram->rules[act.loc].len; ++i) {
					rl[rl_curr]->subs[i] = ar[ar_curr - gram->rules[act.loc].len + i].dat;
				}
				++(rl_curr);
				ar_curr -= gram->rules[act.loc].len;
			} else {
				if (look == t_top) t_top = NULL;
				else --rl_curr;
				DYNARR_CHK(ar_curr, ar_cap, ar,res);
				if (res) return;
				ar[ar_curr].dat = look;
				ar[ar_curr].state = act.loc;
				++ar_curr;
			}
		}
	}
	print_node(t_top, gram);
	fputc('\n', stdout);
}
