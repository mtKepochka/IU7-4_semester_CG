#ifndef ALGOS_H
#define ALGOS_H
#include "request.h"
#include <cmath>

#define eps 0.00000000001

typedef enum
{
    CANONICAL = 0,
    PARAMETR,
    MIDDLE_POINT,
    BREZENHEM,
    LIBRARY
} algo_type;

int draw_ellipse_library(request_t request);
int draw_ellipse_canonical(request_t request, int draw_bool = 1);
int draw_circle_canonical(request_t request, int draw_bool = 1);
int parametric_ellipse(request_t request, int draw_bool = 1);
int parametric_circle(request_t request, int draw_bool = 1);
int middle_point_ellipse(request_t request, int draw_bool = 1);
int middle_point_circle(request_t request, int draw_bool = 1);
int bresenham_ellipse(request_t request, int draw_bool = 1);
int bresenham_circle(request_t request, int draw_bool = 1);
long time_measurement_ellipse(request_t req, int (*alg)(request_t request, int draw_bool));
long time_measurement_circle(request_t req, int (*alg)(request_t request, int draw_bool));

#endif // ALGOS_H
