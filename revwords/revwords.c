#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "helpers.h"

const size_t BUF_SIZE = 4096 * 2;
const char DELIMITER = ' ';

int main() {
    char buf[BUF_SIZE];
    while (1) {
	ssize_t bytes_read = read_until(STDIN_FILENO, buf, BUF_SIZE, DELIMITER);
	if (bytes_read == -1) {
	    perror("Error while read");
	    return EXIT_FAILURE;
	} else if (bytes_read == 0) {
	    break;
	}
	int end = bytes_read;
	if (buf[end - 1] == DELIMITER) {
	    end--;
	}
	int i;
	for (i = 0; i < end / 2; i++) {
	    char t = buf[i];
	    buf[i] = buf[end - 1 - i];
	    buf[end - 1 - i] = t;
	}
	ssize_t wr = write_(STDOUT_FILENO, buf, (size_t) bytes_read);
	if (wr == -1) {
	    perror("Error while write");
	    return -1;
	}
    }
    return 0;
}
