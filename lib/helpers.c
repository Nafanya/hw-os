#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

#include "helpers.h"

#ifdef DEBUG
#define DBG(msg) printf("%s AT %s:%d\n", msg, __FILE__, __LINE__);
#else
#define DBG(msg)
#endif

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
  pid_t pid = fork();
  if (pid == -1) {
    return -1;
  } else if (pid == 0) {
    /*
      set child's group same as parent's
      to send SIGINT to all children catching SIGINT in parent
    */
    signal(SIGINT, SIG_DFL);
    execvp(args->prog, args->args);
    exit(EXIT_FAILURE);
  }
  return pid;
}

static struct sigaction prev;
static int this_stdin;
static int this_stdout;
static int* pids;
static int pids_cnt;

void stopall() {
  for (int i = 0; i < pids_cnt; i++) {
    kill(pids[i], SIGTERM);
    waitpid(pids[i], NULL, 0);
  }
}

void sig_handler(int sig) {
  stopall();
}

#define CHK(_res)                               \
  if (_res == -1) {                             \
    stopall();                                  \
    return -1;                                  \
  }

int runpiped(execargs_t** programs, size_t n) {
  struct sigaction sa;
  sa.sa_handler = sig_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  CHK(sigaction(SIGINT, &sa, &prev));

  int pipes[2 * n];
  int children_pids[n];
  pids = children_pids;
  pids_cnt = n;
  this_stdin = dup(STDIN_FILENO);
  this_stdout = dup(STDOUT_FILENO);

  for (int i = 0; i < n - 1; i++) {
    CHK(pipe(pipes + 2 * i));
    int stdin = pipes[i * 2];
    int stdout = pipes[i * 2 + 1];
    // can't make it work with dup3, so call fcntl instead
    CHK(fcntl(stdout, F_SETFD, FD_CLOEXEC));
    CHK(dup2(stdout, STDOUT_FILENO));
    children_pids[i] = exec(programs[i]);
    CHK(children_pids[i]);
    CHK(dup2(stdin, STDIN_FILENO));
  }
  CHK(dup2(this_stdout, STDOUT_FILENO));
  CHK(close(this_stdout));
  children_pids[n - 1] = exec(programs[n - 1]);
  CHK(children_pids[n - 1]);
  wait(NULL);
  return 0;
}
