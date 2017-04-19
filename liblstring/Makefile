#
# Makefile
#
# liblstring - Length-based string library
# Copyright (c) 2017 Ammon Smith
#
# liblstring is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version
# 2 of the License, or (at your option) any later version.
#
# liblstring is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General
# Public License along with liblstring.
# If not, see <http://www.gnu.org/licenses/>.
#

.PHONY: all debug install clean

SOURCES := $(wildcard *.c)
PIC_OBJ := $(SOURCES:.c=.pic.o)
STA_OBJ := $(SOURCES:.c=.o)
DEPENDS := $(SOURCES:.c=.d)

ifeq ($(shell uname),Darwin)
SO_EXT  := dylib
else
SO_EXT  := so
endif

AR_DEST := liblstring.a
SO_DEST := liblstring.$(SO_EXT)

FLAGS   := -ansi -pipe -Wall -Wextra -pedantic -I.

all: CFLAGS+=-O2
all: $(AR_DEST) $(SO_DEST)

debug: CFLAGS+=-g
debug: $(LIBRARY)

$(AR_DEST): $(OBJECTS)
	ar rcs $@ $^

$(SO_DEST): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) -shared -o $@ $<

%.o: %.c
	$(CC) $(FLAGS) $(CFLAGS) -c -o $@ $<

%.pic.o: %.c
	$(CC) $(FLAGS) $(CFLAGS) -fpic -c -o $@ $<

%.d: %.c
	$(CC) $(FLAGS) -MM $< > $@

install:
	mkdir -p '$(ROOT)/usr/lib' '$(ROOT)/usr/include/ais'
	install -m644 $(LIBRARY) '$(ROOT)/usr/lib/$(LIBRARY)'
	for include in include/*.h; do \
		install -m644 "$${include}" "$(ROOT)/usr/include/ais/$${include}"; \
	done

clean:
	rm -f $(LIBRARY) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)
