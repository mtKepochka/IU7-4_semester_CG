#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtMath>
#include <QGraphicsTextItem>
#include <QMessageBox>

const double EPS = 1e-6;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QLocale locale(QLocale::C);
    locale.setNumberOptions(QLocale::RejectGroupSeparator);

    auto validator_x = new QDoubleValidator(ui->lineEdit_x);
    validator_x->setLocale(locale);
    auto validator_y = new QDoubleValidator(ui->lineEdit_y);
    validator_y->setLocale(locale);

    QString str = QString("Ответ:\n\n\n\n");
    ui->label_ans->setText(str);

    ui->lineEdit_x->setValidator(validator_x);
    ui->lineEdit_y->setValidator(validator_y);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->horizontalHeader()->setMaximumSectionSize(132);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdittextChanged()
{
    int row = ui->tableWidget->currentRow();
    if (row != -1)
    {
        QLineEdit *edit_x = qobject_cast<QLineEdit*>\
            (ui->tableWidget->cellWidget(row, 0));
        QLineEdit *edit_y = qobject_cast<QLineEdit*>\
            (ui->tableWidget->cellWidget(row, 1));
        edit_x->setText(QString::number(edit_x->text().toDouble()));
        edit_y->setText(QString::number(edit_y->text().toDouble()));
    }
    delete ui->graphicsView->scene();
    QString str = QString("Ответ:\n\n\n\n");
    ui->label_ans->setText(str);
}

void MainWindow::on_addPoint_clicked()
{
    if (ui->lineEdit_x->text().size() > 0 && ui->lineEdit_y->text().size() > 0)
    {
        QTableWidget *table = ui->tableWidget;
        int row = table->rowCount();
        table->insertRow(row);
        QLineEdit *edit_x = new QLineEdit(table);
        QLineEdit *edit_y = new QLineEdit(table);

        QLocale locale(QLocale::C);
        locale.setNumberOptions(QLocale::RejectGroupSeparator);

        auto validator_x = new QDoubleValidator(edit_x);
        validator_x->setLocale(locale);
        auto validator_y = new QDoubleValidator(edit_y);
        validator_y->setLocale(locale);

        edit_x->setValidator(validator_x);
        edit_y->setValidator(validator_y);
        edit_x->setText(QString::number(ui->lineEdit_x->text().toDouble()));
        table->setCellWidget(row, 0, edit_x);
        connect(edit_x, SIGNAL(editingFinished()), this,\
                SLOT(on_lineEdittextChanged()));
        edit_y->setText(QString::number(ui->lineEdit_y->text().toDouble()));
        table->setCellWidget(row, 1, edit_y);
        connect(edit_y, SIGNAL(editingFinished()), this,\
                SLOT(on_lineEdittextChanged()));
    }
}


void MainWindow::on_deletePoint_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row == -1)
        return;
    ui->tableWidget->removeRow(row);
    delete ui->graphicsView->scene();
    QString str = QString("Ответ:\n\n\n\n");
    ui->label_ans->setText(str);
}


void MainWindow::on_deleteAll_clicked()
{
    //int row_count = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(0);
    delete ui->graphicsView->scene();
    QString str = QString("Ответ:\n\n\n\n");
    ui->label_ans->setText(str);
}

int ispointsinline(QPointF a, QPointF b, QPointF c)
{
    return qFabs((c.y() - a.y()) * (b.x() - a.x()) - \
           (b.y() - a.y()) * (c.x() - a.x())) < EPS;
}

bool istriangle(QPointF a, QPointF b, QPointF c)
{
    return !ispointsinline(a, b, c);
}

typedef struct
{
    QPointF p1;
    QPointF p2;
    QPointF p3;
    double l23;
    double l13;
    double l12;
    int np1;
    int np2;
    int np3;
} triangle_t;

typedef struct
{
    QPointF center;
    double r;
} circle_t;

void inscribed_center_find(double *x, double *y, triangle_t triangle)
{
    *y = (triangle.l23*triangle.p1.y() + triangle.l13*triangle.p2.y()\
          + triangle.l12*triangle.p3.y())/(triangle.l12 + triangle.l23\
            + triangle.l13);
    *x = (triangle.l23*triangle.p1.x() + triangle.l13*triangle.p2.x()\
          + triangle.l12*triangle.p3.x())/(triangle.l12 + triangle.l23\
            + triangle.l13);
}

