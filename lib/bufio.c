#include <stdlib.h>
#include <string.h>

#include "bufio.h"

#ifdef DEBUG
#define assert_(p) if (!(p)) abort();
#else
#define assert_(p)
#endif

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
	assert_(buf != NULL);
	free(buf->data);
	free(buf);
}

size_t buf_capacity(buf_t * buf) {
	assert_(buf != NULL);
	return buf->capacity;
}

size_t buf_size(buf_t * buf) {
	assert_(buf != NULL);
	return buf->size;
}

ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required) {
	assert_(buf != NULL);
	assert_(buf->size <= required);

	while (buf->size < required) {
		ssize_t rd = read(fd, buf->data + buf->size, buf->capacity - buf->size);
		if (rd == -1) {
			return -1;
		} else if (rd == 0) {
			break;
		}
		buf->size += rd;
	}
	return buf->size;
}

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required) {
	assert_(buf != NULL);

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

ssize_t buf_find_lf(fd_t fd, buf_t *buf, char *dst) {
	assert_(buf != NULL);

	for (size_t i = 0; i < buf->size; i++) {
		if (buf->data[i] == '\n') {
			memcpy(dst, buf->data, i);
			memmove(buf->data, buf->data + i + 1, buf->size - i - 1);
			buf->size -= i + 1;
			return i;
		}
	}

	return -1;
}

ssize_t buf_getline(fd_t fd, buf_t *buf, char *dst) {
	assert_(buf != NULL);

	ssize_t lf = buf_find_lf(fd, buf, dst);
	if (lf != -1) {
		return lf;
	}
	buf_fill(fd, buf, 1);
	lf = buf_find_lf(fd, buf, dst);
	if (lf != -1) {
		return lf;
	}
	return 0;
}

ssize_t buf_fill_once(fd_t fd, buf_t *buf) {
	assert_(buf != NULL);

	if (buf->size < buf->capacity) {
		ssize_t rd = read(fd, buf->data + buf->size, buf->capacity - buf->size);
		if (rd == -1) {
			return -1;
		}
		buf->size += rd;
	}
	return buf->size;
}