#ifndef ___FILTER_TABLE_H___
#define ___FILTER_TABLE_H___

#include "../core/mz_datatype.h"
#include <stdio.h>
#include <stdlib.h>

typedef void		(*FilterProcess)(byte* yuv, int width, int height);
typedef void		(*ImageFilterProcess)(byte* image, int width, int height, int depth);


typedef enum
{
	FILTER_TYPE_START					=		0x100,
	FILTER_TYPE_BW,
	FILTER_TYPE_BRIGHTNESS,
	FILTER_TYPE_FILLLIGHT,
	FILTER_TYPE_PRINT,
	FILTER_TYPE_PRO,
	FILTER_TYPE_GREENHOUSE,
	FILTER_TYPE_MOONLIGHT,
	FILTER_TYPE_SUNNY,
	FILTER_TYPE_TOY,
	FILTER_TYPE_VIGNETTE,
	FILTER_TYPE_LOMO,
	FILTER_TYPE_MULTIPLY,
	FILTER_TYPE_TIME,
	FILTER_TYPE_END,
};

void		mz_filterProcess(int filterType, byte* yuv, int width, int height);
void		mz_imageFilterProcess(int filterType, byte* image, int width, int height, int depth);

#define YUYV_PY1(c)				(*(c))
#define YUYV_PU(c)				(*(c + 1))
#define YUYV_PY2(c)				(*(c + 2))
#define YUYV_PV(c)				(*(c + 3))

#define RGB_PR(c)				(*(c + 2))
#define RGB_PG(c)				(*(c + 1))
#define RGB_PB(c)				(*(c))

#endif
