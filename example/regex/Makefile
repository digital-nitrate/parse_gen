MAKEFLAGS += --no-builtin-rules

SRC:=src
OBJ:=obj
GEN:=gen

ifeq ($(origin CC), undefined)
CC:=clang
endif

CFLAGS:=-Werror -Wall -Wextra -std=gnu99 -I$(SRC) -I$(GEN) -fPIC -flto -Ofast $(D)
LDFLAGS:=-flto -fPIE -Ofast
RELEASE_FLAGS:=-g0 -fno-asynchronous-unwind-tables -fno-unwind-tables

TARGET:=regex

.SUFFIXES:
.SECONDARY:
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)/regex_parse.o $(OBJ)/regex_lex.o
	$(CC) $(LDFLAGS) $(RELEASE_FLAGS) $^ -o $@

$(OBJ)/regex_parse.o: $(GEN)/regex_parse.c $(GEN)/regex_lex.c | $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(OBJ)/regex_lex.o: $(GEN)/regex_lex.c $(GEN)/regex_parse.c | $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(GEN)/regex_parse.c: $(SRC)/regular.cfg | $(GEN)
	../../parse_gen $(SRC)/regular.cfg $(GEN)/regex_parse.c $(GEN)/regex_parse.h

$(GEN)/regex_lex.c: $(SRC)/regular.l | $(GEN)
	flex -F -o $(GEN)/regex_lex.c --header-file=$(GEN)/regex_lex.h $(SRC)/regular.l

clean:
	rm -rf $(TARGET) $(GEN) $(OBJ)

$(GEN):
	@mkdir -p $@

$(OBJ):
	@mkdir -p $@
