CFLAGS = -Wall -Werror
CXXFLAGS = -Wall -Werror

all: libhelpers.so

debug: CFLAGS += -DDEBUG -g
debug: CXXFLAGS += -DDEBUG -g
debug: libhelpers.so

clean:
	rm -f *.o
	rm -f *.so
	rm -f main

libhelpers.so: helpers.o
	$(CC) -shared -o libhelpers.so helpers.o

helpers.o: helpers.c
	$(CC) $(CFLAGS) -c -fpic helpers.c

main: libhelpers.so
	$(CC) $(CFLAGS) -o main main.c -L. -Wl,-rpath=. -lhelpers
