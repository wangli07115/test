//#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
//#include <android/log.h>
//#include <android/bitmap.h>
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include<stdlib.h>
#include "blur.h"

#define LOG_TAG "libbitmaputils"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define PI 3.14159265359
#ifndef MAX
#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b)       ((a) < (b) ? (a) : (b))
#endif
#define ROUND(x)        ((int)((x) + 0.5))

#if defined(WIN32) || defined(WIN64)
#define PA(color)               *(color + 3)
#define PR(color)               *(color + 2)
#define PG(color)               *(color + 1)
#define PB(color)               *(color)
#elif defined(ANDROID)
#define PA(color)               *(color + 3)
#define PR(color)               *(color)
#define PG(color)               *(color + 1)
#define PB(color)               *(color + 2)
#endif

static void  Blur(unsigned char *pix, int width, int height, int bytes, int radius) {
    if (radius < 1) return;
    int wm = width - 1;
    int hm = height - 1;
    int wh = width * height;
    int div = radius + radius + 1;
    unsigned char* r = (unsigned char*) malloc(wh);
    unsigned char* g = (unsigned char*) malloc(wh);
    unsigned char* b = (unsigned char*) malloc(wh);
    int rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw;
    int* vMIN = (int*) malloc(MAX(width, height) * sizeof(int));
    int* vMAX = (int*) malloc(MAX(width, height) * sizeof(int));
    unsigned char* dv = (unsigned char*) malloc(256 * div);
    unsigned char* ptr = NULL;
    unsigned char* ptr1 = NULL;
    unsigned char* ptr2 = NULL;
    for (i = 0;i < 256 * div;i++) {
        dv[i] = (i / div);
    }

    yw = yi =0;
    for (y = 0; y < height; y++) {
        rsum = gsum = bsum = 0;
        for(i=-radius;i <= radius; i++) {
            ptr = pix + (yi + MIN(wm, MAX(i,0))) * bytes;
            rsum += PR(ptr);
            gsum += PG(ptr);
            bsum += PB(ptr);
        }
        for (x = 0;x < width; x++) {
            *(r + yi) = *(dv + rsum);
            *(g + yi) = *(dv + gsum);
            *(b + yi) = *(dv + bsum);

            if(y == 0) {
                *(vMIN + x) = MIN(x + radius + 1,wm);
                *(vMAX + x) = MAX(x - radius,0);
            }
            ptr1 = pix + (yw + *(vMIN + x)) * bytes;
            ptr2 = pix + (yw + *(vMAX + x)) * bytes;

            rsum += PR(ptr1) - PR(ptr2);
            gsum += PG(ptr1) - PG(ptr2);
            bsum += PB(ptr1) - PB(ptr2);

            yi++;
        }
        yw += width;
    }

    for (x = 0; x < width; x++) {
        rsum = gsum = bsum = 0;
        yp =-radius * width;
        for(i=-radius;i <= radius; i++) {
            yi = MAX(0,yp) + x;
            rsum += *(r + yi);
            gsum += *(g + yi);
            bsum += *(b + yi);
            yp += width;
        }
        yi = x;
        for (y = 0; y < height; y++) {
            ptr = pix + yi * bytes;
            PR(ptr) = *(dv + rsum);
            PG(ptr) = *(dv + gsum);
            PB(ptr) = *(dv + bsum);
            if(x == 0) {
                *(vMIN + y) = MIN(y + radius + 1, hm) * width;
                *(vMAX + y) = MAX(y - radius, 0) * width;
            }
            p1 = x + vMIN[y];
            p2 = x + vMAX[y];

            rsum += *(r + p1) - *(r + p2);
            gsum += *(g + p1) - *(g + p2);
            bsum += *(b + p1) - *(b + p2);

            yi += width;
        }
    }

    free(r);
    free(g);
    free(b);
    free(vMIN);
    free(vMAX);
    free(dv);
}

