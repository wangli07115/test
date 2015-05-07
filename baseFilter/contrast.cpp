#include <math.h>
#include "baseFilters.h"

void contrast(unsigned char *image, int width, int height, int depth, int stride, int scale) {
    register int i, j;
    float m =  (float)pow(2, scale/100.);
    float c =  127- m*127;
    register unsigned char *pBuf;

    for (j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for (i = 0; i < width; i++, pBuf += depth) {
            PR(pBuf) = CLAMP((int)(m*PR(pBuf)+c));
            PG(pBuf) = CLAMP((int)(m*PG(pBuf)+c));
            PB(pBuf) = CLAMP((int)(m*PB(pBuf)+c));
        }
    }
}

void contrastNew(unsigned char *image, int width, int height, int depth, int stride, int scale) {
    register int i, j;
    register unsigned char *pBuf = NULL;
    float tempB;
    unsigned char gTable[256] = {0};

    for (i = 0; i < 256; i++) {
        tempB = i - sin(i * 6.2832f / 255) * scale / 4 + 0.5f;
        gTable[i] = (unsigned char)CLAMP(tempB);
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
