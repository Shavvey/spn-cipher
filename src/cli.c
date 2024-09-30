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
    }
    flag++;
  }
  fprintf(stderr,
          "Failed to decode mode!\nUsage: spn [-e or -d] [text] [key]\n");
  exit(EXIT_FAILURE);
}

Block decode_text(const char *text) {
  Block block = 0;
  // assuuming big endianess
  uint32_t idx = 0;
  unsigned int len = strlen(text);
  if (len != BLOCK_SIZE) {
    fprintf(stderr, "Expected a 16 bit block of text! Recevied: %d bits\n",
            len);
  }
  while (*text != '\0') {
    printf("index %d\n", idx);
    // we can safely ingore the zeroes, they don't add any information
    if (*text == '1') {
      printf("Val %d\n", (1 << (BLOCK_SIZE - 1 - idx)));
      block += (1 << (BLOCK_SIZE - 1 - idx));
    }
    idx++;
    text++;
  }
  return block;
}

Block decode_key(const char *key) {
  Key k = 0;
  // assuuming big endianess
  uint32_t idx = 0;
  unsigned int len = strlen(key);
  if (len != BLOCK_SIZE) {
    fprintf(stderr, "Expected a 16 bit block of text! Recevied: %d bits\n",
            len);
  }
  while (*key != '\0') {
    // we can safely ingore the zeroes, they don't add any information
    if (*key == '1') {
      k += (1 << (KEY_SIZE - 1 - idx));
    }
    idx++;
    key++;
  }
  return k;
}
