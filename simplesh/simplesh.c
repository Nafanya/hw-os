#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "helpers.h"
#include "bufio.h"

const int BUF_SIZE = 8192;

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

void prompt() {
  if (write_(STDOUT_FILENO, "$", 1) == -1) {
    exit(EXIT_FAILURE);
  }
}

execargs_t* parse_prog(char* s) {
  int arg_cnt = 2;
  for (char* c = s; *c; c++) {
    arg_cnt += *c == ' ';
  }
  char** args = malloc(sizeof(char*) * arg_cnt);
  args[arg_cnt - 1] = NULL;
  execargs_t* ret = malloc(sizeof(execargs_t));

  char* end_str;
  char* tok = strtok_r(s, " ", &end_str);
  int index = 0;
  while (tok != NULL) {
    args[index++] = strdup(tok);
    //printf("arg #%d: %s\n", index - 1, tok);
    tok = strtok_r(NULL, " ", &end_str);
  }
  ret->prog = args[0];
  ret->args = args;
  return ret;
}

void main_sig(int sig) {
  if (write_(STDOUT_FILENO, "\n$", 2) == -1) {
    //exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  signal(SIGINT, main_sig);
  buf_t* buf = buf_new(BUF_SIZE);
  char s[BUF_SIZE];
  while (1) {
    prompt();
    ssize_t lf = buf_getline(STDIN_FILENO, buf, s);
    if (lf == -1) {
      return EXIT_FAILURE;
    } else if (lf == 0) {
      return EXIT_SUCCESS;
    }
    int prog_cnt = 1;
    for (size_t i = 0; i < lf; i++) {
      if (s[i] == '|') {
        prog_cnt++;
      }
    }
    execargs_t* progs[prog_cnt];
    char* end_str;
    char* tok = strtok_r(s, "|", &end_str);
    int index = 0;
    while (tok != NULL) {
      progs[index++] = parse_prog(tok);
      tok = strtok_r(NULL, "|", &end_str);
    }
    runpiped(progs, prog_cnt);
  }
  return EXIT_SUCCESS;
}
