#include <math.h>
#include "baseFilters.h"

void vibrance(unsigned char *image, int width, int height, int depth, int stride, float scale) {
    int i, j;
    unsigned char  *pBuf;
    float Vib = scale / 100.f;
    float mVibranceTab[512] = {0};
    int r, g, b, gray;
    int maxgb, red;
    float gs, sx, S;
    int dr,dg,db;

    for(i = -255; i < 256; i++) {
        mVibranceTab[i + 255] = Vib / (1+exp(-(i / 256.0f)*3));
    }

    for(j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for(i = 0; i < width; i++, pBuf += depth, pBuf += depth) {
            r = PR(pBuf);
            g = PG(pBuf);
            b = PB(pBuf);

            gray = GRAY(r, g, b);

            if(g > b) {
                maxgb = g;
            } else {
                maxgb = b;
            }

            red = r - maxgb + 255;

            sx = mVibranceTab[red];

            dr = (int)(r + (r - gray) * sx);
            dg = (int)(g + (g - gray) * sx);
            db = (int)(b + (b - gray) * sx);

            PR(pBuf)  = CLAMP(dr);
            PG(pBuf) = CLAMP(dg);
            PB(pBuf)  = CLAMP(db);
        }
    }
}
