#
# Makefile
#
# libais - Ammon's C library
# Copyright (c) 2016 Ammon Smith
#
# libais is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version
# 2 of the License, or (at your option) any later version.
#
# libais is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General
# Public License along with libais.
# If not, see <http://www.gnu.org/licenses/>.
#

.PHONY: all clean debug

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
DEPENDS := $(SOURCES:.c=.d)
LIBRARY := libais.a

FLAGS   := -ansi -pipe -Wall -Wextra -pedantic -I.

all: CFLAGS+=-O2
all: $(LIBRARY)

debug: CFLAGS+=-g
debug: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(FLAGS) $(CFLAGS) -c -o $@ $<

%.d: %.c
	$(CC) $(FLAGS) -MM $< > $@

clean:
	rm -f $(LIBRARY) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)
