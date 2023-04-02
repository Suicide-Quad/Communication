#include "Struct/line.h"
#include <err.h>

Line *initLine(double theta, double rho, double score, int x1, int y1, int x2, int y2)
{
    Line *line = malloc(sizeof(Line));

    if (line == NULL)
        errx(-1, "Error while allocating a pointer (initLine)");

    line->theta = theta;
    line->rho = rho;
    line->score = score;
    line->x1 = x1;
    line->x2 = x2;
    line->y1 = y1;
    line->y2 = y2;

    return line;
}

/*Image *drawLine(Image *image, Line *line)
{
    double a = ((double) line->y1 - (double)line->y2) / ((double)line->x1 - (double)line->x2);
    double b = line->y1 - a*line->x1;
    for(int x = 0; x < image->width; x++)
    {
        double y = a*(double)x + b;
        if(y >= 0 && y < image->height)
        {
            image->pixels[x][(int) y].red = 255;
            image->pixels[x][(int) y].blue = 0;
            image->pixels[x][(int) y].green = 0;
        }
    }
    return image;
}*/

void setPixelRed(Pixel *pixel)
{
    pixel->blue = 0;
    pixel->red = 255;
    pixel->green = 0;
}

void setPixelGreen(Pixel *pixel)
{
    pixel->blue = 0;
    pixel->red = 0;
    pixel->green = 255;
}

void setPixel(Pixel *pixel)
{
    pixel->blue = 255;
    pixel->red = 255;
    pixel->green = 255;
}

Image *drawLine(Image *image, Line *line)
{
    int x0 = line->x1;
    int y0 = line->y1;

    int x1 = line->x2;
    int y1 = line->y2;

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        if (0 <= x0 && x0 < image->width && 0 <= y0 && y0 < image->height)
        {
            if (image->pixels[x0][y0].red == 0 && image->pixels[x0][y0].green != 255)
            {
                setPixelRed(&image->pixels[x0][y0]);
            }
            else
            {
                if(!(image->pixels[x0][y0].green == 0 && image->pixels[x0][y0].blue == 0))
                setPixelGreen(&image->pixels[x0][y0]);
            }
        }
        if (1 <= x0 && x0 < image->width-1 && 1 <= y0 && y0 < image->height -1)
        {
            setPixel(&image->pixels[x0][y0]);
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
    return image;
}