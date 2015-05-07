#include <stdio.h>
#include "filters.h"

void filters_gray(unsigned char *image, int width, int height, int depth, int stride) {
	register int i, j;
	register int gray;
	register unsigned char *pBuf = NULL;

	for(j = 0; j < height; j++) {
		pBuf = image + j * stride;
		for(i = 0; i < width; i++, pBuf += depth) {
			//	((19595 * r + 38470 * g + 7471 * b) >> 16)
			gray =  (PR(pBuf) * 38 + PG(pBuf) * 75 + PB(pBuf) * 15) >> 7;
			gray = CLAMP(gray);
			PR(pBuf)	= gray;
			PG(pBuf)	= gray;
			PB(pBuf)	= gray;
		}
	}
}
