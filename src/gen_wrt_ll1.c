#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "generator.h"

static char const defprelo[] = "yy";
static char const defprehi[] = "YY";

int gen_wrt_ll1_h(gen_type const* restrict gen, char const* restrict outpath) {
	FILE* out = fopen(outpath, "wb");
	if (out == NULL) return 1;
	char const* prehi = gen->prefixhi == NULL ? defprehi : gen->prefixhi;
	int curr_line = 1;
	curr_line += 6 + gen->token_cnt;
	fprintf(out, "#ifndef %sPLL_H\n"
		"#define %sPLL_H 1\n"
		"enum {\n", prehi, prehi);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "\t%s = %u,\n", gen->tokens[i].name, gen->tokens[i].id);
	}
	fprintf(out, "\t%sEOF = 0,\n"
		"\t%sUNDEF = -1\n"
		"};\n", prehi, prehi);
	if (gen->req != NULL) {
		curr_line += 3 + gen->req_loc.last_line - gen->req_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n"
			"%s\n"
			"#line %d \"%s\"\n", gen->req_loc.first_line, gen->fname, gen->req, curr_line, outpath);
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
		fprintf(out, "#line %d \"%s\"\n"
			"%s\n"
			"#line %d \"%s\"\n", gen->prov_loc.first_line, gen->fname, gen->prov, curr_line, outpath);
	}
	++curr_line;
	fputs("#endif\n", out);
	fclose(out);
	return 0;
}

