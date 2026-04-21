#include "math_utils.h"

#include <math.h>

float clampf(float v, float lo, float hi) {
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

float length2f(float x, float y) {
    return sqrtf(x * x + y * y);
}

void clamp_speed(float *vx, float *vy, float max_speed) {
    float speed = length2f(*vx, *vy);
    if (speed > max_speed && speed > 0.0f) {
        float s = max_speed / speed;
        *vx *= s;
        *vy *= s;
    }
}