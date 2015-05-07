#include "baseFilters.h"

#define POINTS_NUM         10

void solveSystem(double *solution, float *points, int n) {
    double system[POINTS_NUM][3] = {0.0};
    double result[POINTS_NUM] = {0.0}; 
    system[0][1] = 1;
    system[n - 1][1] = 1;
    double d6 = 1.0 / 6.0;
    double d3 = 1.0 / 3.0;

    for (int i = 1; i < n - 1; i++) {
        double deltaPrevX = points[i * 2] - points[(i - 1) * 2];
        double deltaX = points[(i + 1) * 2] - points[(i - 1) * 2];
        double deltaNextX = points[(i + 1) * 2] - points[i * 2];
        double deltaNextY = points[(i + 1) * 2 + 1] - points[i * 2 + 1];
        double deltaPrevY = points[i * 2 + 1] - points[(i - 1) * 2 + 1];
        system[i][0] = d6 * deltaPrevX; // a_i
        system[i][1] = d3 * deltaX; // b_i
        system[i][2] = d6 * deltaNextX; // c_i
        result[i] = (deltaNextY / deltaNextX) - (deltaPrevY / deltaPrevX); // d_i
    }

    for (int i = 1; i < n; i++) {
        // m = a_i/b_i-1
        double m = system[i][0] / system[i - 1][1];
        // b_i = b_i - m(c_i-1)
        system[i][1] = system[i][1] - m * system[i - 1][2];
        // d_i = d_i - m(d_i-1)
        result[i] = result[i] - m * result[i - 1];
    }

    // Back substitution
    solution[n - 1] = result[n - 1] / system[n - 1][1];
    for (int i = n - 2; i >= 0; --i) {
        solution[i] = (result[i] - system[i][2] * solution[i + 1]) / system[i][1];
    }
}

//curve[256]
void curve(unsigned char *curve, float* points, int num) {
    int i, j;
    int value = 0;
    double solution[POINTS_NUM] = {0.0};
    if(num <= 2 || POINTS_NUM < num) {
        for(i = 0; i < 256; i++) {
            curve[i] = i;
        }
        return;
    }

    solveSystem(solution, points, num);

    for (i = 0; i < 256; i++) {
        double x = i / 256.0;
        int pivot = 0;
        for (j = 0; j < num - 1; j++) {
            if (x >= points[j * 2] && x <= points[(j + 1) * 2]) {
                pivot = j;
            }
        }
        if (x <= points[(pivot + 1) * 2]) {
            double x1 = points[pivot * 2];
            double x2 = points[(pivot + 1) * 2];
            double y1 = points[pivot * 2 + 1];
            double y2 = points[(pivot + 1) * 2 + 1];

            double delta = (x2 - x1);
            double delta2 = delta * delta;
            double b = (x - x1) / delta;
            double a = 1 - b;
            double ta = a * y1;
            double tb = b * y2;
            double tc = (a * a * a - a) * solution[pivot];
            double td = (b * b * b - b) * solution[pivot + 1];
            double y = ta + tb + (delta2 / 6) * (tc + td);

            value = (int)((1.0 - y) * 255);
            curve[i] = CLAMP(value);
        } else {
            curve[i] = (unsigned char )((1.0f - points[(pivot + 1) * 2 + 1]) * 255);
        }
    }
}

//curveTab[256 * 3], pointsRGB[num, x,y,x,y....];
void curveRGB(unsigned char *curveTable, float* pointsRGB, float* pointsR, float* pointsG, float* pointsB) {
    int i = 0;
    int num = 0;
    unsigned char table[256];
    float* temp;
    float* points;

    temp = pointsRGB;
    num = (int)(*temp);
    points = temp + 1;
    curve(table, points, num);
    for(i = 0; i < 256; i++) {
       curveTable[3 * i] = table[i];
       curveTable[3 * i + 1] = table[i];
       curveTable[3 * i + 2] = table[i];
    }
    temp = pointsR;
    num = (int)(*temp);
    points = temp + 1;
    curve(table, points, num);
    for(i = 0; i < 256; i++) {
        curveTable[3 * i] = table[curveTable[3 * i]];
    }
    temp = pointsG;
    num = (int)(*temp);
    points = temp + 1;
    curve(table, points, num);
    for(i = 0; i < 256; i++) {
        curveTable[3 * i + 1] = table[curveTable[3 * i + 1]];
    }
    temp = pointsB;
    num = (int)(*temp);
    points = temp + 1;
    curve(table, points, num);
    for(i = 0; i < 256; i++) {
        curveTable[3 * i + 2] = table[curveTable[3 * i + 2]];
    }
}
