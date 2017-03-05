#
# Makefile
#
# calc - a simple CLI calculator
# Copyright (c) 2017 Ammon Smith
#
# calc is available free of charge under the terms of the MIT
# License. You are free to redistribute and/or modify it under those
# terms. It is distributed in the hopes that it will be useful, but
# WITHOUT ANY WARRANTY. See the LICENSE file for more details.
#

.SUFFIXES:

V          ?= 0
READLINE   ?= 1

ECHO_CC_0   = @echo '[CC]  $@'; $(CC)
ECHO_CC_1   = $(CC)
ECHO_CC     = $(ECHO_CC_$(V))

ECHO_DEP_0  = @echo '[DEP] $@'; $(CC)
ECHO_DEP_1  = $(CC)
ECHO_DEP    = $(ECHO_DEP_$(V))

ECHO_LD_0   = @echo '[LD]  $@'; $(CC)
ECHO_LD_1   = $(CC)
ECHO_LD     = $(ECHO_LD_$(V))

ECHO_LEX_0  = @echo '[LEX] $@'; $(LEX)
ECHO_LEX_1  = $(LEX)
ECHO_LEX    = $(ECHO_LEX_$(V))

ECHO_YAC_0  = @echo '[YAC] $@'; $(YACC)
ECHO_YAC_1  = $(YACC)
ECHO_YAC    = $(ECHO_YAC_$(V))

ECHO_STR_0  = @echo '[STR] $<'; strip
ECHO_STR_1  = strip
ECHO_STR    = $(ECHO_STR_$(V))

SOURCES    := $(wildcard *.c)
OBJECTS    := $(SOURCES:.c=.o)
DEPENDS    := $(SOURCES:.c=.d)
TARGET     := calc

FLAGS      := -pipe
WARN_FLAGS := -ansi -pedantic -Wall -Wextra -Wshadow -Wmissing-prototypes
DEFN_FLAGS := -DGIT_HASH='"$(shell git rev-parse --short HEAD)"'
COMP_FLAGS := -I.
LINK_FLAGS := -lm
YACC_FLAGS :=
LEX_FLAGS  := -f

ifeq ($(READLINE),1)
DEFN_FLAGS += -DUSE_READLINE
LINK_FLAGS += -lreadline
endif

.PHONY: all
all: COMP_FLAGS = -O3
all: $(TARGET)

.PHONY: debug
debug: CFLAGS = -Og -g
debug: $(TARGET)

.PHONY: release
release: CFLAGS = -O2 -Werror -fstack-protector-strong -DNDEBUG
release: $(TARGET)
	$(ECHO_STR) $<

.PHONY: clean
clean:
	rm -f *.o *.d y.tab.? lex.yy.? $(TARGET)

y.tab.c y.tab.h: calc.y
	$(ECHO_YAC) $(YACC_FLAGS) $(YFLAGS) -d $<

y.tab.o: y.tab.c
	$(ECHO_CC) $(FLAGS) $(CFLAGS) $(COMP_FLAGS) $(DEFN_FLAGS) -c -o $@ $<

lex.yy.c: calc.l y.tab.h
	$(ECHO_LEX) $(FLEX_FLAGS) $(LFLAGS) $<

lex.yy.o: lex.yy.c
	$(ECHO_CC) $(FLAGS) $(CFLAGS) $(COMP_FLAGS) $(DEFN_FLAGS) -c -o $@ $<

%.d: %.c y.tab.h
	$(ECHO_DEP) $(FLAGS) $(CFLAGS) $(COMP_FLAGS) $(DEFN_FLAGS) -MM $< > $@

%.o: %.c
	$(ECHO_CC) $(FLAGS) $(CFLAGS) $(WARN_FLAGS) $(COMP_FLAGS) $(DEFN_FLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS) lex.yy.o y.tab.o
	$(ECHO_LD) $(FLAGS) $(CFLAGS) $(LINK_FLAGS) -o $@ $^

-include $(DEPENDS)
