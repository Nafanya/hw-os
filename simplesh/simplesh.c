#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

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
  test();
  return 0;
}
