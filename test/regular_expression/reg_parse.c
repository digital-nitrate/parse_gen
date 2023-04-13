#ifndef YYPLL_H
#define YYPLL_H 1
enum {
YYEOF = 0,
YYUNDEF = -1,
tokpipe = 1,
tokkleene = 2,
tokplus = 3,
tokopen = 4,
tokclose = 5,
tokchar = 6,
tokdot = 7,
tokdash = 8,
};
#line 1 "regular.cfg"

	#include "regular.h"

#line 20 "reg_parse.c"
typedef union YYSTYPE {
	char tokchar;
	reg_in NTALT;
	struct reg_link* NTALTLIST;
	reg_in NTSEQ;
	struct reg_link* NTSEQLIST;
	reg_in NTATOM;
	enum atommod_type NTATOMMOD;
	reg_in NTNUCLEUS;
	struct charrng NTCHARRNG;
} YYSTYPE;
#line 5 "regular.cfg"

	extern int yyparse(reg_nfa* out, void* scanner);

#line 36 "reg_parse.c"

#endif
#line 9 "regular.cfg"

	#include <stdlib.h>
	#include "reg_lex.h"

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

#line 67 "reg_parse.c"
static unsigned int _yytran(int code) {
switch (code) {
case tokpipe: return 0;
case tokkleene: return 1;
case tokplus: return 2;
case tokopen: return 3;
case tokclose: return 4;
case tokchar: return 5;
case tokdot: return 6;
case tokdash: return 7;
case 0: return 8;
default: return 9;
}
}
static unsigned int const _yytab[] = {
0, 17, 17, 0, 17, 0, 0, 17, 0, 17,
1, 17, 17, 1, 1, 1, 1, 17, 1, 17,
2, 17, 17, 17, 3, 17, 17, 17, 3, 17,
5, 17, 17, 4, 5, 4, 4, 17, 5, 17,
7, 17, 17, 6, 7, 6, 6, 17, 7, 17,
17, 17, 17, 8, 17, 8, 8, 17, 17, 17,
11, 9, 10, 11, 11, 11, 11, 17, 11, 17,
17, 17, 17, 12, 17, 13, 14, 17, 17, 17,
16, 16, 16, 16, 16, 16, 16, 15, 16, 17,
};
static int _yyreq(unsigned int _yyid, unsigned int* _yytid, YYSTYPE* _yytvl, YYSTYPE* _yylvalp, reg_nfa* out, void* scanner) {
YYSTYPE _yybuffer[3];
switch(_yytab[_yyid * 10 + *_yytid]) {
case 0:
if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
{
#line 65 "regular.cfg"
out->specials = (_yybuffer[0].NTALT);
#line 101 "reg_parse.c"
}
break;
case 1:
if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;
{
#line 69 "regular.cfg"

		if ((_yybuffer[1].NTALTLIST) == NULL) {
			(_yylvalp->NTALT) = (_yybuffer[0].NTSEQ);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), (_yybuffer[0].NTSEQ).start)) exit(1);
			if (add_path(&(out->table.nodes[(_yybuffer[0].NTSEQ).end]), out->table.node_cnt - 1)) exit(1);
			struct reg_link* curr = (_yybuffer[1].NTALTLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), tmp->sub.start)) exit(1);
				if (add_path(&(out->table.nodes[tmp->sub.end]), out->table.node_cnt - 1)) exit(1);
				curr = tmp->next;
				free(tmp);
			}
			(_yylvalp->NTALT).start = out->table.node_cnt - 2;
			(_yylvalp->NTALT).end = out->table.node_cnt - 1;
		}
	
#line 129 "reg_parse.c"
}
break;
case 2:
if (*_yytid == 0) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, scanner));
} else {
return 1;
}
if (_yyreq(3, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, out, scanner)) return 1;
{
#line 92 "regular.cfg"

		struct reg_link* top = malloc(sizeof *top);
		if (top == NULL) exit(1);
		top->next = (_yybuffer[2].NTALTLIST);
		top->sub = (_yybuffer[1].NTSEQ);
		(_yylvalp->NTALTLIST) = top;
	
#line 150 "reg_parse.c"
}
break;
case 3:
{
#line 99 "regular.cfg"
(_yylvalp->NTALTLIST) = NULL;
#line 157 "reg_parse.c"
}
break;
case 4:
if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;
{
#line 103 "regular.cfg"

		if ((_yybuffer[1].NTSEQLIST) == NULL) {
			(_yylvalp->NTSEQ) = (_yybuffer[0].NTATOM);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), (_yybuffer[0].NTATOM).start)) exit(1);
			unsigned int save = (_yybuffer[0].NTATOM).end;
			struct reg_link* curr = (_yybuffer[1].NTSEQLIST);
			while (curr != NULL) {
				struct reg_link* tmp = curr;
				if (add_path(&(out->table.nodes[save]), tmp->sub.start)) exit(1);
				save = tmp->sub.end;
				curr = tmp->next;
				free(tmp);
			}
			if (add_path(&(out->table.nodes[save]), out->table.node_cnt - 1)) exit(1);
			(_yylvalp->NTSEQ).start = out->table.node_cnt - 2;
			(_yylvalp->NTSEQ).end = out->table.node_cnt - 1;
		}
	
#line 186 "reg_parse.c"
}
break;
case 5:
{
#line 124 "regular.cfg"

		if (init_node(&(out->table))) exit(1);
		if (init_node(&(out->table))) exit(1);
		if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), out->table.node_cnt - 1)) exit(1);
		(_yylvalp->NTSEQ).start = out->table.node_cnt - 2;
		(_yylvalp->NTSEQ).end = out->table.node_cnt - 1;
	
