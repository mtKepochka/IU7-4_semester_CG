#ifndef REQUEST_H
#define REQUEST_H
#include "point.h"
#include <QGraphicsScene>

typedef struct
{
    point_t start;
    point_t end;
    QGraphicsScene *scene;
    QColor color_line;
} request_t;

#endif // REQUEST_H
