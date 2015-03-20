#include "stdio.h"
#include "errno.h"

#include "helpers.h"

ssize_t read_(int fd, void *buf, size_t count) {
    size_t bytes_read = 0;
    while (1) {
	ssize_t rd = read(fd, (char *)buf + bytes_read, count - bytes_read);
	if (rd == -1) {
	    return -1;
	} else if (rd == 0) {
	    return bytes_read;
	}
	bytes_read += rd;
    }
}

ssize_t write_(int fd, const void *buf, size_t count) {
    size_t bytes_written = 0;
    while (1) {
        ssize_t wr = write(fd, (char *)buf + bytes_written, count - bytes_written);
        if (wr == -1) {
	    return -1;
	}
	bytes_written += wr;
	if (bytes_written == count) {
	    return count;
        }

    }
}

ssize_t read_until(int fd, void *buf, size_t count, char delimiter) {
    size_t bytes_read = 0;
    while (1) {
	ssize_t rd = read(fd, (char *)buf + bytes_read, count - bytes_read);
	if (rd == -1) {
	    return -1;
	} else if (rd == 0) {
	    return bytes_read;
	}
	int i;
	for (i = 0; i < rd; i++) {
	    if (*((char *)buf + bytes_read + i) == delimiter) {
		return bytes_read + rd;
	    }
	}
	bytes_read += rd;
    }
}
