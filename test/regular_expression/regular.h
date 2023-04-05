#ifndef REGULAR_H
#define REFULAR_H 1

enum reg_type {
	REG_LAMBDA,
	REG_CHAR,
	REG_RANGE,
	REG_DOT,
	REG_KLEENE,
	REG_PLUS,
	REG_SEQ,
	REG_ALT,
};

typedef struct reg {
	enum reg_type type;
	union {
		char c;
		struct {char c1; char c2;};
		struct reg* kleene;
		struct reg* plus;
		struct reg_link* seq;
		struct reg_link* alt;
	};
} reg;

struct reg_link {
	struct reg_link* next;
	reg sub;
};

enum atommod_type {
	MOD_NONE,
	MOD_PLUS,
	MOD_KLEENE,
};

enum charrng_type {
	RNG_NONE,
	RNG_TOP,
};

struct charrng {
	enum charrng_type type;
	char top;
};

#endif
