#ifndef REQUEST_H
#define REQUEST_H
#include "point.h"
#include <QGraphicsScene>

typedef struct
{
    point_t center;
    double radius_x;
    double radius_y;
    QGraphicsScene *scene;
    QColor color_line;
} request_t;

#endif // REQUEST_H
