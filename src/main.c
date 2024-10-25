#include "cli.h"
#include "spc.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // if we don't have enough args to continue, just print an error and exit
  if (argc < NUM_ARGS) {
    const char *error =
        "Not enough args!\nUsage: spn [-e or -d] [text] [key]\n";
    fprintf(stderr, "%s", error);
    return EXIT_FAILURE;
    // now we begin to try to decode the args we are given
  } else {
    const char *flag = argv[FLAG];
    const char *text = argv[TEXT];
    const char *key = argv[KEY];
    Mode mode = decode_flag(flag);
    Block block = decode_text(text);
    Key k = decode_key(key);
    char *strb;
    switch (mode) {
    case ENCRYPT:
      strb = block_as_bitstring(block);
      printf("Original block:  %s -> %d\n", strb, block);
      free(strb);
      block = encrypt(block, k, ROUNDS);
      strb = block_as_bitstring(block);
      printf("Encrypted block: %s -> %d\n", strb, block);
      free(strb);
      break;
    case DECRYPT:
      strb = block_as_bitstring(block);
      printf("Original block:  %s -> %d\n", strb, block);
      free(strb);
      block = decrypt(block, k, ROUNDS);
      strb = block_as_bitstring(block);
      printf("Decrypted block: %s -> %d\n", strb, block);
      free(strb);
      break;
    default:
      fprintf(stderr, "[ERROR]: Unimplemented!");
      break;
    }
  }
  return EXIT_SUCCESS;
}
