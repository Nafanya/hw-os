CFLAGS = -Wall -std=gnu99 -D_POSIX_SOURCE
DEPS_DIR = ../lib

all: libhelpers.so simplesh

debug: CFLAGS += -g
debug: all

clean:
	rm -f simplesh

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<

libbufio.so: $(DEPS_DIR)
	$(MAKE) -C $<

simplesh: libhelpers.so libbufio.so
	$(CC) $(CFLAGS) -o $@ $@.c -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers -lbufio
