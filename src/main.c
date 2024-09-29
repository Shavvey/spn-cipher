#include "spc.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  Key key = 32768;
  key = left_circ_shift(&key);
  printf("Key: %d\n", key);
  return EXIT_SUCCESS;
}
