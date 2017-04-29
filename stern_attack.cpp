#include "stern_attack.h"
#include "QTime"

stern_attack::stern_attack(int **_codematrix, int _len, int _dim, int _w)
{
    codematrix = _codematrix;
    length_n = _len;
    dimension_k = _dim;
    weight_w = _w;
    n_k = length_n - dimension_k;

};

stern_attack::~stern_attack()
{
    if (codematrix != NULL)
    {
        for(int i=0; i < n_k; ++i)
        delete [] codematrix[i];

        delete [] codematrix;
    };
};

int stern_attack::proceed()
{
// preparations

    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

// step 1
// modified gaussian elimination

    int* chosencolumns = new int[length_n];
    int* pivots = new int[n_k];

    int i = 0;
    int col_num;

    while(i < n_k)
    {
        col_num = select_column(chosencolumns);


    };



 /*   for(int i=0; i < n_k; ++i)
        for(int j=0; j < n_k; ++j)
        {
            int chsn = chosencolumns[i];
            squarematrix[i][j] = codematrix[chsn][j];
        };*/

// gaussian elimination


// freeing memory and returning result

    delete [] chosencolumns; //see step 1
    delete [] pivots; //step 1
    return 0;
};

int stern_attack::select_column(int *selected)
{
   int result = -1;
   int tries = 0;
   int _res;

   while((tries < 1000) and (result == -1))
   {
    _res = qrand() % length_n;
    if (selected[_res] == 0)
    {
        result = _res;
        selected[_res] = 1;
    }; // endif
    tries += 1;
   }; // endwhile

   return result;
};
/*
int stern_attack::chose_columns(int *_chosen)
{
    bool* freecolumns = new bool[length_n];

    int curr_num = 0;
    int num_to_chose = length_n - dimension_k;
    int attempt = -1;

    while(curr_num < num_to_chose)
    {
        while(attempt == -1) attempt = select_column(freecolumns);

        _chosen[curr_num] = attempt;

        attempt = -1;
        curr_num += 1;

    };

    delete[] freecolumns;

    return 0;
};
*/

   /*
int stern_attack::array_settle_down(int **array, int width, int height)
{
    if (_q <= 0) return 1;

    for(int i=0; i < width; ++i)
    for(int j=0; j < height; ++j)
    {
        int curr = array[i][j];

        while (curr < 0) curr += 8;

        curr = curr % 2;

        array[i][j] = curr;
    };

    return 0;
};
*/
   /*
int stern_attack::gauss_elim_field2(int **array, int width, int height)
{
    int * piv_points = new int [height];
    for(int i = 0; i < height; ++i) piv_points[i] = -1;

    array_settle_down(array, width, height, 2); // приводим числа в поле F_2

    for(int currcolumn = 0; currcolumn < height; ++currcolumn)
    {
        //поиск единички в столбце
        int row = 0; int point = -1;
        while((row < height) and (point == -1))
        {
            int point = array[row][currcolumn];
            if ((point == 1) and (piv_points[row] != -1))
                piv_points[row] = currcolumn;
            else
            {
                point = -1;
                row += 1;
            };

        };//endwhile row

        //теперь единичкой зануляем верхние и нижние строки




    };//endfor currcolumn



    return 0;
};
*/
