#include <stdlib.h>
#include <string.h>

#include "bufio.h"

struct buf_t *buf_new(size_t capacity) {
	buf_t * buf = (buf_t *) malloc(sizeof(buf_t));
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
	size_t total_read = 0;
	while (buf->size < required) {
		ssize_t rd = read(fd, buf->data + total_read, buf->capacity - total_read);
		if (rd == -1) {
			return -1;
		} else if (rd == 0) {
			break;
		}
		total_read += rd;
		buf->size = total_read;
	}
	return total_read;
}

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required) {
	size_t prev_size = buf->size;
	while (buf->size > 0 && prev_size - buf->size < required) {
		ssize_t wr = write(fd, buf->data, buf->size);
		if (wr == -1) {
			return -1;
		} else if (wr == 0) {
			break;
		}
		buf->size -= wr;
		memmove(buf->data, buf->data + wr, buf->size);
	}
	return prev_size - buf->size;
}