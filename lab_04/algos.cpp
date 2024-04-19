#include "algos.h"
#include <QDebug>

int draw_point(point_t point, QGraphicsScene *scene, QColor color)
{
    QPen pen;
    pen.setColor(color);
    scene->addLine(point.x, point.y, point.x, point.y, pen);
    return 0;
}

int draw_simetric_points(point_t center, point_t point, QGraphicsScene *scene, int circle)
{
    point_t tmp = { 0, 0, point.color_point };
    if (circle)
    {
        tmp.x = point.y - center.y + center.x;
        tmp.y = point.x - center.x + center.y;
        draw_point(tmp, scene, point.color_point);
        tmp.x = -point.y + center.y + center.x;
        tmp.y = point.x - center.x + center.y;
        draw_point(tmp, scene, point.color_point);
        tmp.x = point.y - center.y + center.x;
        tmp.y = -point.x + center.x + center.y;
        draw_point(tmp, scene, point.color_point);
        tmp.x = -point.y + center.y + center.x;
        tmp.y = -point.x + center.x + center.y;
        draw_point(tmp, scene, point.color_point);
    }
    tmp.x = point.x;
    tmp.y = point.y;
    draw_point(tmp, scene, point.color_point);
    tmp.x = -point.x + 2 * center.x;
    tmp.y = point.y;
    draw_point(tmp, scene, point.color_point);
    tmp.x = point.x;
    tmp.y = -point.y + 2 * center.y;
    draw_point(tmp, scene, point.color_point);
    tmp.x = -point.x + 2 * center.x;
    tmp.y = -point.y + 2 * center.y;
    draw_point(tmp, scene, point.color_point);
    return 0;
}

int draw_ellipse_library(request_t request)
{
    QPen pen;
    pen.setColor(request.color_line);
    request.scene->addEllipse(request.center.x - request.radius_x,\
                request.center.y - request.radius_y, request.radius_x * 2, request.radius_y * 2, pen);
    return 0;
}

int draw_circle_canonical(request_t request, int draw_bool)
{
    int radius_sqr = round(request.radius_x * request.radius_x);
    int border = round(request.center.x + request.radius_x/sqrt(2));
    int y;
    for (int x = round(request.center.x); x <= border; x++)
    {
        y = request.center.y + sqrt(radius_sqr - (x - request.center.x) * (x - request.center.x));
        if (draw_bool)
        {
            point_t point = { (double)x, (double)y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 1);
        }
    }
    return 0;
}

int draw_ellipse_canonical(request_t request, int draw_bool)
{
    double radius_x_sqr = round(request.radius_x * request.radius_x);
    double radius_y_sqr = round(request.radius_y * request.radius_y);
    int border_x = round(request.radius_x + request.radius_x/sqrt(1 + radius_y_sqr/radius_x_sqr));
    int border_y = round(request.radius_y + request.radius_y/sqrt(1 + radius_x_sqr/radius_y_sqr));

    for (int x = round(request.center.x); x <= border_x; x++)
    {
        double y = request.center.y + sqrt(radius_x_sqr * radius_y_sqr - (x - request.center.x) * (x - request.center.x) * radius_y_sqr) / request.radius_x;
        if (draw_bool)
        {
            point_t point = { (double)x, (double)y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 0);
        }
    }

    for (int y = round(request.center.y); y <= border_y; y++)
    {
        double x = request.center.x + sqrt(radius_x_sqr * radius_y_sqr - (y - request.center.y) * (y - request.center.y) * radius_x_sqr) / request.radius_y;
        if (draw_bool)
        {
            point_t point = { (double)x, (double)y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 0);
        }
    }
    return 0;
}
