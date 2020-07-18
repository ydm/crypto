#pragma once

#include <stdint.h>             // uint64_t
#include <string.h>             // memcpy


static inline uint64_t
string_to_u64(const char *s)
{
     uint64_t u;
     memcpy(&u, s, 8);
     u = __builtin_bswap64(u);  // Convert to big endian.
     return u;
}

static inline void
u64_to_string(uint64_t u, char *s)
{
     u = __builtin_bswap64(u);  // Convert to little endian.
     memcpy(s, &u, 8);
}

void encode(const char *key, const char *plaintext, char *output);
void decode(const char *key, const char *ciphertext, char *output);
