#include "baseFilters.h"

unsigned char  interp(const unsigned char  *src, int p , int *off ,float dr,float dg, float db) {
    unsigned char ret = 0;
    float fr00 = (src[p+off[0]])*(1-dr)+(src[p+off[1]])*dr;
    float fr01 = (src[p+off[2]])*(1-dr)+(src[p+off[3]])*dr;
    float fr10 = (src[p+off[4]])*(1-dr)+(src[p+off[5]])*dr;
    float fr11 = (src[p+off[6]])*(1-dr)+(src[p+off[7]])*dr;
    float frb0 = fr00 * (1-db)+fr01*db;
    float frb1 = fr10 * (1-db)+fr11*db;
    float frbg = frb0 * (1-dg)+frb1*dg;

    frbg = frbg < 0 ? 0 : frbg > 255 ? 255 : (frbg);

    ret = (unsigned char)frbg;

    return ret;
}

void fx(unsigned char *image, const unsigned char *colorTable, int width, int height, int depth, int stride) {
    int i, j;
    static int STEP = 3;
    float scale = 15 / 256.f;
    static int off[8] =  {0, 3, 48, 51, 768, 771, 816, 819};
    unsigned char  *pBuf;

    int r, g, b;
    int lut_r, lut_g, lut_b;
    float fr, fg, fb;
    float dr, dg, db;
    int p;

    for(j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for(i = 0; i < width; i++, pBuf += depth) {

            r = PR(pBuf);
            g = PG(pBuf);
            b = PB(pBuf);

            fb = b*scale;
            fg = g*scale;
            fr = r*scale;
            lut_b = (int)fb;
            lut_g = (int)fg;
            lut_r = (int)fr;
            dr = fr-lut_r;
            dg = fg-lut_g;
            db = fb-lut_b;

            p = lut_r+lut_b * 16+lut_g * 256;
            p *= STEP;

            PR(pBuf) = interp(colorTable,p  ,off,dr,dg,db);
            PG(pBuf) = interp(colorTable,p+1,off,dr,dg,db);
            PB(pBuf) = interp(colorTable,p+2,off,dr,dg,db);
        }
    }
}
