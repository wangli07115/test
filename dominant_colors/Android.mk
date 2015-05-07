LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libdominant_colors
LOCAL_SRC_FILES :=  \
				colorExtraction.cpp \
				dominant_colors.cpp \
				nativeDominant_colors.cpp \
				supDominant_colors.cpp 
LOCAL_LDLIBS    := -lm -llog -ljnigraphics
include $(BUILD_SHARED_LIBRARY)