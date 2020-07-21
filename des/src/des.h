#pragma once

#include <stdint.h>             // uint64_t
#include <string.h>             // memcpy

/*
 * len(output) should be >= n
 */
int encode_ecb(const char *key, const char *plaintext, size_t n, char *output);
int decode_ecb(const char *key, const char *ciphertext, size_t n, char *output);
int encode_cbc(const char *key, const char *initial,
               const char *plaintext, size_t n,
               char *output);
int decode_cbc(const char *key, const char *initial,
               const char *ciphertext, size_t n,
               char *output);

