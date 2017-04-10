.PHONY: all release debug clean

FLAGS     := -ansi -Wall -Wextra -pipe -O3
REL_FLAGS := -fstack-protector-strong

MORE      := \
	bin/isbe \
	bin/isle

SOURCES   := $(wildcard *.c)
TARGETS   := $(patsubst %.c,bin/%,$(SOURCES)) $(MORE)

ifneq ($(shell uname),Linux)
BLACKLIST := \
	bin/auto-renamer \
	bin/latex-autocompile \
	bin/xfiller
TARGETS   := $(filter-out $(BLACKLIST),$(TARGETS))
endif

all: bin $(TARGETS)

release: bin
	@make EXTRA_FLAGS='$(REL_FLAGS)' $(TARGETS)

bin:
	@echo '[MKDIR] bin'
	@mkdir -p bin

bin/getch: getch.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -lncurses -o $@ $<

bin/i3_lock: i3_lock.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -lX11 -lXrandr -o $@ $<

bin/xfiller: xfiller.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -lX11 -o $@ $<

bin/latex-autocompile: latex-autocompile.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -pthread -o $@ $<

bin/int-values: int-values.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -Wno-shift-count-overflow -Wno-overflow -o $@ $<

bin/isbe bin/isle: bin/endian
	@echo '[LN] $(@F)'
	@ln -sf endian $@

bin/%: %.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -o $@ $<

debug:
	@make EXTRA_FLAGS='-g -Og'

clean:
	@echo '[RMDIR] bin'
	@rm -rf bin

