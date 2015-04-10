CFLAGS = -Wall -Werror -std=c99
DEPS_DIR = ../lib

all: libs foreach

debug: CFLAGS += -g
debug: all

clean:
	rm -f foreach

libs: $(DEPS_DIR)
	$(MAKE) -C $<

foreach: libs
	$(CC) $(CFLAGS) -o $@ $@.c -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lbufio -lhelpers
