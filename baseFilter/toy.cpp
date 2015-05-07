#include <stdio.h>
#include "baseFilters.h"

void toy(unsigned char *image, int width, int height, int depth, int stride) {
    register int i, j;
    register int r = 0, g = 0, b = 0, gray = 0;
    float rr = 0, gg = 0, bb = 0, max = 0;
    register unsigned char *pBuf;

    for (j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for (i = 0; i < width; i++, pBuf += depth) {

            r = PR(pBuf);
            g = PG(pBuf);
            b = PB(pBuf);

            gray = GRAY(r,g, b);
            r = gray + ((430 * (r - gray)) >> 8);
            g = gray + ((430 * (g - gray)) >> 8);
            b = gray + ((430 * (b - gray)) >> 8);

            r = CLAMP(r);
            g = CLAMP(g);
            b = CLAMP(b);

            rr = r / 255.f;
            gg = g / 255.f;
            bb = b / 255.f;

            rr = rr + rr * (1.f - rr) * 0.2f;//rr <=1.0
            bb = bb - bb * (1.f - bb) * 0.2f;//bb <= 1.0
            gg = gg + gg * (1.f - gg) * 0.05f;//gg <=1.0

            PR(pBuf) = (int)(rr * 255);
            PG(pBuf) = (int)(gg * 255);
            PB(pBuf) = (int)(gg * 255);
        }
    }
}
