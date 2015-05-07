#include "mz_filter_reminiscence.h"


void		mz_filterReminiscence(byte* yuv, int width, int height) 
{
	register int i = width * height / 2;
	register int r = 0, g = 0, b = 0;
	register int y1 = 0, u1 = 0, v1 = 0, y2 = 0, u2 = 0, v2 = 0;
	register byte* dp = yuv;
	int max = 0;
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

		r = y1 + ((141 * (r - y1)) >> 8);
		g = y1 + ((141 * (g - y1)) >> 8);
		b = y1 + ((141 * (b - y1)) >> 8);

		r = ((r * 301) >> 8);
		g = ((g * 301) >> 8);
		b = ((b * 301) >> 8);

		r = Clamp(r + 5);
		g = Clamp(g + 5);
		b = Clamp(b - 5);


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

		r = y2 + ((141 * (r - y2)) >> 8);
		g = y2 + ((141 * (g - y2)) >> 8);
		b = y2 + ((141 * (b - y2)) >> 8);

		r = ((r * 301) >> 8);
		g = ((g * 301) >> 8);
		b = ((b * 301) >> 8);

		r = Clamp(r + 5);
		g = Clamp(g + 5);
		b = Clamp(b - 5);

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