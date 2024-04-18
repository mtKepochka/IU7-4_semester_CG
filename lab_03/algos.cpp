#include "algos.h"
#include <QDebug>

int draw_point(point_t point, QGraphicsScene *scene, QColor color)
{
    QPen pen;
    pen.setColor(color);
    scene->addLine(point.x, point.y, point.x, point.y, pen);
    return 0;
}

int draw_line_library(request_t request)
{
    QPen pen;
    pen.setColor(request.color_line);
    request.scene->addLine(request.start.x, request.start.y, request.end.x, request.end.y, pen);
    return 0;
}

int draw_points(point_t points[3000], int size, QGraphicsScene *scene, QColor color)
{
    for (int i = 0; i < size; i++)
        draw_point(points[i], scene, points[i].color_point);
    return 0;
}

int draw_line_cda(request_t request, int stepmode, int draw_bool)
{
    point_t points[3000];
    int size = 0;
    int steps = 1;
    point_t start = request.start;
    point_t end = request.end;
    double length = 0;
    // start.x = round(start.x);
    // start.y = round(start.y);
    // end.x = round(end.x);
    // end.y = round(end.y);
    if ((fabs(start.x - end.x) < eps) && (fabs(start.y - end.y) < eps))
    {
        points[size++] = start;
    }
    else
    {
        double dx = fabs(round(end.x) - round(start.x)) * 1.0;
        double dy = fabs(round(end.y) - round(start.y)) * 1.0;
        if (dx >= dy)
            length = dx;
        else
            length = dy;
        dx = (round(end.x) - round(start.x))/length * 1.0;
        dy = (round(end.y) - round(start.y))/length * 1.0;

        double x, y;
        x = start.x;
        y = start.y;

        for (int i = 0; i <= (int)length; i++)
        {
            if (!stepmode)
            {
                points[size++] = { round(x), round(y), request.color_line };
            }
            else if ((round(x + dx) != round(x)) && (round(y + dy) != round(y)) && (i + 1 <= round(length)))
            {
                steps++;
            }
            x += dx;
            y += dy;
        }
    }

    if (stepmode)
        return steps;
    if (draw_bool)
        draw_points(points, size, request.scene, request.color_line);

    return 0;
}

int draw_line_brenzenhem_d(request_t request, int stepmode, int draw_bool)
{
    point_t points[3000];
    int size = 0;
    int steps = 1;
    point_t start = request.start;
    point_t end = request.end;
    start.x = round(start.x);
    start.y = round(start.y);
    end.x = round(end.x);
    end.y = round(end.y);
    if ((fabs(start.x - end.x) < eps) && (fabs(start.y - end.y) < eps))
    {
        points[size++] = start;
    }
    else
    {
        int dx = (int)(end.x - start.x);
        int dy = (int)(end.y - start.y);

        int exchange = 0;

        int sx, sy;
        if (dx < 0)
            sx = -1;
        else
            sx = 1;
        if (dy < 0)
            sy = -1;
        else
            sy = 1;

        dx = abs(dx);
        dy = abs(dy);

        if (dy > dx)
        {
            int tmp;
            tmp = dx;
            dx = dy;
            dy = tmp;
            exchange = 1;
        }
        double tg = 1.0 * dy/dx;
        double e = tg - 0.5;

        int x, y;
        x = (int)start.x;
        y = (int)start.y;

        int xb, yb;
        xb = (int)start.x;
        yb = (int)start.y;
        int i = 0;
        while (i <= dx)
        {
            i++;
            if (!stepmode)
                points[size++] = { (double)x, (double)y, request.color_line };
            if (e >= 0)
            {
                if (exchange == 1)
                    x += sx;
                else
                    y += sy;
                e -= 1;
            }
            if (e <= 0)
            {
                if (exchange == 0)
                    x += sx;
                else
                    y += sy;
                e += tg;
            }
            if (stepmode)
            {
                if ((xb != x) && (yb != y) && (i <= dx))
                    steps += 1;
                xb = x;
                yb = y;
            }
        }
        //if ((fabs(x - end.x) < eps) && (fabs(y - end.y) < eps))
        //    points[size++] = { x, y, request.color_line };
    }
    if (stepmode)
        return steps;
    if (draw_bool)
        draw_points(points, size, request.scene, request.color_line);

    return 0;
}

