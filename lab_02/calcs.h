#ifndef CALCS_H
#define CALCS_H

#include "figure.h"
#include <cmath>

void rotate_figure(figure_t *figure, double angle, point_t rotate_center);
void scale_figure(figure_t *figure, point_t scale, point_t scale_center);
void move_figure(figure_t *figure, point_t move_offset);

#endif // CALCS_H
