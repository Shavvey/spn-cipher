#include "spc.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  Key key = 32768;
  Block block = 7;
  block = encrypt(&block, &key, ROUNDS);
  printf("New block %d\n", block);
  block = decrypt(&block, &key, ROUNDS);
  printf("Old block (hopefully) %d\n", block);
  return EXIT_SUCCESS;
}
