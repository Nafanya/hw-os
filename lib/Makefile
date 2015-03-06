CC = gcc

all: libhelpers.so main

clean:
	rm -f *.o
	rm -f *.so
	rm -f main

libhelpers.so: helpers.o
	$(CC) -shared -o libhelpers.so helpers.o
	
helpers.o: helpers.c
	$(CC) -c -Wall -Werror -fpic helpers.c

main: libhelpers.so
	$(CC) -Wall -o main main.c -L. -Wl,-rpath=. -lhelpers

	


