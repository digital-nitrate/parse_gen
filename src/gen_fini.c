#include <stdlib.h>

#include "generator.h"

extern void gen_fini(gen_type* gen) {
	for (size_t i = 0; i < gen->prologue_cnt; ++i) free(gen->prologue[i].data);
	free(gen->prologue);
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		free(gen->tokens[i].name.data);
		free(gen->tokens[i].type.data);
		free(gen->tokens[i].rh.rules);
	}
	free(gen->tokens);
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		free(gen->nterms[i].name.data);
		free(gen->nterms[i].type.data);
		free(gen->nterms[i].rh.rules);
		free(gen->nterms[i].lh.rules);
	}
	free(gen->nterms);
	for (size_t i = 0; i < gen->param_cnt; ++i) {
		free(gen->params[i].type.data);
		free(gen->params[i].name.data);
	}
	free(gen->params);
	for (size_t i = 0; i < gen->rule_cnt; ++i) {
		free(gen->rules[i].rhs.syms);
		gen_act* curr = gen->rules[i].act;
		while (curr != NULL) {
			gen_act* tmp = curr;
			curr = tmp->next;
			switch (tmp->type) {
				case GEN_CODE_SELF:
				case GEN_CODE_COMP:
					break;
				case GEN_CODE_RAW:
					free(tmp->data.data);
					break;
			}
			free(tmp);
		}
	}
	free(gen->rules);
	free(gen->prefix.data);
	free(gen->epilogue.data);
}
