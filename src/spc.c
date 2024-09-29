#include "spc.h"
#include <stdint.h>
#include <stdio.h>
// creates the mapping used for sboxes
const STable S_TABLE = {.sbox = {{0xE},
                                 {0x4},
                                 {0xD},
                                 {0x1},
                                 {0x2},
                                 {0xF},
                                 {0xB},
                                 {0x8},
                                 {0x3},
                                 {0xA},
                                 {0x6},
                                 {0xC},
                                 {0x5},
                                 {0x9},
                                 {0x0},
                                 {0x7}}};

const PTable P_TABLE = {
    .map = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16}};

// source: https://stackoverflow.com/questions/13289397/circular-shift-in-c
// preform a complementary right shift each time we preform the left shift
// and then OR the results
Key left_circ_shift(Key *key) {
  return (*key << 1) | (*key >> ((sizeof(*key) * BYTE - 1)));
}
// simple wrapper to get an sbox from the table
Sbox get_sbox(uint8_t idx) {
  Sbox out = S_TABLE.sbox[idx];
  return out;
}

uint8_t get_inverse_sbox(Sbox output) {
  int t_size = 1 << S_INPUT_SIZE;
  int idx = 0;
  for (; idx < t_size; idx++) {
    Sbox s = get_sbox(idx);
    if (s.box == output.box) {
      break;
    }
  }
  return idx;
}

Block bit_permutation(Block *block) {
  Block new_block = 0;
  int size = 1 << 4;
  for (int i = 0; i < size; i++) {
    // get new position values from permutation table
    Block npos = P_TABLE.map[i];
    Block pos = i + 1;
    int diff = npos - pos;
    printf("Old pos: %d -> New pos: %d, Diff %d\n", pos, npos, diff);
    Block bit = *block & (1 << i);
    printf("Bit %d: %d\n", pos, bit);
    if (diff > 0) {
      bit = bit << diff;
    } else {
      bit = bit >> (-1 * diff);
    }
    new_block += bit;
  }
  return new_block;
}
