
#include <stdlib.h>
#include "regular.h"

static int init_node(reg_nfa_table* table) {
	unsigned int* p = malloc((sizeof *p) * 16);
	if (p == NULL) return 1;
	int res = 0;
	DYNARR_CHK(table->node_cnt, table->node_cap, table->nodes, res);
	if (res) {free(p); return 1;}
	table->nodes[table->node_cnt].lambda = (reg_nfa_lam){.paths=p,.cnt=0,.cap=16};
	for (size_t i = 0; i < (1 << CHAR_BIT); ++i) {
		table->nodes[table->node_cnt].tran[i] = UINT_MAX;
	}
	++(table->node_cnt);
	return 0;
}

static int add_path(reg_nfa_node* node, unsigned int goal) {
	int res = 0;
	DYNARR_CHK(node->lambda.cnt, node->lambda.cap, node->lambda.paths, res);
	if (res) return 1;
	node->lambda.paths[node->lambda.cnt] = goal;
	++(node->lambda.cnt);
	return 0;
}


typedef union YYSTYPE {
	char chara;
	reg_in ALT;
	struct reg_link* ALTLIST;
	reg_in SEQ;
	struct reg_link* SEQLIST;
	reg_in ATOM;
	enum atommod_type ATOMMOD;
	reg_in NUCLEUS;
	struct charrng CHARRNG;
} YYSTYPE;


extern int yylex(YYSTYPE* lvalp, reg_nfa* out, void* scanner);

static unsigned int _yytran(int code) {
	switch (code) {
		case 1:
			return 0;
		case 2:
			return 1;
		case 3:
			return 2;
		case 4:
			return 3;
		case 5:
			return 4;
		case 6:
			return 5;
		case 7:
			return 6;
		case 8:
			return 7;
		case -1:
			return 8;
		default:
			return 9;
	}
}

static int _yyreq(unsigned int _yyid, unsigned int* _yytid, YYSTYPE* _yytvl, YYSTYPE* _yylvalp, reg_nfa* out, void* scanner) {
	YYSTYPE _yybuffer[3];
	switch (_yyid) {
		case 0:
			switch (*_yytid) {
				case 0: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
out->specials = ((_yybuffer + 0)->ALT);
					break;
				}
				case 3: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
out->specials = ((_yybuffer + 0)->ALT);
					break;
				}
				case 5: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
out->specials = ((_yybuffer + 0)->ALT);
					break;
				}
				case 6: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
out->specials = ((_yybuffer + 0)->ALT);
					break;
				}
				case 8: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
