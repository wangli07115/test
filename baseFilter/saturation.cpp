#include "baseFilters.h"
#include "../core/hsv.h"

void saturation(unsigned char *image, int width, int height, int depth, int stride, int scale) {
    int i, j;
    int r = 0, g = 0, b = 0, gray = 0;
    int saturation = (int)((scale / 100.f + 1.0f) * 256);
    unsigned char* pBuf = NULL;

    for (j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for (i = 0; i < width; i++, pBuf += depth) {
            r = PR(pBuf);
            g = PG(pBuf);
            b = PB(pBuf);
            gray = GRAY(r, g, b);

            r = gray + ((saturation * (r - gray)) >> 8);
            g = gray + ((saturation * (g - gray)) >> 8);
            b = gray + ((saturation * (b - gray)) >> 8);

            PR(pBuf) = CLAMP(r);
            PG(pBuf) = CLAMP(g);
            PB(pBuf) = CLAMP(b);
        }
    }
}

//scale[7]:主色，红色，黄色，绿色，青色，蓝色，紫红色
#define MAX_CHANELS         7
void saturationNew(unsigned char *image, int width, int height, int depth, int stride, int *scale) {
    int i, j;
    int r = 0, g = 0, b = 0, gray = 0, h, s, v;
    int dstScale = (int)((scale[0] / 100.f + 1.0f) * 256);
    unsigned char* pBuf = NULL;
    int temp, imax, imin;

    int satLut[360];
    int master = scale[0];
    for(i = 0; i < 360; i++) {
       float p =  i * 6.f / 360;
       int ip = ((int)(p + .5f)) % 6;
       int v = master + scale[ip + 1];
       satLut[i] = (int)(256 * (v / 100.f + 1));
    }

    for (j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for (i = 0; i < width; i++, pBuf += depth) {
            r = PR(pBuf);
            g = PG(pBuf);
            b = PB(pBuf);
            gray = GRAY(r, g, b);

            rgb2hsv(pBuf, h, s, v);
            dstScale = satLut[h];

            r = gray + (((r - gray) * dstScale) >> 8);
            g = gray + (((g - gray) * dstScale) >> 8);
            b = gray + (((b - gray) * dstScale) >> 8);

            PR(pBuf) = CLAMP(r);
            PG(pBuf) = CLAMP(g);
            PB(pBuf) = CLAMP(b);
        }
    }

}
