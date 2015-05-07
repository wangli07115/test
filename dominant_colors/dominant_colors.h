#ifndef		___DOMINANT_COLORS_H___
#define	___DOMINANT_COLORS_H___

#include "../core/imagebase.h"
#include "../core/mp_log.h"

void getcolor(int color, unsigned char &red, unsigned char &green, unsigned char &blue);

void  color_analyze(unsigned char* rgba, int width, int height, int edge_size, int& bg_color, int& text_color);

int color_extraction(byte *image, int width, int height, int bpp, int stride, int &rgbValue, int &BlackOrWhite);

//supDominant_colors.cpp---------------------------------------------------------
void  fillcolor(unsigned char* rgba, int width, int height, int color);

void  topEdge(unsigned char* image, int width, int height, int depth, int stride, int color);

#endif