out->specials = ((_yybuffer + 0)->ALT);
					break;
				}
				default:
					return 1;
			}
			break;
		case 1:
			switch (*_yytid) {
				case 0: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->SEQ).start)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->SEQ).end]), out->table.node_cnt - 1)) exit(1);
			struct reg_link* curr = ((_yybuffer + 1)->ALTLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), tmp->sub.start)) exit(1);
				if (add_path(&(out->table.nodes[tmp->sub.end]), out->table.node_cnt - 1)) exit(1);
				curr = tmp->next;
				free(tmp);
			}
			(_yylvalp->ALT).start = out->table.node_cnt - 2;
			(_yylvalp->ALT).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 3: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->SEQ).start)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->SEQ).end]), out->table.node_cnt - 1)) exit(1);
			struct reg_link* curr = ((_yybuffer + 1)->ALTLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), tmp->sub.start)) exit(1);
				if (add_path(&(out->table.nodes[tmp->sub.end]), out->table.node_cnt - 1)) exit(1);
				curr = tmp->next;
				free(tmp);
			}
			(_yylvalp->ALT).start = out->table.node_cnt - 2;
			(_yylvalp->ALT).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 4: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->SEQ).start)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->SEQ).end]), out->table.node_cnt - 1)) exit(1);
			struct reg_link* curr = ((_yybuffer + 1)->ALTLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), tmp->sub.start)) exit(1);
				if (add_path(&(out->table.nodes[tmp->sub.end]), out->table.node_cnt - 1)) exit(1);
				curr = tmp->next;
				free(tmp);
			}
			(_yylvalp->ALT).start = out->table.node_cnt - 2;
			(_yylvalp->ALT).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 5: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->SEQ).start)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->SEQ).end]), out->table.node_cnt - 1)) exit(1);
			struct reg_link* curr = ((_yybuffer + 1)->ALTLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), tmp->sub.start)) exit(1);
				if (add_path(&(out->table.nodes[tmp->sub.end]), out->table.node_cnt - 1)) exit(1);
				curr = tmp->next;
				free(tmp);
			}
			(_yylvalp->ALT).start = out->table.node_cnt - 2;
			(_yylvalp->ALT).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 6: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->SEQ).start)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->SEQ).end]), out->table.node_cnt - 1)) exit(1);
			struct reg_link* curr = ((_yybuffer + 1)->ALTLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), tmp->sub.start)) exit(1);
				if (add_path(&(out->table.nodes[tmp->sub.end]), out->table.node_cnt - 1)) exit(1);
				curr = tmp->next;
				free(tmp);
			}
			(_yylvalp->ALT).start = out->table.node_cnt - 2;
			(_yylvalp->ALT).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 8: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->SEQ).start)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->SEQ).end]), out->table.node_cnt - 1)) exit(1);
			struct reg_link* curr = ((_yybuffer + 1)->ALTLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), tmp->sub.start)) exit(1);
				if (add_path(&(out->table.nodes[tmp->sub.end]), out->table.node_cnt - 1)) exit(1);
				curr = tmp->next;
				free(tmp);
			}
			(_yylvalp->ALT).start = out->table.node_cnt - 2;
			(_yylvalp->ALT).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				default:
					return 1;
			}
			break;
		case 2:
			switch (*_yytid) {
				case 0: {
					if (*_yytid == 0) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, out, scanner)) return 1;

		struct reg_link* top = malloc(sizeof *top);
		if (top == NULL) exit(1);
		top->next = ((_yybuffer + 2)->ALTLIST);
		top->sub = ((_yybuffer + 1)->SEQ);
		(_yylvalp->ALTLIST) = top;
	
					break;
				}
				case 4: {
(_yylvalp->ALTLIST) = NULL;
					break;
				}
				case 8: {
(_yylvalp->ALTLIST) = NULL;
					break;
				}
				default:
					return 1;
			}
			break;
		case 3:
			switch (*_yytid) {
				case 0: {

		if (init_node(&(out->table))) exit(1);
		if (init_node(&(out->table))) exit(1);
		if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), out->table.node_cnt - 1)) exit(1);
		(_yylvalp->SEQ).start = out->table.node_cnt - 2;
		(_yylvalp->SEQ).end = out->table.node_cnt - 1;
	
					break;
				}
				case 3: {
					if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->SEQLIST) == NULL) {
			(_yylvalp->SEQ) = ((_yybuffer + 0)->ATOM);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->ATOM).start)) exit(1);
			unsigned int save = ((_yybuffer + 0)->ATOM).end;
			struct reg_link* curr = ((_yybuffer + 1)->SEQLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[save]), tmp->sub.start)) exit(1);
				save = tmp->sub.end;
				curr = tmp->next;
				free(tmp);
			}
			if (add_path(&(out->table.nodes[save]), out->table.node_cnt - 1)) exit(1);
			(_yylvalp->SEQ).start = out->table.node_cnt - 2;
			(_yylvalp->SEQ).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 4: {

		if (init_node(&(out->table))) exit(1);
		if (init_node(&(out->table))) exit(1);
		if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), out->table.node_cnt - 1)) exit(1);
		(_yylvalp->SEQ).start = out->table.node_cnt - 2;
		(_yylvalp->SEQ).end = out->table.node_cnt - 1;
	
					break;
				}
				case 5: {
					if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->SEQLIST) == NULL) {
			(_yylvalp->SEQ) = ((_yybuffer + 0)->ATOM);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->ATOM).start)) exit(1);
			unsigned int save = ((_yybuffer + 0)->ATOM).end;
			struct reg_link* curr = ((_yybuffer + 1)->SEQLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[save]), tmp->sub.start)) exit(1);
				save = tmp->sub.end;
				curr = tmp->next;
				free(tmp);
			}
			if (add_path(&(out->table.nodes[save]), out->table.node_cnt - 1)) exit(1);
			(_yylvalp->SEQ).start = out->table.node_cnt - 2;
			(_yylvalp->SEQ).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 6: {
					if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->SEQLIST) == NULL) {
			(_yylvalp->SEQ) = ((_yybuffer + 0)->ATOM);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->ATOM).start)) exit(1);
			unsigned int save = ((_yybuffer + 0)->ATOM).end;
			struct reg_link* curr = ((_yybuffer + 1)->SEQLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[save]), tmp->sub.start)) exit(1);
				save = tmp->sub.end;
				curr = tmp->next;
				free(tmp);
			}
			if (add_path(&(out->table.nodes[save]), out->table.node_cnt - 1)) exit(1);
			(_yylvalp->SEQ).start = out->table.node_cnt - 2;
			(_yylvalp->SEQ).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 8: {

		if (init_node(&(out->table))) exit(1);
		if (init_node(&(out->table))) exit(1);
		if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), out->table.node_cnt - 1)) exit(1);
		(_yylvalp->SEQ).start = out->table.node_cnt - 2;
		(_yylvalp->SEQ).end = out->table.node_cnt - 1;
	
					break;
				}
				default:
					return 1;
			}
			break;
		case 4:
			switch (*_yytid) {
				case 0: {
(_yylvalp->SEQLIST) = NULL;
					break;
				}
				case 3: {
					if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		struct reg_link* top = malloc(sizeof *top);
		top->next = ((_yybuffer + 1)->SEQLIST);
		top->sub = ((_yybuffer + 0)->ATOM);
		(_yylvalp->SEQLIST) = top;
	
					break;
				}
				case 4: {
(_yylvalp->SEQLIST) = NULL;
					break;
				}
				case 5: {
					if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		struct reg_link* top = malloc(sizeof *top);
		top->next = ((_yybuffer + 1)->SEQLIST);
		top->sub = ((_yybuffer + 0)->ATOM);
		(_yylvalp->SEQLIST) = top;
	
					break;
				}
				case 6: {
					if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		struct reg_link* top = malloc(sizeof *top);
		top->next = ((_yybuffer + 1)->SEQLIST);
		top->sub = ((_yybuffer + 0)->ATOM);
		(_yylvalp->SEQLIST) = top;
	
					break;
				}
				case 8: {
(_yylvalp->SEQLIST) = NULL;
					break;
				}
				default:
					return 1;
			}
			break;
		case 5:
			switch (*_yytid) {
				case 3: {
					if (_yyreq(7, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(6, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ATOMMOD) == MOD_NONE) {
			(_yylvalp->ATOM) = ((_yybuffer + 0)->NUCLEUS);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->NUCLEUS).start)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->NUCLEUS).end]), out->table.node_cnt - 1)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->NUCLEUS).end]), ((_yybuffer + 0)->NUCLEUS).start)) exit(1);
			if (((_yybuffer + 1)->ATOMMOD) == MOD_KLEENE) {
				if (add_path(&(out->table.nodes[((_yybuffer + 0)->NUCLEUS).start]), ((_yybuffer + 0)->NUCLEUS).end)) exit(1);
			}
			(_yylvalp->ATOM).start = out->table.node_cnt - 2;
			(_yylvalp->ATOM).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 5: {
					if (_yyreq(7, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(6, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ATOMMOD) == MOD_NONE) {
			(_yylvalp->ATOM) = ((_yybuffer + 0)->NUCLEUS);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->NUCLEUS).start)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->NUCLEUS).end]), out->table.node_cnt - 1)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->NUCLEUS).end]), ((_yybuffer + 0)->NUCLEUS).start)) exit(1);
			if (((_yybuffer + 1)->ATOMMOD) == MOD_KLEENE) {
				if (add_path(&(out->table.nodes[((_yybuffer + 0)->NUCLEUS).start]), ((_yybuffer + 0)->NUCLEUS).end)) exit(1);
			}
			(_yylvalp->ATOM).start = out->table.node_cnt - 2;
			(_yylvalp->ATOM).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				case 6: {
					if (_yyreq(7, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(6, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ATOMMOD) == MOD_NONE) {
			(_yylvalp->ATOM) = ((_yybuffer + 0)->NUCLEUS);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), ((_yybuffer + 0)->NUCLEUS).start)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->NUCLEUS).end]), out->table.node_cnt - 1)) exit(1);
			if (add_path(&(out->table.nodes[((_yybuffer + 0)->NUCLEUS).end]), ((_yybuffer + 0)->NUCLEUS).start)) exit(1);
			if (((_yybuffer + 1)->ATOMMOD) == MOD_KLEENE) {
				if (add_path(&(out->table.nodes[((_yybuffer + 0)->NUCLEUS).start]), ((_yybuffer + 0)->NUCLEUS).end)) exit(1);
			}
			(_yylvalp->ATOM).start = out->table.node_cnt - 2;
			(_yylvalp->ATOM).end = out->table.node_cnt - 1;
		}
	
					break;
				}
				default:
					return 1;
			}
			break;
		case 6:
			switch (*_yytid) {
				case 0: {
(_yylvalp->ATOMMOD) = MOD_NONE;
					break;
				}
				case 1: {
					if (*_yytid == 1) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}
(_yylvalp->ATOMMOD) = MOD_KLEENE;
					break;
				}
				case 2: {
					if (*_yytid == 2) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}
