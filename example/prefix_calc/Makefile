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

TARGET:=prefix_calc

.SUFFIXES:
.SECONDARY:
.PHONY: all clean

all: $(TARGET)


$(TARGET): $(OBJ)/pca_parse.o $(OBJ)/pca_lex.o
	$(CC) $(LDFLAGS) $(RELEASE_FLAGS) $^ -lm -o $@

$(OBJ)/pca_parse.o: $(GEN)/pca_parse.c $(GEN)/pca_lex.c | $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(OBJ)/pca_lex.o: $(GEN)/pca_lex.c $(GEN)/pca_parse.c | $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(GEN)/pca_parse.c: $(SRC)/pca.cfg | $(GEN)
	../../parse_gen $(SRC)/pca.cfg $(GEN)/pca_parse.c $(GEN)/pca_parse.h

$(GEN)/pca_lex.c: $(SRC)/pca.l | $(GEN)
	flex -F -o $(GEN)/pca_lex.c --header-file=$(GEN)/pca_lex.h $(SRC)/pca.l

clean:
	rm -rf $(TARGET) $(GEN) $(OBJ)

$(GEN):
	@mkdir -p $@

$(OBJ):
	@mkdir -p $@
