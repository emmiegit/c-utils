#
# Makefile
#
# directed-graph-set
# Copyright (c) 2016 Ammon Smith
#
# This file is available to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

.PHONY: all debug force forcedebug clean

CC = gcc
FLAGS = -ansi -Wall -Wextra -pipe -O3 -fPIC
LINK_FLAGS = -shared

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,bin/%.o,$(SOURCES))
TARGET = bin/directed-graph-set.so

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

