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

$(TARGET): $(OBJ)/regex_parse.o $(OBJ)/regular_lex.o
	$(CC) $(LDFLAGS) $(RELEASE_FLAGS) $^ -o $@

$(OBJ)/regex_parse.o: $(GEN)/regex_parse.c | $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(OBJ)/regular_lex.o: $(SRC)/regular_lex.c | $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(GEN)/regex_parse.c: $(SRC)/regular.cfg | $(GEN)
	../../parse_gen $(SRC)/regular.cfg $(GEN)/regex_parse.c $(GEN)/regex_parse.h

clean:
	rm -rf $(TARGET) $(GEN) $(OBJ)

$(GEN):
	@mkdir -p $@

$(OBJ):
	@mkdir -p $@
