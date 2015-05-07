#ifndef ___FILTER_SUNNY_H___
#define ___FILTER_SUNNY_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterSunny(byte* yuv, int width, int height);
void		mz_imageFilterSunny(byte* image, int width, int height, int depth);

#endif