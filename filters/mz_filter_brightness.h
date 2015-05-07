#ifndef ___FILTER_BRIGHTNESS_H___
#define ___FILTER_BRIGHTNESS_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterBrightness(byte* yuv, int width, int height);
void		mz_imageFilterBrightness(byte* image, int width, int height, int depth);

#endif