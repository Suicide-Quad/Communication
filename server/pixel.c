#include "Struct/pixel.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

Pixel *initPixel(Uint8 red, Uint8 green, Uint8 blue)
{
    Pixel *pixel = malloc(sizeof(Pixel));

    if (pixel == NULL)
        errx(-1, "Error while allocating a pointer (initPixel)");

    pixel->red = red;
    pixel->green = green;
    pixel->blue = blue;

    return pixel;
}

Pixel *updateSameColorPixel(Pixel *pixel, Uint8 color)
{
    pixel->red = color;
    pixel->green = color;
    pixel->blue = color;
    return pixel;
}

void printPixel(Pixel *pixel)
{
    printf("Pixel: (r=%d;g=%d;b=%d)\n", pixel->red, pixel->green, pixel->blue);
}

void freePixel(Pixel *pixel)
{
    free(pixel);
}