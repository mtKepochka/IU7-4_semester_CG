#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QColorDialog>
#include <QDebug>
#include <QButtonGroup>
#include "algos.h"
#include <QtCharts>
#include <time.h>
#include <sys/time.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette;
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1398, 1078);
    ui->graphicsView->setScene(scene);
    ui->widget_back->setAutoFillBackground(true);
    ui->widget_line->setAutoFillBackground(true);
    palette.setColor(QPalette::Window, QColor::fromRgb(255,255,255));
    ui->widget_back->setPalette(palette);
    palette.setColor(QPalette::Window, QColor::fromRgb(0,0,0));
    ui->widget_line->setPalette(palette);
    color_line = QColor::fromRgb(0,0,0);
    color_back = QColor::fromRgb(255,255,255);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_line_color_clicked()
{
    QColor color = QColorDialog::getColor();
    if (!color.isValid()) return;
    QPalette palette;
    palette.setColor(QPalette::Window, color);
    ui->widget_line->setPalette(palette);
    color_line = color;
}


void MainWindow::on_btn_back_color_clicked()
{
    QColor color = QColorDialog::getColor();
    if (!color.isValid()) return;
    QPalette palette;
    palette.setColor(QPalette::Window, color);
    ui->widget_back->setPalette(palette);
    ui->graphicsView->scene()->setBackgroundBrush(QBrush(color));
    color_back = color;
}


void MainWindow::on_btn_draw_ellipse_clicked()
{
    QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
        group.addButton(allButtons[i],i);

    point_t center = { ui->x_center->text().toDouble(), ui->y_center->text().toDouble(), color_line };
    request_t request = {
        .center = center,
        .radius_x = ui->radius_x->text().toDouble(),
        .radius_y = ui->radius_y->text().toDouble(),
        .scene = ui->graphicsView->scene(),
        .color_line = color_line
    };
    switch (group.checkedId()) {
    case CANONICAL:
        draw_ellipse_canonical(request);
        break;
    case PARAMETR:
        //draw_line_brenzenhem_d(request, 0);
        break;
    case MIDDLE_POINT:
        //draw_line_brenzenhem_i(request, 0);
        break;
    case BREZENHEM:
        //draw_line_brenzenhem_e(request, 0);
        break;
    case LIBRARY:
        draw_ellipse_library(request);
        break;
    default:
        break;
    }
}


void MainWindow::on_btn_draw_circle_clicked()
{
    QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
        group.addButton(allButtons[i],i);

    point_t center = { ui->x_center->text().toDouble(), ui->y_center->text().toDouble(), color_line };
    request_t request = {
        .center = center,
        .radius_x = ui->radius->text().toDouble(),
        .radius_y = ui->radius->text().toDouble(),
        .scene = ui->graphicsView->scene(),
        .color_line = color_line
    };

    switch (group.checkedId()) {
    case CANONICAL:
        draw_circle_canonical(request);
        break;
    case PARAMETR:
        //draw_line_brenzenhem_d(request, 0);
        break;
    case MIDDLE_POINT:
        //draw_line_brenzenhem_i(request, 0);
        break;
    case BREZENHEM:
        //draw_line_brenzenhem_e(request, 0);
        break;
    case LIBRARY:
        draw_ellipse_library(request);
        break;
    default:
        break;
    }
}


void MainWindow::on_btn_drawspectre_ellipse_clicked()
{
    QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
        group.addButton(allButtons[i],i);

    point_t center = { 1398 / 2, 1078 / 2, color_line };


    double step = ui->step->text().toDouble();
    double number = ui->number->text().toDouble();
    double radius_x = ui->radius_x->text().toDouble();
    double radius_y = ui->radius_y->text().toDouble();
    for (int i = 0; i < number; i++)
    {
        request_t request = {
            .center = center,
            .radius_x = radius_x,
            .radius_y = radius_y,
            .scene = ui->graphicsView->scene(),
            .color_line = color_line
        };

        switch (group.checkedId()) {
        case CANONICAL:
            draw_ellipse_canonical(request);
            break;
        case PARAMETR:
            //draw_line_brenzenhem_d(request, 0);
            break;
        case MIDDLE_POINT:
            //draw_line_brenzenhem_i(request, 0);
            break;
        case BREZENHEM:
            //draw_line_brenzenhem_e(request, 0);
            break;
        case LIBRARY:
            draw_ellipse_library(request);
            break;
        default:
            break;
        }
        radius_x += step;
        radius_y += step;
    }
}


void MainWindow::on_btn_drawspectre_circle_clicked()
{
    QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
        group.addButton(allButtons[i],i);

    point_t center = { 1398 / 2, 1078 / 2, color_line };


    double step = ui->step->text().toDouble();
    double number = ui->number->text().toDouble();
    double radius_x = ui->radius->text().toDouble();
    double radius_y = ui->radius->text().toDouble();
    for (int i = 0; i < number; i++)
    {
        request_t request = {
            .center = center,
            .radius_x = radius_x,
            .radius_y = radius_y,
            .scene = ui->graphicsView->scene(),
            .color_line = color_line
        };

        switch (group.checkedId()) {
        case CANONICAL:
            draw_circle_canonical(request);
            break;
        case PARAMETR:
            //draw_line_brenzenhem_d(request, 0);
            break;
        case MIDDLE_POINT:
            //draw_line_brenzenhem_i(request, 0);
            break;
        case BREZENHEM:
            //draw_line_brenzenhem_e(request, 0);
            break;
        case LIBRARY:
            draw_ellipse_library(request);
            break;
        default:
            break;
        }
        radius_x += step;
        radius_y += step;
    }
}


void MainWindow::on_btn_compare_time_circle_clicked()
{

}


void MainWindow::on_btn_compare_time_ellipse_clicked()
{

}


void MainWindow::on_btn_clearscreen_clicked()
{
    ui->graphicsView->scene()->clear();
}

