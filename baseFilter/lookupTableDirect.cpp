#include "baseFilters.h"

//colorTable[256 * 3]
void lookupTableDirect(unsigned char *image, unsigned char *colorTable ,int width, int height, int depth, int stride) {
	register int i, j;
	register unsigned char *pBuf;
	int r, g, b;

	for (j = 0; j < height; j++) {
		pBuf = image + j * stride;
		for (i = 0; i < width; i++, pBuf += depth) {
			r = PR(pBuf);
			g = PG(pBuf);
			b = PB(pBuf);
			PR(pBuf)	= colorTable[r * 3];
			PG(pBuf)	= colorTable[g * 3 + 1];
			PB(pBuf)	= colorTable[b * 3 + 2];
		}
	}
}