int gen_wrt_ll1(gen_type const* restrict gen, unsigned int const* restrict table, char const* restrict outpath) {
	FILE* out = fopen(outpath, "wb");
	if (out == NULL) return 1;
	int curr_line = 1;
	char const* prelo = gen->prefixlo == NULL ? defprelo : gen->prefixlo;
	char const* prehi = gen->prefixhi == NULL ? defprehi : gen->prefixhi;
	if (gen->top != NULL) {
		curr_line += 3 + gen->top_loc.last_line - gen->top_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n"
			"%s\n"
			"#line %d \"%s\"\n", gen->top_loc.first_line, gen->fname, gen->top, curr_line, outpath);
	}
	curr_line += 6 + gen->token_cnt;
	fprintf(out, "#ifndef %sPLL_H\n"
		"#define %sPLL_H 1\n"
		"enum {\n", prehi, prehi);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "\t%s = %u,\n", gen->tokens[i].name, gen->tokens[i].id);
	}
	fprintf(out, "\t%sEOF = 0,\n"
		"\t%sUNDEF = -1\n"
		"};\n", prehi, prehi);
	if (gen->req != NULL) {
		curr_line += 3 + gen->req_loc.last_line - gen->req_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n"
			"%s\n"
			"#line %d \"%s\"\n", gen->req_loc.first_line, gen->fname, gen->req, curr_line, outpath);
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
		fprintf(out, "#line %d \"%s\"\n"
			"%s\n"
			"#line %d \"%s\"\n", gen->prov_loc.first_line, gen->fname, gen->prov, curr_line, outpath);
	}
	++curr_line;
	fputs("#endif\n", out);
	if (gen->code != NULL) {
		curr_line += 3 + gen->code_loc.last_line - gen->code_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n"
			"%s\n"
			"#line %d \"%s\"\n", gen->code_loc.first_line, gen->fname, gen->code, curr_line, outpath);
	}
	curr_line += 79 + gen->token_cnt + gen->nterm_cnt;
	fprintf(out, "static unsigned int _%stran(int code) {\n"
		"\tswitch (code) {\n", prelo);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "\t\tcase %s: return %zu;\n", gen->tokens[i].name, i);
	}
	fprintf(out, "\t\tcase %sEOF: return %zu;\n"
		"\t\tdefault: return %zu;\n"
		"\t}\n"
		"}\n"
		"static unsigned int const _%stab[] = {\n", prehi, gen->token_cnt, gen->token_cnt + 1, prelo);
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		fputc('\t', out);
		for (size_t j = 0; j < gen->token_cnt + 1; ++j) {
			unsigned int v = table[i * (gen->token_cnt + 1) + j];
			fprintf(out, "%zu, ", v == UINT_MAX ? gen->rule_cnt : v);
		}
		fprintf(out, "%zu,\n", gen->rule_cnt);
	}
	fprintf(out, "};\n"
		"static char const* const _%sname[] = {", prelo);
	size_t max_name = 0;
	size_t tot_name = 0;
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "\"%s\", ", gen->tokens[i].name);
		size_t len = strlen(gen->tokens[i].name);
		max_name = max_name > len ? max_name : len;
		tot_name += len;
	}
	fprintf(out, "\"%sEOF\", \"%sUNDEF\"};\n"
		"static int _%stokerrmsg(unsigned int _%seid, unsigned int _%sfid", prehi, prehi, prelo, prelo, prelo);
	for (size_t i = 0; i <gen->pparam_cnt; ++i) {
		fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
	}
	fprintf(out, ") {\n"
		"\tchar _%smsgbuffer[%zu];\n"
		"\tchar* _%scurr = _%smsgbuffer;\n"
		"\tchar const* _%ssrc = \"expected \";\n"
		"\twhile (*_%ssrc != \'\\0\') {\n"
		"\t\t*_%scurr = *_%ssrc;\n"
		"\t\t++_%scurr;\n"
		"\t\t++_%ssrc;\n"
		"\t}\n"
		"\t_%ssrc = _%sname[_%seid];\n"
		"\twhile (*_%ssrc != \'\\0\') {\n"
		"\t\t*_%scurr = *_%ssrc;\n"
		"\t\t++_%scurr;\n"
		"\t\t++_%ssrc;\n"
		"\t}\n"
		"\t_%ssrc = \"; found \";\n"
		"\twhile (*_%ssrc != \'\\0\') {\n"
		"\t\t*_%scurr = *_%ssrc;\n"
		"\t\t++_%scurr;\n"
		"\t\t++_%ssrc;\n"
		"\t}\n"
		"\t_%ssrc = _%sname[_%sfid];\n"
		"\twhile (*_%ssrc != \'\\0\') {\n"
		"\t\t*_%scurr = *_%ssrc;\n"
		"\t\t++_%scurr;\n"
		"\t\t++_%ssrc;\n"
		"\t}\n"
		"\t*_%scurr = \'\\0\';\n"
		"\t%serror(_%smsgbuffer", prelo, max_name * 2 + 18, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s", gen->pparams[i].name);
	}
	fprintf(out, ");\n"
		"\treturn 1;\n"
		"}\n"
		"static int _%sntmerrmsg(unsigned int _%ssid, unsigned _%sfid", prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
	}
	fprintf(out, ") {\n"
		"\tchar _%smsgbuffer[%zu];\n"
		"\tchar* _%scurr = _%smsgbuffer;\n"
		"\tchar const* _%ssrc = \"expected one of\";\n"
		"\twhile (*_%ssrc != \'\\0\') {\n"
		"\t\t*_%scurr = *_%ssrc;\n"
		"\t\t++_%scurr;\n"
		"\t\t++_%ssrc;\n"
		"\t}\n"
		"\tfor (unsigned int i = 0; i < %zu; ++i) {\n"
		"\t\tif (_%stab[_%ssid * %zu + i] != %zu) {\n"
		"\t\t\t*_%scurr = \' \';\n"
		"\t\t\t++_%scurr;\n"
		"\t\t\t_%ssrc = _%sname[i];\n"
		"\t\t\twhile (*_%ssrc != \'\\0\') {\n"
		"\t\t\t\t*_%scurr = *_%ssrc;\n"
		"\t\t\t\t++_%scurr;\n"
		"\t\t\t\t++_%ssrc;\n"
		"\t\t\t}\n"
		"\t\t}\n"
		"\t}\n"
		"\t_%ssrc = \"; found \";\n"
		"\twhile (*_%ssrc != \'\\0\') {\n"
		"\t\t*_%scurr = *_%ssrc;\n"
		"\t\t++_%scurr;\n"
		"\t\t++_%ssrc;\n"
		"\t}\n"
		"\t_%ssrc = _%sname[_%sfid];\n"
		"\twhile (*_%ssrc != \'\\0\') {\n"
		"\t\t*_%scurr = *_%ssrc;\n"
		"\t\t++_%scurr;\n"
		"\t\t++_%ssrc;\n"
		"\t}\n"
		"\t*_%scurr = \'\\0\';\n"
		"\t%serror(_%smsgbuffer", prelo, tot_name + gen->token_cnt + max_name + 25, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, gen->token_cnt + 1, prelo, prelo, gen->token_cnt + 2, gen->rule_cnt, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s", gen->pparams[i].name);
	}
	fprintf(out, ");\n"
		"\treturn 1;\n"
		"}\n"
		"static int _%sreq(unsigned int _%sid, unsigned int* _%stid, YYSTYPE* _%stvl, void* _%slvalp", prelo, prelo, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
	}
	fprintf(out, ") {\n"
		"\tswitch(_%stab[_%sid * %zu + *_%stid]) {\n", prelo, prelo, gen->token_cnt + 2, prelo);
	for (size_t i = 0; i < gen->rule_cnt; ++i) {
		++curr_line;
		gen_slist rhs = gen->rules[i].rhs;
		fprintf(out, "\t\tcase %zu: {\n", i);
		for (size_t j = 0; j < rhs.cnt; ++j) {
			if (rhs.syms[j].term) {
				char const* tn = gen->tokens[rhs.syms[j].ind].type;
				if (tn != NULL) {
					++curr_line;
					fprintf(out, "\t\t\t%s _%sv%zu;\n", tn, prelo, j + 1);
				}
				++curr_line;
				fprintf(out, "\t\t\tif (*_%stid == %u) {\n", prelo, rhs.syms[j].ind);
				if (tn != NULL) {
					++curr_line;
					fprintf(out, "\t\t\t\t_%sv%zu = _%stvl->%s;\n", prelo, j + 1, prelo, gen->tokens[rhs.syms[j].ind].name);
				}
				curr_line += 4;
				fprintf(out, "\t\t\t\t*_%stid = _%stran(%slex(_%stvl", prelo, prelo, prelo, prelo);
				for (size_t i = 0; i < gen->lparam_cnt; ++i) {
					fprintf(out, ", %s", gen->lparams[i].name);
				}
				fprintf(out, "));\n"
					"\t\t\t} else {\n"
					"\t\t\t\treturn _%stokerrmsg(%u, *_%stid", prelo, rhs.syms[j].ind, prelo);
				for (size_t i = 0; i < gen->pparam_cnt; ++i) {
					fprintf(out, ", %s", gen->pparams[i].name);
				}
				fputs(");\n"
					"\t\t\t}\n", out);
			} else {
				char const* tn = gen->nterms[rhs.syms[j].ind].type;
				if (tn != NULL) {
					++curr_line;
					fprintf(out, "\t\t\t%s _%sv%zu;\n", tn, prelo, j + 1);
				}
				++curr_line;
				fprintf(out, "\t\t\tif (_%sreq(%u, _%stid, _%stvl, ", prelo, rhs.syms[j].ind, prelo, prelo);
				if (tn != NULL) {
					fprintf(out, "&_%sv%zu", prelo, j + 1);
				} else {
					fputs("(void*)0", out);
				}
				for (size_t i = 0; i < gen->pparam_cnt; ++i) {
					fprintf(out, ", %s", gen->pparams[i].name);
				}
				fputs(")) return 1;\n", out);
			}
		}
		gen_act* curr = gen->rules[i].act;
		curr_line += 5 + gen->rules[i].act_loc.last_line - gen->rules[i].act_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n", gen->rules[i].act_loc.first_line, gen->fname);
		while (curr != NULL) {
			switch (curr->type) {
				case GEN_CODE_SELF:
					fprintf(out, "(*(%s*)_%slvalp)", gen->nterms[gen->rules[i].lhs.ind].type, prelo);
					break;
				case GEN_CODE_COMP: {
					fprintf(out, "(_%sv%u)", prelo, curr->index + 1);
					break;
				}
				case GEN_CODE_RAW:
					fputs(curr->data, out);
					break;
			}
			curr = curr->next;
		}
		fprintf(out, "\n"
			"#line %d \"%s\"\n"
			"\t\t\tbreak;\n"
			"\t\t}\n", curr_line - 2, outpath);
	}
	curr_line += 10;
	fprintf(out, "\t\tdefault: return _%sntmerrmsg(_%sid, *_%stid", prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s", gen->pparams[i].name);
	}
	fprintf(out, ");\n"
		"\t}\n"
		"\treturn 0;\n"
		"}\n"
		"int %sparse(", prelo);
	if (gen->pparam_cnt != 0) {
		fprintf(out, "%s %s", gen->pparams[0].type, gen->pparams[0].name);
		for (size_t i = 1; i < gen->pparam_cnt; ++i) {
			fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
		}
	} else {
		fputs("void", out);
	}
	fprintf(out, ") {\n"
		"\t%sSTYPE _%slval;\n"
		"\t%sSTYPE _%stvl;\n"
		"\tunsigned int _%stid = _%stran(%slex(&_%stvl", prehi, prelo, prehi, prelo, prelo, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->lparam_cnt; ++i) {
		fprintf(out, ", %s", gen->lparams[i].name);
	}
	fprintf(out, "));\n"
		"\treturn _%sreq(%u, &_%stid, &_%stvl, &_%slval", prelo, gen->start.ind, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s", gen->pparams[i].name);
	}
	fputs(");\n"
		"}\n", out);
	if (gen->epilogue != NULL) {
		curr_line += 3 + gen->epilogue_loc.last_line - gen->epilogue_loc.first_line;
		fprintf(out, "#line %d \"%s\"\n"
			"%s\n"
			"#line %d \"%s\"\n", gen->epilogue_loc.first_line, gen->fname, gen->epilogue, curr_line, outpath);
	}
	fclose(out);
	return 0;
}
