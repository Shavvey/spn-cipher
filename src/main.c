#include "spc.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  STable table = make_table();
  uint8_t x = table.sbox[3].output;
  uint8_t y = table.sbox[15].output;
  uint16_t out = 0;
  out |= (y & 15);
  out |= (x & 15) << 4;
  printf("out: %d\n", out);
  return EXIT_SUCCESS;
}
