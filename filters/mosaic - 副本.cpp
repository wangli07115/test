#include "mosaic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mosaic_rectangle0(byte *image, byte *dst, int width, int height, int depth, int stride, int wlen, int hlen) {
	int i, j;
	int m, n;
	byte *pLin, *pBuf;
	int sum[3];
	int pixelID;

	memcpy(dst, image, stride * height);
	if(wlen < 3 || hlen < 3) {
		return;
	}
	pixelID = (hlen / 2) * stride + (wlen / 2) * depth;
	for(j = 0; j < height - hlen; j += hlen) {
		pLin = dst + j * stride;
		for(i = 0; i < width -  wlen; i += wlen, pLin += wlen * depth) {
			sum[0] = (*(pLin + pixelID));
			sum[1] = (*(pLin + pixelID + 1)); 
			sum[2] = (*(pLin + pixelID + 2)); 
			for(n = 0; n < hlen; n++) {
				pBuf = pLin + n * stride;
				for(m = 0; m < wlen; m++, pBuf += depth) {
					*pBuf = sum[0];
					*(pBuf + 1) = sum[1]; 
					*(pBuf + 2) = sum[2]; 
				}
			}
		}
		for(; i < width; i += wlen, pLin += wlen * depth) {
			sum[0] = (*pLin);
			sum[1] = (*(pLin + 1)); 
			sum[2] = (*(pLin + 2)); 
			for(n = 0; n < hlen; n++) {
				pBuf = pLin + n * stride;
				for(m = 0; m < wlen && i + m < width; m++, pBuf += depth) {
					*pBuf = sum[0];
					*(pBuf + 1) = sum[1]; 
					*(pBuf + 2) = sum[2]; 
				}
			}
		}
	}
	for(; j < height; j += hlen) {
		pLin = image + j * stride;
		for(i = 0; i < width -  wlen; i += wlen, pLin += wlen * depth) {
			sum[0] = (*(pLin));
			sum[1] = (*(pLin + 1)); 
			sum[2] = (*(pLin + 2)); 
			for(n = 0; n < hlen && j + n < height; n++) {
				pBuf = pLin + n * stride;
				for(m = 0; m < wlen; m++, pBuf += depth) {
					*pBuf = sum[0];
					*(pBuf + 1) = sum[1]; 
					*(pBuf + 2) = sum[2]; 
				}
			}
		}
		for(; i < width; i += wlen, pLin += wlen * depth) {
			sum[0] = (*pLin);
			sum[1] = (*(pLin + 1)); 
			sum[2] = (*(pLin + 2)); 
			for(n = 0; n < hlen && j + n < height; n++) {
				pBuf = pLin + n * stride;
				for(m = 0; m < wlen && i + m < width; m++, pBuf += depth) {
					*pBuf = sum[0];
					*(pBuf + 1) = sum[1]; 
					*(pBuf + 2) = sum[2]; 
				}
			}
		}
	}
}

