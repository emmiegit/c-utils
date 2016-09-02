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

GITHASH := $(shell git rev-parse --short @)
FLAGS   := -ansi -pipe -Wall -Wextra -pedantic -O2 -I. -DGIT_HASH='"$(GITHASH)"'

CC      := gcc
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
DEPENDS := $(SOURCES:.c=.d)
EXE     := rmln

all: $(EXE)
debug:
	$(MAKE) $(EXE) CFLAGS='-g'

$(EXE): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(FLAGS) $(CFLAGS) -c -o $@ $<

%.d: %.c
	$(CC) $(FLAGS) -M $< > $@

-include: $(DEPENDS)
