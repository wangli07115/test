#include "filters.h"
#include "fastprocess.h"

#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define CX_PI		3.1415926535898
static int GaussMatrixR1[3] = {85, 86, 85};
static int GaussMatrixR2[5] = {27, 67, 68, 67, 27};
static int GaussMatrixR3[7] = {15, 33, 53, 54, 53, 33, 15};
static int GaussMatrixR4[9] = {10, 20, 33, 43, 44, 43, 33, 20, 10};
static int GaussMatrixR5[11] = {8, 14, 22, 30, 35, 38, 35, 30, 22, 14, 8};
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

void gaussBlurLine(byte *imgSrc, byte *imgDst, int width, const int depth) {

	int i, k;
	int radius = 5;
	int *gaussMat = GaussMatrixR5;
	int r = 0, g = 0, b = 0,sum;
	register byte *pSrcPixel, *pDstPixel, *pSrcPixelR;

	pSrcPixel = imgSrc;
	pDstPixel = imgDst;
	for(i = 0; i < radius; i++) {
		r = 0, g = 0, b = 0;
		sum = 0;
		pSrcPixelR = pSrcPixel;
		for (k = -i; k <= radius; k++) {
			r += (*pSrcPixelR) * gaussMat[k + radius];
			g += (*(pSrcPixelR + 1)) * gaussMat[k + radius];
			b += (*(pSrcPixelR + 2)) * gaussMat[k + radius];
			sum += gaussMat[k + radius];
			pSrcPixelR += depth;
		}
		*pDstPixel = CLAMP(r / sum);
		*(pDstPixel + 1) = CLAMP(g / sum);
		*(pDstPixel + 2) = CLAMP(b / sum);

		pSrcPixel += depth;
		pDstPixel += depth;
	}
	for(; i < width - radius; i++) {
		r = 0, g = 0, b = 0;
		sum = 0;
		pSrcPixelR = pSrcPixel - radius * depth;
		for (k = -radius; k <= radius; k++) {
			r += (*pSrcPixelR) * gaussMat[k + radius];
			g += (*(pSrcPixelR + 1)) * gaussMat[k + radius];
			b += (*(pSrcPixelR + 2)) * gaussMat[k + radius];
			pSrcPixelR += depth;
		}
		*pDstPixel = CLAMP(r >> 8);
		*(pDstPixel + 1) = CLAMP(g >> 8);
		*(pDstPixel + 2) = CLAMP(b >> 8);

		pSrcPixel += depth;
		pDstPixel += depth;
	}
	for(; i < width; i++) {
		r = 0, g = 0, b = 0;
		sum = 0;
		pSrcPixelR = pSrcPixel - radius * depth;
		for (k = -radius; k < width - i; k++) {
			r += (*pSrcPixelR) * gaussMat[k + radius];
			g += (*(pSrcPixelR + 1)) * gaussMat[k + radius];
			b += (*(pSrcPixelR + 2)) * gaussMat[k + radius];
			sum += gaussMat[k + radius];
			pSrcPixelR += depth;
		}
		*pDstPixel = CLAMP(r / sum);
		*(pDstPixel + 1) = CLAMP(g / sum);
		*(pDstPixel + 2) = CLAMP(b / sum);

		pSrcPixel += depth;
		pDstPixel += depth;
	}
}

void JNIFUNCF(MZImageFilterBlur, nativeMask, jobject bitmap, jobject bitmapBlur, jint width, jint height) {
	register int i, j, uoff, voff;
	register byte *src = NULL, *dp = NULL, *ds = NULL;
	int depth = 4;
	int stride = width * depth;
	int len = MAX(width, height) * depth;
	int radius = fabs(width / 50) + 1.0f;
	float sigma = sqrt(radius * radius / (2.0f * log(255.0f)));
	byte *img = NULL, *imgBlur = NULL;

 	AndroidBitmap_lockPixels(env, bitmap, (void**) &img);
 	AndroidBitmap_lockPixels(env, bitmapBlur, (void**) &imgBlur);

	src = (byte *)malloc(len * sizeof(byte));
	gVp = (float *)malloc(len * sizeof(float));
	gVm = (float *)malloc(len * sizeof(float));

	if (NULL == img || NULL == gVp || NULL == gVm || NULL == src) {
		return;
	}
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

	AndroidBitmap_unlockPixels(env, bitmap);
	AndroidBitmap_unlockPixels(env, bitmapBlur);
}

void JNIFUNCF(MZImageFilterBlur, nativeApplyFilter, jobject bitmap, jobject bitmapBlur, jint width, jint height,
			  jint x0, jint y0, jfloat r1) {
	register int i, j;
	register byte *dp = NULL, *dpB = NULL;
	int rDist = (int)((width * width + height * height) * r1  * r1/ 4);
	int rDist2 = rDist * 3 * 3;
	float distR = rDist2 - rDist;
	int dx, dy, dist;
	float rate;
	byte *img = NULL, *imgBlur = NULL;

 	AndroidBitmap_lockPixels(env, bitmap, (void**) &img);
 	AndroidBitmap_lockPixels(env, bitmapBlur, (void**) &imgBlur);

	if (NULL == img || NULL ==  imgBlur || width < 0 || height < 0) {
		return;
	}

	dp = img;
	dpB = imgBlur;
	for(j = 0; j < height; j++){
		for(i = 0; i < width; i++){
			dx = i - x0;
			dy = j - y0;
			dist = (dx * dx + dy * dy);

			if(dist < rDist) {
				dp += 4;
				dpB += 4;
				continue;
			}
			if(dist >= rDist2) {
				PR(dp)	=  PR(dpB);
				PG(dp)	=  PG(dpB);
				PB(dp)	=  PB(dpB);
				dp += 4;
				dpB += 4;
				continue;
			}

			rate = (dist - rDist) / distR;

			PR(dp) = (byte) CLAMP(PR(dp) + ((PR(dpB) - PR(dp)) * rate));
			PG(dp) = (byte) CLAMP(PG(dp) + ((PG(dpB) - PG(dp)) * rate));
			PB(dp) = (byte) CLAMP(PB(dp) + ((PB(dpB) - PB(dp)) * rate));

			dp += 4;
			dpB += 4;
		}
	}

	AndroidBitmap_unlockPixels(env, bitmap);
	AndroidBitmap_unlockPixels(env, bitmapBlur);
}
