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

#line 20 "reg_parse.h"
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

#line 36 "reg_parse.h"

#endif