void inscribed_radius_find(double *r, double p, triangle_t triangle)
{
    *r = qSqrt((p-triangle.l12)*(p-triangle.l23)*(p-triangle.l13)/p);
}

void circumscribed_center_find(double *x, double *y, triangle_t triangle)
{
    *x = ((qPow(triangle.p1.x(), 2) + qPow(triangle.p1.y(), 2))\
              *(triangle.p2.y() - triangle.p3.y())\
         + (qPow(triangle.p2.x(), 2) + qPow(triangle.p2.y(), 2))\
                *(triangle.p3.y() - triangle.p1.y())\
          + (qPow(triangle.p3.x(), 2) + qPow(triangle.p3.y(), 2))\
                *(triangle.p1.y() - triangle.p2.y()))\
         /(2*(triangle.p1.x()*(triangle.p2.y() - triangle.p3.y())\
                + triangle.p2.x()*(triangle.p3.y() \
           - triangle.p1.y()) + triangle.p3.x()*(triangle.p1.y()\
                - triangle.p2.y())));
    *y = ((qPow(triangle.p1.x(), 2) + qPow(triangle.p1.y(), 2))\
              *(triangle.p3.x() - triangle.p2.x())\
          + (qPow(triangle.p2.x(), 2) + qPow(triangle.p2.y(), 2))\
                *(triangle.p1.x() - triangle.p3.x())\
          + (qPow(triangle.p3.x(), 2) + qPow(triangle.p3.y(), 2))\
                *(triangle.p2.x() - triangle.p1.x()))\
         /(2*(triangle.p1.x()*(triangle.p2.y() - triangle.p3.y())\
                + triangle.p2.x()*(triangle.p3.y() \
           - triangle.p1.y()) + triangle.p3.x()*(triangle.p1.y()\
                - triangle.p2.y())));
}

