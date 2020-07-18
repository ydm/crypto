#include <stdio.h>
#include "image.h"


struct image *
image_load_ppm(const char *filename)
{
     
     FILE *f = fopen(filename, "rb");
     size_t width, height;
     fscanf(f, "P6\n");
     fscanf(f, "%zu %zu\n", &width, &height);
     fscanf(f, "255\n");

     struct image *img = image_new(width, height);
     for (size_t y = 0; y < height; y++)
     {
          for (size_t x = 0; x < width; x++)
          {
               unsigned char r, g, b;
               fscanf(f, "%c%c%c", &r, &g, &b);
               size_t index = image_index(img, x, y);
               img->pixels[index] = (struct pixel) {r, g, b, 255};
          }
     }
     fclose(f);

     return img;
}

void
image_save_ppm(struct image *img, const char *filename)
{
     FILE *f = fopen(filename, "wb");
     fprintf(f, "P6 %zu %zu 255 ", img->width, img->height);
     for (size_t y = 0; y < img->height; y++)
     {
          for (size_t x = 0; x < img->width; x++)
          {
               size_t index = image_index(img, x, y);
               struct pixel p = img->pixels[index];
               fprintf(f, "%c%c%c", p.r, p.g, p.b);
          }
     }
     fclose(f);
}
