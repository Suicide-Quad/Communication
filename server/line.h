#ifndef LINE_H
#define LINE_H

#include "Struct/image.h"

struct Line
{
	double theta;
	double rho;
	double score;
	int x1;
	int y1;
	int x2;
	int y2;
};
typedef struct Line Line;

Line *initLine(double theta, double rho, double score, int x1, int y1, int x2, int y2);
Image *drawLine(Image *image, Line *line);

void setPixel(Pixel *pixel);

#endif
