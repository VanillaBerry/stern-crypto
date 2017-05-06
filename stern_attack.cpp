#include "stern_attack.h"
#include "QTime"
#include "qmath.h"

stern_attack::stern_attack(int **_codematrix, int _len, int _dim, int _w, int _p, int _l)
{
    codematrix = _codematrix;
    length_n = _len;
    dimension_k = _dim;
    weight_w = _w;
    n_k = length_n - dimension_k;

    p = _p;
    l = _l;
};

stern_attack::~stern_attack()
{
  /*  if (codematrix != NULL)
    {
        //memory freeing
        for(int i = 0; i < n_k; ++i)
        delete [] codematrix[i];

        delete [] codematrix;
    };*/
};

int stern_attack::proceed()
{
// preparations

    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

// step 1
// modified gaussian elimination

    chosencolumns = new int[length_n];
    int* pivots = new int[n_k];

    int num_columns_processed = 0;
    int col_num;


    while(num_columns_processed < n_k)
    {
        col_num = select_column(chosencolumns);

        int i = 0;
        int exit = false;

        while(!exit)
        {
            if (pivots[i] == 0 and codematrix[col_num][i] == 1)
            {
                elimination(codematrix, n_k, length_n, col_num, i);
                pivots[i] = 1;
                num_columns_processed += 1;
                i = 1;
            }; //endif

            i += 1;

            if (i == n_k) return 1;
        };

        for(int i = 0; i < n_k; i++)
        {
            if (pivots[i] == 0 and codematrix[col_num][i] == 1)
            {
               // elimination(codematrix, n_k, length_n, col_num, i);
                pivots[i] = 1;
                num_columns_processed += 1;
                break;
            }; //endif
        }; //endfor*/
    }; //endwhile*/

    emit completed_step1();

// step 2
// spare columns to the two sets
    int * arrayX = new int[dimension_k]();
    int * arrayY = new int[dimension_k]();

    int coin;
    int index;

    for(int i = 0; i < length_n; i++)
    {
        //если еще не выбрана для Гаусса
        if (chosencolumns[i] == 0)
        {
            coin = qrand() % 2;

            if (coin)
            {
                index = arrayX[0];
                arrayX[index+1] = i;
                arrayX[0] += 1;
            }
            else
            {
                index = arrayY[0];
                arrayY[index+1] = i;
                arrayY[0] += 1;
            };// end if coin
        };// end if
    };

    if (arrayX[0] < p) return 1;
    if (arrayY[0] < p) return 1;

// step 3
// randomly choose a set J
    // будет сгенерировано подмножество из l элементов

    int* arrayJ = new int[n_k];
    int uptol = 0;
    int dice;

    while(uptol < l)
    {
        dice = qrand() % n_k;

        if (arrayJ[dice] == 0)
        {
            arrayJ[dice] = 1;
            uptol += 1;
        };
    };

// step 4
// generation of X and Y subs
    int ** pi_A;
    int ** pi_B;

    int ** guide_values_A;
    int ** guide_values_B;

// num of subsets (and rows) in A and B
    int subsA = c_n_k(arrayX[0], p);
    int subsB = c_n_k(arrayY[0], p);

    int min_combo = qPow(2, p) - 1;
    int max_combo_A = qPow(2, arrayX[0]) - qPow(2, p);
    int max_combo_B = qPow(2, arrayY[0]) - qPow(2, p);

// init the matrices
 /*   pi_A = new int*[subsA];
    guide_values_A = new int*[subsA];

    pi_B = new int*[subsB];
    guide_values_B = new int*[subsB];

    for(int i = 0; i < subsA; i++)
    {
        pi_A[i] = new int[n_k];
        guide_values_A = new int[dimension_k];
    };

    for(int i = 0; i < subsB; i++)
    {
        pi_B[i] = new int[n_k];
        guide_values_B = new int[dimension_k];
    };*/

// time to fill matrices!


// some preparations

/*    guide_values_A[0] = min_combo;

    for(int i = 1; i < subsA; i++)
        guide_values_A[i] = generate_next_p_bits(guide_values_A[i-1]);

    guide_values_B[0] = min_combo;

    for(int i = 1; i < subsB; i++)
        guide_values_B[i] = generate_next_p_bits(guide_values_B[i-1]);

// go!
    int bit;

    for(int i = 1; i < subsA; i++)
    {
        for(int j = 0; j >= n_k; j--)
        {
            bit = guide_values_A[i] <<*/



/*        };


    };*/





// freeing memory and returning result

    //delete [] chosencolumns; // see step 1
   // delete [] pivots; // step 1

 /*   delete [] arrayX; // step 2
    delete [] arrayY; // step 2

    delete [] arrayJ; // step 3

    for(int i = 0; i < subsA; i++)  // step 4
        delete [] pi_A[i];

    for(int i = 0; i < subsB; i++)  // step 4
        delete [] pi_B[i];

    delete [] pi_A; // step 4
    delete [] pi_B; // step 4*/


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

int stern_attack::elimination(int **matrix, int rows, int cols, int piv_row, int piv_col)
{
    int value, newvalue;

    for(int i = 0; i < rows; i++)
    {
        if (i != piv_row and matrix[i][piv_col] != 0)
            for(int j = 0; j < cols; j++)
            {
                value = matrix[i][j];
                newvalue = (value + matrix[piv_row][j]) % 2;
                matrix[i][j] = newvalue;
            };
    };

    return 0;
};

int stern_attack::c_n_k(int n, int k)
{
    int res = 1;
    int n_k = n-k;

    int n_k_fact = 1;

    for(int i = 2; i <= n; i++)
    {
        res *= i;
        if (i <= k) res = res/i;
        if (i <= n_k) n_k_fact *= i;
    };

    res = res/n_k_fact;

    return res;
};

int stern_attack::generate_next_p_bits(int curr, int p)
{
    int newvalue = curr;
    int count;

    for(int i = 1; i < 4000000; i++)
    {
        newvalue = curr + i;
        count = 0;

        for(int j = 0; j < 40; j++)
            count += (newvalue >> j) % 2;

        if (count == p) break;
    };

    return newvalue;
};
