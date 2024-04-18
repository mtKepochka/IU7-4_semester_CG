#ifndef GRAPH_H
#define GRAPH_H

#include "point.h"
#include "edge.h"

#define MAX_POINTS_COUNT 1300
#define MAX_EDGES_COUNT 1300

typedef struct
{
    int points_count;
    point_t points[MAX_POINTS_COUNT];
    int edges_count;
    edge_t edges[MAX_EDGES_COUNT];
} graph_t;

#endif // GRAPH_H
