#include <stdio.h>
#include <string.h>

#include "generator.h"

static char const defprelo[] = "yy";
static char const defprehi[] = "YY";

int gen_wrt_ll1_h(gen_type const* restrict gen, char const* restrict outpath) {
	FILE* out = fopen(outpath, "wb");
	if (out == NULL) return 1;
	char const* prehi = gen->prefixhi == NULL ? defprehi : gen->prefixhi;
	int curr_line = 1;
	fprintf(out, "#ifndef %sPLL_H\n", prehi); ++curr_line;
	fprintf(out, "#define %sPLL_H 1\n", prehi); ++curr_line;
	fprintf(out, "enum {\n"); ++curr_line;
	for (gen_sind i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "\t%s = %u,\n", gen->tokens[i].name, gen->tokens[i].id); ++curr_line;
	}
	fprintf(out, "\t%sEOF = 0,\n", prehi); ++curr_line;
	fprintf(out, "\t%sUNDEF = -1\n", prehi); ++curr_line;
	fprintf(out, "};\n"); ++curr_line;
	if (gen->req.acts != NULL) {
		fprintf(out, "#line %d \"%s\"\n", gen->req.loc.first_line, gen->fname); ++curr_line;
		struct gen_code_unit const* end = gen->req.acts + gen->req.len;
		for (struct gen_code_unit const* curr = gen->req.acts; curr != end; ++curr) {
			switch (curr->type) {
				case GEN_CODE_RAW:
					fprintf(out, "%s", curr->data);
					break;
				default:;
			}
		}
		curr_line += gen->req.loc.last_line - gen->req.loc.first_line;
		fprintf(out, "\n"); ++curr_line;
		fprintf(out, "#line %d \"%s\"\n", curr_line + 1, outpath); ++curr_line;
	}
	fprintf(out, "typedef union %sSTYPE {\n", prehi); ++curr_line;
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		if (gen->tokens[i].type != NULL) {
			fprintf(out, "\t%s %s;\n", gen->tokens[i].type, gen->tokens[i].name); ++curr_line;
		}
	}
	fprintf(out, "} %sSTYPE;\n", prehi); ++curr_line;
	if (gen->prov.acts != NULL) {
		fprintf(out, "#line %d \"%s\"\n", gen->prov.loc.first_line, gen->fname); ++curr_line;
		struct gen_code_unit const* end = gen->prov.acts + gen->prov.len;
		for (struct gen_code_unit const* curr = gen->prov.acts; curr != end; ++curr) {
			switch (curr->type) {
				case GEN_CODE_RAW:
					fprintf(out, "%s", curr->data);
					break;
				default:;
			}
		}
		curr_line += gen->prov.loc.last_line - gen->prov.loc.first_line;
		fprintf(out, "\n"); ++curr_line;
		fprintf(out, "#line %d \"%s\"\n", curr_line + 1, outpath); ++curr_line;
	}
	fprintf(out, "#endif\n"); ++curr_line;
	fclose(out);
	return 0;
}

