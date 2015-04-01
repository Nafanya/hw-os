#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bufio.h"

const size_t BUF_SIZE = 4096;

int main() {
	buf_t * buf = buf_new(BUF_SIZE);
	if (buf == NULL) {
		perror("Can't allocate buf_t");
		return EXIT_FAILURE;
	}

	while (1) {
		ssize_t rd = buf_fill(STDIN_FILENO, buf, 1);
		if (rd == -1) {
			free(buf);
			perror("Error while reading");
			return EXIT_FAILURE;
		} else if (rd == 0) {
			break;
		}
		size_t size = buf_size(buf);
		ssize_t wr = buf_flush(STDOUT_FILENO, buf, size);
		if (wr == -1) {
			free(buf);
			perror("Error while writing");
			return EXIT_FAILURE;
		} else if (wr < size) {
			break;
		}
	}
	free(buf);
	return EXIT_SUCCESS;
}
