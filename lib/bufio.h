#ifndef __BUFIO_H_
#define __BUFIO_H_

#include <unistd.h>

struct buf_t {
	size_t capacity;
	size_t size;
	char * data;
};
typedef struct buf_t buf_t;
typedef int fd_t;

struct buf_t *buf_new(size_t capacity);

void buf_free(struct buf_t *);

size_t buf_capacity(buf_t *);

size_t buf_size(buf_t *);

ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required);

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required);

#endif // __BUFIO_H_