CFLAGS = -Wall -std=c99
DEPS_DIR = ../lib

all: libhelpers.so simplesh

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	rm -f simplesh
	rm *.o

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<

libbufio.so: $(DEPS_DIR)
	$(MAKE) -C $<

simplesh: simplesh.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers -lbufio

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(DEPS_DIR)

.PHONY: clean
