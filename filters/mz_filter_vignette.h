#ifndef ___FILTER_VIGNETTE_H___
#define ___FILTER_VIGNETTE_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterVignette(byte* yuv, int width, int height);
void		mz_imageFilterVignette(byte* image, int width, int height, int depth);

#endif