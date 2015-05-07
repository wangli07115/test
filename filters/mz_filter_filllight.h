#ifndef ___FILTER_FILLLIGHT_H___
#define ___FILTER_FILLLIGHT_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterFilllight(byte* yuv, int width, int height);
void		mz_imageFilterFilllight(byte* image, int width, int height, int depth);

#endif