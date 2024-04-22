#include "algos.h"
#include <QDebug>
#define ITER_COUNT_TIME 1000

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
    if (request.radius_x == 0)
        return 0;
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
    int border_x = round(request.center.x + request.radius_x/sqrt(1 + radius_y_sqr/radius_x_sqr));
    int border_y = round(request.center.y + request.radius_y/sqrt(1 + radius_x_sqr/radius_y_sqr));
    for (int x = round(request.center.x); x <= border_x; x++)
    {
        // double y = request.center.y + sqrt(radius_x_sqr * radius_y_sqr - \
                 (x - request.center.x) * (x - request.center.x) * radius_y_sqr) / request.radius_x;
        double y = request.center.y + request.radius_y * sqrt(1 - ((double)x - request.center.x)*(x - request.center.x)/request.radius_x/request.radius_x);
        if (draw_bool)
        {
            point_t point = { (double)x, (double)y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 0);
        }
    }

    for (int y = round(request.center.y); y <= border_y; y++)
    {
        //double x = request.center.x + sqrt(radius_x_sqr * radius_y_sqr - (y - request.center.y) * (y - request.center.y) * radius_x_sqr) / request.radius_y;
        double x = request.center.x + request.radius_x * sqrt(1 - ((double)y - request.center.y)*(y - request.center.y)/request.radius_y/request.radius_y);
        if (draw_bool)
        {
            point_t point = { (double)x, (double)y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 0);
        }
    }
    return 0;
}

int parametric_circle(request_t request, int draw_bool)
{
    if (request.radius_x == 0)
        return 0;
    double step = 1.0 / request.radius_x;

    double i = 0;
    while (i <= M_PI / 4 + step)
    {
        double x = request.center.x + request.radius_x * cos(i);
        double y = request.center.y + request.radius_x * sin(i);
        if (draw_bool)
        {
            point_t point = { (double)round(x), (double)round(y), request.color_line };
            draw_simetric_points(request.center, point, request.scene, 1);
        }

        i += step;
    }

    return 0;
}

int parametric_ellipse(request_t request, int draw_bool)
{
    double step;
    if (request.radius_x > request.radius_y)
        step = 1.0 / request.radius_x;
    else
        step = 1.0 / request.radius_y;

    double i = 0;
    while (i <= M_PI / 2 + step)
    {
        double x = request.center.x + request.radius_x * cos(i);
        double y = request.center.y + request.radius_y * sin(i);
        if (draw_bool)
        {
            point_t point = { (double)round(x), (double)round(y), request.color_line };
            draw_simetric_points(request.center, point, request.scene, 0);
        }

        i += step;
    }

    return 0;
}

int middle_point_circle(request_t request, int draw_bool)
{
    if (request.radius_x == 0)
        return 0;
    int x = request.radius_x;
    int y = 0;

    if (draw_bool)
    {
        point_t point = { x + request.center.x, y + request.center.y, request.color_line };
        draw_simetric_points(request.center, point, request.scene, 1);
    }

    int delta = 1 - request.radius_x;

    while (x >= y)
    {
        if (delta >= 0)
        {
            x -= 1;
            y += 1;
            delta += 2 * y + 1 - 2 * x;
        }
        else
        {
            y += 1;
            delta += 2 * y + 1;
        }
        if (draw_bool)
        {
            point_t point = { x + request.center.x, y + request.center.y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 1);
        }
    }

    return 0;
}

int middle_point_ellipse(request_t request, int draw_bool)
{
    int sqr_ra = request.radius_x * request.radius_x;
    int sqr_rb = request.radius_y * request.radius_y;

    int x = 0;
    int y = request.radius_y;

    if (draw_bool)
    {
        point_t point = { x + request.center.x, y + request.center.y, request.color_line };
        draw_simetric_points(request.center, point, request.scene, 0);
    }

    int border = round(request.radius_x / sqrt(1.0 + sqr_rb * 1.0 / sqr_ra));
    int delta = sqr_rb - (int)round(sqr_ra * (request.radius_y - 1.0 / 4));

    while (x <= border)
    {
        if (delta < 0)
        {
            x += 1;
            delta += 2 * sqr_rb * x + request.radius_x;
        }
        else
        {
            x += 1;
            y -= 1;
            delta += 2 * sqr_rb * x - 2 * sqr_ra * y + request.radius_y;
        }
        if (draw_bool)
        {
            point_t point = { x + request.center.x, y + request.center.y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 0);
        }
    }

    x = request.radius_x;
    y = 0;

    if (draw_bool)
    {
        point_t point = { x + request.center.x, y + request.center.y, request.color_line };
        draw_simetric_points(request.center, point, request.scene, 0);
    }

    border = round(request.radius_y / sqrt(1 + sqr_ra * 1.0 / sqr_rb));
    delta = sqr_ra - (int)round(sqr_rb * (request.radius_x - 1.0 / 4));

    while (y <= border)
    {
        if (delta < 0)
        {
            y += 1;
            delta += 2 * sqr_ra * y + request.radius_x;
        }
        else
        {
            x -= 1;
            y += 1;
            delta += 2 * sqr_ra * y - 2 * sqr_rb * x + request.radius_y;
        }
        if (draw_bool)
        {
            point_t point = { x + request.center.x, y + request.center.y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 0);
        }
    }

    return 0;
}

