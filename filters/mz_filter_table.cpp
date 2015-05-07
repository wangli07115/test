#include "mz_filter_table.h"
#include "mz_filter_bw.h"
#include "mz_filter_brightness.h"
#include "mz_filter_filllight.h"
#include "mz_filter_print.h"
#include "mz_filter_pro.h"
#include "mz_filter_greenhouse.h"
#include "mz_filter_moonlight.h"
#include "mz_filter_sunny.h"
#include "mz_filter_toy.h"
#include "mz_filter_vignette.h"
#include "mz_filter_lomo.h"
#include "mz_filter_multiply.h"
#include "mz_filter_time.h"
#include "../core/mz_debug.h"

#define IS_VALID_FILTER_TYPE(type)							\
					(filterType > FILTER_TYPE_START && filterType < FILTER_TYPE_END)

FilterProcess			gFilterProcessTable[] = 
{
	mz_filterBlackAndWhite,
	mz_filterBrightness,
	mz_filterFilllight,
	mz_filterPrint,
	mz_filterPRO,
	mz_filterGreenHouse,
	mz_filterMoonlight,
	mz_filterSunny,
	mz_filterToy,
	mz_filterVignette,
	mz_filterLomo,
	mz_filterMultiply,
	mz_filterTime,
};

ImageFilterProcess	gImageFilterProcessTable[] = 
{
	mz_imageFilterBlackWhite,
	mz_imageFilterBrightness,
	mz_imageFilterFilllight, 
	mz_imageFilterPrint,
	mz_imageFilterPRO,
	mz_imageFilterGreenHouse,
	mz_imageFilterMoonlight,
	mz_imageFilterSunny,
	mz_imageFilterToy,
	mz_imageFilterVignette,
	mz_imageFilterLomo,
	mz_imageFilterMultiply,
	mz_imageFilterTime,
};

void		mz_filterProcess(int filterType, byte* yuv, int width, int height)
{
	int	index = 0;
	if (NULL != yuv && width > 0 && height > 0 && IS_VALID_FILTER_TYPE(filterTyp)) {
		index = filterType - FILTER_TYPE_START - 1;
		LOGE("filter type:%d start width:%d, height:%d", index, width, height);
		gFilterProcessTable[index](yuv, width, height);
		LOGE("filter type:%d end", index);

	}
}

void		mz_imageFilterProcess(int filterType, byte* image, int width, int height, int depth) 
{
	int	index = 0;
	if (NULL != image && width > 0 && height > 0 && IS_VALID_FILTER_TYPE(filterTyp)) {
		index = filterType - FILTER_TYPE_START - 1;
		LOGE("filter type:%d start width:%d, height:%d", index, width, height);
		gImageFilterProcessTable[index](image, width, height, depth);
		LOGE("filter type:%d end", index);
	}
};