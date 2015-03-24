CFLAGS = -Wall -Werror
DEPS_DIR = ../lib

all: libhelpers.so filter

debug: CFLAGS += -g
debug: all

clean:
	rm -f filter

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<

filter: libhelpers.so
	$(CC) $(CFLAGS) -o $@ $@.c -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers
