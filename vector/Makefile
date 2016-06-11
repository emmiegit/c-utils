.PHONY: all debug force forcedebug clean

CC = gcc
FLAGS = -ansi -Wall -Wextra -pipe -O3 -fPIC
LINK_FLAGS = -shared

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,bin/%.o,$(SOURCES))
TARGET = bin/aisvector.so

all: bin $(TARGET)

bin:
	@echo '[MKDIR] bin'
	@mkdir -p bin

bin/%.o: %.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -c -o $(@F) $<
	@mv $(@F) -t bin

$(TARGET): $(OBJECTS)
	@echo '[LN] $(@F)'
	@$(CC) $(FLAGS) $(LINK_FLAGS) $(EXTRA_FLAGS) -o $(@F) $^
	@mv $(@F) -t bin

debug:
	@make EXTRA_FLAGS='-g -Og'

force: clean all

forcedebug: clean debug

clean:
	@echo '[RMDIR] bin'
	@rm -rf bin

