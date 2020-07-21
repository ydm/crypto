#include <ctype.h>
#include <stdio.h>
#include "des.h"
#include "image.h"

int
main()
{
    const char *key = "password";
    const char ecb[16] = {
        152, 133, 11,  8,   8,   37,  209, 250,
        4,   31,  207, 230, 119, 207, 226, 113,
    };
    const char cbc[16] = {
        196, 175, 44, 38,  102, 251, 101, 139,
        139, 147, 10, 148, 51,  84,  29,  176,
    };

    char decoded[17];
    decoded[16] = '\0';

    memset(decoded, 0, 16);
    decode_ecb(key, ecb, 16, decoded);
    printf("ECB: %s\n", decoded);

    memset(decoded, 0, 16);
    decode_cbc(key, "initial!", cbc, 16, decoded);
    printf("CBC: %s\n", decoded);

    return 0;
}
