/*
******WANGLI*******
*/
#ifndef ___GAUSS_IRR_H___
#define ___GAUSS_IRR_H___

void gauss_iir(unsigned char* img,unsigned char* imgBlur, int width, int height, int depth, int stride, 
			   float horz, float vert);
#endif