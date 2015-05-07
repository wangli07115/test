#include <math.h>
#include "dominant_colors.h"

#ifndef CLAMP
#define CLAMP(v)										(v > 255 ? 255 : v < 0 ? 0 : v)
#endif
#ifndef MIN
#define MIN(a, b)										(a > b ? b : a)
#endif
#ifndef MAX
#define MAX(a, b)										(a > b ? a : b)	
#endif


void RGB2HSV(byte *rgb, float *hsvH, float* hsvS, float* hsvV)
{
	float R, G, B;
	float minV, maxV, delta,tmp;
	R = PR(rgb) / 255.0f;
	G = PG(rgb) / 255.0f;
	B = PB(rgb) / 255.0f;

	float H = 0.0f;
	// r,g,b values are from 0 to 1
    // h = [0,360], s = [0,1], v = [0,1]
    // if s == 0, then h = -1 (undefined)  
    
    tmp = MIN(R, G);
    minV = MIN( tmp, B );
    tmp = MAX( R, G);
    maxV = MAX(tmp, B );
    
    delta = maxV - minV;
    (*hsvV) = maxV; // v

	// s
    if( maxV != 0 )
      (*hsvS) = delta / maxV; 
    else
    {// r = g = b = 0 // s = 0, v is undefined
      *hsvS = 0;
      *hsvH = 360;
      return;
    }

	if(delta == 0)//r = g = b = V
	{
		*hsvH = 360;
		return;
	}
    if( R == maxV )//[-1,1]
        H = ( G - B ) / delta; // between yellow & magenta
    else if( G == maxV )//[1,3]
        H = 2 + ( B - R ) / delta; // between cyan & yellow
    else//[3,5]
        H = 4 + ( R - G ) / delta; // between magenta & cyan
    H = H * 60; // degrees
    if( H < 0 )
	{
       		H += 360;
	}

	*hsvH = H;
}

