#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Mode decode_flag(const char *flag) {
  // loop through null terminated string
  while (*flag != '\0') {
    if (*flag == '-') {
      if (*(flag + 1) == 'e') {
        return ENCRYPT;
      }
      if (*(flag + 1) == 'd') {
        return DECRYPT;
      }
      if (*(flag + 1) == 't') {
        return TEST;
      }
    }
    flag++;
  }
  fprintf(stderr,
          "Failed to decode mode!\nUsage: spn [-e or -d] [text] [key]\n");
  exit(EXIT_FAILURE);
}

Block decode_text(const char *text) {
  Block block = 0;
  // assuming big endianess
  uint32_t idx = 0;
  unsigned int len = strlen(text);
  if (len != BLOCK_SIZE) {
    fprintf(stderr, "Expected a 16 bit block of text! Recevied: %d bits\n",
            len);
  }
  while (*text != '\0') {
    // for every 1 we encounter shift by it's index and add to block
    // building the block value from the bitstring
    if (*text == '1') {
      block += (1 << (BLOCK_SIZE - 1 - idx));
    }
    idx++;
    text++;
  }
  return block;
}

Key decode_key(const char *key) {
  Key k = 0;
  // assuming big endianess
  uint32_t idx = 0;
  unsigned int len = strlen(key);
  if (len != BLOCK_SIZE) {
    fprintf(stderr, "Expected a 16 bit block of text! Received: %d bits\n",
            len);
  }
  while (*key != '\0') {
    if (*key == '1') {
      k += (1 << (KEY_SIZE - 1 - idx));
    }
    idx++;
    key++;
  }
  return k;
}
