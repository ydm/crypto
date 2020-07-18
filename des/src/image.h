#pragma once

#include <stdlib.h>


struct pixel
{
    unsigned char r, g, b, a;
};

struct image
{
    struct pixel *pixels;
    size_t width;
    size_t height;
};

static inline void
image_init(struct image *img, size_t width, size_t height)
{
    img->pixels = calloc(width * height, sizeof(struct pixel));
    img->width = width;
    img->height = height;
}

static inline void
image_deinit(struct image *img)
{
    free(img->pixels);
}

static inline struct image *
image_new(size_t width, size_t height)
{
    struct image *ans = malloc(sizeof(struct image));
    image_init(ans, width, height);
    return ans;
}

static inline void
image_delete(struct image *img)
{
    image_deinit(img);
    free(img);
}

static inline size_t
image_index(struct image *img, size_t x, size_t y)
{
    return y*img->width + x;
}

static inline void
image_set(struct image *img,
          size_t x, size_t y,
          unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    size_t index = image_index(img, x, y);
    img->pixels[index] = (struct pixel) { r, g, b, a };
}

struct image *image_load_ppm(const char *filename);
void image_save_ppm(struct image *img, const char *filename);
