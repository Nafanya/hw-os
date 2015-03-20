#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "helpers.h"

const size_t BUF_SIZE = 4096;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
    char buf[BUF_SIZE];
    char outbuf[BUF_SIZE];
    char* pattern = argv[1];
    size_t pattern_len = strlen(pattern);
    size_t rest = 0;
    size_t out = 0;

    while (1) {
        ssize_t rd = read_(STDIN_FILENO, buf + rest, BUF_SIZE - rest);
        if (rd == -1) {
            return EXIT_FAILURE;
        } else if (rd == 0) {
            break;
        }
        size_t i;
        size_t count = rest + rd;
        size_t pattern_index = 0;
        out = 0;
        for (i = 0; i < count - pattern_len; i++) {
            int flag = 1;
            size_t j;
            for (j = 0; j < pattern_len; j++) {
                if (pattern[j] != buf[i + j]) {
                    flag = 0;
                }
            }
            if (!flag) {
                outbuf[out++] = buf[i];
            } else {
                i += pattern_len - 1;
            }
        }
        for (; i < count; i++) {
            int flag = 1;
            size_t j;
            for (j = 0; j < count - i; j++) {
                if (pattern[j] != buf[i + j]) {
                    flag = 0;
                }
            }
            if (!flag) {
                outbuf[out++] = buf[i];
            } else {
                break;
            }
        }
        rest = 0;
        for (; i < count; i++) {
            buf[rest++] = buf[i];
        }
        ssize_t wr = write_(STDOUT_FILENO, outbuf, out);
        if (wr == -1) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
