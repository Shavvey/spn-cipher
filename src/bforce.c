#include "cli.h"
#include "spc.h"
#include <stdio.h>
#include <stdlib.h>

// known plaintext and cipher text pair
const char *PLAINTEXT = "0100111000100001";
const char *CIPHERTEXT = "1000000110001111";

int main() {
  Block p_block = decode_text(PLAINTEXT);
  Block c_block = decode_text(CIPHERTEXT);
  Key key = {0};
  // test enciphered block, compared to target plaintext block
  Block t_block = decrypt(c_block, key, ROUNDS);
  unsigned int iters = 0;
  while (t_block != p_block && iters <= 1 << KEY_SIZE) {
    // increment key, get new block
    key++;
    iters++;
    t_block = decrypt(c_block, key, ROUNDS);
  }

  const char *kstr = block_as_bitstring((Block)key);
  printf("Found the key: %s\n", kstr);
  // be nice and free the key string
  free((void *)kstr);
  return EXIT_SUCCESS;
}
