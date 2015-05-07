#ifndef ___FILTER_PRINT_H___
#define ___FILTER_PRINT_H___

#include "mz_filter_table.h"
#include "mz_fastprocess.h"

void		mz_filterPrint(byte* yuv, int width, int height);
void		mz_imageFilterPrint(byte* image, int width, int height, int depth);

#endif