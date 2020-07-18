#include "des.h"


// Remaps all bits except 8, 16, 24, 32, 40, 48, 56, 64 (56 in total).
static const int PERMUTED_CHOICE1[56] = {
    56, 48, 40, 32, 24, 16, 8,
    0,  57, 49, 41, 33, 25, 17,
    9,  1,  58, 50, 42, 34, 26,
    18, 10, 2,  59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,
    6,  61, 53, 45, 37, 29, 21,
    13, 5,  60, 52, 44, 36, 28,
    20, 12, 4,  27, 19, 11, 3,
};

// Remaps all bits except 9, 18, 22, 25, 35, 38, 43, 54, 57, 58, 59,
// 60, 61, 62, 63, 64 (48 in total).
static const int PERMUTED_CHOICE2[48] = {
    13, 16, 10, 23, 0,  4,
    2,  27, 14, 5,  20, 9,
    22, 18, 11, 3,  25, 7,
    15, 6,  26, 19, 12, 1,
    40, 51, 30, 36, 46, 54,
    29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52,
    45, 41, 49, 35, 28, 31,
};

static const int ROTATES[16] = {
     1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1,
};

static const int INITIAL_PERMUTATION[64] = {
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8,  0,
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
};

// The output (of the expansion permutation) consists of eight 6-bit
// (8 * 6 = 48 bits>) pieces, each containing a copy of 4
// corresponding input bits, plus a copy of the immediately adjacent
// bit from each of the input pieces to either side.
static const int EXPANSION[48] = {
    31, 0,  1,  2,  3,  4,
    3,  4,  5,  6,  7,  8,
    7,  8,  9,  10, 11, 12,
    11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20,
    19, 20, 21, 22, 23, 24,
    23, 24, 25, 26, 27, 28,
    27, 28, 29, 30, 31, 0,
};

static const uint64_t SUBSTITUTION_BOX[8][64] = {
     {
          14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7,
          0,  15, 7,  4,  14, 2,  13, 1,  10, 6,  12, 11, 9,  5,  3,  8,
          4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0,
          15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0,  6,  13,
     },
     {
          15, 1,  8,  14, 6,  11, 3,  4,  9,  7,  2,  13, 12, 0,  5,  10,
          3,  13, 4,  7,  15, 2,  8,  14, 12, 0,  1,  10, 6,  9,  11, 5,
          0,  14, 7,  11, 10, 4,  13, 1,  5,  8,  12, 6,  9,  3,  2,  15,
          13, 8,  10, 1,  3,  15, 4,  2,  11, 6,  7,  12, 0,  5,  14, 9,
     },
     {
          10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8,
          13, 7,  0,  9,  3,  4,  6,  10, 2,  8,  5,  14, 12, 11, 15, 1,
          13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7,
          1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5,  2,  12,
     },
     {
          7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15,
          13, 8,  11, 5,  6,  15, 0,  3,  4,  7,  2,  12, 1,  10, 14, 9,
          10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4,
          3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7,  2,  14,
     },
     {
          2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9,
          14, 11, 2,  12, 4,  7,  13, 1,  5,  0,  15, 10, 3,  9,  8,  6,
          4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14,
          11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4,  5,  3,
     },
     {
          12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11,
          10, 15, 4,  2,  7,  12, 9,  5,  6,  1,  13, 14, 0,  11, 3,  8,
          9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6,
          4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0,  8,  13,
     },
     {
          4,  11,  2, 14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1,
          13, 0,  11, 7,  4,  9,  1,  10, 14, 3,  5,  12, 2,  15, 8,  6,
          1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2,
          6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2,  3,  12,
     },
     {
          13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7,
          1,  15, 13, 8,  10, 3,  7,  4,  12, 5,  6,  11, 0,  14, 9,  2,
          7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8,
          2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5,  6,  11,
     },
};

static const int PERMUTATION[32] = {
    15, 6,  19, 20, 28, 11, 27, 16,
    0,  14, 22, 25, 4,  17, 30, 9,
    1,  7,  23, 13, 31, 26, 2,  8,
    18, 12, 29, 5,  21, 10, 3,  24,
};

