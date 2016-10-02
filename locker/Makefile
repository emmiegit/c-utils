SOURCES  := $(wildcard *.c)
OBJECTS  := $(SOURCES:.c=.o)
DEPENDS  := $(SOURCES:.c=.d)
TARGET   := i3_lock_prog

FLAGS     = -I. -D_DEFAULT_SOURCE
CC_FLAGS  = -ansi -Wall -Wextra -pedantic -Wshadow -Wmissing-prototypes $(CFLAGS)
LD_FLAGS  = -lX11 $(LDFLAGS)

V         = 0

ECHO_CC_0 = @echo '[CC]  $@'; $(CC)
ECHO_CC_1 = $(CC)
ECHO_CC   = $(ECHO_CC_$(V))

ECHO_LD_0 = @echo '[LD]  $@'; $(CC)
ECHO_LD_1 = $(CC)
ECHO_LD   = $(ECHO_LD_$(V))

ECHO_DEP_0 = @echo '[DEP] $@'; $(CC)
ECHO_DEP_1 = $(CC)
ECHO_DEP   = $(ECHO_DEP_$(V))

.PHONY: all debug clean

all: $(TARGET)
all: CFLAGS += -Os

debug: $(TARGET)
debug: CFLAGS += -Og -g

clean:
	rm -f *.o *.d $(TARGET)

-include $(DEPENDS)

$(TARGET): $(OBJECTS)
	$(ECHO_LD) $(FLAGS) $(LD_FLAGS) -o $@ $^
	@strip $@

%.o: %.c
	$(ECHO_CC) $(FLAGS) $(CC_FLAGS) -c -o $@ $<

%.d: %.c
	$(ECHO_DEP) -MM $< > $@

