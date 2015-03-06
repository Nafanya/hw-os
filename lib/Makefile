CC = gcc
CC_FLAGS = -Wall -Werror

all: libhelpers.so main

clean:
	rm -f *.o
	rm -f *.so
	rm -f main

libhelpers.so: helpers.o
	$(CC) -shared -o libhelpers.so helpers.o
	
helpers.o: helpers.c
	$(CC) $(CC_FLAGS) -c -fpic helpers.c

main: libhelpers.so
	$(CC) $(CC_FLAGS) -o main main.c -L. -Wl,-rpath=. -lhelpers
