#ifndef ___FILTER_MULTIPLY_H___
#define ___FILTER_MULTIPLY_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterMultiply(byte* yuv, int width, int height);
void		mz_imageFilterMultiply(byte* image, int width, int height, int depth);

#endif