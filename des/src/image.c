#include <stdio.h>
#include "image.h"


struct image *
image_load_pam(const char *filename)
{
     
     FILE *f = fopen(filename, "rb");
     size_t width, height;

     fscanf(f, "P7\n");
     fscanf(f, "WIDTH %zu\n", &width);
     fscanf(f, "HEIGHT %zu\n", &height);
     fscanf(f, "DEPTH 4\n");
     fscanf(f, "MAXVAL 255\n");
     fscanf(f, "TUPLTYPE RGB_ALPHA\n");
     fscanf(f, "ENDHDR\n");

     struct image *img = image_new(width, height);
     for (size_t y = 0; y < height; y++)
     {
          for (size_t x = 0; x < width; x++)
          {
               unsigned char r, g, b, a;
               fscanf(f, "%c%c%c%c", &r, &g, &b, &a);
               size_t index = image_index(img, x, y);
               img->pixels[index] = (struct pixel) {r, g, b, a};
          }
     }
     fclose(f);

     return img;
}

void
image_save_pam(struct image *img, const char *filename)
{
     FILE *f = fopen(filename, "wb");
     fprintf(f, "P7\n");
     fprintf(f, "WIDTH %zu\n", img->width);
     fprintf(f, "HEIGHT %zu\n", img->height);
     fprintf(f, "DEPTH 4\n");
     fprintf(f, "MAXVAL 255\n");
     fprintf(f, "TUPLTYPE RGB_ALPHA\n");
     fprintf(f, "ENDHDR\n");
     for (size_t y = 0; y < img->height; y++)
     {
          for (size_t x = 0; x < img->width; x++)
          {
               size_t index = image_index(img, x, y);
               struct pixel p = img->pixels[index];
               fprintf(f, "%c%c%c%c", p.r, p.g, p.b, p.a);
          }
     }
     fclose(f);
}
