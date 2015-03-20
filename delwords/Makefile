CFLAGS = 
DEPS_DIR = ../lib

all: libhelpers.so delwords

clean:
	rm -f revwords

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<

delwords: libhelpers.so
	$(CC) $(CFLAGS) -o $@ $@.c -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers
