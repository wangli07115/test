#ifndef ___FILTER_GREENHOUSE_H___
#define ___FILTER_GREENHOUSE_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterGreenHouse(byte* yuv, int width, int height);
void		mz_imageFilterGreenHouse(byte* image, int width, int height, int depth);

#endif