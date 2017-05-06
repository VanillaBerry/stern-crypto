#ifndef STERN_ATTACK_H
#define STERN_ATTACK_H

#include <QWidget>

namespace Ui {
    class stern_attack;
}

class stern_attack: public QWidget
{
    Q_OBJECT

public:
    stern_attack(int **_codematrix, int _len, int _dim, int _w, int _p = 1, int _l = 1);

    ~stern_attack();

    int proceed();

// step 1
    int* chosencolumns;
    int** codematrix;


private:    

    int length_n;
    int dimension_k;
    int weight_w;
    int n_k;

    int p = 1;
    int l = 1;


    int select_column(int *selected); // select one point from massive 1..n or return -1
    int elimination(int **matrix, int rows, int cols, int piv_row, int piv_col);
    int c_n_k(int n, int k);
    int generate_next_p_bits(int curr, int p);

signals:
    void completed_step1();
    void completed_step2();
    void completed_step3();
    void completed_step4();
    void completed_step5();
    void completed_step6();

};

#endif // STERN_ATTACK_H
