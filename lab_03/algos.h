#ifndef ALGOS_H
#define ALGOS_H
#include "request.h"
#include <cmath>

#define eps 0.00000000001

typedef enum
{
    CDA = 0,
    BREZENHEM_DOUBLE,
    BREZENHEM_INT,
    BREZENHEM_WITHOUT_EDGES,
    BY,
    LIBRARY
} algo_type;

int draw_line_library(request_t request);
int draw_line_cda(request_t request, int stepmode, int draw_bool = 1);
int draw_line_brenzenhem_d(request_t request, int stepmode, int draw_bool = 1);
int draw_line_brenzenhem_i(request_t request, int stepmode, int draw_bool = 1);
int draw_line_brenzenhem_e(request_t request, int stepmode, int draw_bool = 1);
int draw_line_by(request_t request, int stepmode, int draw_bool = 1);

#endif // ALGOS_H
