#include "Spline.h"

template <typename T> inline T & elem(T * arr, int index, int stride, int offset) {
    return arr[(index * stride) + offset];
}

template <typename T> inline void set_elem(T * arr, int index, T value, int stride, int offset) {
    arr[(index * stride) + offset] = value;
}

Spline::Spline(int dimensions) : dimensions(dimensions) {

}

cubic_t * Spline::compute_simple_spline(float * points, int point_count, int stride, int offset, cubic_t * out) {
    int n = point_count - 1;

    /* compute a 1 dimensional spline as described in */
    /* http://mathworld.wolfram.com/CubicSpline.html */

    /* build derivative array (right of augmented matrix) y-primes */
    float * primes = new float[n + 1];
    primes[0] = 3.0 * (elem(points, 1, stride, offset) - elem(points, 0, stride, offset));
    for (int i = 1; i < n; i++) {
        primes[i] = 3.0 * (elem(points, i + 1, stride, offset) - elem(points, i - 1, stride, offset));
    }
    primes[n] = 3.0 * (elem(points, n, stride, offset) - elem(points, n - 1, stride, offset));

    /* solve tridiagonal matrix with Thomas algorithm */
    /* https://www.cfd-online.com/Wiki/Tridiagonal_matrix_algorithm_-_TDMA_(Thomas_algorithm) */

    /* [ 2 1   ] */
    /* [ 1 4 1 ] */
    /* [   1 2 ] */

    /* elimination */
    float * diagonals = new float[n + 1];
    diagonals[0] = 2.0;
    for (int i = 1; i < n; i++) {
        float m = 1.0 / diagonals[i - 1];
        primes[i] = primes[i] - primes[i - 1] * m;
        diagonals[i] = 4.0 - m;
    }
    float m = 1.0 / diagonals[n - 1];
    primes[n] = primes[n] - primes[n - 1] * m;
    diagonals[n] = 2.0 - m;

    /* [ x 1   ] */
    /* [   x 1 ] */
    /* [     x ] */

    /* substitution */
    primes[n] = primes[n] / diagonals[n];
    for (int i = n - 1; i >= 0; i--) {
        primes[i] = (primes[i] - primes[i + 1]) / diagonals[i];
    }

    /* we now got the derivatives at the ith piecewise (primes), compute the cubic functions */
    for (int i = 0; i < n; i++) {
        cubic_t & cubic = elem(out, i, stride, offset);
        cubic.a = elem(points, i, stride, offset);
        cubic.b = primes[i];
        cubic.c = 3.0 * (elem(points, i + 1, stride, offset) - elem(points, i, stride, offset)) -
            2.0 * primes[i] - primes[i + 1];
        cubic.d = 2.0 * (elem(points, i, stride, offset) - elem(points, i + 1, stride, offset)) +
            primes[i] + primes[i + 1];
    }

    /* clean this shtuff up */
    delete [] primes;
    delete [] diagonals;

    return out;
}

void Spline::compute_spline(float * points, int point_count) {
    section_count = point_count - 1;
    /* allocate sections */
    if (sections != nullptr) delete [] sections;
    sections = new cubic_t[section_count * dimensions];
    /* compute spline for each dimension */
    for (int i = 0; i < dimensions; i++) {
        compute_simple_spline(points, point_count, dimensions, i, sections);
    }
}

float * Spline::get_point(float param, float * out) {
    if (sections != nullptr && param >= 0 && param <= section_count) {
        int section = (int) param;
        float val = param - section;
        for (int i = 0; i < dimensions; i++) {
            out[i] = sections[section * dimensions + i].f(val);
        }
        return out;
    } else return nullptr;
}

Spline::~Spline() {
    /* dispose of all these sections */
    if (sections != nullptr) delete [] sections;
}
