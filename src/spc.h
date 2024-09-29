#ifndef INCLUDE_SRC_SPC_H_
#define INCLUDE_SRC_SPC_H_
#include <stdint.h>

// BLOCK DEFINITIONS
// each block should be 16 bits long
typedef uint16_t Block;

// KEY DEFINITIONS
// key should be 16 bits long
typedef uint16_t Key;
#define BYTE 8 // used to circ-shift

// create a left circular shift
#define S_INPUT_SIZE 4
// S-BOX DEFINITIONS
typedef struct {
  uint8_t map : S_INPUT_SIZE;
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
uint16_t get_inverse(Sbox sbox);
Sbox get_sbox(uint16_t idx);
Key left_circ_shift(Key *key);
Sbox s_box(Block *input);
Block key_mixing(Block *input, Key *key);
#endif // INCLUDE_SRC_SPC_H_
