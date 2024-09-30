#ifndef INCLUDE_SRC_CLI_H_
#define INCLUDE_SRC_CLI_H_
#include "spc.h"
// USAGE: spn [-d] [-e] [text] [key]
#define NUM_ARGS 3
#define FLAG 1 // encryption or decryption flag
#define TEXT 2 // ciphertext or plaintext
#define KEY 3  // key used to decrypt or encrypt
// functions to use
typedef enum { ENCRYPT = 1, DECRYPT } Mode;
Mode decode_flag(const char *flag);
Block decode_text(const char *text);
Key decode_key(const char *key);
#endif // INCLUDE_SRC_CLI_H_
