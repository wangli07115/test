#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#include "dominant_colors.h"

#define NATIVE(type)									extern "C" type
#define FUNCTION(cls, name, ...)				Java_com_meizu_media_effects_dominantcolors_ ## cls ## _  ## name(JNIEnv* env, jobject object, __VA_ARGS__)

extern "C" void   setIntArrayValue(JNIEnv* env, jintArray array, int index, int value) {
	jsize arrayLen = 0;
	jint* arrayBuf = NULL;
	if (NULL != env) {
		arrayLen = env->GetArrayLength(array);
		if (arrayLen > 0 && index >= 0 && index < arrayLen) {
			arrayBuf = env->GetIntArrayElements(array, 0);
			if (NULL != arrayBuf) {
				arrayBuf[index] = value;
				env->SetIntArrayRegion(array, 0, arrayLen, arrayBuf);
			}
		}
	}
}

NATIVE(void)  FUNCTION(DominantColors, nativeProcess, jobject bitmap, jint width, jint height,
		jint edgeSize, jintArray bgColorArray, jintArray textColorArray) {
	int bgColor, textColor;
	unsigned char* rgba = NULL;
	int bpp = 4;
	int stride = bpp * width;
	unsigned char red, green, blue;



	AndroidBitmap_lockPixels(env, bitmap, (void**) &rgba);

	if (NULL != rgba && width > 0 && height > 0) {
		color_analyze(rgba, width, height, edgeSize, bgColor, textColor);
	//	color_extraction(rgba, width, height, bpp, stride, bgColor, textColor);
		setIntArrayValue(env, bgColorArray, 0, bgColor);
		setIntArrayValue(env, textColorArray, 0, textColor);

		getcolor(bgColor, red, green, blue);
		LOG("nativeProcess :r = %d, g = %d, b= %d", (int)red, (int)green, (int)blue);
	}

	AndroidBitmap_unlockPixels(env, bitmap);
}


NATIVE(void)  FUNCTION(DominantColors, nativeFillcolor, jobject bitmap, jint width, jint height, jint color) {
	unsigned char* rgba = NULL;

	AndroidBitmap_lockPixels(env, bitmap, (void**) &rgba);

	color = (255 << 16) | (0 << 8) | 0 | 0xff000000;

	if (NULL != rgba && width > 0 && height > 0) {
		fillcolor(rgba, width, height, color);
	}
	LOG("nativeFillcolor :end");
	AndroidBitmap_unlockPixels(env, bitmap);
}


NATIVE(void)  FUNCTION(DominantColors, nativeTopEdge, jobject bitmap, jint width, jint height, jint color) {
	unsigned char* rgba = NULL;
	int depth = 4;
	int stride = depth * width;

	AndroidBitmap_lockPixels(env, bitmap, (void**) &rgba);

	if (NULL != rgba && width > 0 && height > 0) {
		topEdge(rgba, width, height, depth, stride, color);
	}

	AndroidBitmap_unlockPixels(env, bitmap);
}

