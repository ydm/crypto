#include <math.h>
#include <stdio.h>
#include "des.h"
#include "image.h"


int
main()
{
     // Load the input image
     struct image *img = image_load_ppm("/tmp/tux.ppm");
     const size_t total = img->width * img->height * 4;
     // Check if the nuber of bytes is actually divisible by 8, which
     // is the block size of DES.
     if (total % 8 != 0)
     {
          image_delete(img);
          printf("%zu is not divisible by 8\n", total);
          exit(EXIT_FAILURE);
     }
     const size_t n = total / 8;

     // Create another image that's going to hold the encoded copy.
     struct image *enc = image_new(img->width, img->height);

     // Perform encoding.
     const char *key = "password";
     const char *source = (const char *) img->pixels;
     char *dest = (char *) enc->pixels;

     char output[8];
     for (size_t i = 0; i < n; i++)
     {
          size_t offset = i * 8;
          // Encode
          encode(key, source + offset, output);

          // Store
          memcpy(dest + offset, output, 8);
     }
     image_save_ppm(enc, "/tmp/encoded.ppm");
     image_delete(enc);
     image_delete(img);
     return 0;
}

int
main2()
{
     struct image *img = image_load_ppm("/tmp/encoded.ppm");
     struct image *dec = image_new(img->width, img->height);

     const char *source = (const char *) img->pixels;
     char *dest = (char *) dec->pixels;

     const size_t total = img->width * img->height * 4;
     const size_t n = total / 8;

     for (int i = 0; i < 8; i++)
     {
          printf("%d %d\n", i, (unsigned char) source[i]);
     }

     const char *key = "password";
     char output[8];
     for (size_t i = 0; i < 1; i++)
     {
          size_t offset = i * 8;
          decode(key, source + offset, output);
          for (int j = 0; j < 8; j++)
               printf("j=%d output=%d\n", j, (unsigned char) output[j]);
          memcpy(dest + offset, output, 8);
     }

     image_save_ppm(dec, "/tmp/decoded.ppm");
     image_delete(dec);
     image_delete(img);
     return 0;
}


// static inline unsigned char
// chan(float x)
// {
//      return (unsigned char) round(x * 255.0f);
// }
//
// struct image img;
// image_init(&img, 600, 400);
// for (size_t x = 0; x < 600; x++)
// {
//      for (size_t y = 0; y < 400; y++)
//      {
//           image_set(&img, x, y, chan(x/599.0f), chan(y/499.0f), 0, 255);
//      }
// }
// image_save_ppm(&img, "/tmp/output.ppm");
// image_deinit(&img);


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
