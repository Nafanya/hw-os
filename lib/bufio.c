#include "bufio.h"

struct buf_t *buf_new(size_t capacity) {
	buf_t buf = (buf_t *) malloc(sizeof(buf_t));
	if (buf == NULL) {
		return NULL;
	}
	buf->capacity = capacity;
	buf->size = 0;
	buf->data = (char *) malloc(capacity * sizeof(char));
	if (buf->data == NULL) {
		free(buf);
		return NULL;
	}
	return buf;
}

void buf_free(buf_t * buf) {
	free(buf->data);
	free(buf);
}

size_t buf_capacity(buf_t * buf) {
	return buf->capacity;
}

size_t buf_size(buf_t * buf) {
	return buf->size;
}

ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required) {
	
}

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required) {

}