int draw_line_brenzenhem_i(request_t request, int stepmode, int draw_bool)
{
    point_t points[3000];
    int size = 0;
    int steps = 1;
    point_t start = request.start;
    point_t end = request.end;
    double length = 0;
    start.x = round(start.x);
    start.y = round(start.y);
    end.x = round(end.x);
    end.y = round(end.y);
    if ((fabs(start.x - end.x) < eps) && (fabs(start.y - end.y) < eps))
    {
        points[size++] = start;
    }
    else
    {
        int dx = (int)(end.x - start.x);
        int dy = (int)(end.y - start.y);

        int exchange = 0;

        int sx, sy;
        if (dx < 0)
            sx = -1;
        else
            sx = 1;
        if (dy < 0)
            sy = -1;
        else
            sy = 1;

        dx = abs(dx);
        dy = abs(dy);

        if (dy > dx)
        {
            int tmp;
            tmp = dx;
            dx = dy;
            dy = tmp;
            exchange = 1;
        }
        int e = 2 * dy - dx;

        int x, y;
        x = (int)(start.x);
        y = (int)(start.y);

        int xb, yb;
        xb = (int)(start.x);
        yb = (int)(start.y);
        int i = 0;
        while (i <= (int)(dx))
        {
            i++;
            if (!stepmode)
                points[size++] = { (double)x, (double)y, request.color_line };
            if (e >= 0)
            {
                if (exchange == 1)
                    x += sx;
                else
                    y += sy;
                e -= 2 * dx;
            }
            if (e <= 0)
            {
                if (exchange == 0)
                    x += sx;
                else
                    y += sy;
                e += 2 * dy;
            }
            if (stepmode)
            {
                if ((xb != x) && (yb != y) && (i <= (int)dx))
                    steps += 1;
                xb = x;
                yb = y;
            }
        }
        //points[size++] = { (double)x, (double)y, request.color_line };
    }

    if (stepmode)
        return steps;
    if (draw_bool)
        draw_points(points, size, request.scene, request.color_line);

    return 0;
}

int draw_line_brenzenhem_e(request_t request, int stepmode, int draw_bool)
{
    point_t points[3000];
    int size = 0;
    int steps = 1;
    point_t start = request.start;
    point_t end = request.end;
    start.x = round(start.x);
    start.y = round(start.y);
    end.x = round(end.x);
    end.y = round(end.y);
    if ((fabs(start.x - end.x) < eps) && (fabs(start.y - end.y) < eps))
    {
        points[size++] = start;
    }
    else
    {
        int dx = (int)(end.x - start.x);
        int dy = (int)(end.y - start.y);

        int exchange = 0;

        int sx, sy;
        if (dx < 0)
            sx = -1;
        else
            sx = 1;
        if (dy < 0)
            sy = -1;
        else
            sy = 1;

        dx = abs(dx);
        dy = abs(dy);

        if (dy > dx)
        {
            int tmp;
            tmp = dx;
            dx = dy;
            dy = tmp;
            exchange = 1;
        }
        double tg = 255 * 1.0 * dy/dx;
        double e = 255 / 2;
        double w = 255 - tg;

        int x, y;
        x = (int)start.x;
        y = (int)start.y;

        int xb, yb;
        xb = x;
        yb = y;
        int i = 0;
        while (i <= dx)
        {
            i++;
            if (!stepmode)
            {
                request.color_line.setAlpha(e);
                points[size++] = { (double)x, (double)y, request.color_line };
            }
            if (e < w)
            {
                if (exchange == 0)
                    x += sx;
                else
                    y += sy;
                e += tg;
            }
            else
            {
                x += sx;
                y += sy;
                e -= w;
            }
            if (stepmode)
            {
                if ((xb != x) && (yb != y) && (i <= dx))
                    steps += 1;
                xb = x;
                yb = y;
            }
        }
        //if ((fabs(x - end.x) < eps) && (fabs(y - end.y) < eps))
        //    points[size++] = { x, y, request.color_line };
    }
    if (stepmode)
        return steps;
    if (draw_bool)
        draw_points(points, size, request.scene, request.color_line);

    return 0;
}

