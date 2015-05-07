#include "mz_filter_bw.h"

static const byte 	gBWFilter_BrightnessContrast_Table[] = {
		    0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,113,114,115,116,117,118,119,120,121,122,123,124,125,126,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,255,255,255,255,255,255,255,
};

void		mz_filterBlackAndWhite(byte* yuv, int width, int height)
{
	register int i = height, j = 0;
	register byte* dp = yuv;
	register int gray = 0;
	register int cx = width / 2, cy = height / 2, radius = (cx * cx + cy * cy) * 2;
	register int mask = 87163;

	while (i--) {
		j = width;
		while (j--) {
			gray = *(dp);
			gray = ((gray * mask) >> 16);
			gray = gray - ((i - cy) * (i - cy) + (j - cx) * (j - cx)) * 255 / radius;
			gray = Clamp(gray);

			*(dp) = *(gBWFilter_BrightnessContrast_Table + gray);
			*(dp + 1) = 128;
			dp += 2;
		}
	}
}

void		mz_imageFilterBlackWhite(byte* image, int width, int height, int depth) {
	register int i = height, j = 0;
	register byte* dp = image;
	register int gray = 0;
	register int cx = width / 2, cy = height / 2, radius = (cx * cx + cy * cy) * 2;
	register int mask = 87163;
	register int r = 0, g = 0, b = 0;

	while (i--) {
		j = width;
		while (j--) {
			r = RGB_PR(dp);
			g = RGB_PG(dp);
			b = RGB_PB(dp);

			gray = GRAY(r,g,b);
			gray = ((gray * mask) >> 16);
			gray = gray - ((i - cy) * (i - cy) + (j - cx) * (j - cx)) * 255 / radius;
			gray = Clamp(gray);

			RGB_PR(dp) = RGB_PG(dp) = RGB_PB(dp) = *(gBWFilter_BrightnessContrast_Table + gray);
			dp += depth;
		}
	}
}