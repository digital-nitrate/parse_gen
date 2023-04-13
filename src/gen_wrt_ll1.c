#include <stdio.h>
#include <limits.h>

#include "generator.h"

static char const defprelo[] = "yy";
static char const defprehi[] = "YY";

int gen_wrt_ll1_h(gen_type const* restrict gen, char const* restrict outpath) {
	FILE* out = fopen(outpath, "wb");
	if (out == NULL) return 1;
	char const* prehi = gen->prefixhi == NULL ? defprehi : gen->prefixhi;
	int curr_line = 1;
	curr_line += 6 + gen->token_cnt;
	fprintf(out, "#ifndef %sPLL_H\n#define %sPLL_H 1\nenum {\n%sEOF = 0,\n%sUNDEF = -1,\n", prehi, prehi, prehi, prehi);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "%s = %u,\n", gen->tokens[i].name, gen->tokens[i].id);
	}
	fputs("};\n", out);
	if (gen->req != NULL) {
		curr_line += 3 + gen->req_loc.last_line - gen->req_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n%s\n#line %d \"%s\"\n", gen->req_loc.first_line, gen->fname, gen->req, curr_line, outpath);
	}
	++curr_line;
	fprintf(out, "typedef union %sSTYPE {\n", prehi);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		if (gen->tokens[i].type != NULL) {
			++curr_line;
			fprintf(out, "\t%s %s;\n", gen->tokens[i].type, gen->tokens[i].name);
		}
	}
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		if (gen->nterms[i].type != NULL) {
			++curr_line;
			fprintf(out, "\t%s %s;\n", gen->nterms[i].type, gen->nterms[i].name);
		}
	}
	++curr_line;
	fprintf(out, "} %sSTYPE;\n", prehi);
	if (gen->prov != NULL) {
		curr_line += 3 + gen->prov_loc.last_line - gen->prov_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n%s\n#line %d \"%s\"\n", gen->prov_loc.first_line, gen->fname, gen->prov, curr_line, outpath);
	}
	curr_line += 2;
	fputs("\n#endif\n", out);
	fclose(out);
	return 0;
}

