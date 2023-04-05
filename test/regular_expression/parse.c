
#include <stdlib.h>
#include "regular.h"

typedef union YYSTYPE {
	char chara;
	reg ALT;
	struct reg_link* ALTLIST;
	reg SEQ;
	struct reg_link* SEQLIST;
	reg ATOM;
	enum atommod_type ATOMMOD;
	reg NUCLEUS;
	struct charrng CHARRNG;
} YYSTYPE;


extern int yylex(YYSTYPE* lvalp, reg* out, void* scanner);

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

static int _yyreq(unsigned int _yyid, unsigned int* _yytid, YYSTYPE* _yytvl, YYSTYPE* _yylvalp, reg* out, void* scanner) {
	YYSTYPE _yybuffer[3];
	switch (_yyid) {
		case 0:
			switch (*_yytid) {
				case 0: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
*out = ((_yybuffer + 0)->ALT);
					break;
				}
				case 3: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
*out = ((_yybuffer + 0)->ALT);
					break;
				}
				case 5: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
*out = ((_yybuffer + 0)->ALT);
					break;
				}
				case 6: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
*out = ((_yybuffer + 0)->ALT);
					break;
				}
				case 8: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
*out = ((_yybuffer + 0)->ALT);
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
			struct reg_link* top = malloc(sizeof *top);
			top->next = ((_yybuffer + 1)->ALTLIST);
			top->sub = ((_yybuffer + 0)->SEQ);
			(_yylvalp->ALT).type = REG_ALT;
			(_yylvalp->ALT).alt = top;
		}
	
					break;
				}
				case 3: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			struct reg_link* top = malloc(sizeof *top);
			top->next = ((_yybuffer + 1)->ALTLIST);
			top->sub = ((_yybuffer + 0)->SEQ);
			(_yylvalp->ALT).type = REG_ALT;
			(_yylvalp->ALT).alt = top;
		}
	
					break;
				}
				case 4: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			struct reg_link* top = malloc(sizeof *top);
			top->next = ((_yybuffer + 1)->ALTLIST);
			top->sub = ((_yybuffer + 0)->SEQ);
			(_yylvalp->ALT).type = REG_ALT;
			(_yylvalp->ALT).alt = top;
		}
	
					break;
				}
				case 5: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			struct reg_link* top = malloc(sizeof *top);
			top->next = ((_yybuffer + 1)->ALTLIST);
			top->sub = ((_yybuffer + 0)->SEQ);
			(_yylvalp->ALT).type = REG_ALT;
			(_yylvalp->ALT).alt = top;
		}
	
					break;
				}
				case 6: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			struct reg_link* top = malloc(sizeof *top);
			top->next = ((_yybuffer + 1)->ALTLIST);
			top->sub = ((_yybuffer + 0)->SEQ);
			(_yylvalp->ALT).type = REG_ALT;
			(_yylvalp->ALT).alt = top;
		}
	
					break;
				}
				case 8: {
					if (_yyreq(3, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->ALTLIST) == NULL) {
			(_yylvalp->ALT) = ((_yybuffer + 0)->SEQ);
		} else {
			struct reg_link* top = malloc(sizeof *top);
			top->next = ((_yybuffer + 1)->ALTLIST);
			top->sub = ((_yybuffer + 0)->SEQ);
			(_yylvalp->ALT).type = REG_ALT;
			(_yylvalp->ALT).alt = top;
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
(_yylvalp->SEQ).type = REG_LAMBDA;
					break;
				}
				case 3: {
					if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->SEQLIST) == NULL) {
			(_yylvalp->SEQ) = ((_yybuffer + 0)->ATOM);
		} else {
			struct reg_link* top = malloc(sizeof *top);
			top->next = ((_yybuffer + 1)->SEQLIST);
			top->sub = ((_yybuffer + 0)->ATOM);
			(_yylvalp->SEQ).type = REG_SEQ;
			(_yylvalp->SEQ).seq = top;
		}
	
					break;
				}
				case 4: {
(_yylvalp->SEQ).type = REG_LAMBDA;
					break;
				}
				case 5: {
					if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->SEQLIST) == NULL) {
			(_yylvalp->SEQ) = ((_yybuffer + 0)->ATOM);
		} else {
			struct reg_link* top = malloc(sizeof *top);
			top->next = ((_yybuffer + 1)->SEQLIST);
			top->sub = ((_yybuffer + 0)->ATOM);
			(_yylvalp->SEQ).type = REG_SEQ;
			(_yylvalp->SEQ).seq = top;
		}
	
					break;
				}
				case 6: {
					if (_yyreq(5, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(4, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		if (((_yybuffer + 1)->SEQLIST) == NULL) {
			(_yylvalp->SEQ) = ((_yybuffer + 0)->ATOM);
		} else {
			struct reg_link* top = malloc(sizeof *top);
			top->next = ((_yybuffer + 1)->SEQLIST);
			top->sub = ((_yybuffer + 0)->ATOM);
			(_yylvalp->SEQ).type = REG_SEQ;
			(_yylvalp->SEQ).seq = top;
		}
	
					break;
				}
				case 8: {
(_yylvalp->SEQ).type = REG_LAMBDA;
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

		switch (((_yybuffer + 1)->ATOMMOD)) {
			case MOD_NONE:
				(_yylvalp->ATOM) = ((_yybuffer + 0)->NUCLEUS);
				break;
			case MOD_PLUS: {
				reg* top = malloc(sizeof *top);
				*top = ((_yybuffer + 0)->NUCLEUS);
				(_yylvalp->ATOM).type = REG_PLUS;
				(_yylvalp->ATOM).plus = top;
				break;
			}
			case MOD_KLEENE: {
				reg* top = malloc(sizeof *top);
				*top = ((_yybuffer + 0)->NUCLEUS);
				(_yylvalp->ATOM).type = REG_KLEENE;
				(_yylvalp->ATOM).kleene = top;
				break;
			}
		}
	
					break;
				}
				case 5: {
					if (_yyreq(7, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(6, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		switch (((_yybuffer + 1)->ATOMMOD)) {
			case MOD_NONE:
				(_yylvalp->ATOM) = ((_yybuffer + 0)->NUCLEUS);
				break;
			case MOD_PLUS: {
				reg* top = malloc(sizeof *top);
				*top = ((_yybuffer + 0)->NUCLEUS);
				(_yylvalp->ATOM).type = REG_PLUS;
				(_yylvalp->ATOM).plus = top;
				break;
			}
			case MOD_KLEENE: {
				reg* top = malloc(sizeof *top);
				*top = ((_yybuffer + 0)->NUCLEUS);
				(_yylvalp->ATOM).type = REG_KLEENE;
				(_yylvalp->ATOM).kleene = top;
				break;
			}
		}
	
					break;
				}
				case 6: {
					if (_yyreq(7, _yytid, _yytvl, _yybuffer + 0, out, scanner)) return 1;
					if (_yyreq(6, _yytid, _yytvl, _yybuffer + 1, out, scanner)) return 1;

		switch (((_yybuffer + 1)->ATOMMOD)) {
			case MOD_NONE:
				(_yylvalp->ATOM) = ((_yybuffer + 0)->NUCLEUS);
				break;
			case MOD_PLUS: {
				reg* top = malloc(sizeof *top);
				*top = ((_yybuffer + 0)->NUCLEUS);
				(_yylvalp->ATOM).type = REG_PLUS;
				(_yylvalp->ATOM).plus = top;
				break;
			}
			case MOD_KLEENE: {
				reg* top = malloc(sizeof *top);
				*top = ((_yybuffer + 0)->NUCLEUS);
				(_yylvalp->ATOM).type = REG_KLEENE;
				(_yylvalp->ATOM).kleene = top;
				break;
			}
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

		switch (((_yybuffer + 1)->CHARRNG).type) {
			case RNG_NONE:
				(_yylvalp->NUCLEUS).type = REG_CHAR;
				(_yylvalp->NUCLEUS).c = ((_yybuffer + 0)->chara);
				break;
			case RNG_TOP:
				(_yylvalp->NUCLEUS).type = REG_RANGE;
				(_yylvalp->NUCLEUS).c1 = ((_yybuffer + 0)->chara);
				(_yylvalp->NUCLEUS).c2 = ((_yybuffer + 1)->CHARRNG).top;
				break;
		}
	
					break;
				}
				case 6: {
					if (*_yytid == 6) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, out, scanner));
					} else {
						return 1;
					}
(_yylvalp->NUCLEUS).type = REG_DOT;
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

int yyparse(reg* out, void* scanner) {
	YYSTYPE _yylval;
	YYSTYPE _yytvl;
	unsigned int _yytid = _yytran(yylex(&_yytvl, out, scanner));
	return _yyreq(0, &_yytid, &_yytvl, &_yylval, out, scanner);
}