int gen_wrt_ll1(gen_type const* restrict gen, gen_rind const* restrict table, char const* restrict outpath) {
	FILE* out = fopen(outpath, "wb");
	if (out == NULL) return 1;
	int curr_line = 1;
	char const* prelo = gen->prefixlo == NULL ? defprelo : gen->prefixlo;
	char const* prehi = gen->prefixhi == NULL ? defprehi : gen->prefixhi;
	if (gen->top.acts != NULL) {
		fprintf(out, "#line %d \"%s\"\n", gen->top.loc.first_line, gen->fname); ++curr_line;
		struct gen_code_unit const* end = gen->top.acts + gen->top.len;
		for (struct gen_code_unit const* curr = gen->top.acts; curr != end; ++curr) {
			switch (curr->type) {
				case GEN_CODE_RAW:
					fprintf(out, "%s", curr->data);
					break;
				default:;
			}
		}
		curr_line += gen->top.loc.last_line - gen->top.loc.first_line;
		fprintf(out, "\n"); ++curr_line;
		fprintf(out, "#line %d \"%s\"\n", curr_line + 1, outpath); ++curr_line;
	}
	fprintf(out, "#ifndef %sPLL_H\n", prehi); ++curr_line;
	fprintf(out, "#define %sPLL_H 1\n", prehi); ++curr_line;
	fprintf(out, "enum {\n"); ++curr_line;
	for (gen_sind i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "\t%s = %u,\n", gen->tokens[i].name, gen->tokens[i].id); ++curr_line;
	}
	fprintf(out, "\t%sEOF = 0,\n", prehi); ++curr_line;
	fprintf(out, "\t%sUNDEF = -1\n", prehi); ++curr_line;
	fprintf(out, "};\n"); ++curr_line;
	if (gen->req.acts != NULL) {
		fprintf(out, "#line %d \"%s\"\n", gen->req.loc.first_line, gen->fname); ++curr_line;
		struct gen_code_unit const* end = gen->req.acts + gen->req.len;
		for (struct gen_code_unit const* curr = gen->req.acts; curr != end; ++curr) {
			switch (curr->type) {
				case GEN_CODE_RAW:
					fprintf(out, "%s", curr->data);
					break;
				default:;
			}
		}
		curr_line += gen->req.loc.last_line - gen->req.loc.first_line;
		fprintf(out, "\n"); ++curr_line;
		fprintf(out, "#line %d \"%s\"\n", curr_line + 1, outpath); ++curr_line;
	}
	fprintf(out, "typedef union %sSTYPE {\n", prehi); ++curr_line;
	for (gen_sind i = 0; i < gen->token_cnt; ++i) {
		if (gen->tokens[i].type != NULL) {
			fprintf(out, "\t%s %s;\n", gen->tokens[i].type, gen->tokens[i].name); ++curr_line;
		}
	}
	fprintf(out, "} %sSTYPE;\n", prehi); ++curr_line;
	if (gen->prov.acts != NULL) {
		fprintf(out, "#line %d \"%s\"\n", gen->prov.loc.first_line, gen->fname); ++curr_line;
		struct gen_code_unit const* end = gen->prov.acts + gen->prov.len;
		for (struct gen_code_unit const* curr = gen->prov.acts; curr != end; ++curr) {
			switch (curr->type) {
				case GEN_CODE_RAW:
					fprintf(out, "%s", curr->data);
					break;
				default:;
			}
		}
		curr_line += gen->prov.loc.last_line - gen->prov.loc.first_line;
		fprintf(out, "\n"); ++curr_line;
		fprintf(out, "#line %d \"%s\"\n", curr_line + 1, outpath); ++curr_line;
	}
	fprintf(out, "#endif\n"); ++curr_line;
	if (gen->code.acts != NULL) {
		fprintf(out, "#line %d \"%s\"\n", gen->code.loc.first_line, gen->fname); ++curr_line;
		struct gen_code_unit const* end = gen->code.acts + gen->code.len;
		for (struct gen_code_unit const* curr = gen->code.acts; curr != end; ++curr) {
			switch (curr->type) {
				case GEN_CODE_RAW:
					fprintf(out, "%s", curr->data);
					break;
				default:;
			}
		}
		curr_line += gen->code.loc.last_line - gen->code.loc.first_line;
		fprintf(out, "\n"); ++curr_line;
		fprintf(out, "#line %d \"%s\"\n", curr_line + 1, outpath); ++curr_line;
	}
	fprintf(out, "static unsigned int _%stran(int code) {\n", prelo); ++curr_line;
	fprintf(out, "\tswitch (code) {\n"); ++curr_line;
	for (gen_sind i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "\t\tcase %s: return %"GEN_SIND_PRI";\n", gen->tokens[i].name, i); ++curr_line;
	}
	fprintf(out, "\t\tcase %sEOF: return %"GEN_SIND_PRI";\n", prehi, gen->token_cnt); ++curr_line;
	fprintf(out, "\t\tdefault: return %"GEN_SIND_PRI";\n", gen->token_cnt + 1); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "}\n"); ++curr_line;
	fprintf(out, "static unsigned int const _%stab[] = {\n", prelo); ++curr_line;
	for (gen_sind i = 0; i < gen->nterm_cnt; ++i) {
		fprintf(out, "\t");
		for (gen_sind j = 0; j < gen->token_cnt + 1; ++j) {
			gen_rind v = table[i * (gen->token_cnt + 1) + j];
			fprintf(out, "%"GEN_RIND_PRI", ", v == GEN_RIND_MAX ? gen->rule_cnt : v);
		}
		fprintf(out, "%"GEN_RIND_PRI",\n", gen->rule_cnt); ++curr_line;
	}
	fprintf(out, "};\n"); ++curr_line;
	fprintf(out, "static char const* const _%sname[] = {", prelo);
	size_t max_name = strlen(prehi) + 5;
	size_t tot_name = 2 * strlen(prehi) + 8;
	for (gen_sind i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "\"%s\", ", gen->tokens[i].name);
		size_t len = strlen(gen->tokens[i].name);
		max_name = max_name > len ? max_name : len;
		tot_name += len;
	}
	fprintf(out, "\"%sEOF\", \"%sUNDEF\"};\n", prehi, prehi); ++curr_line;
	fprintf(out, "static void _%stokerrmsg(unsigned int _%seid, unsigned int _%sfid", prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
	}
	fprintf(out, ") {\n"); ++curr_line;
	fprintf(out, "\tchar _%smsgbuffer[%zu];\n", prelo, max_name * 2 + 18); ++curr_line;
	fprintf(out, "\tchar* _%scurr = _%smsgbuffer;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\tchar const* _%ssrc = \"expected \";\n", prelo); ++curr_line;
	fprintf(out, "\twhile (*_%ssrc != \'\\0\') {\n", prelo); ++curr_line;
	fprintf(out, "\t\t*_%scurr = *_%ssrc;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\t\t++_%scurr;\n", prelo); ++curr_line;
	fprintf(out, "\t\t++_%ssrc;\n", prelo); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "\t_%ssrc = _%sname[_%seid];\n", prelo, prelo, prelo); ++curr_line;
	fprintf(out, "\twhile (*_%ssrc != \'\\0\') {\n", prelo); ++curr_line;
	fprintf(out, "\t\t*_%scurr = *_%ssrc;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\t\t++_%scurr;\n", prelo); ++curr_line;
	fprintf(out, "\t\t++_%ssrc;\n", prelo); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "\t_%ssrc = \"; found \";\n", prelo); ++curr_line;
	fprintf(out, "\twhile (*_%ssrc != \'\\0\') {\n", prelo); ++curr_line;
	fprintf(out, "\t\t*_%scurr = *_%ssrc;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\t\t++_%scurr;\n", prelo); ++curr_line;
	fprintf(out, "\t\t++_%ssrc;\n", prelo); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "\t_%ssrc = _%sname[_%sfid];\n", prelo, prelo, prelo); ++curr_line;
	fprintf(out, "\twhile (*_%ssrc != \'\\0\') {\n", prelo); ++curr_line;
	fprintf(out, "\t\t*_%scurr = *_%ssrc;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\t\t++_%scurr;\n", prelo); ++curr_line;
	fprintf(out, "\t\t++_%ssrc;\n", prelo); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "\t*_%scurr = \'\\0\';\n", prelo); ++curr_line;
	fprintf(out, "\t%serror(_%smsgbuffer", prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s", gen->pparams[i].name);
	}
	fprintf(out, ");\n"); ++curr_line;
	fprintf(out, "}\n"); ++curr_line;
	fprintf(out, "static void _%sntmerrmsg(unsigned int _%ssid, unsigned _%sfid", prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
	}
	fprintf(out, ") {\n"); ++curr_line;
	fprintf(out, "\tchar _%smsgbuffer[%zu];\n", prelo, tot_name + max_name + gen->token_cnt + 26); ++curr_line;
	fprintf(out, "\tchar* _%scurr = _%smsgbuffer;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\tchar const* _%ssrc = \"expected one of\";\n", prelo); ++curr_line;
	fprintf(out, "\twhile (*_%ssrc != \'\\0\') {\n", prelo); ++curr_line;
	fprintf(out, "\t\t*_%scurr = *_%ssrc;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\t\t++_%scurr;\n", prelo); ++curr_line;
	fprintf(out, "\t\t++_%ssrc;\n", prelo); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "\tfor (unsigned int i = 0; i < %"GEN_SIND_PRI"; ++i) {\n", gen->token_cnt + 2); ++curr_line;
	fprintf(out, "\t\tif (_%stab[_%ssid * %"GEN_SIND_PRI" + i] != %"GEN_RIND_PRI") {\n", prelo, prelo, gen->token_cnt + 2, gen->rule_cnt); ++curr_line;
	fprintf(out, "\t\t\t*_%scurr = \' \';\n", prelo); ++curr_line;
	fprintf(out, "\t\t\t++_%scurr;\n", prelo); ++curr_line;
	fprintf(out, "\t\t\t_%ssrc = _%sname[i];\n", prelo, prelo); ++curr_line;
	fprintf(out, "\t\t\twhile (*_%ssrc != \'\\0\') {\n", prelo); ++curr_line;
	fprintf(out, "\t\t\t\t*_%scurr = *_%ssrc;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\t\t\t\t++_%scurr;\n", prelo); ++curr_line;
	fprintf(out, "\t\t\t\t++_%ssrc;\n", prelo); ++curr_line;
	fprintf(out, "\t\t\t}\n"); ++curr_line;
	fprintf(out, "\t\t}\n"); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "\t_%ssrc = \"; found \";\n", prelo); ++curr_line;
	fprintf(out, "\twhile (*_%ssrc != \'\\0\') {\n", prelo); ++curr_line;
	fprintf(out, "\t\t*_%scurr = *_%ssrc;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\t\t++_%scurr;\n", prelo); ++curr_line;
	fprintf(out, "\t\t++_%ssrc;\n", prelo); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "\t_%ssrc = _%sname[_%sfid];\n", prelo, prelo, prelo); ++curr_line;
	fprintf(out, "\twhile (*_%ssrc != \'\\0\') {\n", prelo); ++curr_line;
	fprintf(out, "\t\t*_%scurr = *_%ssrc;\n", prelo, prelo); ++curr_line;
	fprintf(out, "\t\t++_%scurr;\n", prelo); ++curr_line;
	fprintf(out, "\t\t++_%ssrc;\n", prelo); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "\t*_%scurr = \'\\0\';\n", prelo); ++curr_line;
	fprintf(out, "\t%serror(_%smsgbuffer", prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s", gen->pparams[i].name);
	}
	fprintf(out, ");\n"); ++curr_line;
	fprintf(out, "}\n"); ++curr_line;
	fprintf(out, "static int _%sreq(unsigned int _%sid, unsigned int* _%stid, YYSTYPE* _%stvl, void* _%slvalp", prelo, prelo, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
	}
	fprintf(out, ") {\n"); ++curr_line;
	fprintf(out, "\tif (*_%stid == %"GEN_SIND_PRI") *_%stid = _%stran(%slex(_%stvl", prelo, gen->token_cnt + 2, prelo, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->lparam_cnt; ++i) {
		fprintf(out, ", %s", gen->lparams[i].name);
	}
	fprintf(out, "));\n"); ++curr_line;
	fprintf(out, "\tswitch(_%stab[_%sid * %"GEN_SIND_PRI" + *_%stid]) {\n", prelo, prelo, gen->token_cnt + 2, prelo); ++curr_line;
	for (gen_rind i = 0; i < gen->rule_cnt; ++i) {
		gen_slist rhs = gen->rules[i].rhs;
		fprintf(out, "\t\tcase %"GEN_RIND_PRI": {\n", i); ++curr_line;
		for (gen_roff j = 0; j < rhs.cnt; ++j) {
			if (rhs.syms[j].term) {
				char const* tn = gen->tokens[rhs.syms[j].ind].type;
				fprintf(out, "\t\t\tif (*_%stid == %"GEN_SIND_PRI") *_%stid = _%stran(%slex(_%stvl", prelo, gen->token_cnt + 2, prelo, prelo, prelo, prelo);
				for (size_t i = 0; i < gen->lparam_cnt; ++i) {
					fprintf(out, ", %s", gen->lparams[i].name);
				}
				fprintf(out, "));\n"); ++curr_line;
				if (tn != NULL) {
					fprintf(out, "\t\t\t%s _%sv%"GEN_ROFF_PRI";\n", tn, prelo, (gen_roff)(j + 1)); ++curr_line;
				}
				fprintf(out, "\t\t\tif (*_%stid == %"GEN_SIND_PRI") {\n", prelo, rhs.syms[j].ind); ++curr_line;
				if (tn != NULL) {
					fprintf(out, "\t\t\t\t_%sv%"GEN_ROFF_PRI" = _%stvl->%s;\n", prelo, (gen_roff)(j + 1), prelo, gen->tokens[rhs.syms[j].ind].name); ++curr_line;
				}
				fprintf(out, "\t\t\t\t*_%stid = %"GEN_SIND_PRI";\n", prelo, gen->token_cnt + 2); ++curr_line;
				fprintf(out, "\t\t\t} else {\n"); ++curr_line;
				fprintf(out, "\t\t\t\t_%stokerrmsg(%"GEN_SIND_PRI", *_%stid", prelo, rhs.syms[j].ind, prelo);
				for (size_t i = 0; i < gen->pparam_cnt; ++i) {
					fprintf(out, ", %s", gen->pparams[i].name);
				}
				fprintf(out, ");\n"); ++curr_line;
				fprintf(out, "\t\t\t\tgoto _%sR%"GEN_RIND_PRI"_%"GEN_ROFF_PRI";\n", prehi, i, j); ++curr_line;
				fprintf(out, "\t\t\t}\n"); ++curr_line;
			} else {
				char const* tn = gen->nterms[rhs.syms[j].ind].type;
				if (tn != NULL) {
					fprintf(out, "\t\t\t%s _%sv%"GEN_ROFF_PRI";\n", tn, prelo, (gen_roff)(j + 1)); ++curr_line;
				}
				fprintf(out, "\t\t\tif (_%sreq(%"GEN_SIND_PRI", _%stid, _%stvl, ", prelo, rhs.syms[j].ind, prelo, prelo);
				if (tn != NULL) {
					fprintf(out, "&_%sv%"GEN_ROFF_PRI"", prelo, (gen_roff)(j + 1));
				} else {
					fprintf(out, "(void*)0");
				}
				for (size_t i = 0; i < gen->pparam_cnt; ++i) {
					fprintf(out, ", %s", gen->pparams[i].name);
				}
				fprintf(out, ")) goto _%sR%"GEN_RIND_PRI"_%"GEN_ROFF_PRI";\n", prehi, i, j); ++curr_line;
			}
		}
		fprintf(out, "#line %d \"%s\"\n", gen->rules[i].act.loc.first_line, gen->fname); ++curr_line;
		struct gen_code_unit const* end = gen->rules[i].act.acts + gen->rules[i].act.len;
		for (struct gen_code_unit const* curr = gen->rules[i].act.acts; curr != end; ++curr) {
			switch (curr->type) {
				case GEN_CODE_MERR:
					fprintf(out, "return 1");
					break;
				case GEN_CODE_COMP: {
					if (curr->index == 0) {
						fprintf(out, "(*(%s*)_%slvalp)", gen->nterms[gen->rules[i].lhs].type, prelo);
					} else {
						fprintf(out, "(_%sv%"GEN_ROFF_PRI")", prelo, curr->index);
					}
					break;
				}
				case GEN_CODE_RAW:
					fprintf(out, "%s", curr->data);
					break;
			}
		}
		curr_line += gen->rules[i].act.loc.last_line - gen->rules[i].act.loc.first_line;
		fprintf(out, "\n"); ++curr_line;
		fprintf(out, "#line %d \"%s\"\n", curr_line + 1, outpath); ++curr_line;
		fprintf(out, "\t\t\tbreak;\n"); ++curr_line;
		if (rhs.cnt != 0) {
			for (gen_roff j = rhs.cnt - 2; j != (gen_roff)(-1); --j) {
				fprintf(out, "\t\t\t_%sR%"GEN_RIND_PRI"_%"GEN_ROFF_PRI":;\n", prehi, i, (gen_roff)(j + 1)); ++curr_line;
				gen_act const* des = rhs.syms[j].term ? &(gen->tokens[rhs.syms[j].ind].des) : &(gen->nterms[rhs.syms[j].ind].des);
				if (des->acts != NULL) {
					fprintf(out, "#line %d \"%s\"\n", des->loc.first_line, gen->fname); ++curr_line;
					struct gen_code_unit const* end = des->acts + des->len;
					for (struct gen_code_unit const* curr = des->acts; curr != end; ++curr) {
						switch (curr->type) {
							case GEN_CODE_COMP:
								fprintf(out, "(_%sv%"GEN_ROFF_PRI")", prelo, (gen_roff)(j + 1));
								break;
							case GEN_CODE_RAW:
								fprintf(out, "%s", curr->data);
								break;
							default:;
						}
					}
					curr_line += des->loc.last_line - des->loc.first_line;
					fprintf(out, "\n"); ++curr_line;
					fprintf(out, "#line %d \"%s\"\n", curr_line + 1, outpath); ++curr_line;
				}
			}
			fprintf(out, "\t\t\t_%sR%"GEN_RIND_PRI"_0: return 1;\n", prehi, i); ++curr_line;
		}
		fprintf(out, "\t\t}\n"); ++curr_line;
	}
	fprintf(out, "\t\tdefault:\n"); ++curr_line;
	fprintf(out, "\t\t\t_%sntmerrmsg(_%sid, *_%stid", prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s", gen->pparams[i].name);
	}
	fprintf(out, ");\n"); ++curr_line;
	fprintf(out, "\t\t\treturn 1;\n"); ++curr_line;
	fprintf(out, "\t}\n"); ++curr_line;
	fprintf(out, "\treturn 0;\n"); ++curr_line;
	fprintf(out, "}\n"); ++curr_line;
	fprintf(out, "int %sparse(", prelo);
	if (gen->pparam_cnt != 0) {
		fprintf(out, "%s %s", gen->pparams[0].type, gen->pparams[0].name);
		for (size_t i = 1; i < gen->pparam_cnt; ++i) {
			fprintf(out, ", %s %s", gen->pparams[i].type, gen->pparams[i].name);
		}
	} else {
		fprintf(out, "void");
	}
	fprintf(out, ") {\n"); ++curr_line;
	fprintf(out, "\t%sSTYPE _%slval;\n", prehi, prelo); ++curr_line;
	fprintf(out, "\t%sSTYPE _%stvl;\n", prehi, prelo); ++curr_line;
	fprintf(out, "\tunsigned int _%stid = %"GEN_SIND_PRI";\n", prelo, gen->token_cnt + 2); ++curr_line;
	fprintf(out, "\treturn _%sreq(%"GEN_SIND_PRI", &_%stid, &_%stvl, &_%slval", prelo, gen->start.ind, prelo, prelo, prelo);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		fprintf(out, ", %s", gen->pparams[i].name);
	}
	fprintf(out, ");\n"); ++curr_line;
	fprintf(out, "}\n"); ++curr_line;
	if (gen->epilogue != NULL) {
		fprintf(out, "#line %d \"%s\"\n", gen->epilogue_loc.first_line, gen->fname); ++curr_line;
		fprintf(out, "%s", gen->epilogue);
		curr_line += gen->epilogue_loc.last_line - gen->epilogue_loc.first_line;
		fprintf(out, "\n"); ++curr_line;
		fprintf(out, "#line %d \"%s\"\n", curr_line + 1, outpath); ++curr_line;
	}
	fclose(out);
	return 0;
}