#line 199 "reg_parse.c"
}
break;
case 6:
if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;
{
#line 134 "regular.cfg"

		struct reg_link* top = malloc(sizeof *top);
		top->next = (_yybuffer[1].NTSEQLIST);
		top->sub = (_yybuffer[0].NTATOM);
		(_yylvalp->NTSEQLIST) = top;
	
#line 213 "reg_parse.c"
}
break;
case 7:
{
#line 140 "regular.cfg"
(_yylvalp->NTSEQLIST) = NULL;
#line 220 "reg_parse.c"
}
break;
case 8:
if (_yyreq(7, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
if (_yyreq(6, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;
{
#line 144 "regular.cfg"

		if ((_yybuffer[1].NTATOMMOD) == MOD_NONE) {
			(_yylvalp->NTATOM) = (_yybuffer[0].NTNUCLEUS);
		} else {
			if (init_node(&(out->table))) exit(1);
			if (init_node(&(out->table))) exit(1);
			if (add_path(&(out->table.nodes[out->table.node_cnt - 2]), (_yybuffer[0].NTNUCLEUS).start)) exit(1);
			if (add_path(&(out->table.nodes[(_yybuffer[0].NTNUCLEUS).end]), out->table.node_cnt - 1)) exit(1);
			if (add_path(&(out->table.nodes[(_yybuffer[0].NTNUCLEUS).end]), (_yybuffer[0].NTNUCLEUS).start)) exit(1);
			if ((_yybuffer[1].NTATOMMOD) == MOD_KLEENE) {
				if (add_path(&(out->table.nodes[(_yybuffer[0].NTNUCLEUS).start]), (_yybuffer[0].NTNUCLEUS).end)) exit(1);
			}
			(_yylvalp->NTATOM).start = out->table.node_cnt - 2;
			(_yylvalp->NTATOM).end = out->table.node_cnt - 1;
		}
	
#line 244 "reg_parse.c"
}
break;
case 9:
if (*_yytid == 1) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, scanner));
} else {
return 1;
}
{
#line 163 "regular.cfg"
(_yylvalp->NTATOMMOD) = MOD_KLEENE;
#line 257 "reg_parse.c"
}
break;
case 10:
if (*_yytid == 2) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, scanner));
} else {
return 1;
}
{
#line 164 "regular.cfg"
(_yylvalp->NTATOMMOD) = MOD_PLUS;
#line 270 "reg_parse.c"
}
break;
case 11:
{
#line 165 "regular.cfg"
(_yylvalp->NTATOMMOD) = MOD_NONE;
#line 277 "reg_parse.c"
}
break;
case 12:
if (*_yytid == 3) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, scanner));
} else {
return 1;
}
if (_yyreq(1, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;
if (*_yytid == 4) {
_yybuffer[2] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, scanner));
} else {
return 1;
}
{
#line 169 "regular.cfg"
(_yylvalp->NTNUCLEUS) = (_yybuffer[1].NTALT);
#line 297 "reg_parse.c"
}
break;
case 13:
if (*_yytid == 5) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, scanner));
} else {
return 1;
}
if (_yyreq(8, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;
{
#line 170 "regular.cfg"

		if (init_node(&(out->table))) exit(1);
		if (init_node(&(out->table))) exit(1);
		if ((_yybuffer[1].NTCHARRNG).type == RNG_NONE) {
			out->table.nodes[out->table.node_cnt - 2].tran[(unsigned char)(_yybuffer[0].tokchar)] = out->table.node_cnt - 1;
		} else {
			if ((unsigned char)(_yybuffer[0].tokchar) > (unsigned char)((_yybuffer[1].NTCHARRNG).top)) exit(1);
			for (size_t i = (unsigned char)(_yybuffer[0].tokchar); i <= (unsigned char)((_yybuffer[1].NTCHARRNG).top); ++i) {
				out->table.nodes[out->table.node_cnt - 2].tran[i] = out->table.node_cnt - 1;
			}
		}
		(_yylvalp->NTNUCLEUS).start = out->table.node_cnt - 2;
		(_yylvalp->NTNUCLEUS).end = out->table.node_cnt - 1;
	
#line 324 "reg_parse.c"
}
break;
case 14:
if (*_yytid == 6) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, scanner));
} else {
return 1;
}
{
#line 184 "regular.cfg"

		if (init_node(&(out->table))) exit(1);
		if (init_node(&(out->table))) exit(1);
		for (size_t i = 0; i < (1 << CHAR_BIT); ++i) {
			out->table.nodes[out->table.node_cnt - 2].tran[i] = out->table.node_cnt - 1;
		}
		(_yylvalp->NTNUCLEUS).start = out->table.node_cnt - 2;
		(_yylvalp->NTNUCLEUS).end = out->table.node_cnt - 1;
	
#line 345 "reg_parse.c"
}
break;
case 15:
if (*_yytid == 7) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, scanner));
} else {
return 1;
}
if (*_yytid == 5) {
_yybuffer[1] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, scanner));
} else {
return 1;
}
{
#line 196 "regular.cfg"

		(_yylvalp->NTCHARRNG).type = RNG_TOP;
		(_yylvalp->NTCHARRNG).top = (_yybuffer[1].tokchar);
	
#line 367 "reg_parse.c"
}
break;
case 16:
{
#line 200 "regular.cfg"
(_yylvalp->NTCHARRNG).type = RNG_NONE;
#line 374 "reg_parse.c"
}
break;
default: return 1;
}
return 0;
}
int yyparse(reg_nfa* out, void* scanner) {
YYSTYPE _yylval;
YYSTYPE _yytvl;
unsigned int _yytid = _yytran(yylex(&_yytvl, scanner));
return _yyreq(0, &_yytid, &_yytvl, &_yylval, out, scanner);
}
