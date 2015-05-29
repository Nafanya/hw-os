CFLAGS = -Wall -Werror -std=c99
DEPS_DIR = ../lib

all: libhelpers.so simplesh

debug: CFLAGS += -g
debug: all

clean:
	rm -f simplesh

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<

simplesh: libhelpers.so
	$(CC) $(CFLAGS) -o $@ $@.c -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers
