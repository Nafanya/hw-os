#include "helpers.h"

const size_t BUF_SIZE = 1024;

int main() {
    char buf[BUF_SIZE];

    while (1) {
	ssize_t bytes_read = read_(STDIN_FILENO, buf, BUF_SIZE);
	if (bytes_read == 0) {
	    break;
	}
	write_(STDOUT_FILENO, buf, bytes_read);
    }
    return 0;
}
