#include "baseFilters.h"
#include "../core/hsv.h"

void lightness(unsigned char *image, int width, int height, int depth, int stride, int scale) {
    register int i, j;
    int dstScale = (int)((scale / 100.f + 1) * 256);
    register unsigned char *pBuf = NULL;

    for(j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for(i = 0; i < width; i++, pBuf += depth) {
            PR(pBuf) = CLAMP((dstScale * PR(pBuf))>>8);
            PG(pBuf) = CLAMP((dstScale * PG(pBuf))>>8);
            PB(pBuf) = CLAMP((dstScale * PB(pBuf))>>8);
        }
    }
}

#define MAX_CHANELS         7
void lightnessNew(unsigned char *image, int width, int height, int depth, int stride, int *scale) {
    int i, j;
    int r = 0, g = 0, b = 0, gray = 0, h, s, v;
    int dstScale = (int)((scale[0] / 100.f) * 256);
    unsigned char* pBuf = NULL;
    int temp = 0;

    int satLut[360];
    int master = scale[0];
    for(i = 0; i < 360; i++) {

       float p =  i * 6.f / 360;
       int ip = ((int)(p + .5f)) % 6;
       int v = master + scale[ip + 1];
       satLut[i] = (int)(256 * (v / 100.f));
    }

    for (j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for (i = 0; i < width; i++, pBuf += depth) {
            r = PR(pBuf);
            g = PG(pBuf);
            b = PB(pBuf);

            rgb2hsv(pBuf, h, s, v);
            dstScale = satLut[h];

            if(dstScale <= 0) {
                temp = MIN3(r, g, b);
                r = r + (((r - temp) * dstScale) >> 8);
                g = g + (((g - temp) * dstScale) >> 8);
                b = b + (((b - temp) * dstScale) >> 8);
            } else {
                temp = MAX3(r, g, b);
                r = r + (((temp - r) * dstScale) >> 8);
                g = g + (((temp - g) * dstScale) >> 8);
                b = b + (((temp - b) * dstScale) >> 8);
            }

            PR(pBuf) = CLAMP(r);
            PG(pBuf) = CLAMP(g);
            PB(pBuf) = CLAMP(b);
        }
    }
}
