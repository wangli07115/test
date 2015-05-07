#include <math.h>
#include "baseFilters.h"

unsigned char clamp(int value) {
	if(value < 0) {
		value = 0;
	} else if (value > 255) {
		value = 255;
	}

	return value;
	//byte)((((ushort)value | ((short)(255 - value) >> 15)) & ~value >> 15));
}

void sharpen(unsigned char *image, int width, int height, int radius, int th, int rate) {
    register int i, j;
    register unsigned char *pBuf = NULL, *pBlur = NULL;
	int stride = STRIDE(width);
	int value, m;

	int table[512];

	unsigned char *imageBlur = NULL;
	imageBlur = (unsigned char *)malloc(sizeof(unsigned char *) * stride * height);
	if(NULL == imageBlur) {
		return;
	}
	memcpy(imageBlur, image, stride * height);
	gaussiir(imageBlur, width, height, DEPTH, stride, radius, radius);

	for(i = -255; i <= 255; i++) {
		table[i + 255] = (float)pow(2, i/100.) * rate / 500;
	}

	for(j = 0; j < height; j++) {
		pBuf = image + j * stride;
		pBlur = imageBlur + j * stride;
		for(i = 0; i < width; i++, pBuf += DEPTH, pBlur += DEPTH) {
			value = (pBuf[0] + pBuf[1] + pBuf[2] - pBlur[0] - pBlur[1] - pBlur[2]) / 3;
			if(value > th) {
				m =  table[value + 255];
								
				pBuf[0] += clamp((pBuf[0]- 127) * m + 127);
				pBuf[1] += clamp((pBuf[1]- 127) * m + 127);
				pBuf[2] += clamp((pBuf[2]- 127) * m + 127);

				/*
				pBuf[0] += clamp(pBuf[0] + (pBuf[0] - pBlur[0]) * rate / 100);
				pBuf[1] += clamp(pBuf[1] + (pBuf[1] - pBlur[1]) * rate / 100);
				pBuf[2] += clamp(pBuf[2] + (pBuf[2] - pBlur[2]) * rate / 100);
				*/
			}
		}
	}
}
