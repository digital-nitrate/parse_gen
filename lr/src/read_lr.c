#include <stdlib.h>
#include <stdio.h>

#include "lr.h"

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

enum tok_type {
	TOKDOT,
	TOKNAME,
	TOKEND,
	TOKUINT,
	TOKSHIFT,
	TOKREDUCE,
	TOKSREDUCE,
	TOKCOMMA,
	TOKNL,
	TOKEOF,
	TOKERR,
};

union tok_data {
	unsigned int num;
};

struct lexer {
	FILE* src;
	int save;
	_Bool fln;
};

static enum tok_type lex(struct lexer* restrict l, union tok_data* restrict data) {
	if (l->fln == 0) {
		if (l->save == ',') {l->save = fgetc(l->src);return TOKCOMMA;}
		if ((l->save >= 'a' && l->save <= 'z') || (l->save >= 'A' && l->save <= 'Z')) {
			do {
				l->save = fgetc(l->src);
			} while ((l->save >= 'a' && l->save <= 'z') || (l->save >= 'A' && l->save <= 'Z'));	
			return TOKNAME;
		}
		if (l->save == '\n') {l->fln = 1;l->save = fgetc(l->src);return TOKNL;}
		if (l->save == '.') {l->save = fgetc(l->src);return TOKDOT;}
		if (l->save == '$') {l->save = fgetc(l->src);return TOKEND;}
	} else {
		if (l->save == ',') {l->save = fgetc(l->src);return TOKCOMMA;}
		if (l->save == 's') {
			l->save = fgetc(l->src);
			if (l->save != 'h') {fputs("expected 'h'\n", stderr);return TOKERR;}
			l->save = fgetc(l->src);
			if (l->save != '-') {fputs("expected '-'\n", stderr);return TOKERR;}
			l->save = fgetc(l->src);
			if (l->save < '0' || l->save > '9') {fputs("expected digit\n", stderr);return TOKERR;}
			unsigned int num = l->save - '0';
			l->save = fgetc(l->src);
			while (l->save >= '0' && l->save <= '9') {
				num = 10 * num + (l->save - '0');
				l->save = fgetc(l->src);
			}
			data->num = num;
			return TOKSHIFT;
		}
		if (l->save == 'r') {
			l->save = fgetc(l->src);
			if (l->save != '-') {fputs("expected '-'\n", stderr);return TOKERR;}
			l->save = fgetc(l->src);
			if (l->save < '0' || l->save > '9') {fputs("expected digit\n", stderr);return TOKERR;}
			unsigned int num = l->save - '0';
			l->save = fgetc(l->src);
			while (l->save >= '0' && l->save <= '9') {
				num = 10 * num + (l->save - '0');
				l->save = fgetc(l->src);
			}
			data->num = num;
			return TOKREDUCE;
		}
		if (l->save == 'R') {
			l->save = fgetc(l->src);
			if (l->save != '-') {fputs("expected '-'\n", stderr);return TOKERR;}
			l->save = fgetc(l->src);
			if (l->save < '0' || l->save > '9') {fputs("expected digit\n", stderr);return TOKERR;}
			unsigned int num = l->save - '0';
			l->save = fgetc(l->src);
			while (l->save >= '0' && l->save <= '9') {
				num = 10 * num + (l->save - '0');
				l->save = fgetc(l->src);
			}
			data->num = num;
			return TOKSREDUCE;
		}
		if (l->save >= '0' && l->save <= '9') {
			unsigned int num = l->save - '0';
			l->save = fgetc(l->src);
			while (l->save >= '0' && l->save <= '9') {
				num = 10 * num + (l->save - '0');
				l->save = fgetc(l->src);
			}
			data->num = num;
			return TOKUINT;
		}
		if (l->save == '\n') {l->fln = 1;l->save = fgetc(l->src);return TOKNL;}
		if (l->save == EOF) {return TOKEOF;}
	}
	fprintf(stderr, "unexpected \'%c\'\n", l->save);
	return TOKERR;
}

int read_lr(FILE* restrict src, lr_tab* restrict tab) {
	struct lexer l = {.src=src,.save=fgetc(src),.fln=0};
	union tok_data data;
	enum tok_type tok = lex(&l, &data);
	if (tok != TOKDOT) {
		fputs("expected TOKDOT\n", stderr);
		return 1;
	}
	tab->col_cnt = 0;
	while (1) {
		tok = lex(&l, &data);
		if (tok == TOKNL) break;
		if (tok != TOKCOMMA) {
			fputs("expected TOKCOMMA\n", stderr);
			return 1;
		}
		tok = lex(&l, &data);
		if (tok != TOKNAME && tok != TOKEND) {
			fputs("expected TOKNAME or TOKEND\n", stderr);
			return 1;
		}
		++(tab->col_cnt);
	}
	if (tab->col_cnt == 0) {
		fputs("needs at least one column\n", stderr);
		return 1;
	}
	lr_entry* dat = malloc((sizeof *dat) * tab->col_cnt * 16);
	if (dat == NULL) {
		fputs("memory failure\n", stderr);
		return 1;
	}
	size_t cap = tab->col_cnt * 16;
	size_t cnt = 0;
	int res = 0;
	while (1) {
		DYNARR_CHK(cnt,cap,dat,res);
		if (res) {
			fputs("memory failure\n", stderr);
			return 1;
		}
		tok = lex(&l, &data);
		if (tok == TOKEOF) break;
		if (tok != TOKUINT || data.num != tab->row_cnt) {
			fputs("expected correct TOKUINT\n", stderr);
			return 1;
		}
		for (size_t i = 0; i < tab->col_cnt; ++i) {
			if (tok != TOKCOMMA) {
				tok = lex(&l, &data);
				if (tok != TOKCOMMA) {
					fputs("expected TOKCOMMA\n", stderr);
					return 1;
				}
			}
			tok = lex(&l, &data);
			if ((i == tab->col_cnt - 1) ? (tok == TOKNL || tok == TOKEOF) : (tok == TOKCOMMA)) {
				dat[cnt + i] = (lr_entry){.end=1,.reduce=0};
			} else if (tok == TOKSHIFT) {
				dat[cnt + i] = (lr_entry){.end=0,.reduce=0,.loc=data.num};
			} else if (tok == TOKREDUCE) {
				dat[cnt + i] = (lr_entry){.end=0,.reduce=1,.loc=data.num};
			} else if (tok == TOKSREDUCE) {
				dat[cnt + i] = (lr_entry){.end=1,.reduce=1,.loc=data.num};
			} else {
				fputs("expected TOKSHIFT or TOKREDUCE or TOKSREDUCE\n", stderr);
				return 1;
			}
		}
		if (tok != TOKNL && tok != TOKEOF) {
			tok = lex(&l, &data);
			if (tok != TOKEOF && tok != TOKNL) {
				fputs("expected TOKEOF or TOKNL\n", stderr);
				return 1;
			}
		}
		cnt += tab->col_cnt;
		++(tab->row_cnt);
	}
	tab->data = dat;
	return 0;
}
