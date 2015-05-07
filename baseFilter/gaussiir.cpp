#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "baseFilters.h"

#define  PI         3.1415926535898

typedef struct gaussiirContext
{
    float np[5];
    float nm[5];
    float dp[5];
    float dm[5];
    float bp[5];
    float bm[5];
    float *vp;
    float *vm;
}gaussiirContext, *pGaussiirContext;

static void  calculate_gauss_iir_coefficients(struct gaussiirContext *ctx, float std_dev) {
    int i;
    float sum_np, sum_nm, sum_d, a, b;
    const float div = (float)(sqrt(2.0f * PI) * std_dev);
    const float x0 = -1.783f / std_dev;
    const float x1 = -1.723f / std_dev;
    const float x2 = 0.6318f / std_dev;
    const float x3 = 1.997f  / std_dev;
    const float x4 = 1.6803f / div;
    const float x5 = 3.735f / div;
    const float x6 = -0.6803f / div;
    const float x7 = -0.2598f / div;

    ctx->np[0] = x4 + x6;
    ctx->np[1] = (exp(x1) * (x7 * sin(x3)-(x6 + 2 * x4) * cos(x3)) + exp(x0) * (x5 * sin(x2) - (2 * x6 + x4) * cos (x2)));
    ctx->np[2] = (2 * exp(x0 + x1) * ((x4 + x6) * cos(x3) * cos(x2) - x5 * cos(x3) * sin(x2) - x7 * cos(x2) * sin(x3)) + x6 * exp(2 * x0) + x4 * exp(2 * x1));
    ctx->np[3] = (exp(x1 + 2 * x0) * (x7 * sin(x3) - x6 * cos(x3)) + exp(x0 + 2 * x1) * (x5 * sin(x2) - x4 * cos(x2)));
    ctx->np[4] = 0.0;

    ctx->dp [0] = 0.0;
    ctx->dp [1] = -2 * exp(x1) * cos(x3) - 2 * exp(x0) * cos (x2);
    ctx->dp [2] = 4 * cos(x3) * cos(x2) * exp(x0 + x1) +  exp(2 * x1) + exp(2 * x0);
    ctx->dp [3] = -2 * cos(x2) * exp(x0 + 2 * x1) -  2 * cos(x3) * exp(x1 + 2 * x0);
    ctx->dp [4] = exp(2 * x0 + 2 * x1);

    for (i = 0; i <= 4; i++) {
        ctx->dm[i] = ctx->dp[i];
    }

    ctx->nm[0] = 0.0;
    for (i = 1; i <= 4; i++) {
        ctx->nm[i] = ctx->np[i] - ctx->dp[i] * ctx->np[0];
    }

    sum_np = 0.0;
    sum_nm = 0.0;
    sum_d = 0.0;

    for (i = 0; i <= 4; i++) {
        sum_np += ctx->np[i];
        sum_nm += ctx->nm[i];
        sum_d += ctx->dp[i];
    }

    a = sum_np / (1.0f + sum_d);
    b = sum_nm / (1.0f + sum_d);

    for (i = 0; i <= 4; i++) {
        ctx->bp[i] = ctx->dp[i] * a;
        ctx->bm[i] = ctx->dm[i] * b;
    }
}

static void  merge_pixels(float *src1, float *src2, byte *dst, int depth, int width) {
    register int i, k;
    float sum = 0;

    for(i = 0; i < width; i++, src1 += depth, src2 += depth, dst += depth) {
        for(k = 0; k < 3; k++) {
            sum = src1[k] + src2[k];
            dst[k] = sum > 255 ? 255 : sum < 0 ? 0 : (unsigned char)(sum);
        }
    }
}

