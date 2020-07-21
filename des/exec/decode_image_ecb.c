#include <stdio.h>
#include "des.h"
#include "image.h"

int
main(int argc __attribute__((unused)), char *argv[])
{
    const char *filename = argv[1];
    if (!filename)
    {
        fprintf(stderr, "usage: %s <encoded-image.pam>\n", argv[0]);
        return 1;
    }

    struct image *img = image_load_pam(filename);
    if (!img)
    {
        perror("image_load_pam");
        return 1;
    }
    struct image *dec = image_new(img->width, img->height);

    const char *source = (const char *) img->pixels;
    char *dest = (char *) dec->pixels;

    const size_t total = img->width * img->height * 4;

    const char *key = "password";
    decode_ecb(key, source, total, dest);

    image_save_pam(dec, "decoded.pam");
    // To convert from pam to png, use:
    // $ pamtopng /tmp/encoded.pam > converted.png

    image_delete(dec);
    image_delete(img);
    return 0;
}
