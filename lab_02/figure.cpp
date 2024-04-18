#include "figure.h"

void draw_lines_on_scene(graph_t *graph, QGraphicsScene *scene)
{
    for (int i = 0; i < graph->edges_count; i++)
        scene->addLine(graph->points[graph->edges[i].p1].x, graph->points[graph->edges[i].p1].y,\
                       graph->points[graph->edges[i].p2].x, graph->points[graph->edges[i].p2].y);
}

void draw_figure(figure_t *figure, QGraphicsScene *scene)
{
    scene->clear();
    for (int i = 0; i < CHILDS_COUNT; i++)
    {
        draw_lines_on_scene(&(figure->childs[i]), scene);
    }
    QGraphicsTextItem *text = scene->addText(QString("(%1,%2)").\
                         arg(QString::number(round(figure->center.x)),\
                             QString::number(round(figure->center.y))));
    text->setPos(figure->center.x, figure->center.y);
    scene->addRect(figure->center.x, figure->center.y, 1, 1);
}

int get_graph_from_file(FILE *f, graph_t *graph)
{
    fscanf(f, "%d\n", &(graph->points_count));
    for (int i = 0; i < graph->points_count; i++)
        fscanf(f, "%lf %lf\n", &(graph->points[i].x), &(graph->points[i].y));

    fscanf(f, "%d\n", &(graph->edges_count));
    for (int i = 0; i < graph->edges_count; i++)
        fscanf(f, "%d %d\n", &(graph->edges[i].p1), &(graph->edges[i].p2));
    return ferror(f);
}

void read_figure(figure_t *figure)
{
    FILE *f;
    f = fopen("/home/kepochka/Programming/QTprojects/lab_2/lab_2/figure.txt", "r");
    for (int i = 0; i < CHILDS_COUNT; i++)
        get_graph_from_file(f, &(figure->childs[i]));
    figure->center.x = 0;
    figure->center.y = 25;
    fclose(f);
}


