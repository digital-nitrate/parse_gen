#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "generator.h"

int main(int argc, char** argv) {
	_Static_assert(sizeof(gen_sind) * CHAR_BIT >= GEN_SIND_BIT, "gen_sind type too small");
	_Static_assert(sizeof(gen_rind) * CHAR_BIT >= GEN_RIND_BIT, "gen_rind type too small");
	_Static_assert(sizeof(gen_roff) * CHAR_BIT >= GEN_ROFF_BIT, "gen_roff type too small");
	int status = EXIT_FAILURE;
	if (argc != 3 && argc != 4) {
		fputs("unexpected number of arguments -- expected 3\nusage: parse_gen <srcfile> <outfile> <headerfile>?\n", stderr);
		goto Finish;
	}
	gen_type gen;
	if (gen_bld(argv[1], &gen)) goto Finish;
	unsigned int* ll1 = gen_bld_ll1(&gen);
	if (ll1 == NULL) goto CleanGen;
	if (gen_wrt_ll1(&gen, ll1, argv[2])) goto CleanGen;
	if (argc == 4) {
		if (gen_wrt_ll1_h(&gen, argv[3])) goto CleanGen;
	}
	status = EXIT_SUCCESS;
	free(ll1);
	CleanGen: gen_fini(&gen);
	Finish: return status;
}
