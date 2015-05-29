CFLAGS = -Wall -std=c99 -D_POSIX_SOURCE 
CXXFLAGS = -Wall

all: libhelpers.so libbufio.so

debug: CFLAGS += -DDEBUG -g
debug: CXXFLAGS += -DDEBUG -g
debug: libhelpers.so libbufio.so

clean:
	rm -f *.o
	rm -f *.so

libbufio.so: bufio.o
	$(CC) -shared -o libbufio.so bufio.o

libhelpers.so: helpers.o
	$(CC) -shared -o libhelpers.so helpers.o

helpers.o: helpers.c
	$(CC) $(CFLAGS) -c -fpic helpers.c

bufio.o: bufio.c
	$(CC) $(CFLAGS) -c -fpic bufio.c