void mosaic_rectangle45(byte *image, byte *dst, int width, int height, int depth, int stride, int wlen, int hlen){
	int i, j, m, n;
	byte *pMask = NULL, *pMaskTemp;
	byte *pLin, *pBuf, *pSrcTemp;
	int sum[3] = {0, 0, 0};
	int widthMask, heightMask;
	int pixelID;
	int xstep, ystep, startx, offsetChunk;
	int ioffset, moffset, num;

	memcpy(dst, image, stride * height);
	if(wlen < 3 || hlen < 3) {
		return;
	}

	widthMask = wlen + hlen -1;
	heightMask = wlen + hlen;	
	pMask = (byte *)malloc(widthMask * heightMask);
	if(pMask == NULL) {
		return;
	}
	memset(pMask, 1, widthMask * heightMask);
	for(j = 0; j < wlen - 1; j++) {
		pMaskTemp = pMask + j * widthMask;
		for(i = 0; i < wlen - 1; i++, pMaskTemp++) {
			if(i + j < wlen -1) {
				*pMaskTemp = 0;
			}		
		}
	}
	for(j = 0; j < hlen - 1; j++) {
		pMaskTemp = pMask + j * widthMask + wlen;
		for(i = wlen; i < widthMask; i++, pMaskTemp++) {
			if(i - j > wlen - 1) {//widthMask - 1 - i + j < hlen -1;
				*pMaskTemp = 0;
			}
		}
	}
	for(j = wlen + 1; j < heightMask; j++) {
		pMaskTemp = pMask + j * widthMask;
		for(i = 0; i < hlen - 1; i++, pMaskTemp++) {
			if(j - i > wlen) {//heightMask - 1 - j + i < hlen -1
				*pMaskTemp = 0;
			}
		}
	}
	
	for(j = hlen + 1; j < heightMask; j++) {
		pMaskTemp = pMask + j * widthMask + hlen;
		for(i = hlen; i < widthMask; i++, pMaskTemp++) {
			if(j + i > 2 * hlen + wlen - 2) {
				*pMaskTemp = 0;
			}		
		}
	}

	//----------------------------------
	if(hlen > wlen) {	
		xstep = 2 * wlen;
		ystep = hlen;
		offsetChunk = hlen;
	}else{
		xstep = 2 * hlen;
		ystep = wlen;
		offsetChunk = -wlen;
		while(offsetChunk < 0) {
			offsetChunk += xstep;
		}
	}

	//j < xstep / 2
	pLin = dst;
	sum[0] = (*(pLin));
	sum[1] = (*(pLin + 1)); 
	sum[2] = (*(pLin + 2));	
	for(n = 0; n < wlen; n++) {
		pBuf = pLin + n * stride;
		for(m = 0; m < wlen; m++, pBuf += depth) {	
			*pBuf = sum[0];
			*(pBuf + 1) = sum[1]; 
			*(pBuf + 2) = sum[2]; 			
		}
	}
	pLin = dst + wlen * depth;
	for(i = wlen; i < width; i += xstep, pLin += xstep * depth) {
		sum[0] = (*(pLin));
		sum[1] = (*(pLin + 1)); 
		sum[2] = (*(pLin + 2));
		for(n = 0; n < xstep / 2; n++) {
			pBuf = pLin + n * stride;
			pMaskTemp = pMask + (heightMask - xstep / 2 + n) * widthMask;
			for(m = 0; m < xstep && i + m < width; m++, pBuf += depth) {				
				*pBuf = sum[0];
				*(pBuf + 1) = sum[1]; 
				*(pBuf + 2) = sum[2]; 			
			}
		}
	}

	startx = 0;
	pixelID = (heightMask / 2) * stride + (widthMask / 2) * depth;
	for(j = 0; j < height - heightMask; j += ystep, startx += offsetChunk) {
		while(startx >= xstep){
			startx -= xstep;
		}	
		//i < startx
		pBuf = dst;
		num = (widthMask + xstep - 1) / xstep;	
		ioffset = j * stride + (-num * xstep + startx) * depth;	
		for(i = - num * xstep + startx; i < startx; i += xstep, ioffset += xstep * depth) {
			pSrcTemp = image + ioffset + hlen * stride;
			sum[0] = (*(pSrcTemp));
			sum[1] = (*(pSrcTemp + 1)); 
			sum[2] = (*(pSrcTemp + 2));
			for(n = 0; n < heightMask; n++)	{
				moffset = ioffset + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, moffset += depth, pMaskTemp++) {
					if(m + i < 0) {
						continue;
					}
					if(*pMaskTemp == 1){
						*(pBuf + moffset) = sum[0];
						*(pBuf + moffset + 1) = sum[1]; 
						*(pBuf + moffset + 2) = sum[2]; 
					}
				}
			}
		}
		//startx <= i < width - widthMask
		pLin = dst + j * stride + startx * depth;	
		for(i = startx; i < width - widthMask; i += xstep, pLin += xstep * depth) {
			sum[0] = (*(pLin + pixelID));
			sum[1] = (*(pLin + pixelID + 1)); 
			sum[2] = (*(pLin + pixelID + 2));	
			for(n = 0; n < heightMask; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}
		//i >= width - widthMask
		for(; i < width; i += xstep, pLin += xstep * depth) {
			sum[0] = (*(pLin +  hlen * stride));
			sum[1] = (*(pLin +  hlen * stride + 1)); 
			sum[2] = (*(pLin +  hlen * stride + 2));	
			for(n = 0; n < heightMask; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask &&  m + i < width; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}				
	}
	//y >= height - heightMask
	pixelID = wlen * depth;
	for(; j < height; j += ystep, startx += offsetChunk) {	
		while(startx >= xstep){
			startx -= xstep;
		}	
		//i < startx
		num = (widthMask + xstep - 1) / xstep;	
		pLin = dst + j * stride + (-num * xstep + startx) * depth;	
		for(i = - num * xstep + startx; i < startx; i += xstep, pLin += xstep * depth) {
			sum[0] = -1;
			for(n = 0; n < heightMask && j + n < height; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(m + i < 0) {
						continue;
					}
					if(*pMaskTemp == 1){
						if(sum[0] == -1) {
							sum[0] = *(pBuf);
							sum[1] = *(pBuf + 1); 
							sum[2] = *(pBuf + 2); 	
						}
						*(pBuf) = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 					
					}
				}
			}
		}
		//startx < i < width - widthMask
		pLin = dst + j * stride + startx * depth;	
		for(i = startx; i < width - widthMask; i += xstep, pLin += xstep * depth) {
			sum[0] = -1;
			for(n = 0; n < heightMask && j + n < height; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						if(sum[0] == -1) {
							sum[0] = *(pBuf);
							sum[1] = *(pBuf + 1); 
							sum[2] = *(pBuf + 2); 	
						}
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}
		//i > width - widthMask
		for(; i < width; i += xstep, pLin += xstep * depth) {
			sum[0] = -1;
			for(n = 0; n < heightMask && j + n < height; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask &&  m + i < width; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						if(sum[0] == -1) {
							sum[0] = *(pBuf);
							sum[1] = *(pBuf + 1); 
							sum[2] = *(pBuf + 2); 	
						}
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}				
	}

	if(pMask != NULL) {
		free(pMask);
		pMask = NULL;
	}
}

void mosaic_rectangle45Bar(byte *image, byte *dst, int width, int height, int depth, int stride, int wlen, int hlen){
	int i, j, m, n;
	byte *pMask = NULL, *pMaskTemp;
	byte *pLin, *pBuf, *pSrcTemp;
	int sum[3] = {0, 0, 0};
	int widthMask, heightMask;
	int pixelID;
	int xstep, ystep, startx, offsetChunk;
	int ioffset, moffset, num;

	memcpy(dst, image, stride * height);
	if(wlen < 3 || hlen < 3) {
		return;
	}

	widthMask = wlen + hlen -1;
	heightMask = wlen + hlen;	
	pMask = (byte *)malloc(widthMask * heightMask);
	if(pMask == NULL) {
		return;
	}
	memset(pMask, 1, widthMask * heightMask);
	for(j = 0; j < wlen - 1; j++) {
		pMaskTemp = pMask + j * widthMask;
		for(i = 0; i < wlen - 1; i++, pMaskTemp++) {
			if(i + j < wlen -1) {
				*pMaskTemp = 0;
			}		
		}
	}
	for(j = 0; j < hlen - 1; j++) {
		pMaskTemp = pMask + j * widthMask + wlen;
		for(i = wlen; i < widthMask; i++, pMaskTemp++) {
			if(i - j > wlen - 1) {//widthMask - 1 - i + j < hlen -1;
				*pMaskTemp = 0;
			}
		}
	}
	for(j = wlen + 1; j < heightMask; j++) {
		pMaskTemp = pMask + j * widthMask;
		for(i = 0; i < hlen - 1; i++, pMaskTemp++) {
			if(j - i > wlen) {//heightMask - 1 - j + i < hlen -1
				*pMaskTemp = 0;
			}
		}
	}
	
	for(j = hlen + 1; j < heightMask; j++) {
		pMaskTemp = pMask + j * widthMask + hlen;
		for(i = hlen; i < widthMask; i++, pMaskTemp++) {
			if(j + i > 2 * hlen + wlen - 2) {
				*pMaskTemp = 0;
			}		
		}
	}

	//----------------------------------
	int dist = wlen;//dist > 0;
	if(hlen > wlen) {	
		xstep = 2 * wlen;
		ystep = hlen;
		offsetChunk = hlen;
		xstep += dist;
	}else{
		xstep = 2 * hlen;
		ystep = wlen;
		offsetChunk = -wlen;
		xstep += dist;	
		while(offsetChunk < 0) {
			offsetChunk += xstep;
		}		
	}

	startx = 0;
	pixelID = (heightMask / 2) * stride + (widthMask / 2) * depth;
	for(j = 0; j < height - heightMask; j += ystep, startx += offsetChunk) {
		while(startx >= xstep){
			startx -= xstep;
		}	
		//i < startx
		pBuf = dst;
		num = (widthMask + xstep - 1) / xstep;	
		ioffset = j * stride + (-num * xstep + startx) * depth;	
		for(i = - num * xstep + startx; i < startx; i += xstep, ioffset += xstep * depth) {
			pSrcTemp = image + ioffset + hlen * stride;
			sum[0] = (*(pSrcTemp));
			sum[1] = (*(pSrcTemp + 1)); 
			sum[2] = (*(pSrcTemp + 2));
			for(n = 0; n < heightMask; n++)	{
				moffset = ioffset + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, moffset += depth, pMaskTemp++) {
					if(m + i < 0) {
						continue;
					}
					if(*pMaskTemp == 1){
						*(pBuf + moffset) = sum[0];
						*(pBuf + moffset + 1) = sum[1]; 
						*(pBuf + moffset + 2) = sum[2]; 
					}
				}
			}
		}
		//startx <= i < width - widthMask
		pLin = dst + j * stride + startx * depth;	
		for(i = startx; i < width - widthMask; i += xstep, pLin += xstep * depth) {
			sum[0] = (*(pLin + pixelID));
			sum[1] = (*(pLin + pixelID + 1)); 
			sum[2] = (*(pLin + pixelID + 2));	
			for(n = 0; n < heightMask; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}
		//i >= width - widthMask
		for(; i < width; i += xstep, pLin += xstep * depth) {
			sum[0] = (*(pLin +  hlen * stride));
			sum[1] = (*(pLin +  hlen * stride + 1)); 
			sum[2] = (*(pLin +  hlen * stride + 2));	
			for(n = 0; n < heightMask; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask &&  m + i < width; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}				
	}
	//y >= height - heightMask
	pixelID = wlen * depth;
	for(; j < height; j += ystep, startx += offsetChunk) {	
		while(startx >= xstep){
			startx -= xstep;
		}	
		//i < startx
		num = (widthMask + xstep - 1) / xstep;	
		pLin = dst + j * stride + (-num * xstep + startx) * depth;	
		for(i = - num * xstep + startx; i < startx; i += xstep, pLin += xstep * depth) {
			sum[0] = -1;
			for(n = 0; n < heightMask && j + n < height; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(m + i < 0) {
						continue;
					}
					if(*pMaskTemp == 1){
						if(sum[0] == -1) {
							sum[0] = *(pBuf);
							sum[1] = *(pBuf + 1); 
							sum[2] = *(pBuf + 2); 	
						}
						*(pBuf) = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 					
					}
				}
			}
		}
		//startx < i < width - widthMask
		pLin = dst + j * stride + startx * depth;	
		for(i = startx; i < width - widthMask; i += xstep, pLin += xstep * depth) {
			sum[0] = -1;
			for(n = 0; n < heightMask && j + n < height; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						if(sum[0] == -1) {
							sum[0] = *(pBuf);
							sum[1] = *(pBuf + 1); 
							sum[2] = *(pBuf + 2); 	
						}
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}
		//i > width - widthMask
		for(; i < width; i += xstep, pLin += xstep * depth) {
			sum[0] = -1;
			for(n = 0; n < heightMask && j + n < height; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask &&  m + i < width; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						if(sum[0] == -1) {
							sum[0] = *(pBuf);
							sum[1] = *(pBuf + 1); 
							sum[2] = *(pBuf + 2); 	
						}
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}				
	}

	if(pMask != NULL) {
		free(pMask);
		pMask = NULL;
	}
}

void mosaic_triangle0(byte *image, byte *dst, int width, int height, int depth, int stride, int len, int flag){
	int i, j, m, n;
	byte *pMask[2], *pMaskTemp;
	byte *pLin, *pBuf;
	int widthMask, heightMask;
	int sum[3], sum2[3], offsetMask, imask1, imask2;
	int pixelID1, pixelID2;

	memcpy(dst, image, stride * height);
	if(len < 3) {
		return;
	}

	widthMask = len + 1;
	heightMask = len;	
	pMask[0] = (byte *)malloc(widthMask * heightMask);
	if(pMask[0] == NULL) {
		return;
	}
	pMask[1] = (byte *)malloc(widthMask * heightMask);
	if(pMask[1] == NULL) {
		return;
	}
	memset(pMask[0], 0, widthMask * heightMask);
	memset(pMask[1], 0, widthMask * heightMask);
	if(flag == 1){
		pixelID1 = (len + 1) * 3 / 4 * stride + (len - 1) / 4 * depth;
		pixelID2 = (len - 1) / 4 * stride + ((len + 1)  * 3 / 4  + 1 )* depth;
		for(j = 0; j < heightMask; j++) {
			pMaskTemp = pMask[0] + j * widthMask;
			for(i = 0; i < widthMask; i++, pMaskTemp++) {
				if(j - i >= 0) {
					*pMaskTemp = 1;
				}		
			}
		}
		for(j = 0; j < heightMask; j++) {
			pMaskTemp = pMask[1] + j * widthMask;
			for(i = 0; i < widthMask; i++, pMaskTemp++) {
				if(i - j > 0) {
					*pMaskTemp = 1;
				}		
			}
		}
	}else{
		pixelID1 = (len - 1) / 4 * stride + (len - 1) / 4 * depth;
		pixelID2 = (len + 1) * 3 / 4 * stride + ((len + 1)  * 3 / 4  + 1 )* depth;
		for(j = 0; j < heightMask; j++) {
			pMaskTemp = pMask[0] + j * widthMask;
			for(i = 0; i < widthMask; i++, pMaskTemp++) {
				if(i + j < len) {
					*pMaskTemp = 1;
				}		
			}
		}
		for(j = 0; j < heightMask; j++) {
			pMaskTemp = pMask[1] + j * widthMask;
			for(i = 0; i < widthMask; i++, pMaskTemp++) {
				if(i + j >= len) {
					*pMaskTemp = 1;
				}		
			}
		}
			
	}

	for(j = 0; j < height - heightMask; j += heightMask) {
		pLin = dst + j * stride;
		for(i = 0; i < width - widthMask; i += widthMask, pLin += widthMask * depth) {	
			sum[0] = *(pLin + pixelID1);
			sum[1] = *(pLin + pixelID1 + 1);
			sum[2] = *(pLin + pixelID1 + 2);
			sum2[0] = *(pLin + pixelID2);
			sum2[1] = *(pLin + pixelID2 + 1);
			sum2[2] = *(pLin + pixelID2 + 2);
			for(n = 0; n < heightMask; n++) {
				pBuf = pLin + n * stride;
				offsetMask = n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, offsetMask++) {	
					imask1 = *(pMask[0] + offsetMask);
					imask2 = *(pMask[1] + offsetMask);				
					*pBuf = sum[0] * imask1 + sum2[0] * imask2;
					*(pBuf + 1) = sum[1] * imask1 + sum2[1] * imask2;
					*(pBuf + 2) = sum[2] * imask1 + sum2[2] * imask2;				
				}
			}
		}
	}

	if(pMask[0] != NULL) {
		free(pMask[0]);
		pMask[0] = NULL;
	}
	if(pMask[1] != NULL) {
		free(pMask[1]);
		pMask[1] = NULL;
	}
}

void mosaic_triangle45(byte *image, byte *dst, int width, int height, int depth, int stride, int len){
	int i, j, m, n;
	byte *pMask[2], *pMaskTemp;
	byte *pLin, *pBuf;
	int widthMask, heightMask;
	int xstep, ystep;
	int flagx, flagy;
	int pixelID;
	int sum[3] = {0, 0, 0};

	memcpy(dst, image, stride * height);
	if(len < 3) {
		return;
	}

	widthMask = 2 * len - 1;
	heightMask = len;	
	pMask[0] = (byte *)malloc(widthMask * heightMask);
	if(pMask[0] == NULL) {
		return;
	}
	pMask[1] = (byte *)malloc(widthMask * heightMask);
	if(pMask[1] == NULL) {
		return;
	}
	memset(pMask[0], 0, widthMask * heightMask);
	memset(pMask[1], 0, widthMask * heightMask);
	for(j = 0; j < heightMask; j++) {
		pMaskTemp = pMask[0] + j * widthMask;
		for(i = 0; i < len; i++, pMaskTemp++) {
			if(i + j >= len - 1) {
				*pMaskTemp = 1;
			}		
		}
		for(i = len; i < widthMask; i++, pMaskTemp++) {
			if(i - j <= len - 1) {
				*pMaskTemp = 1;
			}		
		}
	}
	for(j = 0; j < heightMask; j++) {
		pMaskTemp = pMask[1] + j * widthMask;
		for(i = 0; i < len; i++, pMaskTemp++) {
			if(i - j >= 0) {
				*pMaskTemp = 1;
			}		
		}
		for(i = len; i < widthMask; i++, pMaskTemp++) {
			if(i + j <= 2 * len - 1) {
				*pMaskTemp = 1;
			}		
		}
	}

	//----------------------------------
	ystep = len;
	xstep = len;
	flagy = 0;
	pixelID = len * stride + len * depth;
	for(j = 0; j < height - heightMask; j += ystep, flagy++) {
		pLin = dst + j * stride;
		sum[0] = (*(pLin));
		sum[1] = (*(pLin + 1)); 
		sum[2] = (*(pLin + 2));
		for(n = 0; n < heightMask; n++) {
			pBuf = pLin + n * stride;
			for(m = 0; m < widthMask / 2; m++, pBuf += depth) {
				*pBuf = sum[0];
				*(pBuf + 1) = sum[1]; 
				*(pBuf + 2) = sum[2]; 
			}
		}
		flagx = flagy % 2;
		for(i = 0; i < width - widthMask; i += xstep, pLin += xstep * depth, flagx++) {
			sum[0] = (*(pLin + pixelID));
			sum[1] = (*(pLin + pixelID + 1)); 
			sum[2] = (*(pLin + pixelID + 2));
			flagx = flagx % 2;			
			for(n = 0; n < heightMask; n++) {
				pBuf = pLin + n * stride;
				pMaskTemp = pMask[flagx] + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}
		for(; i < width; i += xstep, pLin += xstep * depth, flagx++) {
			sum[0] = -1;
			flagx = flagx % 2;			
			for(n = 0; n < heightMask; n++) {
				pBuf = pLin + n * stride;
				pMaskTemp = pMask[flagx] + n * widthMask;
				for(m = 0; m < widthMask && i + m < width; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						if(sum[0] == -1) {
							sum[0] = (*(pLin));
							sum[1] = (*(pLin + 1)); 
							sum[2] = (*(pLin + 2));
						}
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}
	}
	for(; j < height - heightMask; j += ystep, flagy++) {
		pLin = dst + j * stride;
		sum[0] = (*(pLin));
		sum[1] = (*(pLin + 1)); 
		sum[2] = (*(pLin + 2));
		for(n = 0; n < heightMask && j + n < height; n++) {
			pBuf = pLin + n * stride;
			for(m = 0; m < widthMask / 2; m++, pBuf += depth) {
				*pBuf = sum[0];
				*(pBuf + 1) = sum[1]; 
				*(pBuf + 2) = sum[2]; 
			}
		}
		flagx = flagy % 2;
		for(i = 0; i < width - widthMask; i += xstep, pLin += xstep * depth, flagx++) {
			sum[0] = -1;
			flagx = flagx % 2;			
			for(n = 0; n < heightMask && j + n < height; n++) {
				pBuf = pLin + n * stride;
				pMaskTemp = pMask[flagx] + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						if(sum[0] == -1) {
							sum[0] = (*(pLin));
							sum[1] = (*(pLin + 1)); 
							sum[2] = (*(pLin + 2));
						}
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}
		for(; i < width; i += xstep, pLin += xstep * depth, flagx++) {
			sum[0] = -1;
			flagx = flagx % 2;			
			for(n = 0; n < heightMask && j + n < height; n++) {
				pBuf = pLin + n * stride;
				pMaskTemp = pMask[flagx] + n * widthMask;
				for(m = 0; m < widthMask && i + m < width; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						if(sum[0] == -1) {
							sum[0] = (*(pLin));
							sum[1] = (*(pLin + 1)); 
							sum[2] = (*(pLin + 2));
						}
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}
	}


	if(pMask[0] != NULL) {
		free(pMask[0]);
		pMask[0] = NULL;
	}
	if(pMask[1] != NULL) {
		free(pMask[1]);
		pMask[1] = NULL;
	}
}

void mosaic_rectangle0multi(byte *image, byte *dst, int width, int height, int depth, int stride, int wlen, int hlen, int flag) {
	int i, j, m, n, k;
	int widthMask, heightMask, xstep, ystep;
	int numMask, offsetMask,imask, sum[3], pixelValue[10][3];
	byte *pMask[10];
	byte *pLin, *pBuf;
	int inumMask[10];
	int startx, offsetChunk;
	float factor = 0;


	memcpy(dst, image, stride * height);
	switch(flag){
	case 0:
		if(wlen < 3) {
			return;
		}
		numMask = 5;
		widthMask = 2 * wlen;
		heightMask = 2 * wlen;
		xstep = widthMask;
		ystep = heightMask;
		offsetChunk = 0;
		for(i = 0; i < numMask; i++){
			inumMask[i] = 0;
			pMask[i] = (byte *)malloc(sizeof(byte) * widthMask * heightMask);			
			if(pMask[i] == NULL) {
				for(j = 0; j < i; j++) {
					free(pMask[j]);
					pMask[j] = NULL;
					return;
				}
			}
			memset(pMask[i], 0, widthMask * heightMask);
		}
		inumMask[0] = widthMask * heightMask;
		memset(pMask[0], 1, widthMask * heightMask);
		for(j = 0; j < wlen; j++) {
			pLin = pMask[0] + j * widthMask;
			pBuf = pMask[1] + j * widthMask;	
			for(i = 0; i < wlen; i++, pBuf++, pLin++) {
				if(i + j < wlen) {
					*pBuf = 1;
					*pLin = 0;
					inumMask[0]--;
					inumMask[1]++;
				}
			}
		}
		for(j = 0; j < wlen; j++) {
			pLin = pMask[0] + j * widthMask + wlen;
			pBuf = pMask[2] + j * widthMask + wlen;	
			for(i = wlen; i < widthMask; i++, pBuf++, pLin++) {
				if(i - j >= wlen) {
					*pBuf = 1;
					*pLin = 0;
					inumMask[0]--;
					inumMask[2]++;
				}
			}
		}
		for(j = wlen; j < heightMask; j++) {
			pLin = pMask[0] + j * widthMask;
			pBuf = pMask[3] + j * widthMask;	
			for(i = 0; i < wlen; i++, pBuf++, pLin++) {
				if(j - i >= wlen) {
					*pBuf = 1;
					*pLin = 0;
					inumMask[0]--;
					inumMask[3]++;
				}
			}
		}
		for(j = wlen; j < heightMask; j++) {
			pLin = pMask[0] + j * widthMask + wlen;
			pBuf = pMask[4] + j * widthMask + wlen;	
			for(i = wlen; i < widthMask; i++, pBuf++, pLin++) {
				if(i + j >= 3 * wlen - 1) {
					*pBuf = 1;
					*pLin = 0;
					inumMask[0]--;
					inumMask[4]++;
				}
			}
		}
		break;
	case 1:
		if(wlen < 3) {
			return;
		}
		int dx;
		factor = 1.73f;
		numMask = 3;
		hlen = wlen;
		dx = (int)(factor * hlen + 0.5);
		widthMask = 2 * dx;
		heightMask = 2 * hlen + 2 * hlen - 1;
		xstep = widthMask;
		ystep = 3 * hlen;
		offsetChunk = dx;
		for(i = 0; i < numMask; i++){
			inumMask[i] = 0;
			pMask[i] = (byte *)malloc(sizeof(byte) * widthMask * heightMask);			
			if(pMask[i] == NULL) {
				for(j = 0; j < i; j++) {
					free(pMask[j]);
					pMask[j] = NULL;
					return;
				}
			}
			memset(pMask[i], 0, widthMask * heightMask);
		}

		inumMask[1] = (3 * hlen - 1) * dx;
		for(j = 0; j < 3 * hlen - 1; j++) {
			memset(pMask[1] + j * widthMask, 1, dx);
			memset(pMask[2] + j * widthMask + dx, 1, dx);
		}
		for(j = 0; j < hlen - 1; j++) {
			pLin = pMask[0] + (2 * hlen + j) * widthMask;
			offsetMask =j * widthMask;
			for(i = 0; i < dx -1; i++, pLin++, offsetMask++) {
				if(i / factor + j < hlen - 1) {
					*(pMask[1] + offsetMask) = 0;
					*(pMask[1] + widthMask * (3 * hlen - 1) - dx - offsetMask) = 0;
					*(pMask[2] + j * widthMask + widthMask - 1 - i) = 0;
					*(pMask[2] + widthMask * (3 * hlen - 1 - j) - dx + i) = 0;
					inumMask[1] -= 2;						
				}
			}
		}
		inumMask[2] = inumMask[1];
		inumMask[0] = widthMask * (2 * hlen - 1);
		memset(pMask[0] + 2 * hlen * widthMask, 1, widthMask * (2 * hlen - 1));
		for(j = 2 * hlen; j < heightMask; j++) {
			offsetMask = j * widthMask;
			for(i = 0; i < widthMask; i++, offsetMask++) {
				if(*(pMask[1] + offsetMask) || *(pMask[2] + offsetMask)) {
					*(pMask[0] + offsetMask) = 0;
					inumMask[0]--;
				}				
			}
		}	 	
		break;
	default:
		return;
	}

	startx = 0;
	for(j = 0; j < height - heightMask; j += ystep, startx += offsetChunk) {
		while(startx >= xstep) {
			startx -= xstep;
		}
		pLin = dst + j * stride + startx * depth;
		for(i = startx; i < width - widthMask; i += xstep, pLin += xstep * depth) {
			for(k = 0; k < numMask; k++) {
				pixelValue[k][0] = 0;
				pixelValue[k][1] = 0;
				pixelValue[k][2] = 0;
			}
			for(n = 0; n < heightMask; n++) {
				pBuf = pLin + n * stride;
				offsetMask = n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, offsetMask++) {
					for(k = 0; k < numMask; k++) {
						imask = pMask[k][offsetMask];
						pixelValue[k][0] += imask * (*(pBuf));
						pixelValue[k][1] += imask * (*(pBuf + 1));
						pixelValue[k][2] += imask * (*(pBuf + 2));
					}	
				}
			}
			for(k = 0; k < numMask; k++) {
				pixelValue[k][0] = pixelValue[k][0] / inumMask[k];
				pixelValue[k][1] = pixelValue[k][1] / inumMask[k];
				pixelValue[k][2] = pixelValue[k][2] / inumMask[k];
			}	
			for(n = 0; n < heightMask; n++) {
				pBuf = pLin + n * stride;
				offsetMask = n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, offsetMask++) {
					imask = 0;
					for(k = 0; k < numMask; k++) {
						imask += pMask[k][offsetMask];
					}
					if(imask == 0) {
						continue;
					}
					
					sum[0] = 0;
					sum[1] = 0;
					sum[2] = 0;
					for(k = 0; k < numMask; k++) {
						imask = pMask[k][offsetMask];
						sum[0] += (imask * pixelValue[k][0]);
						sum[1] += (imask * pixelValue[k][1]);
						sum[2] += (imask * pixelValue[k][2]);
					}	
					*(pBuf) = sum[0];
					*(pBuf + 1) = sum[1];
					*(pBuf + 2) = sum[2];
				}
			}
		}
	}

}


void mosaic_hexagon(byte *image, byte *dst, int width, int height, int depth, int stride, int wlen, int hlen){
	int i, j, m, n;
	byte *pMask = NULL, *pMaskTemp;
	byte *pLin, *pBuf;
	int sum[3] = {0, 0, 0};
	int widthMask, heightMask;
	int xstep, ystep, startx, offsetChunk;
	int inumMask;

	memcpy(dst, image, stride * height);

	if(wlen < 3) {
		return;
	}
	widthMask = 2 * wlen + 2 * wlen - 2;
	heightMask = 2 * (int)(wlen * 1.73 + 0.5);
	xstep = widthMask + 2 * wlen;
	ystep = heightMask / 2;
	offsetChunk = widthMask + 1 - wlen;
				
	inumMask = widthMask * heightMask;
	pMask = (byte *)malloc(sizeof(byte) * widthMask * heightMask);
	memset(pMask, 1, widthMask * heightMask);
	if(pMask == NULL) {
			return;
	}
	for(j = 0; j < heightMask / 2 - 1; j++) {
		pBuf = pMask + j * widthMask;	
		for(i = 0; i < wlen - 1; i++, pBuf++) {
			if(i + j / 1.73 < wlen - 1) {
				*pBuf = 0;
				inumMask--;		
			}
		}
	}
	for(j = 0; j < heightMask / 2 - 1; j++) {
		pBuf = pMask + j * widthMask + 3 * wlen - 1;	
		for(i = 3 * wlen - 1; i < widthMask; i++, pBuf++) {
			if(i - j / 1.73 > widthMask - wlen) {
				*pBuf = 0;
				inumMask--;
			}
		}
	}
/*	for(j = heightMask / 2  + 1; j < heightMask; j++) {
		pBuf = pMask + j * widthMask;	
		for(i = 0; i < wlen - 1; i++, pBuf++) {
			if((heightMask - 1 - j) / 1.73 + i < wlen -1) {
				*pBuf = 0;
				inumMask--;		
			}
		}
	}
	for(j =  heightMask / 2  + 1; j < heightMask; j++) {
		pBuf = pMask + j * widthMask + 3 * wlen - 1;	
		for(i = 3 * wlen - 1; i < widthMask; i++, pBuf++) {
			if(i + j > widthMask + wlen -1) {
				*pBuf = 0;
				inumMask--;		
			}
		}
	}
	//*/	
	//---------------------------------
	startx = 0;
	for(j = 0; j < height - heightMask; j += ystep, startx += offsetChunk) {
		while(startx >= xstep){
			startx -= xstep;
		}	
		pLin = dst + j * stride + startx * depth;	
		for(i = startx; i < width - widthMask; i += xstep, pLin += xstep * depth) {
			sum[0] = 0;
			sum[1] = 0; 
			sum[2] = 0;	
			for(n = 0; n < heightMask; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						sum[0] += *pBuf;
						sum[1] += *(pBuf + 1); 
						sum[2] += *(pBuf + 2);	
					}
				}
			}
			sum[0] = sum[0] / inumMask;
			sum[1] = sum[1] / inumMask; 
			sum[2] = sum[2] / inumMask;
			for(n = 0; n < heightMask; n++)	{
				pBuf = pLin + n * stride;
				pMaskTemp = pMask + n * widthMask;
				for(m = 0; m < widthMask; m++, pBuf += depth, pMaskTemp++) {
					if(*pMaskTemp == 1){
						*pBuf = sum[0];
						*(pBuf + 1) = sum[1]; 
						*(pBuf + 2) = sum[2]; 
					}
				}
			}
		}			
	}
	
	if(pMask != NULL) {
		free(pMask);
		pMask = NULL;
	}
}

