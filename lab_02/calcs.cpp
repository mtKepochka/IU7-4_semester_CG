#include "calcs.h"

void move_graph(graph_t *graph, point_t move_offset)
{
    for (int i = 0; i < graph->points_count; i++)
    {
        graph->points[i].x += move_offset.x;
        graph->points[i].y += move_offset.y;
    }
}

void scale_graph(graph_t *graph, point_t scale, point_t scale_center)
{
    for (int i = 0; i < graph->points_count; i++)
    {
        graph->points[i].x = scale.x*graph->points[i].x + scale_center.x*(1 - scale.x);
        graph->points[i].y = scale.y*graph->points[i].y + scale_center.y*(1 - scale.y);
    }
}

void rotate_graph(graph_t *graph, double angle, point_t rotate_center)
{
    double newx, newy;
    for (int i = 0; i < graph->points_count; i++)
    {
        newx = rotate_center.x + (graph->points[i].x - rotate_center.x)*cos(angle) + (graph->points[i].y - rotate_center.y)*sin(angle);
        newy = rotate_center.y - (graph->points[i].x - rotate_center.x)*sin(angle) + (graph->points[i].y - rotate_center.y)*cos(angle);
        graph->points[i].x = newx;
        graph->points[i].y = newy;
    }
}

void move_figure(figure_t *figure, point_t move_offset)
{
    for (int i = 0; i < CHILDS_COUNT; i++)
        move_graph(&(figure->childs[i]), move_offset);
    figure->center.x += move_offset.x;
    figure->center.y += move_offset.y;
}

void scale_figure(figure_t *figure, point_t scale, point_t scale_center)
{
    for (int i = 0; i < CHILDS_COUNT; i++)
        scale_graph(&(figure->childs[i]), scale, scale_center);
    figure->center.x = scale.x*figure->center.x + scale_center.x*(1 - scale.x);
    figure->center.y = scale.y*figure->center.y + scale_center.y*(1 - scale.y);
}

void rotate_figure(figure_t *figure, double angle, point_t rotate_center)
{
    for (int i = 0; i < CHILDS_COUNT; i++)
        rotate_graph(&(figure->childs[i]), angle, rotate_center);
    double newx = rotate_center.x + (figure->center.x - rotate_center.x)*cos(angle) + (figure->center.y - rotate_center.y)*sin(angle);
    double newy = rotate_center.y - (figure->center.x - rotate_center.x)*sin(angle) + (figure->center.y - rotate_center.y)*cos(angle);
    figure->center.x = newx;
    figure->center.y = newy;
}
