#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stern_attack_view.h>

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

    stern_attack_view * strn_atck;
    int matrix_is_loaded = -1;

    int** codematrix;


    int length_n;
    int dimension_k;
    int weight_w;
    int n_k;

    int p;
    int l;

    int load_matrix();

private slots:
    void handlebutton_select();
    void handlebutton_load();
    void handlebutton_proceed();

};

#endif // MAINWINDOW_H
