#ifndef PIXEL_H
#define PIXEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Pixel
{
	Uint8 red;
	Uint8 green;
	Uint8 blue;
};
typedef struct Pixel Pixel;

Pixel *initPixel(Uint8 red, Uint8 green, Uint8 blue);
Pixel *updateSameColorPixel(Pixel *pixel, Uint8 color);
void printPixel(Pixel *pixel);
void freePixel(Pixel *pixel);
#endif
