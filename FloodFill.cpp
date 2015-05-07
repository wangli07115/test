#include "../core/imagebase.h"
#include "../core/native.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


int FloodFill(byte *imgSrc, int width, int height, int cx, int cy, int th);

NATIVE(void) FUNCTION(filtersForm, nativeFloodFill, jbyte *imageYUV, jint width, jint height, jint cx, jint cy, jint th){
	FloodFill(imageYUV, width, height, cx, cy, th);
}

#define MAX3(a, b, c)				(a > b ? (a > c ? a : c) : (b > c ? b : c))
#define MIN3(a, b, c)				(a < b ? (a < c ? a : c) : (b < c ? b : c))

int hTable[360] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
		1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
		10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,
		11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,
		12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,
		13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,
		14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,
		15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int svTable[100] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};

int hTable8[360] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0
};
int svTable3[100] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
};
int hsv256(int h, int s, int v) {
	//return 16 * hTable[h] + 4 * svTable[s] + svTable[v];
	  return hTable8[h] + 3 * svTable3[s] + 3 * 16 * svTable3[v];
}

static void rgb2hsv(int r, int g, int b, int& h, int& s, int& v) {
    float var_r = r / 255.0f;
    float var_g = g / 255.0f;
    float var_b = b / 255.0f;

    float var_max = MAX3(var_r, var_g, var_b);
    float var_min = MIN3(var_r, var_g, var_b);

    float delta_max = var_max - var_min;
    float var_h = 0, var_s = 0, var_v = var_max;

    if(var_max == 0) {
        var_h = var_s = 0;
    }else if (delta_max == 0) {
        var_h = 0;
        var_s = 0;
    } else {
        var_s = delta_max / var_max;
        if (var_max == var_r) {
            var_h = 60 * (var_g - var_b) / delta_max;
            if (var_g < var_b) {
                var_h += 360;
            }
        } else if (var_max == var_g) {
            var_h = 60 * (var_b - var_r) / delta_max + 120;
        } else {
            var_h = 60 * (var_r - var_g) / delta_max + 240;
        }
    }

    h = (int)var_h;
    s = (int)(var_s * 100);
    v = (int)(var_v * 100);
}

static void hsv2rgb(int& r, int& g, int& b, int fh, int fs, int fv) {
	float   h = fh;
	float s = fs / 100.0f;
	float v = fv / 100.0f;
	float  f;
	int m, n, c;
	int i;

	if(v == 0) {
		r = g = b = 0;
		return;
	}
	if(s == 0) {
		r = g = b = (int)(v * 255);
		return;
	}

	h = h / 60;
	i = (int)(floor(h));
	f = h - i;
	if (i % 2 == 0)
	{
		f = 1 - f; // if i is even
	}

	m = (int)(v* (1 - s) * 255);
	n = (int)(v * (1 - s * f) * 255);
	c = (int)(v * 255);
	switch (i) {
	case 6:
	case 0:
		r = c;
		g = n;
		b = m;
		break;
	case 1:
		r = n;
		g = c;
		b = m;
		break;
	case 2:
		r = m;
		g = c;
		b = n;
		break;
	case 3:
		r = m;
		g = n;
		b = c;
		break;
	case 4:
		r = n;
		g = m;
		b = c;
		break;
	case 5:
		r = c;
		g = m;
		b = n;
		break;
	}
}
void ImgGray(byte *pBuf, int &gray, int &hsv) {
	int r,g,b, h,s,v;
	r = PR(pBuf) ;
	g = PG(pBuf);
	b = PB(pBuf);

	rgb2hsv(r, g, b, h, s, v);

	hsv = h;
	//hsv = hsv256(h, s, v);
	gray = (PR(pBuf) * 38 + PG(pBuf) * 75 + PB(pBuf) * 15) >> 7;
}

#define max(a, b)	(a < b ? b : a)
#define min(a, b)	(a > b ? b : a)


