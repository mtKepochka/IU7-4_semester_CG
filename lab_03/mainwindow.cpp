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
    delete ui->graphicsView->scene();
    delete ui;
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


void MainWindow::on_btn_line_color_clicked()
{
    QColor color = QColorDialog::getColor();
    if (!color.isValid()) return;
    QPalette palette;
    //QString str = "rgb(" + QString(color.red()) + "," + QString(color.green()) + "," + QString(color.blue());
    palette.setColor(QPalette::Window, color);
    ui->widget_line->setPalette(palette);
    color_line = color;
}


void MainWindow::on_btn_clearscreen_clicked()
{
    ui->graphicsView->scene()->clear();
}

void MainWindow::on_btn_drawline_clicked()
{
    QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
        group.addButton(allButtons[i],i);

    point_t start = { ui->x_start->text().toDouble(), ui->y_start->text().toDouble(), color_line };
    point_t end = { ui->x_end->text().toDouble(), ui->y_end->text().toDouble(), color_line };
    request_t request = {
        .start = start,
        .end = end,
        .scene = ui->graphicsView->scene(),
        .color_line = color_line
    };

    switch (group.checkedId()) {
    case CDA:
        draw_line_cda(request, 0);
        break;
    case BREZENHEM_DOUBLE:
        draw_line_brenzenhem_d(request, 0);
        break;
    case BREZENHEM_INT:
        draw_line_brenzenhem_i(request, 0);
        break;
    case BREZENHEM_WITHOUT_EDGES:
        draw_line_brenzenhem_e(request, 0);
        break;
    case BY:
        draw_line_by(request, 0);
        break;
    case LIBRARY:
        draw_line_library(request);
        break;
    default:
        break;
    }
}


void MainWindow::on_btn_drawspectre_clicked()
{
    QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
        group.addButton(allButtons[i],i);

    point_t start = { 1398 / 2, 1078 / 2, color_line };


    double step = ui->angle->text().toDouble() * M_PI / 180;
    double radius = ui->length->text().toDouble();
    int i = 0;
    for (double angle = 0; angle < 2 * M_PI && i < 360 / (int)round(ui->angle->text().toDouble()); angle += step, i++)
    {
        point_t end;
        end.x = start.x + radius * cos(angle);
        end.y = start.y + radius * sin(angle);
        end.color_point = color_line;
        request_t request = {
            .start = start,
            .end = end,
            .scene = ui->graphicsView->scene(),
            .color_line = color_line
        };

        switch (group.checkedId()) {
        case CDA:
            draw_line_cda(request, 0);
            break;
        case BREZENHEM_DOUBLE:
            draw_line_brenzenhem_d(request, 0);
            break;
        case BREZENHEM_INT:
            draw_line_brenzenhem_i(request, 0);
            break;
        case BREZENHEM_WITHOUT_EDGES:
            draw_line_brenzenhem_e(request, 0);
            break;
        case BY:
            draw_line_by(request, 0);
            break;
        case LIBRARY:
            draw_line_library(request);
            break;
        default:
            break;
        }
    }
}

unsigned long long cur_ms_gettimeofday()
{
    struct timeval timeval;
    gettimeofday(&timeval, NULL);

    return (timeval.tv_sec * 1000000 + timeval.tv_usec);
}

void MainWindow::on_btn_compare_time_clicked()
{
    //QWidget *wdg = new QWidget;
    QChartView *wdg = new QChartView;
    wdg->setMinimumWidth(1000);
    QChart *chrt = new QChart;
    //chrt->setGeometry(0, 0, 500, 500);

    chrt->setTitle("График сравнения времени работы в микросекундах при длине " + ui->length_2->text());

    // кривые, отображаемые на графике
    QBarSeries *series1 = new QBarSeries();
    QBarSet *set0 = new QBarSet("CDA");
    QBarSet *set1 = new QBarSet("brenzenhem_float");
    QBarSet *set2 = new QBarSet("brenzenhem_int");
    QBarSet *set3 = new QBarSet("brenzenhem_smooth");
    QBarSet *set4 = new QBarSet("By");
    // построение графиков функций

    point_t end;
    double angle = 45 * M_PI / 180.0;
    double radius = ui->length_2->text().toDouble();
    end.x = radius * cos(angle);
    end.y = radius * sin(angle);
    //end.x = 0;
    //end.y = radius;
    end.color_point = color_line;
    QGraphicsScene scenel;
    request_t request = {
        .start = {0, 0, color_line},
        .end = end,
        .scene = &scenel,
        .color_line = color_line
    };

    unsigned long long start_time = 0;
    unsigned long long end_time = 0;
    for (size_t k = 0; k < 20000; k++)
    {
        start_time += cur_ms_gettimeofday();
        draw_line_cda(request, 0, 0);
        end_time += cur_ms_gettimeofday();
    }
    *set0 << (end_time - start_time)/20000.0;//замеры

    start_time = 0;
    end_time = 0;
    for (size_t k = 0; k < 20000; k++)
    {
        start_time += cur_ms_gettimeofday();
        draw_line_brenzenhem_d(request, 0, 0);
        end_time += cur_ms_gettimeofday();
    }
    *set1 << (end_time - start_time)/20000.0;

    start_time = 0;
    end_time = 0;
    for (size_t k = 0; k < 20000; k++)
    {
        start_time += cur_ms_gettimeofday();
        draw_line_brenzenhem_i(request, 0, 0);
        end_time += cur_ms_gettimeofday();
    }
    *set2 << (end_time - start_time)/20000.0;

    start_time = 0;
    end_time = 0;
    for (size_t k = 0; k < 20000; k++)
    {
        start_time += cur_ms_gettimeofday();
        draw_line_brenzenhem_e(request, 0, 0);
        end_time += cur_ms_gettimeofday();
    }
    *set3 << (end_time - start_time)/20000.0;

    start_time = 0;
    end_time = 0;
    for (size_t k = 0; k < 20000; k++)
    {
        start_time += cur_ms_gettimeofday();
        draw_line_by(request, 0, 0);
        end_time += cur_ms_gettimeofday();
    }
    *set4 << (end_time - start_time)/20000.0;
    series1->append(set0);
    series1->append(set1);
    series1->append(set2);
    series1->append(set3);
    series1->append(set4);
    // связываем график с построенными кривыми
    chrt->addSeries(series1);
    // устанавливаем оси для каждого графика
    chrt->createDefaultAxes();

    wdg->setChart(chrt);
    wdg->show();
}