static void find_iir_constants(double *n_p, double *n_m, double *d_p, double *d_m, double *bd_p, double *bd_m, double std_dev) {
	const double div = sqrt(2.0 * PI) * std_dev;
	const double x0 = -1.783 / std_dev;
	const double x1 = -1.723 / std_dev;
	const double x2 = 0.6318 / std_dev;
	const double x3 = 1.997 / std_dev;
	const double x4 = 1.6803 / div;
	const double x5 = 3.735 / div;
	const double x6 = -0.6803 / div;
	const double x7 = -0.2598 / div;
	double sum_n_p = 0.0, sum_n_m = 0.0, sum_d = 0.0;
	double a, b;
	int i;

	n_p[0] = x4 + x6;
	n_p[1] = (exp(x1) * (x7 * sin(x3) - (x6 + 2 * x4) * cos(x3)) + exp(x0)
			* (x5 * sin(x2) - (2 * x6 + x4) * cos(x2)));
	n_p[2] = (2 * exp(x0 + x1) * ((x4 + x6) * cos(x3) * cos(x2) - x5 * cos(x3)
			* sin(x2) - x7 * cos(x2) * sin(x3)) + x6 * exp(2 * x0) + x4 * exp(
			2 * x1));
	n_p[3] = (exp(x1 + 2 * x0) * (x7 * sin(x3) - x6 * cos(x3)) + exp(
			x0 + 2 * x1) * (x5 * sin(x2) - x4 * cos(x2)));
	n_p[4] = 0.0;

	d_p[0] = 0.0;
	d_p[1] = -2 * exp(x1) * cos(x3) - 2 * exp(x0) * cos(x2);
	d_p[2] = 4 * cos(x3) * cos(x2) * exp(x0 + x1) + exp(2 * x1) + exp(2 * x0);
	d_p[3] = -2 * cos(x2) * exp(x0 + 2 * x1) - 2 * cos(x3) * exp(x1 + 2 * x0);
	d_p[4] = exp(2 * x0 + 2 * x1);

	for (i = 0; i <= 4; i++) {
		d_m[i] = d_p[i];
	}
	n_m[0] = 0.0;
	for (i = 1; i <= 4; i++) {
		n_m[i] = n_p[i] - d_p[i] * n_p[0];
	}

	for (i = 0; i <= 4; i++) {
		sum_n_p += n_p[i];
		sum_n_m += n_m[i];
		sum_d += d_p[i];
	}
	a = sum_n_p / (1.0 + sum_d);
	b = sum_n_m / (1.0 + sum_d);
	for (i = 0; i <= 4; i++) {
		bd_p[i] = d_p[i] * a;
		bd_m[i] = d_m[i] * b;
	}
}

static void multiply_alpha(unsigned char *buf, int width, int bytes) {
	int i, j;
	for (i = 0; i < width; i++, buf += bytes) {
		double alpha = buf[bytes - 1] * (1.0f / 255.0f);
		for (j = 0; j < bytes - 1; j++) {
			buf[j] = ROUND (buf[j] * alpha);
		}
	}
}

static void transfer_pixels(const double *src1, const double *src2, unsigned char *dest, int bytes, int width) {
	int b;
	int bend = bytes * width;
	double sum;
	for (b = 0; b < bend; b++) {
		sum = *src1++ + *src2++;
		if (sum > 255)
			sum = 255;
		else if (sum < 0)
			sum = 0;
		*dest++ = (unsigned char)sum;
	}
}

static void separate_alpha(unsigned char *buf, int width, int bytes) {
	int i, j;
	for (i = 0; i < width; i++, buf += bytes) {
		unsigned char alpha = buf[bytes - 1];
		switch (alpha) {
		case 0:
		case 255:
			break;
		default: {
			double recip_alpha = 255.0 / alpha;
			for (j = 0; j < bytes - 1; j++) {
				int new_val = ROUND (buf[j] * recip_alpha);
				buf[j] = MIN (255, new_val);
			}
		}
			break;
		}
	}
}

