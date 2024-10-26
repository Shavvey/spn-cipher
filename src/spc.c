#include "spc.h"
#include <stdint.h>
#include <stdlib.h>
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
Key left_circ_shift(Key key, int n) {
  Key k = (key << n) | (key >> ((sizeof(key) * BYTE - n)));
  return k;
}
// simple wrapper to get an sbox from the table
Sbox get_sbox(uint8_t idx) {
  Sbox out = S_TABLE.sbox[idx];
  return out;
}

uint8_t get_inverse_sbox(Sbox output) {
  int t_size = 1 << S_INPUT_SIZE;
  int idx = 0;
  // loop through entires of sbox arr and
  // the index that maps the sbox
  for (; idx < t_size; idx++) {
    Sbox s = get_sbox(idx);
    if (s.box == output.box) {
      break;
    }
  }
  return idx;
}

Block s_box(Block block) {
  Block new_block = 0;
  for (int i = 0; i < S_BOXES_PER_BLOCK; i++) {
    Block bit_width = (15 << (4 * i));
    uint8_t idx = (uint8_t)((block & bit_width) >> 4 * i);
    Sbox s = get_sbox(idx);
    // append new 4-bits obtained from s-box mapping
    new_block += (uint16_t)(s.box << 4 * i);
  }
  return new_block;
}

Block inverse_s_box(Block block) {
  Block new_block = 0;
  for (int i = 0; i < S_BOXES_PER_BLOCK; i++) {
    Block bit_width = (15 << (4 * i));
    Sbox s = {.box = (uint8_t)((block & bit_width) >> 4 * i)};
    uint8_t idx = get_inverse_sbox(s);
    // append new 4-bits obtained from reverse s-box mapping
    new_block += (uint16_t)(idx << 4 * i);
  }
  return new_block;
}

Block bit_permutation(Block block) {
  Block new_block = 0;
  int size = 1 << 4;
  for (int i = 0; i < size; i++) {
    // get new position values from permutation table
    Block npos = P_TABLE.map[i];
    Block pos = i + 1;
    int diff = npos - pos;
    // printf("Old pos: %d -> New pos: %d, Diff %d\n", pos, npos, diff);
    Block bit = block & (1 << i);
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
Key get_sub_key(Key key, uint8_t n) {
  // k_0 = k
  Key k = key;
  // apply the left circular shift
  for (uint8_t i = 0; i < n; i++) {
    if (i % 2 == 0) {
      k = left_circ_shift(k, 1);
    } else {
      k = left_circ_shift(k, 2);
    }
  }
  return k;
}

Block sub_key_mix(Block block, Key key) {
  // mix the sub-key we calculated via the XOR
  return key ^ block;
}

Block encrypt(Block block, Key key, uint32_t rounds) {
  uint32_t i = 1;
  for (; i <= rounds; i++) {
    // obtain round sub key via left circ shift
    Key k = get_sub_key(key, i);
    // mix new subkey
    block = sub_key_mix(block, k);
    // then use s-boxes to substitute 4 bit portions of block
    block = s_box(block);
    // then use permutation on s-box block output
    if (i < rounds) {
      block = bit_permutation(block);
    }
    // repeat 3 more times for a total of 4 rounds
  }
  Key k = get_sub_key(key, i);
  block = sub_key_mix(block, k);
  // give back the block mem
  return block;
}

Block decrypt(Block block, Key key, uint32_t rounds) {
  uint32_t i = rounds;
  Key k = get_sub_key(key, i + 1);
  block = sub_key_mix(block, k);
  for (; i >= 1; i--) {
    // do the inverse of the rounds and operations we did during encryption step
    if (i < rounds) {
      block = bit_permutation(block);
    }
    block = inverse_s_box(block);
    Key k = get_sub_key(key, i);
    block = sub_key_mix(block, k);
    // repeat three more times
  }
  return block;
}

char *block_as_bitstring(Block block) {
  char *str = (char *)malloc(sizeof(char) * BLOCK_SIZE);
  uint16_t mask = 1;
  int size = BLOCK_SIZE - 1;
  for (int i = size; i >= 0; i--) {
    uint16_t b = (block & (mask << (size - i)));
    b = (b >> (size - i));
    if (b == 1) {
      *(str + i) = '1';
    } else {
      *(str + i) = '0';
    }
  }
  // null terminate the string
  str[BLOCK_SIZE] = '\0';
  return str;
}
