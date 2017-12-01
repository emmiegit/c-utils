#
# Makefile
#
# latex-autocompile - Automatically recompile LaTeX sources and
#                     refresh the MuPDF viewer
# Copyright (c) 2016-2017 Ammon Smith
#
# latex-autocompile is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# latex-autocompile is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with latex-autocompile.  If not, see <http://www.gnu.org/licenses/>.
#

.SUFFIXES:
.PHONY: all clean debug

V          ?= 0

ECHO_CC_0   = @echo '[CC] $@'; $(CC)
ECHO_CC_1   = $(CC)
ECHO_CC     = $(ECHO_CC_$(V))

ECHO_LD_0   = @echo '[LD] $@'; $(CC)
ECHO_LD_1   = $(CC)
ECHO_LD     = $(ECHO_LD_$(V))

CC_FLAGS   := \
	-ansi -pedantic -Wall -Wextra -Wshadow -Wmissing-prototypes -Wcast-qual \
	-I. -pthread \
	-fPIC \
	-DGIT_HASH='"$(shell git rev-parse --short HEAD)"' -D_XOPEN_SOURCE=500
LD_FLAGS   := \
	-pthread

SOURCES    := $(wildcard *.c)
OBJECTS    := $(SOURCES:.c=.o)
DEPENDS    := $(SOURCES:.c=.d)
EXE        := latex-autocompile

all: CFLAGS += -pipe -O2
all: $(EXE)

debug: CFLAGS += -pipe -g
debug: $(EXE)

$(EXE): $(OBJECTS)
	$(ECHO_LD) $(FLAGS) $(CFLAGS) $(CC_FLAGS) -o $@ $^

%.o: %.c
	$(ECHO_CC) $(FLAGS) $(CFLAGS) $(LD_FLAGS) -c -o $@ $<

clean:
	rm -f $(EXE) *.o *.d *~

