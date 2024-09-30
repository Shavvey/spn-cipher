#include "spc.h"
#include <stdint.h>
#include <stdio.h>
// creates the mapping used for sboxes
const STable S_TABLE = {.sbox = {{0xE},   // 0
                                 {0x4},   // 1
                                 {0xD},   // 2
                                 {0x1},   // 3
                                 {0x2},   // 4
                                 {0xF},   // 5
                                 {0xB},   // 6
                                 {0x8},   // 7
                                 {0x3},   // 8
                                 {0xA},   // 9
                                 {0x6},   // 10
                                 {0xC},   // 11
                                 {0x5},   // 12
                                 {0x9},   // 13
                                 {0x0},   // 14
                                 {0x7}}}; // 15

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

Block s_box(Block *block) {
  Block new_block = 0;
  for (int i = 0; i < S_BOXES_PER_BLOCK; i++) {
    Block bit_width = (15 << (4 * i));
    uint8_t idx = (uint8_t)((*block & bit_width) >> 4 * i);
    Sbox s = get_sbox(idx);
    new_block += (uint16_t)(s.box << 4 * i);
  }
  *block = new_block;
  return *block;
}

Block inverse_s_box(Block *block) {
  Block new_block = 0;
  for (int i = 0; i < S_BOXES_PER_BLOCK; i++) {
    Block bit_width = (15 << (4 * i));
    Sbox s = {.box = (uint8_t)((*block & bit_width) >> 4 * i)};
    uint8_t idx = get_inverse_sbox(s);
    new_block += (uint16_t)(idx << 4 * i);
  }
  *block = new_block;
  return *block;
}
Block bit_permutation(Block *block) {
  Block new_block = 0;
  int size = 1 << 4;
  for (int i = 0; i < size; i++) {
    // get new position values from permutation table
    Block npos = P_TABLE.map[i];
    Block pos = i + 1;
    int diff = npos - pos;
    // printf("Old pos: %d -> New pos: %d, Diff %d\n", pos, npos, diff);
    Block bit = *block & (1 << i);
    // printf("Bit %d: %d\n", pos, bit);
    if (diff > 0) {
      bit = bit << diff;
    } else {
      bit = bit >> (-1 * diff);
    }
    new_block += bit;
  }
  return new_block;
}
// gets K_b to K_b +n
Key get_sub_key(Key *key, uint8_t n) {
  Key k = *key;
  for (int i = 0; i < n; i++) {
    k = left_circ_shift(&k);
  }
  return k;
}

Block sub_key_mix(Block *block, Key *key) {
  // create new subkey via the left circular shift
  *block = *block ^ *key;
  return *block;
}

Block encrypt(Block *block, Key *key, uint32_t rounds) {
  for (int i = 1; i <= rounds; i++) {
    printf("Round: %d\n", i);
    // first obtain new sub via key shifting
    *key = get_sub_key(key, i);
    printf("Round key: %d\n", *key);
    // mix new subkey
    *block = sub_key_mix(block, key);
    // then use s-boxes to substitute 4 bit portions of block
    *block = s_box(block);
    // then use permutation on s-box block output
    *block = bit_permutation(block);
    // repeat 3 more times for a total of 4 rounds
  }
  // give back the block mem
  return *block;
}

Block decrypt(Block *block, Key *key, uint32_t rounds) {
  for (int i = rounds; i >= 1; i--) {
    printf("Round: %d\n", i);
    // first generate subkey
    *key = get_sub_key(key, i);
    printf("Round key: %d\n", *key);
    // then mix sub key
    *block = sub_key_mix(block, key);
    // then use sboxes, inverted this time
    *block = inverse_s_box(block);
    // then use bit permutation
    *block = bit_permutation(block);
    // repeat three more times
  }
  return *block;
}