static const int INVERSE_PERMUTATION[64] = {
    39, 7,  47, 15, 55, 23, 63, 31,
    38, 6,  46, 14, 54, 22, 62, 30,
    37, 5,  45, 13, 53, 21, 61, 29,
    36, 4,  44, 12, 52, 20, 60, 28,
    35, 3,  43, 11, 51, 19, 59, 27,
    34, 2,  42, 10, 50, 18, 58, 26,
    33, 1,  41, 9,  49, 17, 57, 25,
    32, 0,  40, 8,  48, 16, 56, 24,
};

// static void
// printb(uint64_t x, int width)
// {
//      uint64_t one = 1;
//      int i = width;
//      // Skip initial zeros.
//      if (x > 0)
//      {
//           i = 63;
//           while ((x & one<<i) == 0 && i > width)
//           {
//                i--;
//           }
//      }
//      printf("0b");
//      while (i >= 0)
//      {
//           putchar((x & one<<i) ? '1' : '0');
//           i--;
//      }
// }

/**
 * @param n Length of the mapper array.
 */
static uint64_t
permute(uint64_t data, int bits, const int *mapper, int n)
{
     uint64_t one = 1;
     uint64_t ret = 0;
     for (int i = 0; i < n; ++i)
     {
          int v = mapper[i];
          int from = bits - 1 - v;
          if (data & one<<from)
          {
               int to = n - 1 - i;
               ret |= one << to;
          }
     }
     return ret;
}

static inline uint64_t
rotate_left(uint64_t i28, int k)
{
     return (i28 << k & 0x0fffffff) | i28 >> (28-k);
}

/**
 * Sixteen 46-bit subkeys - one for each round - are derived from the
 * main key using the key schedule.
 */
static void
derive_keys(uint64_t key, uint64_t out[16])
{
     uint64_t one = 1;
     // Initially, 56 bits are selected from the initial 64 by
     // Permuted Choice 1 (PC-1) -- the remaining eight bits are
     // either discarded (our case) or used as parity check bits.
     uint64_t next_key = permute(key, 64, PERMUTED_CHOICE1, 56);
     // The 56 bits are then divided into two 28-bit halves; each half
     // is thereafter treated separately.
     uint64_t lhs = next_key >> 28;
     uint64_t rhs = next_key & ((one<<28) - 1);
     for (int i = 0; i < 16; i++)
     {
          // In successive rounds, both halves are rotated left by one
          // or two bits (specified for each round).
          int bits = ROTATES[i];
          lhs = rotate_left(lhs, bits);
          rhs = rotate_left(rhs, bits);
          // Then 48 subkey bits are selected by Permuted Choice 2
          // (PC-2) -- 24 bits from the left half and 24 bits from the
          // right half.
          uint64_t united = lhs<<28 | rhs;
          out[i] = permute(united, 56, PERMUTED_CHOICE2, 48);
     }
}

/**
 * The Feistel (F) function.
 */
static uint64_t
f(uint64_t block, uint64_t key)
{
     // 1. Expansion: 32-bit half-block gets expanded to 48 bits by
     // duplicating half of the bits.  Output consists of eight 6-bit
     // (8 x 6 = 48 bits) pieces.
     uint64_t expanded = permute(block, 32, EXPANSION, 48);
     // 2. Key mixing: the result is combined with a subkey using an
     // XOR operation.
     uint64_t mixed = expanded ^ key;
     uint64_t ret = 0;
     // 3. Substitution: after mixing the subkey, the block is divided
     // into eight 6-bit pieces before processing by the S-boxes, or
     // substitution boxes.  Each of the eight S-boxes replaces its
     // six input bits with four output bits according to a non-linear
     // transformation, provided in the form of a lookup table.  The
     // S-boxes provide the core of the security of DES - without
     // them, the cipher would be linear, and trivially brokable.
     for (int i = 0; i < 8; i++)
     {
          // 0x20 = 0b100000
          // 0x3f = 0b111111
          // 0x1e = 0b011110
          int offset = 42 - i*6;
          uint64_t i6 = mixed>>offset & 0x3f;
          uint64_t index = (i6 & 0x20) | (i6 & 0x01) << 4 | (i6 & 0x1e) >> 1;
          const uint64_t *box = SUBSTITUTION_BOX[i];
          ret = ret<<4 | box[index];
     }
     // 4. Permutation: finally, the 32 outputs from the S-boxes are
     // rearranged according to a fixed permutation, the P-box.  This
     // is designed so that, after permutation, the bits from the
     // output of each S-box in this round are spread across four
     // different S-boxes in the next round.
     return permute(ret, 32, PERMUTATION, 32);
}

