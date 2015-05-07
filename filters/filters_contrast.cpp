#include <stdio.h>
#include <math.h>
#include "filters.h"

static byte gTable[256] = {0};
void filters_contrast(unsigned char *image, int width, int height, int depth, int stride, int icontrast) {
	register int i, j;
	register unsigned char *pBuf = NULL;
	float tempB;

	if (icontrast > 0) {
		icontrast = icontrast * 2;
	}else{
		icontrast = icontrast * 80 / 100;
	}

	for (i = 0; i < 256; i++) {
		tempB = i - sin(i * 6.2832f / 255) * icontrast / 4 + 0.5f;
		gTable[i] = CLAMP(tempB);
	}

	for(j = 0; j < height; j++) {
		pBuf = image + j * stride;
		for(i = 0; i < width; i++, pBuf += depth) {
	    	PR(pBuf) = *(gTable + PR(pBuf));
	    	PG(pBuf) = *(gTable + PG(pBuf));
	    	PB(pBuf) = *(gTable + PB(pBuf));
		}
	}

}
