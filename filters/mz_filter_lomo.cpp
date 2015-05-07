#include "mz_filter_lomo.h"

void		mz_filterLomo(byte* yuv, int width, int height)
{
	register int i = height * width / 2;
	register byte* dp = yuv;
	register int y1 = 0, u1 = 0, v1 = 0, y2 = 0, u2 = 0, v2 = 0;
	register int r = 0, g = 0, b = 0, gray = 0;
	int a1 = 55706, a2 = 9830;
	int roff = 255, goff = 255, boff = 255;
	while (i--) {
		y1 = YUYV_PY1(dp);
		u1 = u2 = YUYV_PU(dp);
		y2 = YUYV_PY2(dp);
		v1 = v2 = YUYV_PV(dp);

		r = *(gYUV2RGB_YTable + y1) + *(gYUV2R_VTable + v1);
		g = *(gYUV2RGB_YTable + y1) - *(gYUV2G_VTable + v1) - *(gYUV2G_UTable + u1);
		b = *(gYUV2RGB_YTable + y1) + *(gYUV2B_UTable + u1);

		r = Clamp(r);
		g = Clamp(g);
		b = Clamp(b);

		r = *(gCrossProcess_RColorTable + r);
		g = *(gCrossProcess_GColorTable + g);
		b = *(gCrossProcess_BColorTable + b);

		gray = (r + g + b) / 3;
		r = gray + ((r - gray) >> 1);
		g = gray + ((g - gray) >> 1);
		b = gray + ((b - gray) >> 1);

		//r = Clamp(((r * a1 + roff * a2) >> 16));
		//g = Clamp(((g * a1 + goff * a2) >> 16));
		//b = Clamp(((b * a1 + boff * a2) >> 16));

		y1 = *(gRGB2Y_RTable + r) + *(gRGB2Y_GTable + g) + *(gRGB2Y_BTable + b) + 16;
		u1 = *(gRGB2U_RTable + r) + *(gRGB2U_GTable + g) + *(gRGB2U_BTable + b) + 128;
		v1 = *(gRGB2V_RTable + r) + *(gRGB2V_GTable + g) + *(gRGB2V_BTable + b) + 128;

		// -------------------------------

		r = *(gYUV2RGB_YTable + y2) + *(gYUV2R_VTable + v2);
		g = *(gYUV2RGB_YTable + y2) - *(gYUV2G_VTable + v2) - *(gYUV2G_UTable + u2);
		b = *(gYUV2RGB_YTable + y2) + *(gYUV2B_UTable + u2);

		r = Clamp(r);
		g = Clamp(g);
		b = Clamp(b);

		r = *(gCrossProcess_RColorTable + r);
		g = *(gCrossProcess_GColorTable + g);
		b = *(gCrossProcess_BColorTable + b);

		gray = (r + g + b) / 3;
		r = gray + ((r - gray) >> 1);
		g = gray + ((g - gray) >> 1);
		b = gray + ((b - gray) >> 1);

		//r = Clamp(((r * a1 + roff * a2) >> 16));
		//g = Clamp(((g * a1 + goff * a2) >> 16));
		//b = Clamp(((b * a1 + boff * a2) >> 16));

		y2 = *(gRGB2Y_RTable + r) + *(gRGB2Y_GTable + g) + *(gRGB2Y_BTable + b) + 16;
		u2 = *(gRGB2U_RTable + r) + *(gRGB2U_GTable + g) + *(gRGB2U_BTable + b) + 128;
		v2 = *(gRGB2V_RTable + r) + *(gRGB2V_GTable + g) + *(gRGB2V_BTable + b) + 128;

		YUYV_PY1(dp) = y1;
		YUYV_PY2(dp) = y2;
		YUYV_PU(dp) = ((u1 + u2) >> 1);
		YUYV_PV(dp) = ((v1 + v2) >> 1);
		dp += 4;
	}
}

void		mz_imageFilterLomo(byte* image, int width, int height, int depth) {
	register int i = height * width;
	register byte* dp = image;
	register int r = 0, g = 0, b = 0, gray = 0;
	int a1 = 55706, a2 = 9830;
	while (i--) {
		r = *(gCrossProcess_RColorTable + RGB_PR(dp));
		g = *(gCrossProcess_GColorTable + RGB_PG(dp));
		b = *(gCrossProcess_BColorTable + RGB_PB(dp));

		gray = (r + g + b) / 3;
		RGB_PR(dp) = gray + ((r - gray) >> 1);
		RGB_PG(dp) = gray + ((g - gray) >> 1);
		RGB_PB(dp) = gray + ((b - gray) >> 1);
		dp += depth;
	}
}