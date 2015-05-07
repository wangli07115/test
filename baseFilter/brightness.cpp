#include <math.h>
#include "baseFilters.h"

void brightness(unsigned char *image, int width, int height, int depth, int stride, int scale) {
    register int i, j;
    register unsigned char *pBuf = NULL;

    for(j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for(i = 0; i < width; i++, pBuf += depth) {
            PR(pBuf) = CLAMP(scale + PR(pBuf));
            PG(pBuf) = CLAMP(scale + PG(pBuf));
            PB(pBuf) = CLAMP(scale + PB(pBuf));
        }
    }
}

void brightnessNew(unsigned char *image, int width, int height, int depth, int stride, int scale) {
    register int i, j;
    register unsigned char *pBuf = NULL;
    double c;
    int tempC;
    unsigned char gTable[256] = {0};

    if (scale > 0) {
        c = 0.000172 * scale * scale + 0.001093 * scale + 0.2;
        for (i = 0; i < 256; i++) {
            tempC = (int)(255 - (255 - i) * pow(1 - i / 255.0, c) - 0.5);
            gTable[i] = CLAMP(tempC);
        }
    }else {
        c = 0.00001077 * scale * scale + 0.00582 * scale + 0.88;
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
