#ifndef YYPLL_H
#define YYPLL_H 1
enum {
YYEOF = 0,
YYUNDEF = -1,
NUM = 257,
ADD = 258,
SUB = 259,
MUL = 260,
DIV = 261,
POW = 262,
NEG = 263,
NL = 264,
};
typedef union YYSTYPE {
	double NUM;
	double exp;
} YYSTYPE;
#line 1 "reg.cfg"

	extern int yyparse(void* state);

#line 24 "reg_par.c"

#endif
#line 5 "reg.cfg"

	#include <math.h>
	#include <stdio.h>
	#include "lex.h"

#line 33 "reg_par.c"
static unsigned int _yytran(int code) {
switch (code) {
case NUM: return 0;
case ADD: return 1;
case SUB: return 2;
case MUL: return 3;
case DIV: return 4;
case POW: return 5;
case NEG: return 6;
case NL: return 7;
case 0: return 8;
default: return 9;
}
}
static unsigned int const _yytab[] = {
1, 1, 1, 1, 1, 1, 1, 1, 0, 11,
3, 3, 3, 3, 3, 3, 3, 2, 11, 11,
4, 5, 6, 7, 8, 9, 10, 11, 11, 11,
};
static int _yyreq(unsigned int _yyid, unsigned int* _yytid, YYSTYPE* _yytvl, YYSTYPE* _yylvalp, void* state) {
YYSTYPE _yybuffer[3];
switch(_yytab[_yyid * 10 + *_yytid]) {
case 0:
{
#line 31 "reg.cfg"

#line 60 "reg_par.c"
}
break;
case 1:
if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
if (_yyreq(0, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
{
#line 32 "reg.cfg"

#line 69 "reg_par.c"
}
break;
case 2:
if (*_yytid == 7) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, state));
} else {
return 1;
}
{
#line 35 "reg.cfg"

#line 82 "reg_par.c"
}
break;
case 3:
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
if (*_yytid == 7) {
_yybuffer[1] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, state));
} else {
return 1;
}
{
#line 36 "reg.cfg"
printf("\t%.10g\n", (_yybuffer[0].exp));
#line 96 "reg_par.c"
}
break;
case 4:
if (*_yytid == 0) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, state));
} else {
return 1;
}
{
#line 39 "reg.cfg"
(_yylvalp->exp) = (_yybuffer[0].NUM);
#line 109 "reg_par.c"
}
break;
case 5:
if (*_yytid == 1) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, state));
} else {
return 1;
}
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
{
#line 40 "reg.cfg"
(_yylvalp->exp) = (_yybuffer[1].exp) + (_yybuffer[2].exp);
#line 124 "reg_par.c"
}
break;
case 6:
if (*_yytid == 2) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, state));
} else {
return 1;
}
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
{
#line 41 "reg.cfg"
(_yylvalp->exp) = (_yybuffer[1].exp) - (_yybuffer[2].exp);
#line 139 "reg_par.c"
}
break;
case 7:
if (*_yytid == 3) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, state));
} else {
return 1;
}
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
{
#line 42 "reg.cfg"
(_yylvalp->exp) = (_yybuffer[1].exp) * (_yybuffer[2].exp);
#line 154 "reg_par.c"
}
break;
case 8:
if (*_yytid == 4) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, state));
} else {
return 1;
}
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
{
#line 43 "reg.cfg"
(_yylvalp->exp) = (_yybuffer[1].exp) / (_yybuffer[2].exp);
#line 169 "reg_par.c"
}
break;
case 9:
if (*_yytid == 5) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, state));
} else {
return 1;
}
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
{
#line 44 "reg.cfg"
(_yylvalp->exp) = pow((_yybuffer[1].exp),(_yybuffer[2].exp));
#line 184 "reg_par.c"
}
break;
case 10:
if (*_yytid == 6) {
_yybuffer[0] = *_yytvl;
*_yytid = _yytran(yylex(_yytvl, state));
} else {
return 1;
}
if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
{
#line 45 "reg.cfg"
(_yylvalp->exp) = -(_yybuffer[1].exp);
#line 198 "reg_par.c"
}
break;
default: return 1;
}
return 0;
}
int yyparse(void* state) {
YYSTYPE _yylval;
YYSTYPE _yytvl;
unsigned int _yytid = _yytran(yylex(&_yytvl, state));
return _yyreq(0, &_yytid, &_yytvl, &_yylval, state);
}
#line 48 "reg.cfg"



#line 215 "reg_par.c"
