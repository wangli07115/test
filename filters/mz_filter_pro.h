#ifndef ___FILTER_PRO_H___
#define ___FILTER_PRO_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterPRO(byte* yuv, int width, int height);
void		mz_imageFilterPRO(byte* image, int width, int height, int depth);

#endif