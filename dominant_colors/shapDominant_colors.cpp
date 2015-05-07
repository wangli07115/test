#include "dominant_colors.h"

#define NATIVE(type)							extern "C" __declspec(dllexport) type
#define FUNCTION(cls, name, ...)				name(__VA_ARGS__)

NATIVE(void)  FUNCTION(filtersForm, nativeProcess, byte *image, int width, int height,
		int edgeSize, int* bgColorArray, int *textColorArray) {
	int bgColor, textColor;
	unsigned char* rgba = NULL;
	int bpp = 4;
	int stride = bpp * width;
	unsigned char red, green, blue;

	if (NULL != image && width > 0 && height > 0) {
		color_analyze(image, width, height, edgeSize, bgColor, textColor);

		getcolor(bgColor, red, green, blue);
	//	LOG("nativeProcess :r = %d, g = %d, b= %d", (int)red, (int)green, (int)blue);
	}
}

/*
NATIVE(void)  FUNCTION(DominantColors, nativeFillcolor, byte * image, jint width, jint height, jint color) {
	unsigned char* rgba = NULL;

	color = (255 << 16) | (0 << 8) | 0 | 0xff000000;

	if (NULL != rgba && width > 0 && height > 0) {
		fillcolor(rgba, width, height, color);
	}
}
*/

NATIVE(void)  FUNCTION(filtersForm, nativeTopEdge, byte *image, int width, int height, int color) {
	unsigned char* rgba = NULL;
	int depth = 3;
	int stride = depth * width;
	if(stride % 4 != 0) {
		stride = (stride / 4 + 1) * 4;
	}

	color = (128 << 16) | (128 << 8) | 0 | 0xff000000;
	if (NULL != image && width > 0 && height > 0) {
		topEdge(image, width, height, depth, stride, color);
	}
}

