#include "mz_filter_toy.h"

static const byte gToyFilter_Curve_Table[] = {
	0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7,8, 8, 9, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14,15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 21,22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28, 29, 29,30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39,40, 41, 42, 43, 43, 44, 45, 46, 47, 48, 49, 50, 51,52, 53, 54, 55, 56, 57, 58, 59, 60, 62, 63, 64, 65,66, 67, 69, 70, 71, 72, 74, 75, 76, 78, 79, 81, 82,83, 85, 86, 88, 89, 91, 92, 94, 95, 97, 98, 100, 101,103, 105, 106, 108, 110, 111, 113, 114 ,116, 118,119, 121, 123, 125, 126, 128, 130, 131, 133, 135,136, 138, 140, 142, 143, 145, 147, 148, 150, 152,154, 155, 157, 159, 160, 162, 164, 165, 167, 168,170, 172, 173, 175, 177, 178, 180, 181, 183, 184,186, 187, 189, 190, 192, 193, 195, 196, 197, 199,200, 201, 203, 204, 205, 206, 208, 209, 210, 211,212, 214, 215, 216, 217, 218, 219, 220, 221 ,222,223, 224, 225, 226, 227, 228, 229, 230, 230, 231,232, 233, 234, 234, 235, 236, 236, 237, 238, 239,239, 240, 240, 241, 242, 242, 243, 243, 244, 245,245, 246, 246, 247, 247, 248, 248, 249, 249, 250,250, 251, 251, 252, 252, 253, 253, 254, 254, 255,
};

void		mz_filterToy(byte* yuv, int width, int height)
{
	register int i = width * height / 2;
	register int r = 0, g = 0, b = 0, gray = 0;
	register int y1 = 0, u1 = 0, v1 = 0, y2 = 0, u2 = 0, v2 = 0;
	register byte* dp = yuv;
	register int s = 95683;
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

		r = *(gToyFilter_Curve_Table + r);
		g = *(gToyFilter_Curve_Table + g);
		b = *(gToyFilter_Curve_Table + b);

		gray = (r + g + b) / 3;
		r = Clamp(gray + (((r - gray) * s) >> 16) - 1);
		g = Clamp(gray + (((g - gray) * s) >> 16) + 10);
		b = Clamp(gray + (((b - gray) * s) >> 16) + 20);

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

		r = *(gToyFilter_Curve_Table + r);
		g = *(gToyFilter_Curve_Table + g);
		b = *(gToyFilter_Curve_Table + b);

		gray = (r + g + b) / 3;
		r = Clamp(gray + (((r - gray) * s) >> 16) - 1);
		g = Clamp(gray + (((g - gray) * s) >> 16) + 10);
		b = Clamp(gray + (((b - gray) * s) >> 16) + 20);

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

void		mz_imageFilterToy(byte* image, int width, int height, int depth) {
	register int i = width * height;
	register int r = 0, g = 0, b = 0, gray = 0;
	register byte* dp = image;
	register int s = 95683;
	while (i--) {
		r = *(gToyFilter_Curve_Table + RGB_PR(dp));
		g = *(gToyFilter_Curve_Table + RGB_PG(dp));
		b = *(gToyFilter_Curve_Table + RGB_PB(dp));

		gray = (r + g + b) / 3;
		RGB_PR(dp) = Clamp(gray + (((r - gray) * s) >> 16) - 1);
		RGB_PG(dp) = Clamp(gray + (((g - gray) * s) >> 16) + 10);
		RGB_PB(dp) = Clamp(gray + (((b - gray) * s) >> 16) + 20);
		dp += depth;
	}
}