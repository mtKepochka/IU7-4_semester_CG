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
    void on_btn_back_color_clicked();

    void on_btn_line_color_clicked();

    void on_btn_clearscreen_clicked();

    void on_btn_drawline_clicked();

    void on_btn_drawspectre_clicked();

    void on_btn_compare_time_clicked();

    void on_btn_compare_edges_clicked();

private:
    Ui::MainWindow *ui;
    QColor color_line;
    QColor color_back;
};
#endif // MAINWINDOW_H
