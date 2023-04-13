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

#line 24 "reg_par.h"

#endif
