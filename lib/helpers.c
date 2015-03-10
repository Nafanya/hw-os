#include "helpers.h"
#include "errno.h"

ssize_t read_(int fd, void *buf, size_t count) {
    size_t bytes_read = 0;
    ssize_t rd;
    while (bytes_read < count) {
	rd = read(fd, (char *)buf + bytes_read, count - bytes_read);
	if (rd == -1 || rd == 0) {
	    break;
	} else {
	    bytes_read += rd;
        }
    }
    if (rd == -1) {
	return rd;
    }
    return bytes_read;
}

ssize_t write_(int fd, const void *buf, size_t count) {
    size_t bytes_written = 0;
    ssize_t wr;
    while (bytes_written < count) {
        wr = write(fd, (char *)buf + bytes_written, count - bytes_written);
        if (wr == -1 || wr == 0) {
	    break;
	} else {
	    bytes_written += wr;
        }
    }
    if (wr == -1) {
	return wr;
    }
    return bytes_written;
}

ssize_t read_until(int fd, void *buf, size_t count, char delimiter) {
    return 0;
}
