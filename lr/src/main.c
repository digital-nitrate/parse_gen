#include <stdio.h>

#include "lr.h"

int main(int argc, char** argv) {
	if (argc != 4) {
		fputs("usage: lr_parse <cfg> <lr> <tok>\n", stderr);
		return 1;
	}
	FILE* cfg_in = fopen(argv[1], "rb");
	if (cfg_in == NULL) {
		fprintf(stderr, "could not open \"%s\"\n", argv[1]);
		return 1;
	}
	gr_ty gram;
	int res = read_gr(cfg_in, &gram);
	fclose(cfg_in);
	if (res) return 1;
	FILE* lr_in = fopen(argv[2], "rb");
	if (lr_in == NULL) {
		fprintf(stderr, "could not open \"%s\"\n", argv[2]);
		return 1;
	}
	lr_tab tab;
	res = read_lr(lr_in, &tab, &gram);
	fclose(lr_in);
	if (res) return 1;
	FILE* tok_in = fopen(argv[3], "rb");
	if (tok_in == NULL) {
		fprintf(stderr, "could not open \"%s\"\n", argv[3]);
		return 1;
	}
	parse_lr(tok_in, &tab, &gram);
	fclose(tok_in);
	return 0;
}