int draw_line_by(request_t request, int stepmode, int draw_bool)
{
    point_t points[3000];
    int size = 0;
    int steps = 1;
    point_t start = request.start;
    point_t end = request.end;
    // start.x = round(start.x);
    // start.y = round(start.y);
    // end.x = round(end.x);
    // end.y = round(end.y);
    if ((fabs(start.x - end.x) < eps) && (fabs(start.y - end.y) < eps))
    {
        points[size++] = start;
    }
    else
    {
        double x1, x2, y1, y2;
        x1 = start.x;
        x2 = end.x;
        y1 = start.y;
        y2 = end.y;

        bool steep = abs(y2 - y1) > abs(x2 - x1);
        if (steep) {
            double buf = x1;
            x1 = y1;
            y1 = buf;

            buf = x2;
            x2 = y2;
            y2 = buf;
        }

        if (x2 < x1)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }


        double dx = x2 - x1;
        double dy = y2 - y1;
        double gradient;

        if (abs(dx) < eps)
            gradient = 1.0;
        else
            gradient = dy / dx;

        double xend = round(x1);
        double yend = y1 + gradient * (xend - x1);
        double xgap = 1 - ((x1 + 0.5) - (int)(x1 + 0.5));
        double xpxl1 = xend;
        double ypxl1 = (int)yend;
        if (!steep)
        {
            request.color_line.setAlpha(((1 - (yend - (int)yend)) * xgap)*255);
            points[size++] = { xpxl1, ypxl1, request.color_line };
        }
        else
        {
            request.color_line.setAlpha(((1 - (yend - (int)yend)) * xgap)*255);
            points[size++] = { ypxl1, xpxl1, request.color_line };
        }

        if (!steep)
        {
            request.color_line.setAlpha(((yend - (int)yend) * xgap)*255);
            points[size++] = { xpxl1, ypxl1 + 1, request.color_line };
        }
        else
        {
            request.color_line.setAlpha(((yend - (int)yend) * xgap)*255);
            points[size++] = { ypxl1 + 1, xpxl1, request.color_line };
        }
        double intery = yend + gradient;


        xend = round(x2);
        yend = y2 + gradient * (xend - x2);
        xgap = ((x2 + 0.5) - (int)(x2 + 0.5));
        double xpxl2 = xend;
        double ypxl2 = (int)yend;

        if (!steep)
        {
            request.color_line.setAlpha(((1 - (yend - (int)yend)) * xgap)*255);
            points[size++] = { xpxl2, ypxl2, request.color_line };
        }
        else
        {
            request.color_line.setAlpha(((1 - (yend - (int)yend)) * xgap)*255);
            points[size++] = { ypxl2, xpxl2, request.color_line };
        }

        if (!steep)
        {
            request.color_line.setAlpha(((yend - (int)yend) * xgap)*255);
            points[size++] = { xpxl2, ypxl2 + 1, request.color_line };
        }
        else
        {
            request.color_line.setAlpha(((yend - (int)yend) * xgap)*255);
            points[size++] = { ypxl2 + 1, xpxl2, request.color_line };
        }
        int x_prev = steep ? ypxl1 + 1 : xpxl1 + 1;

        if (!steep)
        {
            for (int x = xpxl1 + 1; x <= xpxl2; x++)
            {
                request.color_line.setAlpha((1 - (intery - (int)intery))*255);
                points[size++] = { (double)x, (double)((int)intery), request.color_line };

                request.color_line.setAlpha((intery - (int)intery)*255);
                points[size++] = { (double)x, (double)((int)intery + 1), request.color_line };

                if (stepmode)
                {
                    if (x_prev != (double)((int)intery))
                        steps++;

                    x_prev = (double)((int)intery);
                }

                intery = intery + gradient;
            }
        }
        else
        {
            for (int x = xpxl1 + 1; x <= xpxl2; x++)
            {
                request.color_line.setAlpha((1 - (intery - (int)intery))*255);
                points[size++] = { (double)((int)intery), (double)x, request.color_line };

                request.color_line.setAlpha((intery - (int)intery)*255);
                points[size++] = { (double)((int)intery + 1), (double)x, request.color_line };

                if (stepmode)
                {
                    if (x_prev != (double)((int)intery))
                        steps++;

                    x_prev = (double)((int)intery);
                }

                intery = intery + gradient;
            }
        }
    }

    if (stepmode)
        return steps;
    if (draw_bool)
        draw_points(points, size, request.scene, request.color_line);

    return 0;
}
