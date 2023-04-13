#include <stdlib.h>

#include "regular.h"
#include "regular_lex.h"
#include "regex_parse.h"

struct scan_t {
	FILE* src;
	unsigned int bad : 1, end : 1;
};

void* init_scanner(FILE* src) {
	struct scan_t* out = malloc(sizeof *out);
	if (out == NULL) return NULL;
	out->src = src;
	out->bad = 0;
	out->end = 0;
	return out;
}

void fini_scanner(void* yyscanner) {
	struct scan_t* scan = yyscanner;
	fclose(scan->src);
	free(scan);
}

int yylex(YYSTYPE* yylvalp, void* yyscanner) {
	struct scan_t* scan = yyscanner;
	if (scan->bad) return YYUNDEF;
	if (scan->end) return YYEOF;
	int c = fgetc(scan->src);
	if (c == EOF) {
		scan->end = 1;
		return YYEOF;
	}
	if (c < 0) {
		scan->bad = 1;
		return YYUNDEF;
	}
	switch (c) {
		case '\n':
			scan->end = 1;
			return YYEOF;
		case '|':
			return tokpipe;
		case '*':
			return tokkleene;
		case '+':
			return tokplus;
		case '(':
			return tokopen;
		case ')':
			return tokclose;
		case '.':
			return tokdot;
		case '-':
			return tokdash;
		case '\\':
			c = fgetc(scan->src);
			if (c == EOF || c < 0) {
				scan->bad = 1;
				return YYUNDEF;
			}
			switch (c) {
				case '|':
				case '+':
				case '(':
				case '-':
				case '*':
				case '.':
				case ')':
				case '\\':
					yylvalp->tokchar = c;
					return tokchar;
				case 'n':
					yylvalp->tokchar = '\n';
					return tokchar;
				case 's':
					yylvalp->tokchar = ' ';
					return tokchar;
				default:
					scan->bad = 1;
					return YYUNDEF;
			}
		default:
			yylvalp->tokchar = c;
			return tokchar;
	}
}
