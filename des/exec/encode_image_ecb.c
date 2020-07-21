#include <stdio.h>              // fprintf
#include <stdlib.h>             // exit
#include "des.h"
#include "image.h"


int
main(int argc __attribute__((unused)), char *argv[])
{
    int status = EXIT_SUCCESS;

    const char *filename = argv[1]; // NULL if argc is 1
    struct image *img = NULL;
    struct image *enc = NULL;

    if (!filename)
    {
        status = EXIT_FAILURE;
        fprintf(stderr, "usage: %s <image.pam>\n", argv[0]);
        goto error;
    }

    // Load the input image
    img = image_load_pam(filename);
    if (!img)
    {
        status = EXIT_FAILURE;
        perror("image_load_pam");
        goto error;
    }
    const size_t total = img->width * img->height * 4;
    // Check if the nuber of bytes is actually divisible by 8, which
    // is the block size of DES.  My DES implementation doesn't
    // support padding.
    if (total % 8 != 0)
    {
        status = EXIT_FAILURE;
        fprintf(stderr, "Image size (%zu) is not divisible by 8\n", total);
        goto error;
    }

    // Create another image that's going to hold the encoded copy.
    enc = image_new(img->width, img->height);

    // Perform encoding.
    const char *key = "password";

    const char *source = (const char *) img->pixels;
    char *dest = (char *) enc->pixels;
    encode_ecb(key, source, total, dest);
    // encode_cbc(key, "love cbc", source, total, dest);

    image_save_pam(enc, "encoded.pam");
    // To convert from pam to png, use:
    // $ pamtopng /tmp/encoded.pam > converted.png

  error:
    image_delete(enc);
    image_delete(img);
    exit(status);
}
