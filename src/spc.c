#include "spc.h"
#include <stdint.h>
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
// and then OR the results (to make sure the code doesn't have undefined
// behavior if one shift is larger than the width we also do a modulus)
Key left_circ_shift(Key *key) {
  return (*key << 1) |
         (*key >> ((sizeof(*key) * BYTE - 1) % (sizeof(*key) * BYTE)));
}
// simple wrapper to get an sbox from the table
Sbox get_sbox(uint16_t idx) {
  Sbox out = S_TABLE.sbox[idx];
  return out;
}
