#ifndef ___FILTER_BW_H___
#define ___FILTER_BW_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterBlackAndWhite(byte* yuv, int width, int height);
void		mz_imageFilterBlackWhite(byte* image, int width, int height, int depth);

#endif