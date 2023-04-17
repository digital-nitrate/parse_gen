#include <stdio.h>

#include "lr.h"

int main(void) {
	lr_tab tab;
	if (read_lr(stdin, &tab)) return 1;
	fprintf(stdout, "Row: %u\nCol: %u\nData:\n", tab.row_cnt, tab.col_cnt);
	for (size_t i = 0; i < tab.row_cnt; ++i) {
		for (size_t j = 0; j < tab.col_cnt; ++j) {
			fprintf(stdout, "(%zu,%zu) -> ", i, j);
			lr_entry ent = tab.data[i * tab.col_cnt + j];
			if (ent.end && !ent.reduce) {
				fputs("Error\n", stdout);
			} else if (ent.end && ent.reduce) {
				fprintf(stdout, "Accept Reduce %u\n", ent.loc);
			} else if (!ent.end && !ent.reduce) {
				fprintf(stdout, "Shift %u\n", ent.loc);
			} else if (!ent.end && ent.reduce) {
				fprintf(stdout, "Reduce %u\n", ent.loc);
			}
		}
	}
	return 0;
}
