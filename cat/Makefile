CC = gcc
CC_FLAGS = -Wall -Werror
DEPS_DIR = ../lib

all: libhelpers.so cat

clean:
#	$(MAKE) -C $(DEPS_DIR) clean
	rm -f cat

libhelpers.so: $(DEPS_DIR)
	$(MAKE) -C $<
	
cat: libhelpers.so
	$(CC) $(CC_FLAGS) -o $@ $@.c -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lhelpers