void MainWindow::on_makeCalc_clicked()
{
    if (ui->tableWidget->rowCount() <= 2)
    {
        ui->label_ans->setText(QString("Ошибка:\nНедостаточное"
                                       " количество\nточек.\n\n"));
        delete ui->graphicsView->scene();
        return;
    }
    int rows = ui->tableWidget->rowCount();
    QPointF points[rows];
    for (int i = 0; i < rows; i++)
    {
        QLineEdit *edit_x = qobject_cast<QLineEdit*>\
            (ui->tableWidget->cellWidget(i, 0));
        QLineEdit *edit_y = qobject_cast<QLineEdit*>\
            (ui->tableWidget->cellWidget(i, 1));
        points[i].setX(edit_x->text().toDouble());
        points[i].setY(edit_y->text().toDouble());
    }
    triangle_t triangles[rows*rows*rows];
    int size = 0;
    for (int i = 0; i < rows - 2; i++)
        for (int j = i + 1; j < rows - 1; j++)
            for (int k = j + 1; k < rows; k++)
            {
                if (istriangle(points[i], points[j], points[k]))
                {
                    double l23 = qSqrt(qPow(points[j].x() - points[k].x(),2)\
                                      + qPow(points[j].y() - points[k].y(),2));
                    double l13 = qSqrt(qPow(points[i].x() - points[k].x(),2)\
                                      + qPow(points[i].y() - points[k].y(),2));
                    double l12 = qSqrt(qPow(points[j].x() - points[i].x(),2)\
                                      + qPow(points[j].y() - points[i].y(),2));
                    triangles[size] = {points[i], points[j], points[k],\
                                       l23, l13, l12, i, j, k};
                    size++;
                }
            }
    if (size == 0)
    {
        ui->label_ans->setText(QString("Ошибка:\nПо точкам"
                               " невозможно\nпостроить ни один треугольник.\n\n"));
        delete ui->graphicsView->scene();
        return;
    }
    double p = (triangles[0].l12 + triangles[0].l23 + triangles[0].l13)/2;
    double x0_inscribed;
    double y0_inscribed;
    double r0_inscribed;
    inscribed_center_find(&x0_inscribed, &y0_inscribed, triangles[0]);
    inscribed_radius_find(&r0_inscribed, p, triangles[0]);
    circle_t inscribed = {{x0_inscribed, y0_inscribed}, r0_inscribed};
    double x0_circumscribed;
    double y0_circumscribed;
    double r0_circumscribed;
    circumscribed_center_find(&x0_circumscribed,\
                              &y0_circumscribed, triangles[0]);
    r0_circumscribed = qSqrt(qPow(x0_circumscribed - triangles[0].p1.x(),2) \
                            + qPow(y0_circumscribed - triangles[0].p1.y(),2));
    circle_t circumscribed = {{x0_circumscribed,\
                               y0_circumscribed}, r0_circumscribed};
    double min = qFabs(M_PI*qPow(circumscribed.r,2)\
                       - M_PI*qPow(inscribed.r,2));
    triangle_t res = triangles[0];
    for (int i = 1; i < size; i++)
    {
        p = (triangles[i].l12 + triangles[i].l23 + triangles[i].l13)/2;
        inscribed_center_find(&x0_inscribed, &y0_inscribed, triangles[i]);
        inscribed_radius_find(&r0_inscribed, p, triangles[i]);
        circumscribed_center_find(&x0_circumscribed,\
                                  &y0_circumscribed, triangles[i]);
        r0_circumscribed = qSqrt(qPow(x0_circumscribed \
                                 - triangles[i].p1.x(),2)\
                                 + qPow(y0_circumscribed\
                                 - triangles[i].p1.y(),2));
        if (min - qFabs(M_PI*qPow(r0_circumscribed,2)\
                        - M_PI*qPow(r0_inscribed,2)) > EPS)
        {
            min = qFabs(M_PI*qPow(r0_circumscribed,2)\
                        - M_PI*qPow(r0_inscribed,2));
            circumscribed = {{x0_circumscribed,\
                              y0_circumscribed}, r0_circumscribed};
            inscribed = {{x0_inscribed, y0_inscribed}, r0_inscribed};
            res = triangles[i];
        }
    }
    triangle_t res_copy = res;
    delete ui->graphicsView->scene();
    QGraphicsScene *scene = new QGraphicsScene();
    QPolygonF *tr = new QPolygonF();

    QGraphicsTextItem *text = scene->addText(QString("(%1,%2)").\
                              arg(QString::number(circumscribed.center.x()),\
                                  QString::number(circumscribed.center.y())));
    text->setPos(0, 0);

    text = scene->addText(QString("(%1,%2)").\
                          arg(QString::number(inscribed.center.x()),\
                              QString::number(inscribed.center.y())));

    double scale = (circumscribed.r * 2)/(ui->graphicsView->height() - 40);

    circumscribed.center.setY(-1*circumscribed.center.y());

    res.p1.setY(-1*res.p1.y());
    res.p2.setY(-1*res.p2.y());
    res.p3.setY(-1*res.p3.y());

    inscribed.center.setY(-1*inscribed.center.y());

    res.p1.setX(res.p1.x() - circumscribed.center.x());
    res.p1.setY(res.p1.y() - circumscribed.center.y());
    res.p2.setX(res.p2.x() - circumscribed.center.x());
    res.p2.setY(res.p2.y() - circumscribed.center.y());
    res.p3.setX(res.p3.x() - circumscribed.center.x());
    res.p3.setY(res.p3.y() - circumscribed.center.y());


    inscribed.center.setX(inscribed.center.x() - circumscribed.center.x());
    inscribed.center.setY(inscribed.center.y() - circumscribed.center.y());
    inscribed.r = inscribed.r/scale;

    circumscribed.center.setX(0);
    circumscribed.center.setY(0);
    circumscribed.r = circumscribed.r/scale;

    res.p1.setX((res.p1.x())/scale);
    res.p1.setY((res.p1.y())/scale);
    res.p2.setX((res.p2.x())/scale);
    res.p2.setY((res.p2.y())/scale);
    res.p3.setX((res.p3.x())/scale);
    res.p3.setY((res.p3.y())/scale);

    inscribed.center.setX(inscribed.center.x()/scale);
    inscribed.center.setY(inscribed.center.y()/scale);
    text->setPos(inscribed.center.x(), inscribed.center.y());

    res.l12 /= scale;
    res.l13 /= scale;
    res.l23 /= scale;

    tr->append(res.p1);
    tr->append(res.p2);
    tr->append(res.p3);
    QPen pentr(Qt::black, 1);
    QPen pencir(Qt::green, 1);
    QPen penins(Qt::blue, 1);
    scene->addPolygon(*tr, pentr);
    scene->addEllipse(circumscribed.center.x()-circumscribed.r,\
                      circumscribed.center.y()-circumscribed.r,\
                      circumscribed.r*2, circumscribed.r*2, pencir);
    scene->addEllipse(inscribed.center.x() - inscribed.r, \
                      inscribed.center.y() - inscribed.r,\
                      inscribed.r*2, inscribed.r*2, penins);

    scene->addLine(res.p1.x(), res.p1.y(), inscribed.center.x(),\
                                    inscribed.center.y(), penins);
    scene->addLine(res.p2.x(), res.p2.y(), inscribed.center.x(),\
                                    inscribed.center.y(), penins);
    scene->addLine(res.p3.x(), res.p3.y(), inscribed.center.x(),\
                                    inscribed.center.y(), penins);

    scene->addLine((res.p2.x() - res.p1.x())/2 + res.p1.x(),\
                   (res.p2.y() - res.p1.y())/2 + res.p1.y(),\
                   circumscribed.center.x(), circumscribed.center.y(), pencir);
    scene->addLine((res.p2.x() - res.p3.x())/2 + res.p3.x(),\
                   (res.p2.y() - res.p3.y())/2 + res.p3.y(),\
                   circumscribed.center.x(), circumscribed.center.y(), pencir);
    scene->addLine((res.p3.x() - res.p1.x())/2 + res.p1.x(),\
                   (res.p3.y() - res.p1.y())/2 + res.p1.y(),\
                   circumscribed.center.x(), circumscribed.center.y(), pencir);

    QString str = QString("Ответ:\nТочка %1: %2; %3;\n"
                          "Точка %4: %5; %6;\nТочка %7: %8; %9;"
                          "\nМин. разность: %10;").\
                  arg(QString::number(res_copy.np1 + 1),\
                           QString::number(res_copy.p1.x()),\
                  QString::number(res_copy.p1.y()),\
                           QString::number(res_copy.np2 + 1),\
                  QString::number(res_copy.p2.x()),\
                           QString::number(res_copy.p2.y()),\
                  QString::number(res_copy.np3 + 1),\
                           QString::number(res_copy.p3.x()),\
                  QString::number(res_copy.p3.y()), QString::number(min));
    ui->label_ans->setText(str);

    text = scene->addText(QString("%3 (%1,%2)").\
                          arg(QString::number(res_copy.p1.x()),\
                          QString::number(res_copy.p1.y()),\
                          QString::number(res.np1 + 1)));
    text->setPos(res.p1.x(), res.p1.y());

    text = scene->addText(QString("%3 (%1,%2)").\
                          arg(QString::number(res_copy.p2.x()),\
                          QString::number(res_copy.p2.y()),\
                          QString::number(res.np2 + 1)));
    text->setPos(res.p2.x(), res.p2.y());

    text = scene->addText(QString("%3 (%1,%2)").\
                          arg(QString::number(res_copy.p3.x()),\
                          QString::number(res_copy.p3.y()),\
                          QString::number(res.np3 + 1)));
    text->setPos(res.p3.x(), res.p3.y());

    ui->graphicsView->setScene(scene);
}

void MainWindow::on_action_triggered()
{
    QMessageBox box;
    box.setText("Вариант 12.\nНа плоскости дано множество точек.\n"
                "Найти такой треугольник с вершинами в этих точках,\n"
                "для которого разность площадей описанной и вписанной"
                " окружностей минимальна.\nСделать "
                "в графическом режиме вывод изображения.");
    box.setWindowTitle("О программе.");
    box.exec();
}

void MainWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    delete ui->graphicsView->scene();
    QString str = QString("Ответ:\n\n\n\n");
    ui->label_ans->setText(str);
}

