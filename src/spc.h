#ifndef INCLUDE_SRC_SPC_H_
#define INCLUDE_SRC_SPC_H_
#include <stdint.h>
// KEY DEFINITIONS
// key should be 16 bits long
typedef uint16_t Key;
// create a left circular shift
Key *left_circ_shift(Key *key);

#define S_INPUT_SIZE 4
// S-BOX DEFINITIONS
typedef struct {
  uint8_t output : S_INPUT_SIZE;
} Sbox;
// table of sbox input and output values, essentially just a static array list
// (no resizing / rebuilding options)
typedef struct {
  // should be 16 entires long, enough to map every 4 bit input to another 4-bit
  // output
  Sbox sbox[1 << S_INPUT_SIZE];
} STable;

void s_box(uint16_t *input);
STable make_table(void);

#endif // INCLUDE_SRC_SPC_H_
