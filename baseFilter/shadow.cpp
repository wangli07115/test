#include "baseFilters.h"

void shadowsScale(unsigned char  *src, unsigned char *dst, int *shadowTable) {
    int iMin,iMax;
    int r, g, b;
    short v0,v1;
    float fShadows = 1.0f;

    r = PR(src);
    g = PG(src);
    b = PB(src);

    if (r > g) {
        iMax = r > b ? r : b;
        iMin = g < b ? g : b;
    } else {
        iMax = g > b ? g : b;
        iMin = r < b ? r : b;
    }

    v0 = iMax ;

    if(v0 == 0) {
        PR(dst) = 0;
        PG(dst) = 0;
        PB(dst) = 0;
        return;
    }

    v1 = shadowTable[v0];
    fShadows = v1 * 1.f / v0;

    PR(dst)  = (unsigned char)CLAMP(PR(src)  * fShadows);
    PG(dst)  = (unsigned char)CLAMP(PG(src) * fShadows);
    PB(dst)  = (unsigned char)CLAMP(PB(src) * fShadows);
}

double fastevalPoly(double *poly,int n, double x)
{
    double f = x;
    double sum = poly[0] + poly[1] * f;
    int i;
    for (i = 2; i < n; i++) {
        f *= x;
        sum += poly[i] * f;
    }
    return sum;
}

void shadow(unsigned char *image, int width, int height, int depth, int stride, float scale) {
    int i, j;
    unsigned char  *pBuf;
    double shadowFilterMap[] = {
            -0.00591,  0.0001,
             1.16488,  0.01668,
            -0.18027, -0.06791,
            -0.12625,  0.09001,
             0.15065, -0.03897
    };
    double s = (scale>=0)?scale:scale/5;
    double poly[5] = {0.0};
    int tempv;
    for (i = 0; i < 5; i++) {
        poly[i] = fastevalPoly(shadowFilterMap+i*2, 2, s);
    }

    int shadowTable[256];
    for (i = 0; i < 256; i++) {
        tempv = (int)(fastevalPoly(poly, 5, i /255.0) * 255);
        shadowTable[i] = tempv < 0 ? 0 : tempv > 255 ? 255 : tempv;
    }

    for(j = 0; j < height; j++) {
        pBuf = image + j * stride;
        for(i = 0; i < width; i++, pBuf += depth) {
            shadowsScale(pBuf, pBuf, shadowTable);
        }
    }
}

