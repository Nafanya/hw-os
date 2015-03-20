#include "helpers.h"
#include "stdio.h"

const size_t BUF_SIZE = 1024;

int main() {
    char buf[BUF_SIZE];

    while (1) {
	ssize_t bytes_read = read_(STDIN_FILENO, buf, BUF_SIZE);
	if (bytes_read == 0) {
	    break;
	} else if (bytes_read == -1) {
	    perror("Error: ");
	    return EXIT_FAILURE;
	}
	ssize_t wr = write_(STDOUT_FILENO, buf, bytes_read);
	if (wr == -1) {
	    perror("Error: ");
	    return EXIT_FAILURE;
	}
    }
    return 0;
}
