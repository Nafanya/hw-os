#include "helpers.h"
#include "errno.h"

ssize_t read_(int fd, void *buf, size_t count) {
    ssize_t bytes_read = 0;
    while (bytes_read < count) {
        ssize_t rd = read(fd, buf + bytes_read, count - bytes_read);
        if (rd == -1) {
	    if (errno != EAGAIN && errno != EWOULDBLOCK) {
		break;
	    }
        } else if (rd == 0) {
	    break;
        } else {
            bytes_read += rd;
        }
    }
    return bytes_read;
}

ssize_t write_(int fd, const void *buf, size_t count) {
    ssize_t bytes_written = 0;
    while (bytes_written < count) {
        ssize_t wr = write(fd, buf + bytes_written, count - bytes_written);
        if (wr == -1) {
	    if (errno != EAGAIN && errno != EWOULDBLOCK) {
		break;
	    }
        } else if (wr == 0) {
	    break;
        } else {
            bytes_written += wr;
        }
    }
    return bytes_written;
}

ssize_t read_until(int fd, void *buf, size_t count, char delimiter) {
    return 0;
}