int getDist(byte *pBuf, byte *pBuf1) {
	int dist;
	int r, g, b;
	int imax, imin;
	r = PR(pBuf) - PR(pBuf1);
	g = PG(pBuf) - PG(pBuf1);
	b = PB(pBuf) - PB(pBuf1);

	if(r > g) {
		imax = r;
		imin = g;
	} else {
		imax = g;
		imin = b;
	}
	imax = max(imax, max(b, 0));
	imin = min(imin, min(b, 0));

	dist = imax - imin;
	return dist;
}

//yuyv
int FloodFill(byte *imgSrc, int width, int height, int cx, int cy, int th) {
	int depth = 3;
	int stride = width * depth;
	int gray = imgSrc[cy * stride + cx * depth];
	int *mask = (int *)malloc(sizeof(int) * width * height);
	int *fillPoints = (int *)malloc(sizeof(int) * width * height * 2);
	int count = 0, idx = 0;
	byte *pxy, *pij;

	int i, j;
	int ix, iy;
	int lft, top, rit, btm;
	int radius = 1000;
	int temp = 0;
	int hsv, hsv0;
	int thHSV = 5;
	lft = cx - radius < 1 ? 1 : cx - radius;
	top = cy - radius < 1 ? 1 : cy - radius;
	rit = cx + radius > width - 1 ? width - 1 : cx - radius;
	btm = cy + radius > height - 1 ? height -1 : cy + radius;

	if(stride % 4 != 0) {
		stride = (stride / 4 + 1) * 4;
	}
	memset(mask, 0, sizeof(int) * width * height);
	memset(fillPoints, 0, sizeof(int) * width * height * 2);
	
	mask[cy * width + cx] = 1;
	fillPoints[0] = cx;
	fillPoints[1] = cy;
	idx = 0;
	count++;
	pxy = imgSrc + cy * stride + cx * depth;
	for(; count - idx > 0; idx++){
		ix = fillPoints[2 * idx];
		iy = fillPoints[2 * idx + 1];

	//	pxy = imgSrc + iy * stride + ix * depth;
		pij = imgSrc + iy * stride + (ix - 1) * depth;
		//&& abs(hsv - hsv0) < thHSV
		if (ix > lft && (mask[ix - 1 + iy * width] != 1) 
			&& getDist(pxy, pij) < th){
			fillPoints[count * 2] = ix - 1;
			fillPoints[count * 2 + 1] = iy;
			mask[ix - 1 + iy * width] = 1;
			count++;
		}
		pij = imgSrc + iy * stride + (ix + 1) * depth;
		if (ix < rit && (mask[ix + 1+ iy  * width] != 1) 
			&& getDist(pxy, pij) < th ) {
			fillPoints[count * 2] = ix + 1;
			fillPoints[count * 2 + 1] = iy;
			mask[ix + 1+ iy  * width] = 1;
			count++;
		}
		pij = imgSrc + (iy - 1) * stride + ix * depth;
		if (iy > top && (mask[ix + (iy - 1)  * width] != 1) 
			&& getDist(pxy, pij) < th) {
			fillPoints[count * 2] = ix;
			fillPoints[count * 2 + 1] = iy - 1;
			mask[ix + (iy - 1)  * width] = 1;
			count++;
		}
		pij = imgSrc + (iy + 1) * stride + ix * depth;
		if (iy < btm && (mask[ix + (iy + 1) * width] != 1) 
			&& getDist(pxy, pij) < th) {
			fillPoints[count * 2] = ix;
			fillPoints[count * 2 + 1] = iy + 1;
			mask[ix + (iy + 1)  * width] = 1;
			count++;
		}
	}

	
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (mask[i + j * width] == 1) {
				imgSrc[j * stride + i * depth] = 0;
				imgSrc[j * stride + i * depth + 1] = 0;
				imgSrc[j * stride + i * depth + 2] = 0;
			}
		}
	}

	if(mask != NULL) {
		free(mask);
		mask = NULL;
	}
	if(fillPoints != NULL) {
		free(fillPoints);
		fillPoints = NULL;
	}
	return 0;
}

