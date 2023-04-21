#include <stdlib.h>

#include "generator.h"

static void free_act(gen_act* act) {
	struct gen_code_unit const* end = act->acts + act->len;
	for (struct gen_code_unit* curr = act->acts; curr != end; ++curr) {
		switch (curr->type) {
			case GEN_CODE_COMP:
				break;
			case GEN_CODE_RAW:
				free(curr->data);
				break;
		}
	}
	free(act->acts);
}

extern void gen_fini(gen_type* gen) {
	free_act(&(gen->top));
	free_act(&(gen->req));
	free_act(&(gen->prov));
	free_act(&(gen->code));
	for (size_t i = 0; i < gen->token_cnt; ++i) {
		free(gen->tokens[i].name);
		free(gen->tokens[i].type);
		free(gen->tokens[i].rh.rules);
	}
	free(gen->tokens);
	for (size_t i = 0; i < gen->nterm_cnt; ++i) {
		free(gen->nterms[i].name);
		free(gen->nterms[i].type);
		free(gen->nterms[i].rh.rules);
		free(gen->nterms[i].lh.rules);
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
		free_act(&(gen->rules[i].act));
	}
	free(gen->rules);
	free(gen->prefixlo);
	free(gen->prefixhi);
	free(gen->epilogue);
}
