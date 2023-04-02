#include "Struct/image.h"
#include "Struct/pixel.h"
#include "Struct/line.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "Struct/line.h"

Image *createEmptyImage(int width, int height)
{
  Image *image = malloc(sizeof(Image));
  if (image == NULL)
    errx(-1, "Error while allocating a image (createEmptyImage)");

  image->width = width;
  image->height = height;
  image->pixels = calloc(width, sizeof(Pixel *));

  if (image->pixels == NULL)
    errx(-1, "Error while allocating all the pixel (createEmptyImage)");

  for (int x = 0; x < width; x++)
  {
    image->pixels[x] = calloc(height, sizeof(Pixel *));
    if (image->pixels[x] == NULL)
      errx(-1, "Error while allocating a row pixel (createEmptyImage)");
  }

  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
      image->pixels[x][y] = *initPixel(255, 255, 255);
  }

  return image;
}

void cleanImage(Image* image)
{
    for(int y = 0; y < 4; y++)
    {
      for(int x = 0; x < 28; x++)
      {
        setPixel(&image->pixels[x][y]);
        setPixel(&image->pixels[y][x]);
        setPixel(&image->pixels[27 - x][y]);
        setPixel(&image->pixels[27 - y][x]);
      }
    }
  }

Image *importImage(char *filename)
{
  SDL_Surface *surface = IMG_Load(filename);

  if (surface == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);

  if (surface == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  Image *image = createEmptyImage(surface->w, surface->h);
  Uint32 *pixels = surface->pixels;
  SDL_PixelFormat *format = surface->format;

  if (SDL_LockSurface(surface) != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  for (int x = 0; x < image->width; x++)
  {
    for (int y = 0; y < image->height; y++)
    {
      Pixel *pixel = &image->pixels[x][y];
      SDL_GetRGB(pixels[y * image->width + x], format, &pixel->red, &pixel->green, &pixel->blue);
    }
  }

  SDL_UnlockSurface(surface);
  SDL_FreeSurface(surface);
  return image;
}

void saveImage(Image *image, char *filename)
{
  SDL_Surface *surface = SDL_CreateRGBSurface(0, image->width, image->height, 32, 0, 0, 0, 0);

  if (surface == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);

  if (surface == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_PixelFormat *format = surface->format;
  Uint32 *pixels = surface->pixels;

  if (SDL_LockSurface(surface) != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  for (int x = 0; x < image->width; x++)
  {
    for (int y = 0; y < image->height; y++)
    {
      Pixel *pixel = &image->pixels[x][y];
      pixels[y * image->width + x] = SDL_MapRGB(format, pixel->red, pixel->green, pixel->blue);
    }
  }

  SDL_UnlockSurface(surface);

  if (SDL_SaveBMP(surface, filename) != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_FreeSurface(surface);
}

void freeImage(Image *image)
{
  for (int x = 0; x < image->width; x++)
    free(image->pixels[x]);

  free(image->pixels);
  free(image);
}
