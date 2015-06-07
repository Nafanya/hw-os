CFLAGS = -Wall -Werror
DEPS_DIR = ../lib

all: libhelpers.so filter

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	rm -f filter
	rm *.o

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<

filter: filter.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(DEPS_DIR)

.PHONY: clean
