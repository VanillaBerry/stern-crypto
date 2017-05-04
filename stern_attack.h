#ifndef STERN_ATTACK_H
#define STERN_ATTACK_H


class stern_attack
{
public:
    stern_attack(int **_codematrix, int _len, int _dim, int _w);
    ~stern_attack();

    int proceed();

private:
    int** codematrix = nullptr;

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

};

#endif // STERN_ATTACK_H
