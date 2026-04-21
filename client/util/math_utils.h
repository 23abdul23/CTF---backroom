#ifndef MATH_UTILS_H
#define MATH_UTILS_H

float clampf(float v, float lo, float hi);
float length2f(float x, float y);
void clamp_speed(float *vx, float *vy, float max_speed);

#endif