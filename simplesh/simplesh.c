#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"

const int MAX_SIZE = 8192;

void test() {
  char* argv1[] = {"find", "/", NULL};
  char* argv2[] = {"grep", "\\.c$", NULL};
  char* argv3[] = {"head", "-n", "30000", NULL};

  execargs_t prog1 = {"find", argv1};
  execargs_t prog2 = {"grep", argv2};
  execargs_t prog3 = {"head", argv3};

  execargs_t* progs[3] = {
    &prog1, &prog2, &prog3
  };

  runpiped(progs, 3);
  printf("DONE!\n");
}

int main(int argc, char *argv[]) {
  char buf[MAX_SIZE];
  size_t has = 0;
  ssize_t rd;
  while (1) {
    if (rd = write_(STDOUT_FILENO, "$", 1) == -1) {
      return EXIT_FAILURE;
    }
    if ((rd = read_(STDIN_FILENO, buf, buf + has, MAX_SIZE - has)) == -1) {
      return EXIT_FAILURE;
    }
    int pipes = 1;
    int lf = -1;
    for (int i = 0; i < has + rd; i++) {
      if (buf[i] == '|') {
        pipes++;
      } else if (buf[i] == '\n') {
        lf = i;
        buf[i] = '\0';
        break;
      }
    }
    execargs_t** progs = malloc(sizeof(execargs_t) * pipes);
    char* tok_prog = strtok(buf, "|");
    while (tok_prog != NULL) {
      int argcnt = 0;
      char** args = malloc(sizeof(char*));
      char* tok_arg = strtok(tok_prog, " ");
      while (tok_arg != NULL) {
        
      }
    }
  }
  return 0;
}