void gauss_iir(unsigned char* image, int bytes, int stride, double horz, double vert, int x, int y, int width, int height) {
	int has_alpha;
	unsigned char *dest = NULL, *src = NULL, *sp_p = NULL, *sp_m = NULL;
	double *val_p = NULL;
	double *val_m = NULL;
	double *vp, *vm;
    double *vpptr, *vmptr;
	int i, j, uoff = 0, voff = 0;
	int row, col, b;
	int terms;
	int initial_p[4];
	int initial_m[4];
    double n_p[5], n_m[5];
	double d_p[5], d_m[5];
	double bd_p[5], bd_m[5];
	double std_dev;

	has_alpha = 0;//bytes == 4 ? 1 : 0;

	val_p = (double*) malloc(MAX(width, height) * bytes * sizeof(double));
	val_m = (double*) malloc(MAX(width, height) * bytes * sizeof(double));
	src = (unsigned char*) malloc(MAX(width, height) * bytes);
	dest = (unsigned char*) malloc(MAX(width, height) * bytes);

	if (vert > 0.0) {
		vert = fabs(vert) + 1.0;
		std_dev = sqrt(-(vert * vert) / (2.0f * log(1.0f / 255.0f)));
		find_iir_constants(n_p, n_m, d_p, d_m, bd_p, bd_m, std_dev);
		for (col = 0; col < width; col++) {
			memset(val_p, 0, height * bytes * sizeof(double));
			memset(val_m, 0, height * bytes * sizeof(double));
			for (i = 0; i < height; i++) {
				uoff = i * bytes;
				voff = i * stride + col * bytes;
				for (j = 0; j < bytes; j++) {
					src[uoff + j] = image[voff + j];
				}
			}
			if (has_alpha) {
				multiply_alpha(src, height, bytes);
			}
			sp_p = src;
			sp_m = src + (height - 1) * bytes;
			vp = val_p;
			vm = val_m + (height - 1) * bytes;
			for (i = 0; i < bytes; i++) {
				initial_p[i] = sp_p[i];
				initial_m[i] = sp_m[i];
			}

			for (row = 0; row < height; row++) {
				terms = (row < 4) ? row : 4;
				for (b = 0; b < bytes; b++) {
					vpptr = vp + b;
					vmptr = vm + b;
					for (i = 0; i <= terms; i++) {
						*vpptr += n_p[i] * sp_p[(-i * bytes) + b] - d_p[i] * vp[(-i * bytes) + b];
						*vmptr += n_m[i] * sp_m[(i * bytes) + b] - d_m[i] * vm[(i * bytes) + b];
					}
					for (j = i; j <= 4; j++) {
						*vpptr += (n_p[j] - bd_p[j]) * initial_p[b];
						*vmptr += (n_m[j] - bd_m[j]) * initial_m[b];
					}
				}

				sp_p += bytes;
				sp_m -= bytes;
				vp += bytes;
				vm -= bytes;
			}
			transfer_pixels(val_p, val_m, dest, bytes, height);
			if (has_alpha)
				separate_alpha(dest, height, bytes);
			for (i = 0; i < height; i++) {
				uoff = i * bytes;
				voff = i * stride + col * bytes;
				for (j = 0; j < bytes; j++) {
					image[voff + j] = dest[uoff + j];
				}
			}
		}
	}
	if (horz > 0.0) {
		horz = fabs(horz) + 1.0;
		if (horz != vert) {
			std_dev = sqrt(-(horz * horz) / (2 * log(1.0 / 255.0)));
			find_iir_constants(n_p, n_m, d_p, d_m, bd_p, bd_m, std_dev);
		}
		for (row = 0; row < height; row++) {
			memset(val_p, 0, width * bytes * sizeof(double));
			memset(val_m, 0, width * bytes * sizeof(double));
			memcpy(src, image + row * stride, width * bytes);
			if (has_alpha)
				multiply_alpha(src, width, bytes);
			sp_p = src;
			sp_m = src + (width - 1) * bytes;
			vp = val_p;
			vm = val_m + (width - 1) * bytes;
			for (i = 0; i < bytes; i++) {
				initial_p[i] = sp_p[i];
				initial_m[i] = sp_m[i];
			}
			for (col = 0; col < width; col++) {
				terms = (col < 4) ? col : 4;
				for (b = 0; b < bytes; b++) {
					vpptr = vp + b;
					vmptr = vm + b;
					for (i = 0; i <= terms; i++) {
						*vpptr += n_p[i] * sp_p[(-i * bytes) + b] - d_p[i] * vp[(-i * bytes) + b];
						*vmptr += n_m[i] * sp_m[(i * bytes) + b] - d_m[i] * vm[(i * bytes) + b];
					}
					for (j = i; j <= 4; j++) {
						*vpptr += (n_p[j] - bd_p[j]) * initial_p[b];
						*vmptr += (n_m[j] - bd_m[j]) * initial_m[b];
					}
				}
				sp_p += bytes;
				sp_m -= bytes;
				vp += bytes;
				vm -= bytes;
			}
			transfer_pixels(val_p, val_m, dest, bytes, width);
			if (has_alpha)
				separate_alpha(dest, width, bytes);
			memcpy(image + row * stride, dest, width * bytes);
		}
	}
	free(val_p);
	free(val_m);
	free(src);
	free(dest);
}
/*
extern "C" void Java_com_meizu_media_effects_Blur_nativeBlur(JNIEnv* env, jobject object, jobject bitmap, jint width, jint height, jint radius)
{
	unsigned char* image = NULL;
	int i = 0, j = 0, row = height / 4;
	AndroidBitmap_lockPixels(env, bitmap, (void**) &image);

	LOGE("Java_com_meizu_media_effects_MusicCoverView_blurCover 1 width:%d, height:%d, radius:%d", width, height, radius);
	//Blur(image, width, height, 4, radius);
	gauss_iir(image, 4, width * 4, radius, radius, 0, 0, width, height);

	LOGE("Java_com_meizu_media_effects_MusicCoverView_blurCover 2");
	AndroidBitmap_unlockPixels(env, bitmap);
}
*/
