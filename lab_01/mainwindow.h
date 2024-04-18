#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_addPoint_clicked();

    void on_deletePoint_clicked();

    void on_deleteAll_clicked();

    void on_makeCalc_clicked();

    void on_action_triggered();

    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_lineEdittextChanged();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
