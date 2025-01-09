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
// obtain s-box mapping using it's known index in the array
Sbox get_sbox(uint8_t idx);
// implemented left circular shift
Key left_circ_shift(Key key, int n);
// provides mapping of s-boxes, uses predefined array called S_TABLE
Block s_box(Block block);
// provide inverse mapping of the s-boxes
Block inverse_s_box(Block block);
// mix each sub key with the XOR provided the block, return back the block
Block sub_key_mix(Block block, Key key);
// return sub key using round key function (should be based on the left circular
// shift)
Key get_sub_key(Key key, uint8_t n);
// permute the block and return it
Block bit_permutation(Block block);
// wrapper function that preform the encryption for the spn block cipher
Block encrypt(Block block, Key key, uint32_t rounds);
// wrapper function that preform the decryption for the spn block cipher
Block decrypt(Block block, Key key, uint32_t rounds);
// return back the bit string representation of a given block
char *block_as_bitstring(Block block);
#endif // INCLUDE_SRC_SPC_H_
