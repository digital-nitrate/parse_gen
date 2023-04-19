#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "lr.h"

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
	char* name;
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
			char* buffer = malloc(16);
			size_t curr = 0;
			size_t cap = 16;
			int res = 0;
			while ((l->save >= 'a' && l->save <= 'z') || (l->save >= 'A' && l->save <= 'Z')) {
				DYNARR_CHK(curr, cap, buffer, res);
				if (res) return 1;
				buffer[curr] = l->save;
				++curr;
				l->save = fgetc(l->src);
			}
			DYNARR_CHK(curr, cap, buffer, res);
			if (res) return 1;
			buffer[curr] = '\0';
			data->name = buffer;
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

int read_lr(FILE* restrict src, lr_tab* restrict tab, gr_ty const* restrict gram) {
	unsigned int* map = malloc((sizeof *map) * (gram->e_cnt - 1));
	if (map == NULL) return 1;
	for (size_t i = 0; i < gram->e_cnt - 1; ++i) map[i] = UINT_MAX;
	struct lexer l = {.src=src,.save=fgetc(src),.fln=0};
	union tok_data data;
	enum tok_type tok = lex(&l, &data);
	if (tok != TOKDOT) {
		fputs("expected TOKDOT\n", stderr);
		return 1;
	}
	for (size_t i = 0; i < gram->e_cnt - 1; ++i) {
		tok = lex(&l, &data);
		if (tok != TOKCOMMA) {
			fputs("expected TOKCOMMA\n", stderr);
			return 1;
		}
		tok = lex(&l, &data);
		if (tok != TOKNAME && tok != TOKEND) {
			fputs("expected TOKNAME\n", stderr);
			return 1;
		}
		unsigned int cd;
		if (tok == TOKNAME) {
			cd = sym_ld(gram, data.name);
			free(data.name);
		} else {
			cd = sym_ld(gram, "");
		}
		if (cd == UINT_MAX) {
			fputs("unknown symbol\n", stderr);
			return 1;
		}
		if (map[i] != UINT_MAX) {
			fputs("symbol already set\n", stderr);
			return 1;
		}
		map[i] = cd;
	}
	tok = lex(&l, &data);
	if (tok != TOKNL) {
		fputs("expected TOKNL\n", stderr);
		return 1;
	}
	lr_entry* dat = malloc((sizeof *dat) * gram->e_cnt * 16);
	if (dat == NULL) {
		fputs("memory failure\n", stderr);
		return 1;
	}
	size_t cap = gram->e_cnt * 16;
	size_t cnt = 0;
	int res = 0;
	tab->row_cnt = 0;
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
		for (size_t i = 0; i < gram->e_cnt - 1; ++i) {
			if (tok != TOKCOMMA) {
				tok = lex(&l, &data);
				if (tok != TOKCOMMA) {
					fputs("expected TOKCOMMA\n", stderr);
					return 1;
				}
			}
			tok = lex(&l, &data);
			if ((i == gram->e_cnt - 2) ? (tok == TOKNL || tok == TOKEOF) : (tok == TOKCOMMA)) {
				dat[cnt + map[i]] = (lr_entry){.end=1,.reduce=0};
			} else if (tok == TOKSHIFT) {
				dat[cnt + map[i]] = (lr_entry){.end=0,.reduce=0,.loc=data.num};
			} else if (tok == TOKREDUCE) {
				dat[cnt + map[i]] = (lr_entry){.end=0,.reduce=1,.loc=data.num - 1};
			} else if (tok == TOKSREDUCE) {
				dat[cnt + map[i]] = (lr_entry){.end=1,.reduce=1,.loc=data.num - 1};
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
		cnt += gram->e_cnt;
		++(tab->row_cnt);
	}
	tab->data = dat;
	return 0;
}
