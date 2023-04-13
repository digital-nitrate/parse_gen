#ifndef REGULAR_LEX_H
#define REGULAR_LEX_H 1

#include <stdio.h>

#include "regex_parse.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int yylex(YYSTYPE* yylvalp, void* yyscanner);
extern void* init_scanner(FILE* src);
extern void fini_scanner(void* yyscanner);

#ifdef __cplusplus
}
#endif

#endif
