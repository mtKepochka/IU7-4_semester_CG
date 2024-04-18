#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "calcs.h"

#define SCENE_HEIGHT 968
#define SCENE_WIDTH 1258

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(-SCENE_WIDTH/2, -SCENE_HEIGHT/2, SCENE_WIDTH, SCENE_HEIGHT);

    read_figure(&figure);
    draw_figure(&figure, scene);

    ui->graphicsView->setScene(scene);
    figure_past = figure;
}

MainWindow::~MainWindow()
{
    delete ui->graphicsView->scene();
    delete ui;
}

void MainWindow::on_set_start_clicked()
{
    figure_past = figure;
    read_figure(&figure);
    draw_figure(&figure, ui->graphicsView->scene());
}


void MainWindow::on_move_btn_clicked()
{
    figure_past = figure;
    point_t move = { ui->dx->text().toDouble(),
                    ui->dy->text().toDouble() };
    move_figure(&figure, move);
    draw_figure(&figure, ui->graphicsView->scene());
}


void MainWindow::on_rotate_btn_clicked()
{
    figure_past = figure;
    point_t rotate = { ui->rx->text().toDouble(),
                      ui->ry->text().toDouble() };
    double angle = ui->angle->text().toDouble() * M_PI / 180;
    rotate_figure(&figure, angle, rotate);
    draw_figure(&figure, ui->graphicsView->scene());
}


void MainWindow::on_scale_btn_clicked()
{
    figure_past = figure;
    point_t scale = { ui->kx->text().toDouble(),
                      ui->ky->text().toDouble() };
    point_t scale_center = { ui->mx->text().toDouble(),
                            ui->my->text().toDouble() };
    scale_figure(&figure, scale, scale_center);
    draw_figure(&figure, ui->graphicsView->scene());
}


void MainWindow::on_go_back_clicked()
{
    figure = figure_past;
    draw_figure(&figure, ui->graphicsView->scene());
}

