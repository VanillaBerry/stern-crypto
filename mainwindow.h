#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString filepath;

    int** codematrix;

    int length_n;
    int dimension_k;
    int weight_w;
    int n_k;

    int load_matrix();

private slots:
    void handlebutton_select();
    void handlebutton_load();

};

#endif // MAINWINDOW_H
