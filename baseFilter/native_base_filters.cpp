#include "baseFilters.h"
#include "../core/imageBase.h"
#include "../core/native.h"

#if defined(ANDROID)
	#define BASE_FILTER(name, ...)						Java_com_meizu_media_effects_filters_ ## baseFilter ## _  ## name(JNIEnv* env, jobject object, __VA_ARGS__)
#elif defined(WIN32)
	#define BASE_FILTER(cls, name, ...)					name(__VA_ARGS__)
#endif

NATIVE(void) BASE_FILTER(filtersForm, nativeSharpen, jobject bitmap, int width, int height, int radius, int th, int rate) {
    unsigned char* image = NULL;

    LOCK(bitmap, image);
    if(NULL != image) {
		Extrusion(image, width, height, width / 2, height / 2, width, th);
	//	sharpen(image, width, height, radius, th, rate);
    }
	UNLOCK(bitmap);
}

NATIVE(void) BASE_FILTER(filtersForm, brightness, jobject bitmap, int width, int height, int ibright) {
    int stride = STRIDE(width);
    unsigned char* image = NULL;

     if(width < 0 || height < 0 || 0 == ibright) {
         return;
     }

    LOCK(bitmap, image);
    if(NULL != image) {
        if (ibright > 0) {
            ibright = ibright * 150 / 100;
        }else {
            ibright = ibright * 180 / 100;
        }
        brightnessNew(image, width, height, DEPTH, stride, ibright);
    }
	UNLOCK(bitmap);
}

/*
NATIVE(void) FUNCTION(FiltersContrast, nativeProcess, jobject bitmap, int width, int height, int icontrast) {
    jint depth = 4;
    jint stride = depth * width;
    unsigned char* image = NULL;

     if(width < 0 || height < 0 || 0 == icontrast) {
         return;
     }

    AndroidBitmap_lockPixels(env, bitmap, (void**) &image);

    if(NULL != image) {
        if (icontrast > 0) {
            icontrast = icontrast * 2;
        }else{
            icontrast = icontrast * 80 / 100;
        }

        contrastNew(image, width, height, depth, stride, icontrast);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

NATIVE(void) FUNCTION(FiltersGray, nativeProcess, jobject bitmap, int width, int height) {
    jint depth = 4;
    jint stride = depth * width;
    unsigned char* image = NULL;

    if(width < 0 || height < 0) {
        return;
    }

    AndroidBitmap_lockPixels(env, bitmap, (void**) &image);

    if(NULL != image) {
        gray(image, width, height, depth, stride);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}


NATIVE(void) FUNCTION(FiltersToy, nativeProcess, jobject bitmap, int width, int height) {
    jint depth = 4;
    jint stride = depth * width;
    unsigned char* image = NULL;

    if(width < 0 || height < 0) {
        return;
    }

    AndroidBitmap_lockPixels(env, bitmap, (void**) &image);

    if(NULL != image) {
        toy(image, width, height, depth, stride);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

NATIVE(void) FUNCTION(FiltersGaussiir, nativeProcess, jobject bitmap, int width, int height, int radius) {
    jint depth = 4;
    jint stride = depth * width;
    float horz = radius * 1.0f;

    unsigned char* image = NULL;

    if( width < 0 || height < 0 || 0 == radius) {
        return;
    }

    AndroidBitmap_lockPixels(env, bitmap, (void**) &image);

    if(NULL != image) {
        gaussiir(image, width, height, depth, stride, horz, horz);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}


NATIVE(void) FUNCTION(FiltersFilm, nativeProcess, jobject bitmap, int width, int height) {
    jint depth = 4;
    jint stride = depth * width;
    unsigned char* image = NULL;

    if(width < 0 || height < 0) {
        return;
    }

    AndroidBitmap_lockPixels(env, bitmap, (void**) &image);

    if(NULL != image) {
        film(image, width, height, depth, stride);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

*/
