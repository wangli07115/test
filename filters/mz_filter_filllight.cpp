#include "mz_filter_filllight.h"

static const byte gFillLightFilter_Curve_Table[] = {
		0, 1, 3, 4, 6, 7, 9, 11, 12, 14, 15, 17, 18, 20, 22, 23, 25, 26, 28, 29,31, 33, 34, 36, 37, 39, 40, 42, 44, 45, 47, 48, 50, 51, 53, 54, 56, 57,59, 60, 62, 64, 65, 67, 68, 70, 71, 73, 74, 76, 77, 79, 80, 82, 83, 85,86, 88, 89, 91, 92, 94, 95, 96, 98, 99, 101, 102, 104, 105, 107, 108,109, 111, 112, 114, 115, 116, 118, 119, 121, 122, 123, 125, 126, 127,129, 130, 132, 133, 134, 136, 137, 138, 139, 141, 142, 143, 145, 146,147, 149, 150, 151, 152, 154, 155, 156, 157, 158, 160, 161, 162, 163,164, 166, 167, 168, 169, 170, 171, 173, 174, 175, 176, 177, 178, 179,180, 181, 182, 183, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,195, 196, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 205, 206,207, 208, 209, 210, 210, 211, 212, 213, 214, 214, 215, 216, 216, 217,218, 219, 219, 220, 221, 221, 222, 223, 223, 224, 224, 225, 226, 226,227, 227, 228, 228, 229, 230, 230, 231, 231, 232, 232, 233, 233, 234,234, 235, 235, 236, 236, 236, 237, 237, 238, 238, 239, 239, 239, 240,240, 241, 241, 241, 242, 242, 243, 243, 243, 244, 244, 244, 245, 245,245, 246, 246, 246, 247, 247, 247, 248, 248, 248, 249, 249, 249, 250,250, 250, 251, 251, 251, 252, 252, 252, 252, 253, 253, 253, 254, 254,254, 255,
};

void		mz_filterFilllight(byte* yuv, int width, int height)
{
	register int i = width * height / 2;
	register int r = 0, g = 0, b = 0;
	register int y1 = 0, u1 = 0, v1 = 0, y2 = 0, u2 = 0, v2 = 0;
	register int gray = 0, s = 91095;
	register byte* dp = yuv;
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

		r = *(gFillLightFilter_Curve_Table + r);
		g = *(gFillLightFilter_Curve_Table + g);
		b = *(gFillLightFilter_Curve_Table + b);

		gray = (r + g + b) / 3;
		r = Clamp(gray + (((r - gray) * s) >> 16));
		g = Clamp(gray + (((g - gray) * s) >> 16));
		b = Clamp(gray + (((b - gray) * s) >> 16));

		y1 = *(gRGB2Y_RTable + r) + *(gRGB2Y_GTable + g) + *(gRGB2Y_BTable + b) + 16;
		u1 = *(gRGB2U_RTable + r) + *(gRGB2U_GTable + g) + *(gRGB2U_BTable + b) + 128;
		v1 = *(gRGB2V_RTable + r) + *(gRGB2V_GTable + g) + *(gRGB2V_BTable + b) + 128;

		//-------------------
		r = *(gYUV2RGB_YTable + y2) + *(gYUV2R_VTable + v2);
		g = *(gYUV2RGB_YTable + y2) - *(gYUV2G_VTable + v2) - *(gYUV2G_UTable + u2);
		b = *(gYUV2RGB_YTable + y2) + *(gYUV2B_UTable + u2);

		r = Clamp(r);
		g = Clamp(g);
		b = Clamp(b);

		r = *(gFillLightFilter_Curve_Table + r);
		g = *(gFillLightFilter_Curve_Table + g);
		b = *(gFillLightFilter_Curve_Table + b);

		gray = (r + g + b) / 3;
		r = Clamp(gray + (((r - gray) * s) >> 16));
		g = Clamp(gray + (((g - gray) * s) >> 16));
		b = Clamp(gray + (((b - gray) * s) >> 16));

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

void		mz_imageFilterFilllight(byte* image, int width, int height, int depth) {
	register int i = width * height;
	register int r = 0, g = 0, b = 0;
	register int gray = 0, s = 91095;
	register byte* dp = image;
	while (i--) {
		r = RGB_PR(dp);
		g = RGB_PG(dp);
		b = RGB_PB(dp);

		r = *(gFillLightFilter_Curve_Table + r);
		g = *(gFillLightFilter_Curve_Table + g);
		b = *(gFillLightFilter_Curve_Table + b);

		gray = (r + g + b) / 3;
		RGB_PR(dp) = Clamp(gray + (((r - gray) * s) >> 16));
		RGB_PG(dp) = Clamp(gray + (((g - gray) * s) >> 16));
		RGB_PB(dp) = Clamp(gray + (((b - gray) * s) >> 16));

		dp += depth;
	}
}