void MainWindow::on_btn_compare_edges_clicked() // несколько графиков
{
    QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
        group.addButton(allButtons[i],i);
    //QWidget *wdg = new QWidget;
    QChartView *wdg = new QChartView;
    wdg->setMinimumWidth(1000);
    QChartView *wdg2 = new QChartView;
    wdg2->setMinimumWidth(1000);

    QChart *chrt = new QChart;
    QChart *chrt2 = new QChart;

    //chrt->setGeometry(0, 0, 500, 500);

    chrt->setTitle("График ступенчатости при длине " + ui->length_2->text());
    chrt2->setTitle("График ступенчатости при длине " + ui->length_2->text());

    // кривые, отображаемые на графике
    QLineSeries* series1 = new QLineSeries();
    series1->setName("CDA");
    QLineSeries* series2 = new QLineSeries();
    series2->setName("brenzenhem_float");
    series2->setPen(QPen(Qt::darkRed,2,Qt::DotLine));
    QLineSeries* series3 = new QLineSeries();
    series3->setName("brenzenhem_int");
    series3->setPen(QPen(Qt::darkGreen,2,Qt::DashLine));
    QLineSeries* series4 = new QLineSeries();
    series4->setName("brenzenhem_smooth");
    series4->setPen(QPen(Qt::darkCyan,2,Qt::DashDotLine));
    QLineSeries* series5 = new QLineSeries();
    series5->setName("By");
    series5->setPen(QPen(Qt::darkYellow,2,Qt::DashDotDotLine));
    // построение графиков функций
    for(int i = 0; i <= 90; i+=5)
    {
        point_t end;
        double angle = i * M_PI / 180.0;
        double radius = ui->length_2->text().toDouble();
        end.x = 1400 + radius * cos(angle);
        end.y = 1080 + radius * sin(angle);
        end.color_point = color_line;
        qDebug() << end.x << " " << end.y << Qt::endl;
        request_t request = {
            .start = {1400, 1080, color_line},
            .end = end,
            .scene = ui->graphicsView->scene(),
            .color_line = color_line
        };
        series1->append(i, draw_line_cda(request, 1));
        series2->append(i, draw_line_brenzenhem_d(request, 1));
        series3->append(i, draw_line_brenzenhem_i(request, 1));
        series4->append(i, draw_line_brenzenhem_e(request, 1));
        series5->append(i, draw_line_by(request, 1));
    }
    // связываем график с построенными кривыми
    chrt->addSeries(series1);
    chrt->addSeries(series2);
    chrt->addSeries(series3);
    chrt->addSeries(series4);
    chrt->addSeries(series5);
    // устанавливаем оси для каждого графика
    chrt->createDefaultAxes();
    wdg->setChart(chrt);
    wdg->show();
    switch (group.checkedId()) {
    case CDA:
        chrt2->addSeries(series1);
        break;
    case BREZENHEM_DOUBLE:
        chrt2->addSeries(series2);
        break;
    case BREZENHEM_INT:
        chrt2->addSeries(series3);
        break;
    case BREZENHEM_WITHOUT_EDGES:
        chrt2->addSeries(series4);
        break;
    case BY:
        chrt2->addSeries(series5);
        break;
    case LIBRARY:
        return;
        break;
    default:
        return;
        break;
    }
    chrt2->createDefaultAxes();
    wdg2->setChart(chrt2);
    wdg2->show();
}

