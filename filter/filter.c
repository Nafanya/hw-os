#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

const size_t BUF_SIZE = 4096;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    char buf[BUF_SIZE];

    char ** args = malloc((argc + 1) * sizeof (char*));
    if (args == NULL) {
        return EXIT_FAILURE;
    }
    int i;
    for (i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }
    args[argc - 1] = buf;
    args[argc] = NULL;
    char * cmd = argv[0];

    while (1) {
        ssize_t rd = read_until(STDIN_FILENO, buf, BUF_SIZE, '\n');
        if (rd == -1) {
            free(args);
            return EXIT_FAILURE;
        } else if (rd == 0) {
            free(args);
            return 0;
        }
        if (buf[rd - 1] == '\n') {
            buf[rd - 1] = '\0';
        }
        if (spawn(cmd, args) == 0) {
            printf("%s\n", buf);
        } else {
            printf("Failed cmd: %s\n", buf);
        }
    }
    return 0;
}