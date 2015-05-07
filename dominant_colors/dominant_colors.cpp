#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>

#include "dominant_colors.h"
using namespace std;

#define COLOR_BLACK			0xff000000
#define COLOR_WHITE			0xffffffff

#define MAX(a, b, c)				(a > b ? (a > c ? a : c) : (b > c ? b : c))
#define MIN(a, b, c)				(a < b ? (a < c ? a : c) : (b < c ? b : c))

static void rgb2hsv(int r, int g, int b, int& h, int& s, int & v)
{
    float var_r = r / 255.0f;
    float var_g = g / 255.0f;
    float var_b = b / 255.0f;

    float var_max = MAX(var_r, var_g, var_b);
    float var_min = MIN(var_r, var_g, var_b);

    float delta_max = var_max - var_min;
    float var_h = 0, var_s = 0, var_v = var_max;

    if (delta_max == 0) {
        var_h = var_s = 0;
    } else {
        var_s = delta_max / var_max;
        if (var_max == r) {
            var_h = 60 * (var_g - var_b) / delta_max;
            if (var_g >= var_b) {
                var_h += 360;
            }
        } else if (var_max == var_g) {
            var_h = 60 * (var_b - var_r) / delta_max + 120;
        } else {
            var_h = 60 * (var_r - var_g) / delta_max + 240;
        }
    }
    h = (int)var_h;
    s = (int)(var_s * 100);
    v = (int)(var_v * 100);
}

static bool  is_dark_color(int color) {
    int r = color & 0x000000ff;
    int g = (color >> 8) & 0x000000ff;
    int b = (color >> 16) & 0x000000ff;
    int h = 0, s = 0, v = 0;
    rgb2hsv(r, g, b, h, s, v);
    if (v < 50) {
        return true;
    }
    return false;
}

int intcolor(int red, int green, int blue) {
	int color = (red << 16) | (green << 8) | blue;
	return color;
}

void getcolor(int color, unsigned char &red, unsigned char &green, unsigned char &blue) {
	red = (unsigned char)((color >> 16) & 0x000000ff);
	green = (unsigned char)((color >> 8) & 0x000000ff);
	blue = (unsigned char)(color & 0x000000ff);
}

void  color_analyze(unsigned char* rgba, int width, int height, int edge_size, int& bg_color, int& text_color) {
    int r = 0, g = 0, b = 0, color = 0, index = 0, max = 0, maxIndex = 0;
    int* countTable = NULL;
    unsigned char* image = rgba;
    map<int, int> colorMap;
    bg_color = COLOR_BLACK;
    if (rgba != NULL && width > 0 && height > 0) {
        countTable = (int*) malloc(width * height * sizeof(int));
        if (NULL != countTable) {
            memset(countTable, 0, width * height * sizeof(int));
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if ((i <= edge_size || i >= height - edge_size) || (j <= edge_size || j >= width - edge_size)) {
                        r = PR(image);
                        g = PG(image);
                        b = PB(image);
                        color = intcolor(r, g, b);
                        if (colorMap.find(color) == colorMap.end()) {
                            colorMap.insert(map<int, int>::value_type(color, index));
                            countTable[index]++;
                        } else {
                            countTable[colorMap[color]]++;
                        }
                        index++;
                    }
                    image += 4;
                }
            }
            max = countTable[0];
            maxIndex = 0;
            for (int i = 1; i < index; i++) {
                if (max <= countTable[i]) {
                    max = countTable[i];
                    maxIndex = i;
                }
            }
            color = 0;
            for (map<int, int>::iterator it = colorMap.begin(); it != colorMap.end(); it++) {
                if (maxIndex == it->second) {
                    color = it->first;
                    break;
                }
            }
            bg_color = color | 0xff000000;
        }
    }
    if (NULL != countTable) {
        free(countTable);
        countTable = NULL;
    }
    colorMap.clear();
    if (is_dark_color(bg_color)) {
        text_color = COLOR_WHITE;
    } else {
        text_color = COLOR_BLACK;
    }
}


