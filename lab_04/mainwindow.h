#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_line_color_clicked();

    void on_btn_back_color_clicked();

    void on_btn_draw_ellipse_clicked();

    void on_btn_draw_circle_clicked();

    void on_btn_drawspectre_ellipse_clicked();

    void on_btn_drawspectre_circle_clicked();

    void on_btn_compare_time_circle_clicked();

    void on_btn_compare_time_ellipse_clicked();

    void on_btn_clearscreen_clicked();

private:
    Ui::MainWindow *ui;
    QColor color_line;
    QColor color_back;
};
#endif // MAINWINDOW_H
