#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "lr.h"

enum tok_type {
	TOK_SYM,
	TOK_LAM,
	TOK_ARR,
	TOK_MID,
	TOK_NL,
	TOK_EOF,
	TOK_UND,
};

union tok_data {
	char* name;
};

struct lexer {
	FILE* src;
	int save;
};

static enum tok_type lex(struct lexer* restrict l, union tok_data* restrict data) {
	while (l->save == ' ' || l->save == '\t') {
		l->save = fgetc(l->src);
	}
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
		if (strcmp(buffer, "lambda") == 0) {
			free(buffer);
			return TOK_LAM;
		} else {
			data->name = buffer;
			return TOK_SYM;
		}
	}
	if (l->save == '|') {
		l->save = fgetc(l->src);
		return TOK_MID;
	}
	if (l->save == '\n') {
		l->save = fgetc(l->src);
		return TOK_NL;
	}
	if (l->save == '-') {
		l->save = fgetc(l->src);
		if (l->save == '>') {
			l->save = fgetc(l->src);
			return TOK_ARR;
		}
		return TOK_UND;
	}
	if (l->save == '$') {
		l->save = fgetc(l->src);
		data->name = malloc(1);
		data->name[0] = '\0';
		return TOK_SYM;
	}
	if (l->save == EOF) return TOK_EOF;
	return TOK_UND;
}

int read_gr(FILE* restrict src, gr_ty* restrict gram) {
	struct lexer l = {.src=src,.save=fgetc(src)};
	union tok_data data;
	gram->bins = malloc((sizeof *(gram->bins)) * 19);
	gram->b_cnt = 19;
	for (size_t i = 0; i < 19; ++i) {
		gram->bins[i].id = UINT_MAX;
	}
	gram->e_cnt = 0;
	gram->rules = malloc((sizeof *(gram->rules)) * 16);
	gram->rule_cnt = 0;
	size_t rule_cap = 16;
	enum tok_type tok;
	unsigned int lhs = UINT_MAX;
	while (1) {
		tok = lex(&l, &data);
		if (tok == TOK_EOF) break;
		if (tok == TOK_SYM) {
			lhs = sym_mad(gram, data.name);
			free(data.name);
			if (lhs == UINT_MAX) {
				fputs("memory failure\n", stderr);
				return 1;
			}
			tok = lex(&l, &data);
			if (tok != TOK_ARR) return 1;
		} else if (lhs == UINT_MAX || tok != TOK_MID) {
			fputs("unexpected token\n", stderr);
			return 1;
		}
		int res = 0;
		DYNARR_CHK(gram->rule_cnt, rule_cap, gram->rules, res);
		if (res) return 1;
		gr_rl* rule = gram->rules + gram->rule_cnt;
		rule->rsids = malloc((sizeof *(rule->rsids)) * 16);
		rule->len = 0;
		rule->lsid = lhs;
		size_t rc = 16;
		while (1) {
			tok = lex(&l, &data);
			if (tok == TOK_NL || tok == TOK_EOF) break;
			if (tok == TOK_SYM) {
				unsigned int hc = sym_mad(gram, data.name);
				free(data.name);
				if (hc == UINT_MAX) return 1;
				DYNARR_CHK(rule->len, rc, rule->rsids, res);
				if (res) return 1;
				rule->rsids[rule->len] = hc;
				++(rule->len);
			} else if (tok == TOK_MID) {
				++(gram->rule_cnt);
				DYNARR_CHK(gram->rule_cnt, rule_cap, gram->rules, res);
				if (res) return 1;
				rule = gram->rules + gram->rule_cnt;
				rule->rsids = malloc((sizeof *(rule->rsids)) * 16);
				rule->len = 0;
				rule->lsid = lhs;
				rc = 16;
			} else if (tok != TOK_LAM) return 1;
		}
		++(gram->rule_cnt);
	}
	return 0;
}
