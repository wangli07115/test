#include "filters_version_1p.h"
#include "../baseFilter/baseFilters.h"
#include "../raws/color_table.h"
#include "../raws/curve_table.h"

void newFilters(unsigned char *image, int width, int height, int depth, int stride, int type) {
    const unsigned char* mFxTab = NULL;
    unsigned char *mCurveTab = NULL;

    if (NULL == image || width < 0 || height < 0 || EFFECT_TYPE_NONE > type || type > EFFECT_TYPE_MAX) {
        return;
    }

    switch (type) {
    case EFFECT_TYPE_MAROON:
        mFxTab = gVintage_ColorTable;
        fx(image, mFxTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_BW:
        gray(image, width, height, depth, stride) ;
        contrast(image, width, height, depth, stride,  80) ;
        break;
    case EFFECT_TYPE_FILM:
        mFxTab = gInstant_ColorTable;
        fx(image, mFxTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_OXIDIZE:
        mFxTab = gBleach_ColorTable;
        fx(image, mFxTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_LEAF:
        mFxTab = gXProcess_ColorTable;
        mCurveTab = curveRGB5;
        fx(image, mFxTab, width, height, depth, stride);
        direct_table(image, mCurveTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_VIOLET:
        mFxTab = gBlueCrush_ColorTable;
        fx(image, mFxTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_LIGHT_YELLOW:
        mFxTab = gInstant_ColorTable;
        mCurveTab = curveRGB7;
        fx(image, mFxTab, width, height, depth, stride);
        shadow(image, width, height, depth, stride, 100);
        direct_table(image, mCurveTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_AZURE:
        mFxTab = gXProcess_ColorTable;
        mCurveTab = curveRGB8;
        fx(image, mFxTab, width, height, depth, stride);
        direct_table(image, mCurveTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_VIRIDITY:
        mFxTab = gInstant_ColorTable;
        mCurveTab = curveRGB9;
        fx(image, mFxTab, width, height, depth, stride);
        direct_table(image, mCurveTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_WOOD:
        mFxTab = gInstant_ColorTable;
        mCurveTab = curveRGB10;
        fx(image, mFxTab, width, height, depth, stride);
        direct_table(image, mCurveTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_PALE_GREEN:
        mCurveTab = curveRGB11;
        direct_table(image, mCurveTab, width, height, depth, stride);
        shadow(image, width, height, depth, stride, -86);
        break;
    case EFFECT_TYPE_FADING:
        vibrance(image, width, height, depth, stride, -75);
        mCurveTab = curveRGB13;
        direct_table(image, mCurveTab, width, height, depth, stride);
        contrast(image, width, height, depth, stride,  38) ;
        break;
    case EFFECT_TYPE_CORNFIED:
        mFxTab = gInstant_ColorTable;
        mCurveTab = curveRGB13;
        fx(image, mFxTab, width, height, depth, stride);
        direct_table(image, mCurveTab, width, height, depth, stride);
        shadow(image, width, height, depth, stride, 100);
        break;
    case EFFECT_TYPE_OLIVE_YELLOW:
        mFxTab = gBlueCrush_ColorTable;
        mCurveTab = curveRGB14;
        fx(image, mFxTab, width, height, depth, stride);
        direct_table(image, mCurveTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_TOY:
        toy(image, width, height, depth, stride);
        break;
    case EFFECT_TYPE_PALE_BLUE:
        mCurveTab = curveRGB16;
        direct_table(image, mCurveTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_CLAY_BANK:
        mFxTab = gWashout_ColorTable;
        mCurveTab = curveRGB17;
        fx(image, mFxTab, width, height, depth, stride);
        direct_table(image, mCurveTab, width, height, depth, stride);
        break;
    case EFFECT_TYPE_SUNNY:
        mFxTab = gInstant_ColorTable;
        mCurveTab = curveRGB18;
        fx(image, mFxTab, width, height, depth, stride);
        direct_table(image, mCurveTab, width, height, depth, stride);
        break;
    default:
        type = EFFECT_TYPE_NONE;
        break;
    }
}

void LoadTableRGB(unsigned char *table, int stepr, int stepg, int stepb, int type) {
    int i, j, k;
    int mHeight = stepr;
    int mWidth = stepg * stepb;
    int len = mWidth * mHeight* 3;
    float dr = 255.0f / (stepr - 1);
    float dg = 255.0f / (stepg - 1);
    float db = 255.0f / (stepb - 1);
    unsigned char *pbuf = NULL;

    pbuf = table;

    for(i = 0; i < stepr; i++) {//r
        for(j = 0; j < stepg; j++) {//g
            for(k = 0; k < stepb; k++) {//b
                pbuf[0] = CLAMP(i * dr);
                pbuf[1] = CLAMP(j * dg);
                pbuf[2] = CLAMP(k * db);
                pbuf += 3;
            }
        }
    }

    newFilters(table, mWidth * mHeight, 1, 3, len, type);
}
