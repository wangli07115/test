#include "mz_filter_moonlight.h"

static const byte	gMoonlightFilter_ColorRTable[] = {
	0,0,1,2,2,3,4,5,5,6,7,8,8,9,10,11,11,12,13,14,14,15,16,17,17,18,19,20,20,21,22,23,24,24,25,26,27,28,28,29,30,31,32,32,33,34,35,36,37,38,38,39,40,41,42,43,44,45,46,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,77,78,79,80,81,82,83,85,86,87,88,89,91,92,93,94,96,97,98,99,101,102,103,105,106,108,109,110,112,113,114,116,117,119,120,122,123,125,126,128,129,131,132,134,135,137,138,139,141,142,143,145,146,148,149,150,152,153,154,156,157,158,159,161,162,163,164,166,167,168,169,170,171,173,174,175,176,177,178,179,180,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,206,207,208,209,210,211,212,213,214,214,215,216,217,218,219,220,220,221,222,223,224,225,225,226,227,228,229,229,230,231,232,233,233,234,235,236,236,237,238,239,239,240,241,242,242,243,244,245,245,246,247,248,248,249,250,251,251,252,253,254,255,
};

static const byte	gMoonlightFilter_ColorGTable[] = {
	0,0,1,2,2,3,4,5,5,6,7,8,9,9,10,11,12,13,13,14,15,16,17,18,18,19,20,21,22,23,24,24,25,26,27,28,29,29,30,31,32,33,34,35,36,37,38,39,40,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,72,73,74,75,76,77,78,79,80,81,82,83,85,86,87,88,89,90,92,93,94,95,96,97,98,100,101,102,103,104,105,107,108,109,110,111,113,114,115,116,117,119,120,121,122,124,125,126,128,129,130,131,133,134,135,136,137,139,140,141,142,144,145,146,147,148,149,151,152,153,154,155,156,158,159,160,161,162,163,164,166,167,168,169,170,171,172,174,175,176,177,178,179,180,181,182,183,184,185,186,187,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,223,224,225,226,227,228,228,229,230,231,232,233,234,235,235,236,237,238,239,240,240,241,242,243,244,244,245,246,247,248,248,249,250,251,251,252,253,254,255,
};

static const byte	gMoonlightFilter_ColorBTable[] = {
	16,17,18,19,19,20,21,22,23,24,25,26,26,27,28,29,30,31,32,33,33,34,35,36,37,38,39,40,40,41,42,43,44,45,45,46,47,48,49,50,51,52,52,53,54,55,56,57,58,59,59,60,61,62,63,64,65,66,66,67,68,69,70,71,72,73,73,74,75,76,77,78,79,80,80,81,82,83,84,85,85,86,87,88,89,90,91,92,92,93,94,95,96,97,98,99,99,100,101,102,103,104,105,106,106,107,108,109,110,111,112,113,113,114,115,116,117,118,119,120,120,121,122,123,124,125,126,127,127,128,129,130,131,132,132,133,134,135,136,137,138,139,139,140,141,142,143,144,145,146,146,147,148,149,150,151,152,153,153,154,155,156,157,158,159,160,160,161,162,163,164,165,166,167,167,168,169,170,171,172,172,173,174,175,176,177,178,179,179,180,181,182,183,184,185,186,186,187,188,189,190,191,192,193,193,194,195,196,197,198,199,200,200,201,202,203,204,205,206,207,207,208,209,210,211,212,213,214,214,215,216,217,218,219,219,220,221,222,223,224,225,226,226,227,228,229,230,231,232,233,233,234,235,236,237,238,
};

void		mz_filterMoonlight(byte* yuv, int width, int height)
{
	register int i = width * height / 2;
	register int r = 0, g = 0, b = 0, gray = 0;
	register int y1 = 0, u1 = 0, v1 = 0, y2 = 0, u2 = 0, v2 = 0;
	register byte* dp = yuv;
	register int s = 52429;
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

		r = *(gMoonlightFilter_ColorRTable + r);
		g = *(gMoonlightFilter_ColorGTable + g);
		b = *(gMoonlightFilter_ColorBTable + b);

		gray = (r + g + b) / 3;
		r = Clamp(gray + ((s * (r - gray)) >> 16) + 10);
		g = Clamp(gray + ((s * (g - gray)) >> 16) + 20);
		b = Clamp(gray + ((s * (b - gray)) >> 16) + 30);

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

		r = *(gMoonlightFilter_ColorRTable + r);
		g = *(gMoonlightFilter_ColorGTable + g);
		b = *(gMoonlightFilter_ColorBTable + b);

		gray = (r + g + b) / 3;
		r = Clamp(gray + ((s * (r - gray)) >> 16) + 10);
		g = Clamp(gray + ((s * (g - gray)) >> 16) + 20);
		b = Clamp(gray + ((s * (b - gray)) >> 16) + 30);

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

void		mz_imageFilterMoonlight(byte* image, int width, int height, int depth) {
	register int i = width * height;
	register int r = 0, g = 0, b = 0, gray = 0;
	register byte* dp = image;
	register int s = 52429;
	while (i--) {
		r = *(gMoonlightFilter_ColorRTable + RGB_PR(dp));
		g = *(gMoonlightFilter_ColorGTable + RGB_PG(dp));
		b = *(gMoonlightFilter_ColorBTable + RGB_PB(dp));
		gray = (r + g + b) / 3;
		RGB_PR(dp) = Clamp(gray + ((s * (r - gray)) >> 16) + 10);
		RGB_PG(dp) = Clamp(gray + ((s * (g - gray)) >> 16) + 20);
		RGB_PB(dp) = Clamp(gray + ((s * (b - gray)) >> 16) + 30);
		dp += depth;
	}
}