//void HSV2RGB(int &r, int &g, int &b, float h, float s, float v)
void HSV2RGB(int *rgbR, int *rgbG, int* rgbB, float h, float s, float v)
{
	//h(0,360)
	// H is given on [0->6] or -1. S and V are given on [0->1].
	// RGB are each returned on [0->1].
	float  f;
	int m, n, c;
	int i;
	int r = 0, g = 0, b = 0;

	if(v == 0)
	{
		(*rgbR) = (*rgbG) = (*rgbB) = 0;
		return;
	}
	if(s == 0)
	{
		(*rgbR) = (*rgbG) = (*rgbB) = (int)(v * 255);
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
	switch (i)
	{
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
	
	*rgbR = r;
	*rgbG = g;
	*rgbB  = b;
}


int find_max_hist(int *hist, int num)
{
	int max = 0;
	int maxID = 0;
	int i;
	for (i = 0; i < num; i++)
	{
		if(max < hist[i])
		{
			max = hist[i];
			maxID = i;
		}
	}
	
	return maxID;
}

void normalization(float hsv[3],int num[3], int* h, int* s, int* v)
{
	
	*h = (int)(hsv[0] * num[0] / 360.01f);
	*s = (int)(hsv[1] * num[1] / 1.01f);
	*v = (int)(hsv[2] * num[2] / 1.01f);

}
void getHSV(int iHsv[3],int num[3], float* h, float* s, float* v)
{
	*h = iHsv[0] * 360.01f / num[0];
	*s = iHsv[1] * 1.01f / num[1];
	*v = iHsv[2] * 1.01f / num[2];
}

/*
void copyColor(byte *image, int width, int height, int bpp, int stride,int *color, int radius)
{
	int i, j;
	byte *pBuf, *pTemp;
	int r2 = radius * radius;
	float rate = 0, ratey = 0,ratex = 0;

	for(j = radius; j < height - radius; j++)
	{
		pBuf = image + j * stride;
		pTemp = image + j * stride + (width - 1) * bpp;
		for(i = 0; i < radius; i++, pBuf += bpp, pTemp -= bpp)
		{
			rate = ((float)radius - i) / radius;
			rate = rate * rate;
			PR(pBuf) = PR(pBuf) + (color[0] - PR(pBuf)) * rate;
			PG(pBuf) = PG(pBuf) + (color[1] - PG(pBuf)) * rate;
			PB(pBuf) = PB(pBuf) + (color[2] - PB(pBuf)) * rate;

			PR(pTemp) = PR(pTemp) + (color[0] - PR(pTemp)) * rate;
			PG(pTemp) = PG(pTemp) + (color[1] - PG(pTemp)) * rate;
			PB(pTemp) = PB(pTemp) + (color[2] - PB(pTemp)) * rate;
		}
	}

	for(j = 0; j < radius; j++)
	{
		pBuf = image + j * stride;
		pTemp = image + (height - 1 - j) * stride;
		rate = ((float)radius - j) / radius;
		rate = rate * rate;
		for(i = 0; i < radius; i++, pBuf += bpp, pTemp += bpp)
		{
			ratex = ((float)radius - i) / radius;
			ratex = ratex * ratex;
			ratex = MAX(ratex, rate);
		
			PR(pBuf) = color[0];//PR(pBuf) + (color[0] - PR(pBuf)) * ratex;
			PG(pBuf) = color[1];//PG(pBuf) + (color[1] - PG(pBuf)) * ratex;
			PB(pBuf) = color[2];//PB(pBuf) + (color[2] - PB(pBuf)) * ratex;

			//PR(pBuf) = PR(pBuf) + (color[0] - PR(pBuf)) * ratex;
			//PG(pBuf) = PG(pBuf) + (color[1] - PG(pBuf)) * ratex;
			//PB(pBuf) = PB(pBuf) + (color[2] - PB(pBuf)) * ratex;

			PR(pTemp) = PR(pTemp) + (color[0] - PR(pTemp)) * ratex;
			PG(pTemp) = PG(pTemp) + (color[1] - PG(pTemp)) * ratex;
			PB(pTemp) = PB(pTemp) + (color[2] - PB(pTemp)) * ratex;
		}

		for(i = radius; i < width - radius; i++, pBuf += bpp, pTemp += bpp)
		{		
			PR(pBuf) = PR(pBuf) + (color[0] - PR(pBuf)) * rate;
			PG(pBuf) = PG(pBuf) + (color[1] - PG(pBuf)) * rate;
			PB(pBuf) = PB(pBuf) + (color[2] - PB(pBuf)) * rate;

			PR(pTemp) = PR(pTemp) + (color[0] - PR(pTemp)) * rate;
			PG(pTemp) = PG(pTemp) + (color[1] - PG(pTemp)) * rate;
			PB(pTemp) = PB(pTemp) + (color[2] - PB(pTemp)) * rate;
		}
		for(i = width - radius; i < width; i++, pBuf += bpp, pTemp += bpp)
		{
			ratex = ((float)radius - (width - i)) / radius;
			ratex = ratex * ratex;
			ratex = MAX(ratex, rate);
		
			PR(pBuf) = PR(pBuf) + (color[0] - PR(pBuf)) * ratex;
			PG(pBuf) = PG(pBuf) + (color[1] - PG(pBuf)) * ratex;
			PB(pBuf) = PB(pBuf) + (color[2] - PB(pBuf)) * ratex;

			PR(pTemp) = PR(pTemp) + (color[0] - PR(pTemp)) * ratex;
			PG(pTemp) = PG(pTemp) + (color[1] - PG(pTemp)) * ratex;
			PB(pTemp) = PB(pTemp) + (color[2] - PB(pTemp)) * ratex;
		}
	}
}
*/
int color_extraction(byte *image, int width, int height, int bpp, int stride, int &rgbValue, int &BlackOrWhite)
{
	int i, j;
	byte *pBuf;
	int color[3] = {0};
	int radius = MIN(width, height) / 10;
	int histH[362] = {0};
	int histS[256] = {0};
	int histV[256] = {0};

	int iHsv[3] = {0};
	float hsv[3];
	int  num[3] = {360,100,100};

	for(j = 0; j < height; j++)
	{
		pBuf = image + j * stride;
		for(i = 0; i < width; i++, pBuf += bpp)
		{
			RGB2HSV(pBuf, &hsv[0], &hsv[1], &hsv[2]);	
			normalization(hsv, num,& iHsv[0], &iHsv[1], &iHsv[2]);
			if(i < radius || i > width - radius || j < radius || j > height - radius)
			{
				histH[iHsv[0]]++;
				histS[iHsv[1]]++;
				histV[iHsv[2]]++;
			}
		}
	}

	iHsv[0] = find_max_hist(histH, num[0]);
	iHsv[1] = find_max_hist(histS, num[1]);
	iHsv[2] = find_max_hist(histV, num[2]);


	getHSV(iHsv, num, &hsv[0], &hsv[1], &hsv[2]);
	HSV2RGB(&color[0], &color[1], &color[2], hsv[0], hsv[1], hsv[2]);

	rgbValue = (color[0] << 16) | (color[1] << 8) | color[2] | 0xff000000;

	LOG("color_extraction :r = %d, g = %d, b= %d", (int)color[0], (int)color[1], (int)color[2]);

//	rgbValue = ((color[0] &0xff) << 24) +  ((color[0] &0xff) << 16)  +  ((color[0] &0xff) << 8)  +  ((color[0] &0xff) );
	

//	copyColor(image, width, height, bpp, stride, color, radius * 2);

	BlackOrWhite = 0;
	if(hsv[3] < 0.5)
	{
		BlackOrWhite = 1;
	}


	return 1;
}

/*
int colorExtractionProcess(JNIEnv* env, jobject bitmap,  int width, int height, int *rgbValue, int *BlackOrWhite)
{
		byte* image = NULL;
	//	int width = bitmap.getWidth();
       	//	int height = bitmap.getHeight();
		int bpp = 4;
		int stride = width * bpp;
		
		AndroidBitmap_lockPixels(env, bitmap, (void**) &image);
		if(image == NULL)
		{
			return 0;
		}

		
		color_extraction(image, width, height, bpp, stride, rgbValue, BlackOrWhite);
	

		AndroidBitmap_unlockPixels(env, bitmap);
	
		return BlackOrWhite;
}
*/
