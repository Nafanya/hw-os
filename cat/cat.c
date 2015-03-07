#include "helpers.h"

const size_t BUF_SIZE = 1024;

int main() {
    char buf[BUF_SIZE];
    
    ssize_t bytes_read, bytes_written, off;
    
    while ((bytes_read = read(STDIN_FILENO, buf, BUF_SIZE)) != -1 && bytes_read != 0) {
		for (off = 0; bytes_read; bytes_read -= bytes_written, off += bytes_written) {
			if ((bytes_written = write(STDOUT_FILENO, buf + off, (size_t)bytes_read)) == 0) {
                return 0;
            } else if (bytes_written == -1) {
                return 0;
            }
        }
    }
	if (bytes_read < 0) {
        return 1;
    }
    return 0;
}
