#include "spc.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  Block block = 23;
  block = bit_permutation(&block);
  printf("New block val: %d\n", block);
  block = bit_permutation(&block);
  printf("Old block val: %d\n", block);
  return EXIT_SUCCESS;
}
