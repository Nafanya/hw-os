#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "helpers.h"

ssize_t read_(int fd, void *buf, size_t count) {
    size_t bytes_read = 0;
    while (1) {
        ssize_t rd = read(fd, (char *)buf + bytes_read, count - bytes_read);
        if (rd == -1) {
            return -1;
        } else if (rd == 0) {
            return bytes_read;
        }
        bytes_read += rd;
    }
}

ssize_t write_(int fd, const void *buf, size_t count) {
    size_t bytes_written = 0;
    while (1) {
        ssize_t wr = write(fd, (char *)buf + bytes_written, count - bytes_written);
        if (wr == -1) {
            return -1;
        }
        bytes_written += wr;
        if (bytes_written == count) {
            return count;
        }
    }
}

ssize_t read_until(int fd, void *buf, size_t count, char delimiter) {
    size_t bytes_read = 0;
    while (1) {
        ssize_t rd = read_(fd, (char *)buf + bytes_read, 1);
        if (rd == -1) {
            return -1;
        } else if (rd == 0) {
            return bytes_read;
        }
        bytes_read++;
        char c = *((char *) buf + bytes_read - 1);
        if (c == delimiter) {
            return bytes_read;
        }
    }
}

int spawn(const char * file, char * const argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        return -1;
    }
    if (pid != 0) {
        int status;
        do {
            if (waitpid(pid, &status, 0) == -1) {
                return -1;
            }
        } while (!WIFEXITED(status));
        return status;
        //int status;
        //wait(&status);
        //return status;
    } else {
        int nullfd = open("/dev/null", O_WRONLY);
        if (nullfd == -1) {
            return -1;
        }
        if (dup2(nullfd, STDOUT_FILENO) == -1 || dup2(nullfd, STDERR_FILENO) == -1) {
            return -1;
        }
        if (close(nullfd) == -1) {
            return -1;
        }
        return execvp(file, argv);
    }
}

int exec(execargs_t* args) {
  return 0;
}

int runpiped(execargs_t** programs, size_t n) {
  return 0;
}
