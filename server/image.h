#ifndef IMAGE_H
#define IMAGE_H

#include "Struct/pixel.h"

struct Image
{
    int width;
    int height;
    Pixel **pixels;
};
typedef struct Image Image;

Image *createEmptyImage(int width, int height);
Image *importImage(char *filename);
void saveImage(Image *image, char *filename);
void freeImage(Image *image);
void cleanImage(Image *image);

#endif