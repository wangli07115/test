#include <math.h>
#include "baseFilters.h"

void exposure(unsigned char *image, int width, int height, int depth, int stride, int scale) {
    register int i, j;
    register unsigned char *pBuf;
    int m =   (255 - scale);

    for (j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for (i = 0; i < width; i++, pBuf += depth) {
            PR(pBuf) = CLAMP((int)(255 * PR(pBuf) / m));
            PG(pBuf) = CLAMP((int)(255 * PG(pBuf) / m));
            PB(pBuf) = CLAMP((int)(255 * PB(pBuf) / m));
        }
    }
}
