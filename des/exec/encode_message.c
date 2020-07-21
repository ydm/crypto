#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include "des.h"
#include "image.h"

int
main()
{
    const char *key = "password";
    const char *msg = "Hello, DES world";
    char encoded[16];
    encode_ecb(key, msg, 16, encoded);
    printf("ECB:\n");
    for (int i = 0; i < 16; i++)
    {
        printf("encoded[%3d] = %4d ", i, (unsigned char) encoded[i]);
        if (isprint(encoded[i]))
            printf("(%c)\n", encoded[i]);
        else
            printf("(not printable)\n");
    }

    const char *initial = "initial!";
    encode_cbc(key, initial, msg, 16, encoded);
    printf("CBC:\n");
    for (int i = 0; i < 16; i++)
    {
        printf("encoded[%3d] = %4d ", i, (unsigned char) encoded[i]);
        if (isprint(encoded[i]))
            printf("(%c)\n", encoded[i]);
        else
            printf("(not printable)\n");
    }
    return 0;
}
