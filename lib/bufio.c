#include "bufio.h"

struct buf_t *buf_new(size_t capacity) {
	buf_t buf = (buf_t *) malloc(sizeof(buf_t));
}

void buf_free(struct buf_t *) {

}

size_t buf_capacity(buf_t *) {

}

size_t buf_size(buf_t *) {

}

ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required) {

}

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required) {

}