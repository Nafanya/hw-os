CC = gcc
CC_FLAGS = -Wall -Werror
DEPS_DIR = ../lib

all: libhelpers.so revwords

clean:
	rm -f revwords

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<
	
revwords: libhelpers.so
	$(CC) $(CC_FLAGS) -o $@ $@.c -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers
