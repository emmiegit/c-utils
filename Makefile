.PHONY: all release debug clean

CC = gcc
FLAGS = -ansi -Wall -Wextra -pipe -O3
RELEASE_FLAGS = -fstack-protector-strong

SOURCES = $(wildcard *.c)
TARGETS = $(patsubst %.c,bin/%,$(SOURCES))

all: bin $(TARGETS)

release: bin
	@make EXTRA_FLAGS='$(RELEASE_FLAGS)' $(TARGETS)

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

bin/%: %.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -o $@ $<

debug:
	@make EXTRA_FLAGS='-g -Og'

clean:
	@echo '[RMDIR] bin'
	@rm -rf bin

