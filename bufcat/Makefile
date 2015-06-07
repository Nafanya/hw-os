CFLAGS = -Wall -Werror
DEPS_DIR = ../lib

all: libbufio.so bufcat

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	rm -f bufcat
	rm *.o

libbufio.so: $(DEPS_DIR)
	$(MAKE) -C $<

bufcat: bufcat.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(DEPS_DIR) -L$(DEPS_DIR) -Wl,-rpath=$(DEPS_DIR) -lbufio

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(DEPS_DIR)

.PHONY: clean
