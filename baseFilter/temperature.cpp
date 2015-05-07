#include "baseFilters.h"


void temperature(unsigned char *image, int width, int height, int depth, int stride, float scale) {
    int tableR[256] = {0};
    int tableG[256] = {0};
    int tableB[256] = {0};

    register int i, j;
    int r, g, b, maxRGB, value;
    unsigned char *pBuf = NULL;

    i = 256;
    if(scale > 0.0) {
        while(i--) {
            value = (int)(i * (255 - i) * scale / 255);
            tableR[i] = i + value;
            tableB[i] = i - value;
            tableG[i] = i + value / 4;
        }
    }
    else {
        while(i--) {
            value = (int)(i * (255 - i) * scale / 255);
            tableR[i] = i + value;
            tableB[i] = i - value;
            tableG[i] = i;
        }
    }

    for (j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for (i = 0; i < width; i++, pBuf += depth) {
            r = *(tableR + PR(pBuf));
            g = *(tableG + PG(pBuf));
            b = *(tableB + PB(pBuf));

            maxRGB = MAX(r, MAX(g, b));
            if (maxRGB > 255) {
                PR(pBuf) = CLAMP(r * 255 / maxRGB);
                PG(pBuf) = CLAMP(g * 255 / maxRGB);
                PB(pBuf) = CLAMP(b * 255 / maxRGB);
            }
            else {
                PR(pBuf) = CLAMP(r);
                PG(pBuf) = CLAMP(g);
                PB(pBuf) = CLAMP(b);
            }
        }
    }

}
