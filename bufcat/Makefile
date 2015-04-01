CFLAGS = -Wall -Werror
DEPS_DIR = ../lib

all: libbufcat.so bufcat

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	$(MAKE) -C $(DEPS_DIR) clean
	rm -f bufcat

libbufcat.so: $(DEPS_DIR)
	$(MAKE) -C $<

bufcat: libbufcat.so bufcat.c
	$(CC) $(CFLAGS) -o $@ $@.c -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lbufio