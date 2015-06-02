CFLAGS = -Wall -std=c99 -D_POSIX_SOURCE
DEPS_DIR = ../lib

all: libhelpers.so libbufio.so filesender

debug: CFLAGS += -g
debug: all

clean:
	rm -f filesender

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<

libbufio.so: $(DEPS_DIR)
	$(MAKE) -C $<

filesender: filesender.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers -lbufio

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(DEPS_DIR)
