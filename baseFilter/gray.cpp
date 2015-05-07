#include "baseFilters.h"

void gray(unsigned char *image, int width, int height, int depth, int stride) {
    register int i, j;
    register unsigned char gray;
    register unsigned char *pBuf;

    for (j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for (i = 0; i < width; i++, pBuf += depth) {
            gray = (unsigned char)GRAY(PR(pBuf), PG(pBuf), PB(pBuf));
            PR(pBuf) = gray;
            PG(pBuf) = gray;
            PB(pBuf) = gray;
        }
    }
}