static uint64_t
encode_block(uint64_t block, uint64_t keys[16])
{
     // There are 16 identical stages of processing, termed rounds.
     // There is also an initial and final permutations, termed IP and
     // FP, which are inverses (IP "undoes" the actions of FP, and
     // vice versa).  IP and FP have no cryptographic significance,
     // but were included in order to facilitate loading blocks in and
     // out of mid-1970s 8bit based hardware.
     uint64_t one = 1;
     block = permute(block, 64, INITIAL_PERMUTATION, 64);
     // Before the main rounds, the block is divided into two 32-bit
     // halves and proessed alternately; this criss-crossing is known
     // as the Feistel scheme.  The Feistel structure ensures that
     // decryption and encryption are very similar processes -- the
     // only difference is that the subkeys are applied in the reverse
     // order when decrypting.  The rest of the algorithm is
     // identical.
     uint64_t lhs = block >> 32;
     uint64_t rhs = block & ((one<<32) - 1);
     for (int i = 0; i < 16; i++)
     {
          uint64_t key = keys[i];
          uint64_t new_lhs = rhs;
          // The F-function scrambles half a block together with some
          // of the key.
          uint64_t output = f(rhs, key);
          // The output from the F-function is then combined with the
          // other half of the block, ...
          uint64_t new_rhs = lhs ^ output;
          // ... and the halves are swapped before the next round.
          lhs = new_lhs;
          rhs = new_rhs;
     }
     return permute(rhs<<32 | lhs, 64, INVERSE_PERMUTATION, 64);
}

static void
reverse(uint64_t ary[16])
{
     for (int i = 0; i < 8; i++)
     {
          uint64_t tmp = ary[i];
          ary[i] = ary[15 - i];
          ary[15 - i] = tmp;
     }
}

static void
perform(const char *key, const char *plaintext, char *output, int decode)
{
     uint64_t k = string_to_u64(key);
     uint64_t m = string_to_u64(plaintext);

     uint64_t keys[16];
     derive_keys(k, keys);
     if (decode)
     {
          reverse(keys);
     }

     uint64_t enc = encode_block(m, keys);
     u64_to_string(enc, output);
}

void
encode(const char *key, const char *plaintext, char *output)
{
     perform(key, plaintext, output, 0);
}

void
decode(const char *key, const char *ciphertext, char *output)
{
     perform(key, ciphertext, output, 1);
}

// #include <stdio.h>
// #include <inttypes.h>
//
// int
// main()
// {
//      const char *msg = "some msg";
//      const char *key = "some key";

//      printf("msg=%s\n", msg);
//      printf("key=%s\n\n", key);

//      // Encode
//      char enc[8];
//      perform(key, msg, enc, 0);

//      // Print result of encoding
//      printf("enc=%" PRIu64 "\n", string_to_u64(enc));
//      for (int i = 0; i < 8; i++)
//      {
//           printf("enc[%d]=%d\n", i, (unsigned char) enc[i]);
//      }
//      putchar('\n');

//      // Decode
//      char dec[9];
//      perform(key, enc, dec, 1);

//      // Print result of decoding
//      dec[8] = '\0';
//      printf("dec=%s\n", dec);

//      return 0;
// }
