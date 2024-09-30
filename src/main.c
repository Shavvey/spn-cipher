#include "cli.h"
#include "spc.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    char *arg = argv[i];
    printf("Arg %d: %s\n", i, arg);
  }
  return EXIT_SUCCESS;
}
