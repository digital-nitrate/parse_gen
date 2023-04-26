#include <stdlib.h>

#include "generator.h"

void gen_act_fini(gen_act* act) {
	struct gen_code_unit const* end = act->acts + act->len;
	for (struct gen_code_unit* curr = act->acts; curr != end; ++curr) {
		switch (curr->type) {
			case GEN_CODE_RAW:
				free(curr->data);
				break;
			default:;
		}
	}
	free(act->acts);
}

extern void gen_fini(gen_type* gen) {
	if (gen->top.acts != NULL) gen_act_fini(&(gen->top));
	if (gen->req.acts != NULL) gen_act_fini(&(gen->req));
	if (gen->prov.acts != NULL) gen_act_fini(&(gen->prov));
	if (gen->code.acts != NULL) gen_act_fini(&(gen->code));
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		free(gen->tokens[i].name);
		free(gen->tokens[i].type);
	}
	free(gen->tokens);
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		free(gen->nterms[i].name);
		free(gen->nterms[i].type);
	}
	free(gen->nterms);
	for (size_t i = 0; i < gen->pparam_cnt; ++i) {
		free(gen->pparams[i].type);
		free(gen->pparams[i].name);
	}
	free(gen->pparams);
	for (size_t i = 0; i < gen->lparam_cnt; ++i) {
		free(gen->lparams[i].type);
		free(gen->lparams[i].name);
	}
	free(gen->lparams);
	for (size_t i = 0; i < gen->rule_cnt; ++i) {
		free(gen->rules[i].rhs.syms);
		gen_act_fini(&(gen->rules[i].act));
	}
	free(gen->rules);
	free(gen->prefixlo);
	free(gen->prefixhi);
	free(gen->epilogue);
}
