#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette;
    MyGraphicsScene *scene = new MyGraphicsScene(this);
    scene->setSceneRect(0, 0, 1398, 1078);
    ui->graphicsView->setScene(scene);
    ui->widget_paint->setAutoFillBackground(true);
    palette.setColor(QPalette::Window, QColor::fromRgb(255,255,255));
    ui->widget_paint->setPalette(palette);
    color_line = QColor::fromRgb(0,0,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

