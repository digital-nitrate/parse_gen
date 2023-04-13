MAKEFLAGS += --no-builtin-rules

SPACE:=$(subst ,, )

SRC:=src
OBJ:=obj
GEN:=gen

CC:=clang
CFLAGS:=-Werror -Wall -Wextra -std=gnu17 -I$(SRC) -I$(GEN) -fPIC -flto -Ofast $(D)
LDFLAGS:=-flto -fPIE -Ofast
RELEASE_FLAGS:=-g0 -fno-asynchronous-unwind-tables -fno-unwind-tables
DEBUG_FLAGS:=-fsanitize=address -fsanitize=undefined -g

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

L_FILE:=$(strip $(call rwildcard,$(SRC),*.l))
LC_FILE:=$(patsubst $(SRC)/%.l,$(GEN)/%.yy.c,$(L_FILE))
LH_FILE:=$(patsubst $(SRC)/%.l,$(GEN)/%.yy.h,$(L_FILE))

Y_FILE:=$(strip $(call rwildcard,$(SRC),*.y))
YC_FILE:=$(patsubst $(SRC)/%.y,$(GEN)/%.tab.c,$(Y_FILE))
YH_FILE:=$(patsubst $(SRC)/%.y,$(GEN)/%.tab.h,$(Y_FILE))

C_FILE:=$(strip $(call rwildcard,$(SRC),*.c))

O_FILE:=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(C_FILE)) $(patsubst $(GEN)/%.tab.c,$(OBJ)/$(GEN)/%.tab.o,$(YC_FILE)) $(patsubst $(GEN)/%.yy.c,$(OBJ)/$(GEN)/%.yy.o,$(LC_FILE))
O_FILE_D:=$(patsubst %.o,%.debug.o,$(O_FILE))

TARGET:=parse_gen

ifeq ($(V),)
cmd=@$(call cmd_$(1)_name,$(2),$(3),$(4)); $(call cmd_$(1),$(2),$(3),$(4))
else
cmd=$(call cmd_$(1),$(2),$(3),$(4))
endif

cmd_rm_name=printf '  %-6s %s\n' "RM" "$(1)"
cmd_rm=rm -rf $(1)

cmd_cc_rl_name=printf '  %-6s %s -> %s\n' "CC" "$(1)" "$(2)"
cmd_cc_rl=mkdir -p $(dir $(2)); $(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $(1) -o $(2)

cmd_ld_rl_name=printf '  %-6s %s -> %s\n' "LD" "$(1)" "$(2)"
cmd_ld_rl=$(CC) $(LDFLAGS) $(RELEASE_FLAGS) $(1) -o $(2)

cmd_cc_db_name=printf '  %-6s %s -> %s\n' "CC" "$(1)" "$(2)"
cmd_cc_db=mkdir -p $(dir $(2)); $(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $(1) -o $(2)

cmd_ld_db_name=printf '  %-6s %s -> %s\n' "LD" "$(1)" "$(2)"
cmd_ld_db=$(CC) $(LDFLAGS) $(DEBUG_FLAGS) $(1) -o $(2)

cmd_bs_name=printf '  %-6s %s -> %s %s\n' "BISON" "$(1)" "$(2)" "$(3)"
cmd_bs=bison -Wall -Werror -L C $(1) -o $(2) --defines=$(3)

cmd_fl_name=printf '  %-6s %s -> %s %s\n' "FLEX" "$(1)" "$(2)" "$(3)"
cmd_fl=flex -F -o $(2) --header-file=$(3) $(1)

.SUFFIXES:
.SECONDARY:
.PHONY: all debug clean

all: $(TARGET)

debug: $(TARGET).debug

clean:
	$(call cmd,rm,$(TARGET) $(TARGET).debug $(OBJ) $(GEN))

$(TARGET): $(O_FILE)
	$(call cmd,ld_rl,$^,$@)

$(OBJ)/$(GEN)/%.o: $(GEN)/%.c $(YC_FILE) $(LC_FILE) | $(OBJ)
	$(call cmd,cc_rl,$<,$@)

$(OBJ)/%.o: $(SRC)/%.c $(YC_FILE) $(LC_FILE) | $(OBJ)
	$(call cmd,cc_rl,$<,$@)

$(GEN)/%.tab.c: $(SRC)/%.y | $(GEN)
	$(call cmd,bs,$<,$(GEN)/$*.tab.c,$(GEN)/$*.tab.h)

$(GEN)/%.yy.c: $(SRC)/%.l | $(GEN)
	$(call cmd,fl,$<,$(GEN)/$*.yy.c,$(GEN)/$*.yy.h)

$(TARGET).debug: $(O_FILE_D)
	$(call cmd,ld_db,$^,$@)

$(OBJ)/$(GEN)/%.debug.o: $(GEN)/%.c $(YC_FILE) $(LC_FILE) | $(OBJ)
	$(call cmd,cc_db,$<,$@)

$(OBJ)/%.debug.o: $(SRC)/%.c $(YC_FILE) $(LC_FILE) | $(OBJ)
	$(call cmd,cc_db,$<,$@)

$(OBJ):
	@mkdir -p $@

$(GEN):
	@mkdir -p $@
