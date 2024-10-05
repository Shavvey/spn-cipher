# Lab 1 - Substitution and Permutation Networks

## Decription

An implementation of a substitution and permutation network (SPN),
a simple symmetric block-cipher used as the base for many different
encryption algorithms.

## Compilation

To compile the code into an executable, you can simple type `make` in the main directory.

## Testing

There is also a few make commands available for testing purposes:

- The `decrypt` command just shows a sample encryption decryption.

```
  make decrypt
      ./spn -d 0111100011111000 0001001000110100
      Original block:  0111100011111000 -> 30968
      Decrypted block: 1010101010101010 -> 43690
```

- The `encrypt` command just shows a sample encryption.

```
  make encrypt
      ./spn -e 1010101010101010 0001001000110100
      Original block:  1010101010101010 -> 43690
      Encrypted block: 0111100011111000 -> 30968
```

- The `test` command will show that encryption and decryption actually undo each other
  by feeding the encryption output into decryption output.

```
  make test
  ./spn -e 1010101010101010 0001001000110100
  Original block: 1010101010101010 -> 43690
  Encrypted block: 0111100011111000 -> 30968
  ./spn -d 0111100011111000 0001001000110100
  Original block: 0111100011111000 -> 30968
  Decrypted block: 1010101010101010 -> 43690
```
