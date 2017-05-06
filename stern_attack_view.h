#ifndef STERN_ATTACK_VIEW_H
#define STERN_ATTACK_VIEW_H

#include <QWidget>
#include <stern_attack.h>

namespace Ui {
class stern_attack_view;
}

class stern_attack_view : public QWidget
{
    Q_OBJECT

public:
    explicit stern_attack_view(QWidget *parent = 0);
    void set_params(int **_codematrix, int _len, int _dim, int _w, int _p, int _l);
    ~stern_attack_view();

private:
    Ui::stern_attack_view *ui;

// parameters
    int ** codematrix;
    int ** codematrix_copy;
    int* chosencolumns;

// step 2
    int* arrayX;
    int numofX;

    int* arrayY;
    int numofY;

// step 3
    int* arrayJ;

// step 4
    int ** pi_A;
    int ** pi_B;

    int ** guide_values_A;
    int ** guide_values_B;

    int subsA;
    int subsB;

// step 5
    int pair1;
    int pair2;

    int length_n;
    int dimension_k;
    int weight_w;
    int n_k;

    int p = 1;
    int l = 1;

// step 1
    int **arraycopy(int **array);
    int select_column(int *selected); // select one point from massive 1..n or return -1
    int elimination(int **matrix, int rows, int cols, int piv_row, int piv_col);

// step 4
    int c_n_k(int n, int k);
    int generate_guides();

// step 5
    bool vectors_are_equal(int *vec1, int* vec2, int len);
    int weight_of_vector(int *vec, int len);

    int **pairs;
    int num_of_pairs;

    int *vector;

    bool candidate = false;
    int candidate_index;

// step 6
    int *x;
    int *y;
    int *z;

// steps
    int step1();
    int step2();
    int step3();
    int step4();
    int step5();
    int step6();

    void step1_view();
    void step2_view();
    void step3_view();
    void step4_view();
    void step5_view();
    void step6_view();

private slots:

    void handle_try_again();
    void handle_saveresult();
    void handle_quit();
};

#endif // STERN_ATTACK_VIEW_H
