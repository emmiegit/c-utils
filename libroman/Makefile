.POSIX:

V         ?= 0

ECHO_CC_0  = @echo '[CC]  $@'; $(CC)
ECHO_CC_1  = $(CC)
ECHO_CC    = $(ECHO_CC_$(V))

ECHO_LD_0  = @echo '[LD]  $@'; $(CC)
ECHO_LD_1  = $(CC)
ECHO_LD    = $(ECHO_LD_$(V))

ECHO_AR_0  = @echo '[AR]  $@'; $(AR)
ECHO_AR_1  = $(AR)
ECHO_AR    = $(ECHO_AR_$(V))

ECHO_DEP_0 = @echo '[DEP] $@'; $(CC)
ECHO_DEP_1 = $(CC)
ECHO_DEP   = $(ECHO_DEP_$(V))

EXE_SRCS  := main.c
EXE_OBJS  := $(EXE_SRCS:.c=.o)
EXE_DEPS  := $(EXE_SRCS:.c=.d)

SOURCES   := $(filter-out $(EXE_SRCS),$(wildcard *.c))
OBJECTS   := $(SOURCES:.c=.o)
DEPENDS   := $(SOURCES:.c=.d)

FLAGS     := \
	-pipe -ansi \
	-pedantic -Wall -Wextra -Wshadow -Wmissing-prototypes \
	-I. -lc

EXE       := roman
LIB       := libroman.a

.PHONY: all debug clean

all: CFLAGS += -O2
all: $(EXE) $(LIB)

debug: CFLAGS += -g
debug: $(EXE) $(LIB)

clean:
	rm -f $(EXE) $(LIB) $(OBJECTS) $(DEPENDS)

%.o: %.c
	$(ECHO_CC) $(FLAGS) $(CFLAGS) -c -o $@ $<

%.d: %.c
	$(ECHO_DEP) $(FLAGS) -MM $< > $@

$(EXE): $(OBJECTS) $(EXE_OBJS)
	$(ECHO_LD) $(FLAGS) $(CFLAGS) -o $@ $^

$(LIB): $(OBJECTS)
	$(ECHO_AR) rcs $@ $^

-include $(DEPENDS) $(EXE_DEPS)
