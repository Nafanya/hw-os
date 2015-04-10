#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "bufio.h"

const int BUF_SIZE = 4096;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		return EXIT_FAILURE;
	}

	char line[BUF_SIZE];
	char ** args = malloc((argc + 1) * sizeof(char*));
	if (args == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < argc - 1; i++) {
		args[i] = argv[i + 1];
	}
	args[argc - 1] = line;
	args[argc] = NULL;

	const char* file = args[0];

	buf_t *buf = buf_new(BUF_SIZE);
	while (1) {
		ssize_t rd = buf_getline(STDIN_FILENO, buf, line);
		if (rd > 0) {
			if (rd % 2 == 0) {
				if (line[rd - 1] == '\n') {
					line[rd - 1] = '\0';
				} else {
					line[rd] = '\0';
				}
				if (spawn(file, args) == 0) {
					write_(STDOUT_FILENO, line, rd);
				}
			}
		} else {
			free(args);
			if (rd == -1) {
				return EXIT_FAILURE;
			}
			return EXIT_SUCCESS;
		}
	}
	free(args);

	return EXIT_SUCCESS;
}