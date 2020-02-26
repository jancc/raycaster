#ifndef MATH_H
#define MATH_H

#ifndef M_PI
#define M_PI 3.14159265
#endif

void vec2Rotate(double* x, double* y, double angle);
double vec2Magnitude(double x, double y);
double vec2SqrMagnitude(double x, double y);
double vec2SqrDist(double x1, double y1, double x2, double y2);
double vec2Dist(double x1, double y1, double x2, double y2);
void vec2Normalize(double* x, double* y);
double radToDeg(double rad);
int clamp(int val, int min, int max);

#endif
