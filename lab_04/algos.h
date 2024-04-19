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

#endif // ALGOS_H
