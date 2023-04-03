#include <stdio.h>
#include <limits.h>

#include "generator.h"

void gen_wrt_ll1(gen_type const* restrict gen, unsigned int const* restrict table, FILE* restrict out) {
	size_t max_len = 0;
	for (size_t i = 0; i < gen->rule_cnt; ++i) {
		if (gen->rules[i].rhs.cnt > max_len) max_len = gen->rules[i].rhs.cnt;
	}
	for (size_t i = 0; i < gen->prologue_cnt; ++i) {
		fwrite(gen->prologue[i].data, 1, gen->prologue[i].len, out);
		fputc('\n', out);
	}
	fprintf(out, "typedef union YYSTYPE {\n");
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		if (gen->tokens[i].type.data != NULL) {
			fputc('\t', out);
			fwrite(gen->tokens[i].type.data, 1, gen->tokens[i].type.len, out);
			fputc(' ', out);
			fwrite(gen->tokens[i].name.data, 1, gen->tokens[i].name.len, out);
			fputs(";\n", out);
		}
	}
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		if (gen->nterms[i].type.data != NULL) {
			fputc('\t', out);
			fwrite(gen->nterms[i].type.data, 1, gen->nterms[i].type.len, out);
			fputc(' ', out);
			fwrite(gen->nterms[i].name.data, 1, gen->nterms[i].name.len, out);
			fputs(";\n", out);
		}
	}
	fputs("} YYSTYPE;\n", out);
	fwrite(gen->epilogue.data, 1, gen->epilogue.len, out);
	fputc('\n', out);
	fputs("static unsigned int _yytran(int code) {\n\tswitch (code) {\n", out);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		fprintf(out, "\t\tcase %u:\n\t\t\treturn %zu;\n", gen->tokens[i].id, i);
	}
	fprintf(out, "\t\tcase -1:\n\t\t\treturn %zu;\n\t\tdefault:\n\t\t\treturn %zu;\n\t}\n}\n\n", gen->token_cnt, gen->token_cnt + 1);
	fputs("static int _yyreq(unsigned int _yyid, unsigned int* _yytid, YYSTYPE* _yytvl, YYSTYPE* _yylvalp", out);
	for (size_t i = 0; i < gen->param_cnt; ++i) {
		fputs(", ", out);
		fwrite(gen->params[i].type.data, 1, gen->params[i].type.len, out);
		fputc(' ', out);
		fwrite(gen->params[i].name.data, 1, gen->params[i].name.len, out);
	}
	fprintf(out, ") {\n\tYYSTYPE _yybuffer[%zu];\n\tswitch (_yyid) {\n", max_len);
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		fprintf(out, "\t\tcase %zu:\n\t\t\tswitch (*_yytid) {\n", i);
		for (size_t j = 0; j < gen->token_cnt + 1; ++j) {
			if (table[i * (gen->token_cnt + 1) + j] != UINT_MAX) {
				fprintf(out, "\t\t\t\tcase %zu: {\n", j);
				gen_slist rhs = gen->rules[table[i * (gen->token_cnt + 1) + j]].rhs;
				for (size_t k = 0; k < rhs.cnt; ++k) {
					if (rhs.syms[k].term) {
						fprintf(out, "\t\t\t\t\tif (*_yytid == %u) {\n\t\t\t\t\t\t_yybuffer[%zu] = *_yytvl;\n\t\t\t\t\t\t*_yytid = _yytran(yylex(_yytvl", rhs.syms[k].ind, k);
						for (size_t i = 0; i < gen->param_cnt; ++i) {
							fputs(", ", out);
							fwrite(gen->params[i].name.data, 1, gen->params[i].name.len, out);
						}
						fputs("));\n\t\t\t\t\t} else {\n\t\t\t\t\t\treturn 1;\n\t\t\t\t\t}\n", out);
					} else {
						fprintf(out, "\t\t\t\t\tif (_yyreq(%u, _yytid, _yytvl, _yybuffer + %zu", rhs.syms[k].ind, k);
						for (size_t i = 0; i < gen->param_cnt; ++i) {
							fputs(", ", out);
							fwrite(gen->params[i].name.data, 1, gen->params[i].name.len, out);
						}
						fputs(")) return 1;\n", out);
					}
				}
				gen_act* curr = gen->rules[table[i * (gen->token_cnt + 1) + j]].act;
				while (curr != NULL) {
					switch (curr->type) {
						case GEN_CODE_SELF:
							fputs("(_yylvalp->", out);
							fwrite(gen->nterms[i].name.data, 1, gen->nterms[i].name.len, out);
							fputc(')', out);
							break;
						case GEN_CODE_COMP: {
							fprintf(out, "((_yybuffer + %u)->", curr->index);
							struct string name = rhs.syms[curr->index].term ? gen->tokens[rhs.syms[curr->index].ind].name : gen->nterms[rhs.syms[curr->index].ind].name;
							fwrite(name.data, 1, name.len, out);
							fputc(')', out);
							break;
						}
						case GEN_CODE_RAW:
							fwrite(curr->data.data, 1, curr->data.len, out);
							break;
					}
					curr = curr->next;
				}
				fputs("\n\t\t\t\t\tbreak;\n\t\t\t\t}\n", out);
			}
		}
		fputs("\t\t\t\tdefault:\n\t\t\t\t\treturn 1;\n\t\t\t}\n\t\t\tbreak;\n", out);
	}
	fputs("\t\tdefault:\n\t\t\treturn 1;\n\t}\n\treturn 0;\n}\n\nint yyparse(", out);
	if (gen->param_cnt != 0) {
		fwrite(gen->params[0].type.data, 1, gen->params[0].type.len, out);
		fputc(' ', out);
		fwrite(gen->params[0].name.data, 1, gen->params[0].name.len, out);
		for (size_t i = 1; i < gen->param_cnt; ++i) {
			fputs(", ", out);
			fwrite(gen->params[i].type.data, 1, gen->params[i].type.len, out);
			fputc(' ', out);
			fwrite(gen->params[i].name.data, 1, gen->params[i].name.len, out);
		}
	}
	fputs(") {\n\tYYSTYPE _yylval;\n\tYYSTYPE _yytvl;\n\tunsigned int _yytid = _yytran(yylex(&_yytvl", out);
	for (size_t i = 0; i < gen->param_cnt; ++i) {
		fputs(", ", out);
		fwrite(gen->params[i].name.data, 1, gen->params[i].name.len, out);
	}
	fprintf(out, "));\n\treturn _yyreq(%u, &_yytid, &_yytvl, &_yylval", gen->start.ind);
	for (size_t i = 0; i < gen->param_cnt; ++i) {
		fputs(", ", out);
		fwrite(gen->params[i].name.data, 1, gen->params[i].name.len, out);
	}
	fputs(");\n}", out);
}
