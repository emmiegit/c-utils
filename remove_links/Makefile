# Makefile
#
# rmln - Only remove links.
# Copyright (c) 2016 Ammon Smith
#
# rmln is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# rmln is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with rmln.  If not, see <http://www.gnu.org/licenses/>.
#

.PHONY: all clean debug

GITHASH   := $(shell git rev-parse --short @)
FLAGS     := \
	-pipe \
	-ansi -pedantic -Wall -Wextra -Wshadow -Wmissing-prototypes \
	-I. \
	-DGIT_HASH='"$(GITHASH)"'

V         ?= 0

ECHO_CC_0  = @echo '[CC] $@'; $(CC)
ECHO_CC_1  = $(CC)
ECHO_CC    = $(ECHO_CC_$(V))

ECHO_LD_0  = @echo '[LD] $@'; $(CC)
ECHO_LD_1  = $(CC)
ECHO_LD    = $(ECHO_LD_$(V))

SOURCES   := $(wildcard *.c)
OBJECTS   := $(SOURCES:.c=.o)
DEPENDS   := $(SOURCES:.c=.d)
EXE       := rmln

all: CFLAGS += -O2
all: $(EXE)

debug: CFLAGS += -g
debug: $(EXE)

$(EXE): $(OBJECTS)
	$(ECHO_LD) $(FLAGS) $(CFLAGS) -o $@ $^

%.o: %.c
	$(ECHO_CC) $(FLAGS) $(CFLAGS) -c -o $@ $<

