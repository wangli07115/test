#include "mz_filter_sunny.h"

static const byte gSunnyFilter_Curve_Table[] = {
	0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38,40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 65, 67, 69, 71, 73,75, 76, 78, 80, 81, 83, 85, 86, 88, 90, 91, 93, 95, 96, 98, 99, 101, 102,104, 105, 107, 108, 110, 111, 112, 114, 115, 117, 118, 119, 121, 122,123, 125, 126, 127, 128, 130, 131, 132, 133, 135, 136, 137, 138, 139,140, 141, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 164, 165, 166, 167,168, 169, 170, 171, 171, 172, 173, 174, 175, 175, 176, 177, 178, 179,179, 180, 181, 181, 182, 183, 184, 184, 185, 186, 186, 187, 188, 189,189, 190, 191, 191, 192, 193, 193, 194, 194, 195, 196, 196, 197, 198,198, 199, 199, 200, 201, 201, 202, 202, 203, 204, 204, 205, 205, 206,207, 207, 208, 209, 209, 210, 210, 211, 212, 212, 213, 213, 214, 215,215, 216, 216, 217, 217, 218, 219, 219, 220, 220, 221, 222, 222, 223,223, 224, 224, 225, 225, 226, 227, 227, 228, 228, 229, 229, 230, 230,231, 232, 232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238,238, 239, 239, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245,246, 246, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252,253, 253, 254, 255 ,
};

void		mz_filterSunny(byte* yuv, int width, int height)
{
	register int i = width * height / 2;
	register int r = 0, g = 0, b = 0;
	register int y1 = 0, u1 = 0, v1 = 0, y2 = 0, u2 = 0, v2 = 0;
	register byte* dp = yuv;
	register int s = 80609;
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

		r = Clamp(y1 + (((r - y1) * s) >> 16));
		g = Clamp(y1 + (((g - y1) * s) >> 16));
		b = Clamp(y1 + (((b - y1) * s) >> 16));
		
		r = *(gSunnyFilter_Curve_Table + r);
		g = *(gSunnyFilter_Curve_Table + g);
		b = *(gSunnyFilter_Curve_Table + b);

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

		r = Clamp(y2 + (((r - y2) * s) >> 16));
		g = Clamp(y2 + (((g - y2) * s) >> 16));
		b = Clamp(y2 + (((b - y2) * s) >> 16));
		
		r = *(gSunnyFilter_Curve_Table + r);
		g = *(gSunnyFilter_Curve_Table + g);
		b = *(gSunnyFilter_Curve_Table + b);

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

void		mz_imageFilterSunny(byte* image, int width, int height, int depth) {
	register int i = width * height;
	register int r = 0, g = 0, b = 0, gray = 0;
	register byte* dp = image;
	register int s = 80609;
	while (i--) {
		r = RGB_PR(dp);
		g = RGB_PG(dp);
		b = RGB_PB(dp);

		gray = GRAY(r, g, b);
		r = Clamp(gray + (((r - gray) * s) >> 16));
		g = Clamp(gray + (((g - gray) * s) >> 16));
		b = Clamp(gray + (((b - gray) * s) >> 16));
		
		RGB_PR(dp) = *(gSunnyFilter_Curve_Table + r);
		RGB_PG(dp) = *(gSunnyFilter_Curve_Table + g);
		RGB_PB(dp) = *(gSunnyFilter_Curve_Table + b);
		dp += depth;
	}
}