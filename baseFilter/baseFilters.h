#ifndef         ___BASE_FILTERS_H___
#define         ___BASE_FILTERS_H___

#include "../core/imageBase.h"

void fx(unsigned char *image, const unsigned char *colorTable, int width, int height, int depth, int stride);

void gray(unsigned char *image, int width, int height, int depth, int stride);

void contrast(unsigned char *image, int width, int height, int depth, int stride, int scale);

void contrastNew(unsigned char *image, int width, int height, int depth, int stride, int scale);

void brightness(unsigned char *image, int width, int height, int depth, int stride, int scale);

void brightnessNew(unsigned char *image, int width, int height, int depth, int stride, int scale);

void saturation(unsigned char *image, int width, int height, int depth, int stride, int scale);

void saturationNew(unsigned char *image, int width, int height, int depth, int stride, int *scale);

void temperature(unsigned char *image, int width, int height, int depth, int stride, int scale);

void exposure(unsigned char *image, int width, int height, int depth, int stride, int scale);

void lightness(unsigned char *image, int width, int height, int depth, int stride, int scale);

void lightnessNew(unsigned char *image, int width, int height, int depth, int stride, int *scale);

void gaussiir(unsigned char *image, int width, int height, int depth, int stride, float horz, float vert);
//curve[256]
void curve(unsigned char *curve, float *points, int num);
//curveTab[256 * 3], pointsRGB[num, x,y,x,y....];
void curveRGB(unsigned char *curveTable, float* pointsRGB, float* pointsR, float* pointsG, float* pointsB);

void negative(unsigned char *image, int width, int height, int depth, int stride);

void negativeNew(byte *image, int width, int height, int depth, int stride);

void toy(unsigned char *image, int width, int height, int depth, int stride);

void film(unsigned char *image, int width, int height, int depth, int stride);

void shadow(unsigned char *image, int width, int height, int depth, int stride, float scale);

void vibrance(unsigned char *image, int width, int height, int depth, int stride, float scale);
//colorTable[256 * 3]
void direct_table(unsigned char *image, unsigned char *colorTable ,int width, int height, int depth, int stride);
//colorTable[stepr * stepg * stepb]
void point_table(unsigned char *image, unsigned char *colorTable, int width, int height, int depth, int stride, int stepr, int stepg, int stepb);

void sharpen(unsigned char *image, int width, int height, int radius, int th, int rate);

int Extrusion(unsigned char *image, int width, int height, int centerX, int centerY, int radius, int powIdx);


#endif/*___FILTER_ENGINE_BASE_FILTER_BASE_FILTERS_H___*/
