#ifndef ___FILTER_LOMO_H___
#define ___FILTER_LOMO_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterLomo(byte* yuv, int width, int height);
void		mz_imageFilterLomo(byte* image, int width, int height, int depth);

#endif