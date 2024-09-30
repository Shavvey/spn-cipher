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
    Key k1 = 1110;
    Block b1 = 11401;
    b1 = encrypt(&b1, &k1, ROUNDS);
    printf("E: %d\n", b1);
    b1 = decrypt(&b1, &k1, ROUNDS);
    printf("D: %d\n", b1);
    switch (mode) {
    case ENCRYPT:
      block = encrypt(&block, &k, ROUNDS);
      printf("Encrypted block: %d\n", block);
      block = decrypt(&block, &k, ROUNDS);
      printf("Decrypted block: %d\n", block);
      break;
    case DECRYPT:
      block = decrypt(&block, &k, ROUNDS);
      printf("Decrypted block: %d\n", block);
      break;
    default:
      fprintf(stderr, "Unimplemented!");
      break;
    }
  }
  return EXIT_SUCCESS;
}