int gen_wrt_ll1(gen_type const* restrict gen, unsigned int const* restrict table, char const* restrict outpath) {
	FILE* out = fopen(outpath, "wb");
	if (out == NULL) return 1;
	size_t max_len = 0;
	int curr_line = 1;
	for (size_t i = 0; i < gen->rule_cnt; ++i) {
		if (gen->rules[i].rhs.cnt > max_len) max_len = gen->rules[i].rhs.cnt;
	}
	char const* prelo = gen->prefixlo == NULL ? defprelo : gen->prefixlo;
	char const* prehi = gen->prefixhi == NULL ? defprehi : gen->prefixhi;
	if (gen->top != NULL) {
		curr_line += 3 + gen->top_loc.last_line - gen->top_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n%s\n#line %d \"%s\"\n", gen->top_loc.first_line, gen->fname, gen->top, curr_line, outpath);
	}
	curr_line += 6 + gen->token_cnt;
	fprintf(out, "#ifndef %sPLL_H\n#define %sPLL_H 1\nenum {\n%sEOF = 0,\n%sUNDEF = -1,\n", prehi, prehi, prehi, prehi);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "%s = %u,\n", gen->tokens[i].name, gen->tokens[i].id);
	}
	fputs("};\n", out);
	if (gen->req != NULL) {
		curr_line += 3 + gen->req_loc.last_line - gen->req_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n%s\n#line %d \"%s\"\n", gen->req_loc.first_line, gen->fname, gen->req, curr_line, outpath);
	}
	++curr_line;
	fprintf(out, "typedef union %sSTYPE {\n", prehi);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		if (gen->tokens[i].type != NULL) {
			++curr_line;
			fprintf(out, "\t%s %s;\n", gen->tokens[i].type, gen->tokens[i].name);
		}
	}
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		if (gen->nterms[i].type != NULL) {
			++curr_line;
			fprintf(out, "\t%s %s;\n", gen->nterms[i].type, gen->nterms[i].name);
		}
	}
	++curr_line;
	fprintf(out, "} %sSTYPE;\n", prehi);
	if (gen->prov != NULL) {
		curr_line += 3 + gen->prov_loc.last_line - gen->prov_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n%s\n#line %d \"%s\"\n", gen->prov_loc.first_line, gen->fname, gen->prov, curr_line, outpath);
	}
	curr_line += 2;
	fputs("\n#endif\n", out);
	if (gen->code != NULL) {
		curr_line += 3 + gen->code_loc.last_line - gen->code_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n%s\n#line %d \"%s\"\n", gen->code_loc.first_line, gen->fname, gen->code, curr_line, outpath);
	}
	curr_line += 11 + gen->token_cnt + gen->nterm_cnt;
	fprintf(out, "static unsigned int _%stran(int code) {\nswitch (code) {\n", prelo);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "case %s: return %zu;\n", gen->tokens[i].name, i);
	}
	fprintf(out, "case 0: return %zu;\ndefault: return %zu;\n}\n}\nstatic unsigned int const _%stab[] = {\n", gen->token_cnt, gen->token_cnt + 1, prelo);
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		for (size_t j = 0; j < gen->token_cnt + 1; ++j) {
			unsigned int v = table[i * (gen->token_cnt + 1) + j];
			fprintf(out, "%zu, ", v == UINT_MAX ? gen->rule_cnt : v);
		}
		fprintf(out, "%zu,\n", gen->rule_cnt);
	}
	fprintf(out, "};\nstatic int _%sreq(unsigned int _%sid, unsigned int* _%stid, YYSTYPE* _%stvl, YYSTYPE* _%slvalp", prelo, prelo, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
	}
	fprintf(out, ") {\n%sSTYPE _%sbuffer[%zu];\nswitch(_%stab[_%sid * %zu + *_%stid]) {\n", prehi, prelo, max_len, prelo, prelo, gen->token_cnt + 2, prelo);
	for (size_t i = 0; i < gen->rule_cnt; ++i) {
		++curr_line;
		gen_slist rhs = gen->rules[i].rhs;
		fprintf(out, "case %zu:\n", i);
		for (size_t j = 0; j < rhs.cnt; ++j) {
			if (rhs.syms[j].term) {
				curr_line += 6;
				fprintf(out, "if (*_%stid == %u) {\n_%sbuffer[%zu] = *_%stvl;\n*_%stid = _%stran(%slex(_%stvl", prelo, rhs.syms[j].ind, prelo, j, prelo, prelo, prelo, prelo, prelo);
				for (size_t i = 0; i < gen->lparam_cnt; ++i) {
					fprintf(out, ", %s", gen->lparams[i].name);
				}
				fputs("));\n} else {\nreturn 1;\n}\n", out);
			} else {
				++curr_line;
				fprintf(out, "if (_%sreq(%u, _%stid, _%stvl, _%sbuffer + %zu", prelo, rhs.syms[j].ind, prelo, prelo, prelo, j);
				for (size_t i = 0; i < gen->pparam_cnt; ++i) {
					fprintf(out, ", %s", gen->pparams[i].name);
				}
				fputs(")) return 1;\n", out);
			}
		}
		gen_act* curr = gen->rules[i].act;
		curr_line += 6 + gen->rules[i].act_loc.last_line - gen->rules[i].act_loc.first_line;
		fprintf(out, "{\n#line %d \"%s\"\n", gen->rules[i].act_loc.first_line, gen->fname);
		while (curr != NULL) {
			switch (curr->type) {
				case GEN_CODE_SELF:
					fprintf(out, "(_%slvalp->%s)", prelo, gen->nterms[gen->rules[i].lhs.ind].name);
					break;
				case GEN_CODE_COMP: {
					char const* name = rhs.syms[curr->index].term ? gen->tokens[rhs.syms[curr->index].ind].name : gen->nterms[rhs.syms[curr->index].ind].name;
					fprintf(out, "(_%sbuffer[%u].%s)", prelo, curr->index, name);
					break;
				}
				case GEN_CODE_RAW:
					fputs(curr->data, out);
					break;
			}
			curr = curr->next;
		}
		fprintf(out, "\n#line %d \"%s\"\n}\nbreak;\n", curr_line - 2, outpath);
	}
	curr_line += 10;
	fprintf(out, "default: return 1;\n}\nreturn 0;\n}\nint %sparse(", prelo);
	if (gen->pparam_cnt != 0) {
		fprintf(out, "%s %s", gen->pparams[0].type, gen->pparams[0].name);
		for (size_t i = 1; i < gen->pparam_cnt; ++i) {
			fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
		}
	} else {
		fputs("void", out);
	}
	fprintf(out, ") {\n%sSTYPE _%slval;\n%sSTYPE _%stvl;\nunsigned int _%stid = _%stran(%slex(&_%stvl", prehi, prelo, prehi, prelo, prelo, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->lparam_cnt; ++i) {
		fprintf(out, ", %s", gen->lparams[i].name);
	}
	fprintf(out, "));\nreturn _%sreq(%u, &_%stid, &_%stvl, &_%slval", prelo, gen->start.ind, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s", gen->pparams[i].name);
	}
	fputs(");\n}\n", out);
	if (gen->epilogue != NULL) {
		curr_line += 3 + gen->epilogue_loc.last_line - gen->epilogue_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n%s\n#line %d \"%s\"\n", gen->epilogue_loc.first_line, gen->fname, gen->epilogue, curr_line, outpath);
	}
	fclose(out);
	return 0;
}
