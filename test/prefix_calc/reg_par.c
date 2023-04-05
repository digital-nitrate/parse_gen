
#include <math.h>
#include <stdio.h>

typedef union YYSTYPE {
	double NUM;
	double exp;
} YYSTYPE;


extern int yylex(YYSTYPE* lvalp, void* state);

static unsigned int _yytran(int code) {
	switch (code) {
		case 257:
			return 0;
		case 258:
			return 1;
		case 259:
			return 2;
		case 260:
			return 3;
		case 261:
			return 4;
		case 262:
			return 5;
		case 263:
			return 6;
		case 264:
			return 7;
		case -1:
			return 8;
		default:
			return 9;
	}
}

static int _yyreq(unsigned int _yyid, unsigned int* _yytid, YYSTYPE* _yytvl, YYSTYPE* _yylvalp, void* state) {
	YYSTYPE _yybuffer[3];
	switch (_yyid) {
		case 0:
			switch (*_yytid) {
				case 0: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (_yyreq(0, _yytid, _yytvl, _yybuffer + 1, state)) return 1;

					break;
				}
				case 1: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (_yyreq(0, _yytid, _yytvl, _yybuffer + 1, state)) return 1;

					break;
				}
				case 2: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (_yyreq(0, _yytid, _yytvl, _yybuffer + 1, state)) return 1;

					break;
				}
				case 3: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (_yyreq(0, _yytid, _yytvl, _yybuffer + 1, state)) return 1;

					break;
				}
				case 4: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (_yyreq(0, _yytid, _yytvl, _yybuffer + 1, state)) return 1;

					break;
				}
				case 5: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (_yyreq(0, _yytid, _yytvl, _yybuffer + 1, state)) return 1;

					break;
				}
				case 6: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (_yyreq(0, _yytid, _yytvl, _yybuffer + 1, state)) return 1;

					break;
				}
				case 7: {
					if (_yyreq(1, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (_yyreq(0, _yytid, _yytvl, _yybuffer + 1, state)) return 1;

					break;
				}
				case 8: {

					break;
				}
				default:
					return 1;
			}
			break;
		case 1:
			switch (*_yytid) {
				case 0: {
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (*_yytid == 7) {
						_yybuffer[1] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
printf("\t%.10g\n", ((_yybuffer + 0)->exp));
					break;
				}
				case 1: {
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (*_yytid == 7) {
						_yybuffer[1] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
printf("\t%.10g\n", ((_yybuffer + 0)->exp));
					break;
				}
				case 2: {
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (*_yytid == 7) {
						_yybuffer[1] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
printf("\t%.10g\n", ((_yybuffer + 0)->exp));
					break;
				}
				case 3: {
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (*_yytid == 7) {
						_yybuffer[1] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
printf("\t%.10g\n", ((_yybuffer + 0)->exp));
					break;
				}
				case 4: {
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (*_yytid == 7) {
						_yybuffer[1] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
printf("\t%.10g\n", ((_yybuffer + 0)->exp));
					break;
				}
				case 5: {
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (*_yytid == 7) {
						_yybuffer[1] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
printf("\t%.10g\n", ((_yybuffer + 0)->exp));
					break;
				}
				case 6: {
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 0, state)) return 1;
					if (*_yytid == 7) {
						_yybuffer[1] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
printf("\t%.10g\n", ((_yybuffer + 0)->exp));
					break;
				}
				case 7: {
					if (*_yytid == 7) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
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
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
(_yylvalp->exp) = ((_yybuffer + 0)->NUM);
					break;
				}
				case 1: {
					if (*_yytid == 1) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
(_yylvalp->exp) = ((_yybuffer + 1)->exp) + ((_yybuffer + 2)->exp);
					break;
				}
				case 2: {
					if (*_yytid == 2) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
(_yylvalp->exp) = ((_yybuffer + 1)->exp) - ((_yybuffer + 2)->exp);
					break;
				}
				case 3: {
					if (*_yytid == 3) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
(_yylvalp->exp) = ((_yybuffer + 1)->exp) * ((_yybuffer + 2)->exp);
					break;
				}
				case 4: {
					if (*_yytid == 4) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
(_yylvalp->exp) = ((_yybuffer + 1)->exp) / ((_yybuffer + 2)->exp);
					break;
				}
				case 5: {
					if (*_yytid == 5) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 2, state)) return 1;
(_yylvalp->exp) = pow(((_yybuffer + 1)->exp),((_yybuffer + 2)->exp));
					break;
				}
				case 6: {
					if (*_yytid == 6) {
						_yybuffer[0] = *_yytvl;
						*_yytid = _yytran(yylex(_yytvl, state));
					} else {
						return 1;
					}
					if (_yyreq(2, _yytid, _yytvl, _yybuffer + 1, state)) return 1;
(_yylvalp->exp) = -((_yybuffer + 1)->exp);
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

int yyparse(void* state) {
	YYSTYPE _yylval;
	YYSTYPE _yytvl;
	unsigned int _yytid = _yytran(yylex(&_yytvl, state));
	return _yyreq(0, &_yytid, &_yytvl, &_yylval, state);
}