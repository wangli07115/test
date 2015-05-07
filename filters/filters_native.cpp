#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "../core/native.h"
#include "mz_filter_film.h"
#include "mz_filter_blackwhite.h"
#include "gauss_iir.h"



NATIVE(void) FUNCTION(filtersForm, nativeFilters, jbyte *imageSrc, jbyte *imageDst,jint width, jint height,
					  jint radiusX,jint radiusY, int filterType){
	int depth, stride;	
	//android(YuYv):depth = 2;stride = width * depth;
	//android(ARGB):depth = 4;stride = width * depth;
	//c#..(RGB):
	depth = 3;
	stride = width * 3;
	while(stride % 4 != 0) {
		stride = (stride / 4 + 1) * 4;
	}

	switch (filterType)
	{
	case 0:
	//	filters_gaussiir(image, width, height, depth, stride, horz, vert);

	//	gauss_iir(imageSrc, imageDst, width, height, 3, stride, radiusX, radiusX);
		break;
	default:
	//	memcpy(imageDst, imageSrc, stride * height);
		break;
	}
}


NATIVE(void) FUNCTION(filtersForm, filterFilm, jbyte *imageYUV, jint width, jint height){
	mz_filterFilm(imageYUV, width, height);
}

NATIVE(void) FUNCTION(filtersForm, filterBlackWhite, jbyte *imageYUV, jint width, jint height){
	mz_filterBlackWhite(imageYUV, width, height);
}
