#include "engine.h"

void vec2Rotate(double * x, double * y, double angle) {
    //angle = radToDeg(angle);
    double original_x = *x;
    double original_y = *y;
    *x = cos(angle)*(original_x) - sin(angle)*(original_y);
    *y = sin(angle)*(original_x) + cos(angle)*(original_y);
}

double vec2SqrDist(double x1, double y1, double x2, double y2) {
    double x = x2 - x1;
    double y = y2 - y1;
    return x*x + y*y;
}

double vec2Dist(double x1, double y1, double x2, double y2) {
    double x = x2 - x1;
    double y = y2 - y1;
    return sqrt(x*x + y*y);
}

double radToDeg(double rad) {
    return (180.0/M_PI)*rad;
}

int clamp(int val, int min, int max) {
    return val < min ? min : (val > max ? max : val);
}
