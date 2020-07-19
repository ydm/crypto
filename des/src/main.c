#include <math.h>
#include <stdio.h>
#include "des.h"
#include "image.h"


static int
main_encode()
{
     // Load the input image
     struct image *img = image_load_pam("/tmp/tux.pam");
     const size_t total = img->width * img->height * 4;
     // Check if the nuber of bytes is actually divisible by 8, which
     // is the block size of DES.
     if (total % 8 != 0)
     {
          image_delete(img);
          printf("%zu is not divisible by 8\n", total);
          exit(EXIT_FAILURE);
     }

     // Create another image that's going to hold the encoded copy.
     struct image *enc = image_new(img->width, img->height);

     // Perform encoding.
     const char *key = "password";
     const char *source = (const char *) img->pixels;
     char *dest = (char *) enc->pixels;
     const char *initial = "kuremure";
     encode_cbc(key, initial, source, dest, total);

     image_save_pam(enc, "/tmp/encoded.pam");
     image_delete(enc);
     image_delete(img);
     return 0;
}

static int
main_decode()
{
     struct image *img = image_load_pam("/tmp/encoded.pam");
     struct image *dec = image_new(img->width, img->height);

     const char *source = (const char *) img->pixels;
     char *dest = (char *) dec->pixels;

     const size_t total = img->width * img->height * 4;

     const char *key = "password";
     decode_ecb(key, source, dest, total);

     image_save_pam(dec, "/tmp/decoded.pam");
     image_delete(dec);
     image_delete(img);
     return 0;
}


// static inline unsigned char
// chan(float x)
// {
//      return (unsigned char) round(x * 255.0f);
// }

// int
// main()
// {
//      struct image *img = image_load_pam("/tmp/tux.pam");

//      for (size_t y = 0; y < img->height; y++)
//      {
//           for (size_t x = 0; x < img->width; x++)
//           {
//                size_t index = image_index(img, x, y);
//                img->pixels[index].a = 255/3;
//           }
//      }
     
//      image_save_pam(img, "/tmp/xexe.pam");
//      return 0;
// }

// int
// main()
// {
//      struct image img;
//      image_init(&img, 600, 400);
//      for (size_t x = 0; x < 600; x++)
//      {
//           for (size_t y = 0; y < 400; y++)
//           {
//                image_set(&img, x, y, chan(x/599.0f), chan(y/499.0f), 0, 255);
//           }
//      }
//      image_save_pam(&img, "/tmp/output.pam");
//      image_deinit(&img);
//      return 0;
// }

// #include <inttypes.h>

// int
// main()
// {
//      const char *key = "password";
//      const char *msg = "pliis be";

//      printf("msg=%s\n", msg);
//      printf("key=%s\n\n", key);

//      // Encode
//      char enc[8];
//      encode(key, msg, enc);

//      // Print result of encoding
//      printf("enc=%" PRIu64 "\n", string_to_u64(enc));
//      for (int i = 0; i < 8; i++)
//      {
//           printf("enc[%d]=%d\n", i, (unsigned char) enc[i]);
//      }
//      putchar('\n');

//      // Decode
//      char dec[9];
//      decode(key, enc, dec);

//      // Print result of decoding
//      dec[8] = '\0';
//      printf("dec=%s\n", dec);

//      return 0;
// }

int
main()
{
     main_encode();
     // main_decode();
}