static void  blur_line(gaussiirContext *ctx, byte *src, byte *dst, int width, int depth) {
    register unsigned char *sp = NULL, *sm = NULL;
    register float *vp = NULL, *vm = NULL;
    register float *pptr = NULL, *mptr = NULL;
    register float *np = ctx->np;
    register float *dp = ctx->dp;
    register float *nm = ctx->nm;
    register float *dm = ctx->dm;
    register float *bp = ctx->bp;
    register float *bm = ctx->bm;
    int i, j, t, terms, col;
    unsigned char initial_p[4];
    unsigned char initial_m[4];

    memset(ctx->vp, 0, width * depth * sizeof(float));
    memset(ctx->vm, 0, width * depth * sizeof(float));

    sp = src;
    sm = src + (width - 1) * depth;
    vp = ctx->vp;
    vm = ctx->vm + (width - 1) * depth;

    for (i = 0; i < depth; i++) {
        initial_p[i] = sp[i];
        initial_m[i] = sm[i];
    }

    for (col = 0; col < 4; col++) {
        terms = col;
        for (t = 0; t < 3; t++) {
            pptr = vp + t;
            mptr = vm + t;
            for (i = 0; i <= terms; i++) {
                *pptr += np[i] * sp[(-i * depth) + t] - dp[i] * vp[(-i * depth) + t];
                *mptr += nm[i] * sm[(i * depth) + t] - dm[i] * vm[(i * depth) + t];
            }
            for (j = i; j <= 4; j++) {
                *pptr += (np[j] - bp[j]) * initial_p[t];
                *mptr += (nm[j] - bm[j]) * initial_m[t];
            }
        }
        sp += depth;
        sm -= depth;
        vp += depth;
        vm -= depth;
    }

    terms = 4;
    for (col = 4; col < width; col++) {
        for (t = 0; t < 3; t++) {
            pptr = vp + t;
            mptr = vm + t;
            for (i = 0; i <= terms; i++) {
                *pptr += np[i] * sp[(-i * depth) + t] - dp[i] * vp[(-i * depth) + t];
                *mptr += nm[i] * sm[(i * depth) + t] - dm[i] * vm[(i * depth) + t];
            }
        }
        sp += depth;
        sm -= depth;
        vp += depth;
        vm -= depth;
    }
    merge_pixels(ctx->vp, ctx->vm, dst, depth, width);
}

void gaussiir(byte *image, int width, int height, int depth, int stride, float horz, float vert) {
    register int i, j;
    register byte *src = NULL, *dp = NULL, *ds = NULL;
    int len = (width > height ? width : height) * depth;
    float radius, sigma;
    gaussiirContext ctx;

    if(horz <= 0 || vert <= 0) {
        return;
    }
    src = (unsigned char *)malloc(len * sizeof(unsigned char));
    ctx.vp = (float *) malloc(len * sizeof(float));
    ctx.vm = (float *) malloc(len * sizeof(float));

    if (NULL == image || NULL == ctx.vp || NULL == ctx.vm || NULL == src) {
        goto gaussiir_error;
    }

    radius = horz + 1.0f;
    sigma = sqrt(radius * radius / (2.0f * log(255.0f)));
    calculate_gauss_iir_coefficients(&ctx, sigma);

    for (j = 0; j < width; j++) {
        ds = src;
        dp = image + j * depth;
        for (i = 0; i < height; i++, ds += depth, dp += stride) {
            memcpy(ds, dp, depth);
        }
        blur_line(&ctx, src, src, height, depth);

        ds = src;
        dp = image + j * depth;
        for (i = 0; i < height; i++, ds += depth, dp += stride) {
            memcpy(dp, ds, depth);
        }
    }

    if(vert != horz) {
        radius = vert + 1.0f;
        sigma = sqrt(radius * radius / (2.0f * log(255.0f)));
        calculate_gauss_iir_coefficients(&ctx, sigma);
    }

    for (j = 0; j < height; j++) {
        dp= image + j * stride;
        blur_line(&ctx, dp, dp, width, depth);
    }

gaussiir_error:
    if(NULL != src) {
        free(src);
        src = NULL;
    }
    if(NULL != ctx.vm) {
        free(ctx.vm);
        ctx.vm = NULL;
    }
    if(NULL != ctx.vp) {
        free(ctx.vp);
        ctx.vp = NULL;
    }
}
