#ifndef FIGURE_H
#define FIGURE_H

#include "graph.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <cmath>

#define CHILDS_COUNT 25

typedef struct
{
    point_t center;
    graph_t childs[CHILDS_COUNT];
} figure_t;

void draw_figure(figure_t *figure, QGraphicsScene *scene);
void read_figure(figure_t *figure);

#endif // FIGURE_H
