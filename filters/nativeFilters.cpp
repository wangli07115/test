#include "../native.h"
#include "filters.h"

#if defined(ANDROID)
	#define NATIVE(type)							extern "C" type
	#define FUNCTION(cls, name, ...)				Java_com_meizu_media_effects_filters_ ## cls ## _  ## name(JNIEnv* env, jobject object, __VA_ARGS__)
#elif defined(WIN32)
	#define NATIVE(type)							extern "C" __declspec(dllexport) type
	#define FUNCTION(cls, name, ...)				name(__VA_ARGS__)
#endif

NATIVE(void) FUNCTION(Filters, nativeGaussiir, jobject bitmap, int width, int height, int edgeSize)
{
	jint depth = 4;
	jint stride = depth * width;
	jint horz = edgeSize;
	jint vert = 0;
	int i = 0;

	unsigned char* rgba = NULL, *pBuf = NULL;
	AndroidBitmap_lockPixels(env, bitmap, (void**) &rgba);
	float radius = horz * 1.0f;

	if(NULL == rgba || width < 0 || height < 0) {
		return;
	}

	LOG("nativeFilters width = %d, height = %d", width, height);

	filters_gaussiir(rgba, width, height, depth, stride, radius, radius);

	LOG("nativeFilters horz = %d", horz);

	AndroidBitmap_unlockPixels(env, bitmap);
}

NATIVE(void) FUNCTION(Filters, nativeBrightness, jobject bitmap, int width, int height, int radius)
{
	jint depth = 4;
	jint stride = depth * width;
	int i = 0;

	unsigned char* rgba = NULL, *pBuf = NULL;
	AndroidBitmap_lockPixels(env, bitmap, (void**) &rgba);

	if(NULL == rgba || width < 0 || height < 0) {
		return;
	}

	//filters_brightness(rgba, width, height, depth, stride, radius);
	filters_contrast(rgba, width, height, depth, stride, radius);
	//filters_gaussiir(rgba, width, height, depth, stride, radius, radius);

	AndroidBitmap_unlockPixels(env, bitmap);
}

NATIVE(void) FUNCTION(Filters, nativeFilm, jobject bitmap, int width, int height) {
	unsigned char* rgba = NULL;
	AndroidBitmap_lockPixels(env, bitmap, (void**) &rgba);

	if(NULL == rgba || width < 0 || height < 0) {
		return;
	}

	filters_film(rgba, width, height);

	AndroidBitmap_unlockPixels(env, bitmap);
}

NATIVE(void) FUNCTION(Filters, nativeToy, jobject bitmap, int width, int height) {
	unsigned char* rgba = NULL;
	AndroidBitmap_lockPixels(env, bitmap, (void**) &rgba);

	if(NULL == rgba || width < 0 || height < 0) {
		return;
	}

	filters_toy(rgba, width, height);

	AndroidBitmap_unlockPixels(env, bitmap);
}

NATIVE(void) FUNCTION(Filters, nativeGray, jobject bitmap, int width, int height) {
	unsigned char* rgba = NULL;
	AndroidBitmap_lockPixels(env, bitmap, (void**) &rgba);

	if(NULL == rgba || width < 0 || height < 0) {
		return;
	}

	filters_gray(rgba, width, height, 4, width * 4);

	AndroidBitmap_unlockPixels(env, bitmap);
}

