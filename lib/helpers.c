#include "helpers.h"

ssize_t read_(int fd, void *buf, size_t count) {
    void *cur_pos = buf;
    ssize_t bytes_read = 0;
    while (bytes_read < count) {
        ssize_t rd = read(fd, cur_pos, count - bytes_read);
        if (rd == -1) {
            return -1;
        }
        if (rd == 0) {
            return bytes_read;
        } else {
            bytes_read += rd;
            cur_pos += rd;
        }
    }
    return bytes_read;
}

ssize_t write_(int fd, const void *buf, size_t count) {
    void *cur_pos = (void *) buf;
    ssize_t bytes_written = 0;
    while (bytes_written < count) {
        ssize_t wr = write(fd, cur_pos, count - bytes_written);
        if (wr == -1) {
            return -1;
        } else if (wr == 0) {
            return bytes_written;
        } else {
            bytes_written += wr;
            cur_pos += wr;
        }
    }
    return bytes_written;
}
