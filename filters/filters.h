/*
******WANGLI*******
*/
#ifndef ___FILTERS_H___
#define ___FILTERS_H___

typedef unsigned char			byte;

#define CLAMP(value)			(value < 0 ? 0 : value > 255 ? 255 : (value))

#define GRAY(r, g, b)											((19595 * r + 38470 * g + 7471 * b) >> 16)

#if defined(WIN32) || defined(WIN64)
	#define PR(color)				*(color + 2)
	#define PG(color)				*(color + 1)
	#define PB(color)				*(color)
#else
	#define PR(color)				*(color)
	#define PG(color)				*(color + 1)
	#define PB(color)				*(color + 2)
	#define PA(c)										(*(c + 3))
#endif

void filters_gaussiir(unsigned char* image, int width, int height, int depth, int stride, float horz, float vert);

void filters_film(unsigned char *image, int width, int height);
void filters_toy(unsigned char *image, int widht, int height);

void filters_gray(unsigned char *image, int width, int height, int depth, int stride);

void filters_brightness(unsigned char *image, int width, int height, int depth, int stride, int ibright);
void filters_contrast(unsigned char *image, int width, int height, int depth, int stride, int icontrast);

#endif
