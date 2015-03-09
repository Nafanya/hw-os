CFLAGS = -Wall -Werror
DEPS_DIR = ../lib

all: libhelpers.so cat

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	$(MAKE) -C $(DEPS_DIR) clean
	rm -f cat

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<

cat: libhelpers.so
	$(CC) $(CFLAGS) -o $@ $@.c -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers
