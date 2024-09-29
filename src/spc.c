#include "spc.h"
#include <stdint.h>
#include <stdio.h>
STable make_table() {
  STable t;
  // using the DES s-box implementation
  t.sbox[0].output = 0xE;
  t.sbox[1].output = 0x4;
  t.sbox[2].output = 0xD;
  t.sbox[3].output = 0x1;
  t.sbox[4].output = 0x2;
  t.sbox[5].output = 0xF;
  t.sbox[6].output = 0xB;
  t.sbox[7].output = 0x8;
  t.sbox[8].output = 0x3;
  t.sbox[9].output = 0xA;
  t.sbox[10].output = 0x6;
  t.sbox[11].output = 0xC;
  t.sbox[12].output = 0x5;
  t.sbox[13].output = 0x9;
  t.sbox[14].output = 0x0;
  t.sbox[15].output = 0x7;
  // loop through entries inside table
  int t_size = 1 << S_INPUT_SIZE;
  for (int i = 0; i < t_size; i++) {
    printf("Input: %d -> Output: 0x%hhX\n", i, t.sbox[i].output);
  }
  return t;
}
