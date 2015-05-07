#ifndef ___MZ_FILTER_BLACKWHITE_H___
#define ___MZ_FILTER_BLACKWHITE_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterBlackWhite(byte* yuv, int width, int height);
void		mz_imageFilterBlackWhite(byte* image, int width, int height, int depth);

#endif