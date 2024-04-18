#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "figure.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    figure_t figure;
    figure_t figure_past;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_set_start_clicked();

    void on_move_btn_clicked();

    void on_rotate_btn_clicked();

    void on_scale_btn_clicked();

    void on_go_back_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
