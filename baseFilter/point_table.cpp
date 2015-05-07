#include "baseFilters.h"

static int  interp(unsigned char  *src, int p , int *off ,float dr,float dg, float db) {
    float fr00 = (src[p + off[0]]) * (1-dr) + (src[p + off[1]]) * dr;
    float fr01 = (src[p + off[2]]) * (1-dr) + (src[p + off[3]]) * dr;
    float fr10 = (src[p + off[4]]) * (1-dr) + (src[p + off[5]]) * dr;
    float fr11 = (src[p + off[6]]) * (1-dr) + (src[p + off[7]]) * dr;
    float frb0 = fr00 * (1 - db) + fr01 * db;
    float frb1 = fr10 * (1 - db) + fr11 * db;
    float frbg = frb0 * (1 - dg) + frb1 * dg;
    int value = (int)frbg;

    value =CLAMP(value);
    return value;
}

//colorTable[stepr * stepg * stepb]
void point_table(unsigned char *image, unsigned char *colorTable, int width, int height, int depth, int stride, int stepr, int stepg, int stepb) {
    int i, j;
    static int STEP = 3;
    unsigned char  *pBuf;

    int r, g, b;
    int lut_r, lut_g, lut_b;
    float fr, fg, fb;
    float dr, dg, db;
    int p;
    int off[8] = {
            STEP * 0 ,                                     STEP * 1,
            STEP * (stepb) ,                            STEP * (stepb+ 1),
            STEP * (stepb * stepg) ,                STEP * (stepb * stepg + 1),
            STEP * (stepb * stepg + stepb),    STEP * (stepb * stepg + stepb +1)};

    float scaler = (stepr - 1) / 256.f;
    float scaleg = (stepg - 1) / 256.f;
    float scaleb = (stepb - 1) / 256.f;

    for(j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for(i = 0; i < width; i++, pBuf += depth) {
            r = PR(pBuf);
            g = PG(pBuf);
            b = PB(pBuf);

            fb = b*scaler;
            fg = g*scaleg;
            fr = r*scaleb;
            lut_b = (int)fb;
            lut_g = (int)fg;
            lut_r = (int)fr;
            dr = fr-lut_r;
            dg = fg-lut_g;
            db = fb-lut_b;

            p = lut_b + lut_g * stepb + lut_r * stepg * stepb;
            p *= STEP;

            PR(pBuf) = interp(colorTable,p  ,off,dr,dg,db);
            PG(pBuf) = interp(colorTable,p+1,off,dr,dg,db);
            PB(pBuf) = interp(colorTable,p+2,off,dr,dg,db);
        }
    }
}
