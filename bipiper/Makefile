CFLAGS = -Wall -std=c99 -D_POSIX_SOURCE
DEPS_DIR = ../lib

all: libhelpers.so libbufio.so forking

debug: CFLAGS += -g
debug: all

clean:
	rm -f forking
	rm -f *.o

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<

libbufio.so: $(DEPS_DIR)
	$(MAKE) -C $<

forking: forking.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers -lbufio

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(DEPS_DIR)

.PHONY: clean
