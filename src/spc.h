#ifndef INCLUDE_SRC_SPC_H_
#define INCLUDE_SRC_SPC_H_
#include <stdint.h>
#define ROUNDS 4
#define BLOCK_SIZE 16
// BLOCK DEFINITIONS
// each block should be 16 bits long
typedef uint16_t Block;

// KEY DEFINITIONS
// key should be 16 bits long
typedef uint16_t Key;
#define KEY_SIZE 16
#define BYTE 8 // used to circ-shift

// create a left circular shift
#define S_INPUT_SIZE 4
#define S_BOXES_PER_BLOCK BLOCK_SIZE / S_INPUT_SIZE
// S-BOX DEFINITIONS
typedef struct {
  uint8_t box : S_INPUT_SIZE;
} Sbox;
// table of sbox input and output values, essentially just a static array list
// (no resizing / rebuilding options)
typedef struct {
  // should be 16 entires long, enough to map every 4 bit input to another 4-bit
  // output
  Sbox sbox[1 << S_INPUT_SIZE];
} STable;

extern const STable S_TABLE;

typedef struct {
  uint16_t map[1 << S_INPUT_SIZE];
} PTable;

extern const PTable P_TABLE;

// FUNCTION DEFINTIONS
// put in block, get a new block according to the s-box mapping
STable make_table(void);
// return input of sbox, just does a linear search to find using output
uint8_t get_inverse_sbox(Sbox output);
Sbox get_sbox(uint8_t idx);
Key left_circ_shift(Key *key, int n);
Block s_box(Block *block);
Block inverse_s_box(Block *block);
Block sub_key_mix(Block *block, Key *key);
Key get_sub_key(Key key, uint8_t n);
Block bit_permutation(Block *block);
Block encrypt(Block block, Key key, uint32_t rounds);
Block decrypt(Block block, Key key, uint32_t rounds);
char *block_as_bitstring(Block block);
#endif // INCLUDE_SRC_SPC_H_
