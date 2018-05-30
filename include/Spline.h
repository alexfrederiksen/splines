#ifndef SPLINE_H
#define SPLINE_H

#include "matrix/matrix.h"

struct vec2_t {
    float x;
    float y;
};

/* ax^3 + bx^2 + cx + d */
struct cubic_t {
    float a;
    float b;
    float c;
    float d;
};

class Spline {
    cubic_t * cubics;

    public:
        Spline();
        cubic_t * compute_spline(vec2_t * points, int point_size);
        vec2_t getPoint(float param);
        virtual ~Spline();
};

#endif // SPLINE_H
