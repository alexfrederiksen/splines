#ifndef SPLINE_H
#define SPLINE_H

/* a + bx + cx^2 + dx^3 */
struct cubic_t {
    float a;
    float b;
    float c;
    float d;

    float f(float t) {
        return a +
            b * t +
            c * t * t +
            d * t * t * t;
    }
};

class Spline {
    int dimensions;
    int section_count = 0;
    cubic_t * sections = nullptr;

    public:
        Spline(int dimensions);
        static cubic_t * compute_simple_spline(float * points, int point_count, int stride, int offset, cubic_t * out);
        void compute_spline(float * points, int point_count);

        int get_dimensions() { return dimensions; }
        int get_section_count() { return section_count; }
        cubic_t * get_sections() { return sections; }

        float * get_point(float param, float * out);
        virtual ~Spline();
};

#endif // SPLINE_H
