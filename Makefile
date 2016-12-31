.PHONY: all release debug force forcedebug clean

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

bin/cgetch: cgetch.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -lncurses -o bin/$(@F) $<

bin/i3_lock: i3_lock.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -lX11 -lXrandr -o bin/$(@F) $<

bin/xfiller: xfiller.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -lX11 -o bin/$(@F) $<

bin/%: %.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) $< -o bin/$(@F)

debug:
	@make EXTRA_FLAGS='-g -Og'

force: clean all

forcedebug: clean debug

clean:
	@echo '[RMDIR] bin'
	@rm -rf bin

