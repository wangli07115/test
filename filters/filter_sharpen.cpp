//#include "filters.h"
//#include "fastprocess.h"

#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef unsigned char byte;

#define CX_PI		3.1415926535898
static float gTabnp[5], gTabnm[5], gTabdp[5], gTabdm[5], gTabbp[5], gTabbm[5];
static float *gVp = NULL, *gVm = NULL;

static void get_gauss_filter_coefficients(float std_dev) {
	const float div = (float)(sqrt(2.0f * CX_PI) * std_dev);
	const float x0 = -1.783f / std_dev;
	const float x1 = -1.723f / std_dev;
	const float x2 = 0.6318f / std_dev;
	const float x3 = 1.997f  / std_dev;
	const float x4 = 1.6803f / div;
	const float x5 = 3.735f / div;
	const float x6 = -0.6803f / div;
	const float x7 = -0.2598f / div;
	int i;
	float sum_np, sum_nm, sum_d, a, b;

	gTabnp[0] = x4 + x6;
	gTabnp[1] = (exp(x1) * (x7 * sin(x3)-(x6 + 2 * x4) * cos(x3)) + exp(x0) * (x5 * sin(x2) - (2 * x6 + x4) * cos (x2)));
	gTabnp[2] = (2 * exp(x0 + x1) * ((x4 + x6) * cos(x3) * cos(x2) - x5 * cos(x3) * sin(x2) - x7 * cos(x2) * sin(x3)) +
		x6 * exp(2 * x0) + x4 * exp(2 * x1));
	gTabnp[3] = (exp(x1 + 2 * x0) * (x7 * sin(x3) - x6 * cos(x3)) + exp(x0 + 2 * x1) * (x5 * sin(x2) - x4 * cos(x2)));
	gTabnp[4] = 0.0;

	gTabdp [0] = 0.0;
	gTabdp [1] = -2 * exp(x1) * cos(x3) -  2 * exp(x0) * cos (x2);
	gTabdp [2] = 4 * cos(x3) * cos(x2) * exp(x0 + x1) +  exp(2 * x1) + exp(2 * x0);
	gTabdp [3] = -2 * cos(x2) * exp(x0 + 2 * x1) -  2 * cos(x3) * exp(x1 + 2 * x0);
	gTabdp [4] = exp(2 * x0 + 2 * x1);

	for (i = 0; i <= 4; i++) {
		gTabdm[i] = gTabdp[i];
	}

	gTabnm[0] = 0.0;
	for (i = 1; i <= 4; i++) {
		gTabnm[i] = gTabnp[i] - gTabdp[i] * gTabnp[0];
	}

	sum_np = 0.0;
	sum_nm = 0.0;
	sum_d = 0.0;

	for (i = 0; i <= 4; i++) {
		sum_np += gTabnp[i];
		sum_nm += gTabnm[i];
		sum_d += gTabdp[i];
	}

	a = sum_np / (1.0f + sum_d);
	b = sum_nm / (1.0f + sum_d);

	for (i = 0; i <= 4; i++) {
		gTabbp[i] = gTabdp[i] * a;
		gTabbm[i] = gTabdm[i] * b;
	}
}

static void merger_pixles(const float *s1, const float *s2, byte *dst, int bytes, int width) {
	register int i;
	int size = bytes * width;
	float sum = 0;
	for (i = 0; i < size; i++) {
		sum = *(s1++) + *(s2++);
		*(dst++) = sum > 255 ? 255 : sum < 0 ? 0 : (byte)(sum);
	}
}

void blurLine(byte *imgSrc, byte *imgDst, int width, const int depth) {
	int i, j, t, terms, col;
	register byte *sp = NULL, *sm = NULL;
	register float *vp = NULL, *vm = NULL;
	register float *pptr = NULL, *mptr = NULL;
	byte initial_p[3];
	byte initial_m[3];

	memset(gVp, 0, width * depth * sizeof(float));
	memset(gVm, 0, width * depth * sizeof(float));

	sp = imgSrc;
	sm = imgSrc + (width - 1) * depth;
	vp = gVp;
	vm = gVm + (width - 1) * depth;

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
				*pptr += gTabnp[i] * sp[(-i * depth) + t] - gTabdp[i] * vp[(-i * depth) + t];
				*mptr += gTabnm[i] * sm[(i * depth) + t] - gTabdm[i] * vm[(i * depth) + t];
			}
			for (j = i; j <= 4; j++) {
				*pptr += (gTabnp[j] - gTabbp[j]) * initial_p[t];
				*mptr += (gTabnm[j] - gTabbm[j]) * initial_m[t];
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
				*pptr += gTabnp[i] * sp[(-i * depth) + t] - gTabdp[i] * vp[(-i * depth) + t];
				*mptr += gTabnm[i] * sm[(i * depth) + t] - gTabdm[i] * vm[(i * depth) + t];
			}
		}
		sp += depth;
		sm -= depth;
		vp += depth;
		vm -= depth;
	}

	merger_pixles(gVp, gVm, imgDst, depth, width);
}

void gauss_iir(unsigned char* img, int bytes, int stride, double horz, double vert, int x, int y, int width, int height) {

	register int i, j, uoff, voff;
	register byte *src = NULL, *dp = NULL, *ds = NULL;
	int depth = 4;
//	int stride = width * depth;
	int len = (width > height ? width : height) * depth;
	int radius;
	float sigma;
	//radius = fabs(width / 50) + 1.0f;
	//sigma = sqrt(radius * radius / (2.0f * log(255.0f)));
	//byte *img = NULL, *imgBlur = NULL;

	byte *imgBlur = img;

	src = (byte *)malloc(len * sizeof(byte));
	gVp = (float *)malloc(len * sizeof(float));
	gVm = (float *)malloc(len * sizeof(float));

	if (NULL == img || NULL == gVp || NULL == gVm || NULL == src) {
		return;
	}

	radius = (horz > 0 ? horz : (-horz) )+ 1.0f;
	sigma = sqrt(radius * radius / (2.0f * log(255.0f)));
	get_gauss_filter_coefficients(sigma);
	for (j = 0; j < width; j++) {
		dp = img + j * depth;
		ds = src;
		for (i = 0; i < height; i++) {
			ds[0] = dp[0];
			ds[1] = dp[1];
			ds[2] = dp[2];
			ds += depth;
			dp += stride;
		}
		blurLine(src, src, height, depth);

		dp = imgBlur + j * depth;
		ds = src;
		for (i = 0; i < height; i++) {
			dp[0] = ds[0];
			dp[1] = ds[1];
			dp[2] = ds[2];
			ds += depth;
			dp += stride;
		}
	}
	radius = (vert > 0 ? vert : (-vert) ) + 1.0f;
	sigma = sqrt(radius * radius / (2.0f * log(255.0f)));
	get_gauss_filter_coefficients(sigma);
	for (j = 0; j < height; j++) {
		dp = imgBlur + j * stride;
		blurLine(dp, dp, width, depth);
	}

	free(gVp);
	free(gVm);
	free(src);
	gVp = NULL;
	gVm = NULL;
	src = NULL;
}

