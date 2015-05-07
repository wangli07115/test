#ifndef ___FILTER_MOONLIGHT_H___
#define ___FILTER_MOONLIGHT_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterMoonlight(byte* yuv, int width, int height);
void		mz_imageFilterMoonlight(byte* image, int width, int height, int depth);

#endif