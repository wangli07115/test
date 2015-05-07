LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libfilters
LOCAL_SRC_FILES :=  \
							nativeFilters.cpp \
							filters_gaussiir.cpp \
							filters_film.cpp \
							filters_gray.cpp \
							filters_toy.cpp \
							filters_brightness.cpp \
							filters_contrast.cpp
							
LOCAL_LDLIBS    := -lm -llog -ljnigraphics
include $(BUILD_SHARED_LIBRARY)