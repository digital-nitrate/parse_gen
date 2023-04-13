#include <stdio.h>
#include <stdlib.h>

#include "generator.h"

int main(int argc, char** argv) {
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
