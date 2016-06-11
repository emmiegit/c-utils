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

bin/%: %.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) $< -o $(@F)
	@mv $(@F) -t bin

debug:
	@make EXTRA_FLAGS='-g -Og'

force: clean all

forcedebug: clean debug

clean:
	@echo '[RMDIR] bin'
	@rm -rf bin

