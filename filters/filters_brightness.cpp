#include <stdio.h>
#include <math.h>
#include "filters.h"
static byte gTable[256] = {0};

void filters_brightness(unsigned char *image, int width, int height, int depth, int stride, int ibright) {
	register int i, j;
	register unsigned char *pBuf = NULL;
	double c;
	int tempC;

	if (ibright > 0) {
		ibright = ibright * 150 / 100;
		c = 0.000172 * ibright * ibright + 0.001093 * ibright + 0.2;
		for (i = 0; i < 256; i++) {
			tempC = (int)(255 - (255 - i) * pow(1 - i / 255.0, c) - 0.5);
			gTable[i] = CLAMP(tempC);
		}
	}
	else {
		ibright = ibright * 180 / 100;
		c = 0.00001077 * ibright * ibright + 0.00582 * ibright + 0.88;
		for (i = 0; i < 256; i++) {
			tempC = (int)(255 - 255 * pow(1 - i / 255.0, c) - 0.5);
			gTable[i] = CLAMP(tempC);
		}
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
