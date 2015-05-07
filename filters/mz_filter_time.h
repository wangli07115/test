#ifndef ___FILTER_TIME_H___
#define ___FILTER_TIME_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterTime(byte* yuv, int width, int height);
void		mz_imageFilterTime(byte* image, int width, int height, int depth);

#endif