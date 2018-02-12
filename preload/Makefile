.PHONY: all release debug clean

FLAGS     := -std=c99 -pedantic -Wall -Wextra -Wno-long-long -pipe -O3

ifeq ($(shell uname),Darwin)
SO_EXT    := dylib
else
SO_EXT    := so
endif

SOURCES   := $(wildcard *.c)
TARGETS   := $(patsubst %.c,lib/%.$(SO_EXT),$(SOURCES))

all: lib $(TARGETS)

release: lib
	@make EXTRA_FLAGS='$(REL_FLAGS)' $(TARGETS)

lib:
	@echo '[MD] lib'
	@mkdir -p lib

lib/%.$(SO_EXT): %.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) -shared -o $@ $<

debug:
	@make EXTRA_FLAGS='-g -Og'

clean:
	@echo '[RMDIR] lib'
	@rm -rf lib