int bresenham_circle(request_t request, int draw_bool)
{
    if (request.radius_x == 0)
        return 0;
    int x = 0;
    int y = request.radius_x;

    if (draw_bool)
    {
        point_t point = { x + request.center.x, y + request.center.y, request.color_line };
        draw_simetric_points(request.center, point, request.scene, 0);
    }

    int delta = 2 * (1 - request.radius_x);

    while (y >= x)
    {
        int d = 2 * (delta + y) - 1;
        x += 1;

        if (d >= 0)
        {
            y -= 1;
            delta += 2 * (x - y + 1);
        }
        else
            delta += x + x + 1;

        if (draw_bool)
        {
            point_t point = { x + request.center.x, y + request.center.y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 0);
        }
    }

    return 0;
}

int bresenham_ellipse(request_t request, int draw_bool)
{
    int x = 0;
    int y = request.radius_y;

    if (draw_bool)
    {
        point_t point = { x + request.center.x, y + request.center.y, request.color_line };
        draw_simetric_points(request.center, point, request.scene, 0);
    }

    int sqr_ra = request.radius_x * request.radius_x;
    int sqr_rb = request.radius_y * request.radius_y;
    int delta = sqr_rb - sqr_ra * (2 * request.radius_y + 1);

    while (y >= 0)
    {
        if (draw_bool)
        {
            point_t point = { x + request.center.x, y + request.center.y, request.color_line };
            draw_simetric_points(request.center, point, request.scene, 0);
        }
        if (delta < 0)
        {
            int d1 = 2 * delta + sqr_ra * (2 * y + 2);

            x += 1;
            if (d1 < 0)
                delta += sqr_rb * (2 * x + 1);
            else
            {
                y -= 1;
                delta += sqr_rb * (2 * x + 1) + sqr_ra * (1 - 2 * y);
            }

        }
        else if (delta > 0)
        {
            int d2 = 2 * delta + sqr_rb * (2 - 2 * x);

            y -= 1;
            if (d2 > 0)
                delta += sqr_ra * (1 - 2 * y);
            else
            {
                x += 1;
                delta += sqr_rb * (2 * x + 1) + sqr_ra * (1 - 2 * y);
            }
        }
        else
        {
            y -= 1;
            x += 1;
            delta += sqr_rb * (2 * x + 1) + sqr_ra * (1 - 2 * y);
        }

        // pixels.push_back(pixel_create(x + center.x, y + center.y));
    }

    return 0;
}

static long delta_time(struct timespec mt1, struct timespec mt2)
{
    return 1000000000 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);
}

long time_measurement_circle(request_t req, int (*alg)(request_t request, int draw_bool))
{
    long time1;
    long sum1 = 0;
    struct timespec tbegin, tend;

    for (size_t i = 0; i < ITER_COUNT_TIME; i++)
    {
        clock_gettime(CLOCK_REALTIME, &tbegin);
        alg(req, 0);
        clock_gettime(CLOCK_REALTIME, &tend);
        sum1 += delta_time(tbegin, tend);
    }
    time1 = sum1 / ITER_COUNT_TIME;

    return time1;
}

long time_measurement_ellipse(request_t req, int (*alg)(request_t request, int draw_bool))
{
    long time1;
    long sum1 = 0;
    struct timespec tbegin, tend;

    for (size_t i = 0; i < ITER_COUNT_TIME; i++)
    {
        clock_gettime(CLOCK_REALTIME, &tbegin);
        alg(req, 0);
        clock_gettime(CLOCK_REALTIME, &tend);
        sum1 += delta_time(tbegin, tend);
    }
    time1 = sum1 / ITER_COUNT_TIME;

    return time1;
}
