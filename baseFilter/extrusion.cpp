#include <math.h>
#include "baseFilters.h"


int Extrusion(unsigned char *image, int width, int height, int centerX, int centerY, int radius, int powIdx)
{
	if (powIdx > 100) powIdx = 100;
	if (powIdx < -100) powIdx = -100;	
	float fPowIdx = (float)powIdx/100.0;
	int bpp = 3;
	int stride = bpp * width;
	while(stride % 4 != 0) {
		stride = (stride / 4 + 1) * 4;
	}

	int x, y, k;
	unsigned char *src = NULL;
	src = (unsigned char *)malloc(sizeof(unsigned char) * stride * height);
	memcpy(src, image, stride * height);

	memset(image, 0, stride * height);

	radius= 100;
	
	radius = radius * MIN(width,height) / 100;
	
	int nRSqrt = radius * radius;

	float *SXPowTable = NULL;
	SXPowTable =(float *)malloc(sizeof(float)*(radius+1));
	for (int k = 0; k <= radius; k++)
	{
		SXPowTable[k] = pow((float)(k*k)/nRSqrt, fPowIdx);
	}

	unsigned char *pdst;
	for(y = 0; y < height; y++)
	{
		pdst = image + y * stride;
		for(x = 0; x < width; x++, pdst += bpp)
		{
		/*	int dxc = x - centerX;
			int dyc = y - centerY;
			float nDist = dxc*dxc + dyc*dyc;

			if(nDist >= nRSqrt)
			{
				continue;
			}

		//	float fFactor = pow(nDist/nRSqrt, fPowIdx);
		//	int indx = (int) sqrt( nDist);			
		//	float fFactor = SXPowTable[idx];
			int indx = 50;
			int fx = centerX + dxc *indx;
			int fy = centerY + dyc *indx;

			fx = fx < 0 ? 0 : fx > width - 1 ? width - 1 : fx;
			fy = fy < 0 ? 0 : fy > height - 1? height - 1: fy;

			memcpy(pdst, src + fy * stride + fx * bpp, bpp);


			/*
			//	float fFactor = SXPow(nDist/nRSqrt, fPowIdx);
			int idx = (int) SXSqrt( nDist);			
			float fFactor = SXPowTable[idx];
			int nFactor = (int)(fFactor*(1<<16));

			int dmx   = dxc*nFactor>>8;
			int dmy   = dyc*nFactor>>8;
			int bx	  = nCX8 + dmx;
			int by	  = nCY8 + dmy;

			if(bx<0) bx = 0;
			if(bx>nMaxX) bx = nMaxX;
			if(by<0) by = 0;
			if(by>nMaxY) by = nMaxY;

			int xx	= bx>>8;
			int yy	= by>>8;
			int dx = bx&0xFF;//µÍ8Î»
			int dy = by&0xFF;


			//²åÖµ
			unsigned char *pSrcPixel00 = (uint8 *)imageTemp->GetPixel(xx,yy);
			unsigned char *pSrcPixel10 = pSrcPixel00+bpp;
			unsigned char *pSrcPixel01 = pSrcPixel00+stride;
			unsigned char *pSrcPixel11 = pSrcPixel10+stride;		

			for (k = 0; k < bpp; k++)
			{
				int fx0,fx1;
				fx0 =pSrcPixel00[k] + ( ((pSrcPixel10[k]-pSrcPixel00[k])*dx)>>8 ) ;
				fx1 =pSrcPixel01[k] + ( ((pSrcPixel11[k]-pSrcPixel01[k])*dx)>>8 ) ;
				pDst[k]  = fx0 + ( ((fx1-fx0)*dy)>>8);
			}
			*/

		}
	}

	if(NULL != src) {
		free(src);
		src = NULL;
	}
	return 0;
}
