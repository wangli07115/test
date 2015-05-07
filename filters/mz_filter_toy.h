#ifndef ___FILTER_TOY_H___
#define ___FILTER_TOY_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterToy(byte* yuv, int width, int height);
void		mz_imageFilterToy(byte* image, int width, int height, int depth);

#endif