#include <stdio.h>
#include <stdlib.h>

#include "generator.h"

int main(int argc, char** argv) {
	int status = EXIT_FAILURE;
	if (argc != 3) {
		fputs("unexpected number of arguments -- expected 3\nusage: parse_gen <srcfile> <outfile>\n", stderr);
		goto Finish;
	}
	FILE* input = fopen(argv[1], "rb");
	if (input == NULL) {
		fprintf(stderr, "could not open \"%s\"\n", argv[1]);
		goto Finish;
	}
	FILE* output = fopen(argv[2], "wb");
	if (output == NULL) {
		fprintf(stderr, "could not open \"%s\"\n", argv[2]);
		goto CleanInput;
	}
	gen_type gen;
	if (gen_bld(input, &gen)) goto CleanOutput;
	unsigned int* ll1 = gen_bld_ll1(&gen);
	if (ll1 == NULL) goto CleanGen;
	gen_wrt_ll1(&gen, ll1, output);
	status = EXIT_SUCCESS;
	free(ll1);
	CleanGen: gen_fini(&gen);
	CleanOutput: fclose(output);
	CleanInput: fclose(input);
	Finish: return status;
}
