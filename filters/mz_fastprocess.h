#ifndef ___FAST_PROCESS_H___
#define ___FAST_PROCESS_H___

#include "../core/mz_datatype.h"

extern const int		gYUV2RGB_YTable[];
extern const int		gYUV2R_VTable[];
extern const int		gYUV2G_VTable[];
extern const int		gYUV2G_UTable[];
extern const int		gYUV2B_UTable[];

extern const int		gRGB2Y_RTable[];
extern const int		gRGB2Y_GTable[];
extern const int		gRGB2Y_BTable[];

extern const int		gRGB2U_RTable[];
extern const int		gRGB2U_GTable[];
extern const int		gRGB2U_BTable[];

extern const int		gRGB2V_RTable[];
extern const int		gRGB2V_GTable[];
extern const int		gRGB2V_BTable[];

extern const byte		gCrossProcess_RColorTable[];
extern const byte		gCrossProcess_GColorTable[];
extern const byte		gCrossProcess_BColorTable[];

#define GRAY(r, g, b)					((19595 * r + 38470 * g + 7471 * b) >> 16)

inline int Clamp(int v)
{
	return v > 255 ? 255 : v < 0 ? 0 : v;
}

#endif