(_yylvalp->ATOMMOD) = MOD_PLUS;
					break;
				}
				case 3: {
(_yylvalp->ATOMMOD) = MOD_NONE;
					break;
				}
				case 4: {
(_yylvalp->ATOMMOD) = MOD_NONE;
					break;
				}
				case 5: {
(_yylvalp->ATOMMOD) = MOD_NONE;
					break;
				}
				case 6: {
(_yylvalp->ATOMMOD) = MOD_NONE;
					break;
				}
				case 8: {
(_yylvalp->ATOMMOD) = MOD_NONE;
					break;
				}
				default:
					return 1;
			}
			break;
		case 7:
			switch (*_yytid) {
				case 3: {
					if (*_yytid == 3) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;
					if (*_yytid == 4) {
						_yybuffer[2] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}
(_yylvalp->NUCLEUS) = ((_yybuffer + 1)->ALT);
					break;
				}
				case 5: {
					if (*_yytid == 5) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}
					if (_yyreq(8, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (init_node(&(out->table))) exit(1);
		if (init_node(&(out->table))) exit(1);
		if (((_yybuffer + 1)->CHARRNG).type == RNG_NONE) {
			out->table.nodes[out->table.node_cnt - 2].tran[(unsigned char)((_yybuffer + 0)->chara)] = out->table.node_cnt - 1;
		} else {
			if ((unsigned char)((_yybuffer + 0)->chara) > (unsigned char)(((_yybuffer + 1)->CHARRNG).top)) exit(1);
			for (size_t i = (unsigned char)((_yybuffer + 0)->chara); i <= (unsigned char)(((_yybuffer + 1)->CHARRNG).top); ++i) {
				out->table.nodes[out->table.node_cnt - 2].tran[i] = out->table.node_cnt - 1;
			}
		}
		(_yylvalp->NUCLEUS).start = out->table.node_cnt - 2;
		(_yylvalp->NUCLEUS).end = out->table.node_cnt - 1;
	
					break;
				}
				case 6: {
					if (*_yytid == 6) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}

		if (init_node(&(out->table))) exit(1);
		if (init_node(&(out->table))) exit(1);
		for (size_t i = 0; i < (1 << CHAR_BIT); ++i) {
			out->table.nodes[out->table.node_cnt - 2].tran[i] = out->table.node_cnt - 1;
		}
		(_yylvalp->NUCLEUS).start = out->table.node_cnt - 2;
		(_yylvalp->NUCLEUS).end = out->table.node_cnt - 1;
	
					break;
				}
				default:
					return 1;
			}
			break;
		case 8:
			switch (*_yytid) {
				case 0: {
(_yylvalp->CHARRNG).type = RNG_NONE;
					break;
				}
				case 1: {
(_yylvalp->CHARRNG).type = RNG_NONE;
					break;
				}
				case 2: {
(_yylvalp->CHARRNG).type = RNG_NONE;
					break;
				}
				case 3: {
(_yylvalp->CHARRNG).type = RNG_NONE;
					break;
				}
				case 4: {
(_yylvalp->CHARRNG).type = RNG_NONE;
					break;
				}
				case 5: {
(_yylvalp->CHARRNG).type = RNG_NONE;
					break;
				}
				case 6: {
(_yylvalp->CHARRNG).type = RNG_NONE;
					break;
				}
				case 7: {
					if (*_yytid == 7) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}
					if (*_yytid == 5) {
						_yybuffer[1] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}

		(_yylvalp->CHARRNG).type = RNG_TOP;
		(_yylvalp->CHARRNG).top = ((_yybuffer + 1)->chara);
	
					break;
				}
				case 8: {
(_yylvalp->CHARRNG).type = RNG_NONE;
					break;
				}
				default:
					return 1;
			}
			break;
		default:
			return 1;
	}
	return 0;
}

int yyparse(reg_nfa* out, void* scanner) {
	YYSTYPE _yylval;
	YYSTYPE _yytvl;
	unsigned int _yytid = _yytran(yylex(&_yytvl, out, scanner));
	return _yyreq(0, &_yytid, &_yytvl, &_yylval, out, scanner);
}