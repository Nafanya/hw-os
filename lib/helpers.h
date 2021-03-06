#ifndef __HELPERS_H_
#define __HELPERS_H_

#include <unistd.h>
#include <stdlib.h>

ssize_t read_(int fd, void *buf, size_t count);

ssize_t write_(int fd, const void *buf, size_t count);

ssize_t read_until(int fd, void * buf, size_t count, char delimiter);

int spawn(const char * file, char * const argv []);

typedef struct execargs_t {
  char* prog;
  char** args;
} execargs_t;

int exec(execargs_t* args);

int runpiped(execargs_t** programs, size_t n);

#endif // __HELPERS_H_
