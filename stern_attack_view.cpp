#include "stern_attack_view.h"
#include "ui_stern_attack_view.h"
#include "QTime"
#include "qmath.h"

stern_attack_view::stern_attack_view(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stern_attack_view)
{
    ui->setupUi(this);

    this->setWindowTitle("Time to attack");

    ui->label_state->setText("Attack status");
    ui->label_step1->setText("STEP 1");
    ui->label_step2->setText("STEP 2");
    ui->label_step3->setText("STEP 3");
    ui->label_step4->setText("STEP 4");
    ui->label_step5->setText("STEP 5");
    ui->label_step6->setText("RESULT");

// connect buttons
    connect(ui->pushButton_tryagain, SIGNAL(pressed()), this, SLOT(handle_try_again()));
    connect(ui->pushButton_saveresult, SIGNAL(pressed()), this, SLOT(handle_saveresult()));
    connect(ui->pushButton_quit, SIGNAL(pressed()), this, SLOT(handle_quit()));

}

stern_attack_view::~stern_attack_view()
{
    delete ui;
}

void stern_attack_view::set_params(int **_codematrix, int _len, int _dim, int _w, int _p, int _l)
{
    // preparations
        QTime midnight(0,0,0);
        qsrand(midnight.secsTo(QTime::currentTime()));

    // parameters
    codematrix = _codematrix;

    length_n = _len;
    dimension_k = _dim;
    weight_w = _w;
    n_k = length_n - dimension_k; //здесь была ошибка в коде, отнявшая два часа
    codematrix_copy = arraycopy(codematrix);

    p = _p;
    l = _l;

    // window title
    QString title = "Length n = " + QString::number(length_n);
    title += ", Dimension k = " + QString::number(dimension_k);
    title += ", Weight w = " + QString::number(weight_w);
    title += ", Param p = " + QString::number(p);
    title += ", Param l = " + QString::number(l);

    this->setWindowTitle(title);
};


void stern_attack_view::handle_try_again()
{
    for(int i = 0; i < n_k; i++)
        for(int j = 0; j < length_n; j++)
        codematrix[i][j] = codematrix_copy[i][j];

    ui->plainTextEdit_state->appendPlainText("New attack is started");

    int result = step1();
    step1_view();
    ui->plainTextEdit_state->appendPlainText("Step 1 is completed");
    ui->plainTextEdit_state->appendPlainText("Step 1 err :" + QString::number(result));

    if (result == 0)
    {
         result = step2();
         step2_view();
         ui->plainTextEdit_state->appendPlainText("Step 2 err :" + QString::number(result));
    };

    if (result == 0)
    {
        result = step3();
        step3_view();
        ui->plainTextEdit_state->appendPlainText("Step 3 err :" + QString::number(result));
    };

    if (result == 0)
    {
        result = step4();
        step4_view();
        ui->plainTextEdit_state->appendPlainText("Step 4 err :" + QString::number(result));
    };

    if (result == 0)
    {
        result = step5();
        step5_view();
        ui->plainTextEdit_state->appendPlainText("Step 5 err :" + QString::number(result));
    };

    if (result == 0)
    {
        result = step6();
        step6_view();
        ui->plainTextEdit_state->appendPlainText("Step 6 err :" + QString::number(result));
    };

};

void stern_attack_view::handle_saveresult() // try 100 times
{
    for (int i = 0; i < 1000; i++)
    {
        handle_try_again();
        if (candidate) break;
    };

};

void stern_attack_view::handle_quit(){};

int ** stern_attack_view::arraycopy(int **array)
{
    int **copy = new int*[n_k];

    for(int i = 0; i < n_k; i++)
        copy[i] = new int[length_n]; //здесь была другая ошибка, отнявшая час

    for(int i = 0; i < n_k; i++)
        for(int j = 0; j < length_n; j++)
        copy[i][j] = array[i][j];

    return copy;
};

int stern_attack_view::step1()
{
    // modified gaussian elimination
        chosencolumns = new int[length_n]();
        int* pivots = new int[n_k]();

        int num_columns_processed = 0;
        int col_num = 0;

        while(num_columns_processed < n_k)
        {
            col_num = select_column(chosencolumns);
            if (col_num == -1) return 1;

            int i = -1;
            int exit = false;

            while(!exit)
            {
                i += 1;

                if (pivots[i] == 0 and codematrix[i][col_num] == 1) // здесь была эпичная ошибка
                {
//                    elimination(codematrix, n_k, length_n, col_num, i);
                    elimination(codematrix, n_k, length_n, i, col_num);
                    pivots[i] = 1;
                    num_columns_processed += 1;
                    exit = true;
                }; //endif

                if (i == n_k)
                {
                    exit = true;
                    chosencolumns[col_num] = 2;
                };

            }; //endwhile
        }; //endwhile

    return 0;
};

int stern_attack_view::step2()
{
    arrayX = new int[length_n](); //тут были мелкие ошибки
    arrayY = new int[length_n]();

    numofX = 0;
    numofY = 0;

    int coin;

    for(int i = 0; i < length_n; i++)
    {
        //если еще не выбрана для Гаусса
        if (chosencolumns[i] != 1) //и тут была мелкая ошибка
        {
            coin = qrand() % 2;

            if (coin == 0)
            {
                arrayX[i] = 1;
                numofX += 1;
            }
            else
            {
                arrayY[i] = 1;
                numofY += 1;
            };// end if coin
        };// end if
    };

    if (numofX < p) return 1;
    if (numofY < p) return 1;

    return 0;
};

void stern_attack_view::step1_view()
{
    QString str = "";
    QString line;

    for (int i = 0; i < length_n; i++)
    {
        str += QString::number(chosencolumns[i]) + " ";
        line += "--";
    };

    ui->plainTextEdit_step1->appendPlainText("");
    ui->plainTextEdit_step1->appendPlainText(str);
    ui->plainTextEdit_step1->appendPlainText(line);

    for (int i = 0; i < n_k; i++)
    {
        str = "";

        for (int j = 0; j < length_n; j++)
                    str += QString::number(codematrix[i][j])+" ";

        ui->plainTextEdit_step1->appendPlainText(str);
    };// end for*/
};

void stern_attack_view::step2_view()
{
    QString str = "";

    for (int i = 0; i < length_n; i++)
    {
        if (arrayX[i] == 1)
            str += "x ";

        if (arrayY[i] == 1)
            str += "y ";

        if (chosencolumns[i] == 1)
            str += "+";
    };

    ui->plainTextEdit_step2->appendPlainText(str);

    str = "";

    for (int i = 0; i < n_k; i++)
    {
        str = "";

        for (int j = 0; j < length_n; j++)
                    str += QString::number(codematrix[i][j])+" ";

        ui->plainTextEdit_step2->appendPlainText(str);
    };

    ui->plainTextEdit_step2->appendPlainText("");
};

int stern_attack_view::step3()
{
    arrayJ = new int[n_k]();

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

    return 0;
};

void stern_attack_view::step3_view()
{
    for(int i = 0; i < n_k; i++)
    {
        if (arrayJ[i] == 0)
        ui->plainTextEdit_step3->appendPlainText(" |");
        else
        ui->plainTextEdit_step3->appendPlainText("j|");
    };

    ui->plainTextEdit_step3->appendPlainText("");
};

int stern_attack_view::select_column(int *selected)
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

int stern_attack_view::elimination(int **matrix, int rows, int cols, int piv_row, int piv_col)
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

int stern_attack_view::step4()
{
    // num of subsets (and rows) in A and B
        subsA = c_n_k(numofX, p);
        subsB = c_n_k(numofY, p);

        if (subsA == -1) return 4;
        if (subsB == -1) return 4;

    // init the matrices A from X
        pi_A = new int*[subsA];
        guide_values_A = new int*[subsA];

        for(int i = 0; i < subsA; i++)
        {
            pi_A[i] = new int[n_k]();
            guide_values_A[i] = new int[numofX]();
        };

    // init the matrices B from Y
        pi_B = new int*[subsB];
        guide_values_B = new int*[subsB];

        for(int i = 0; i < subsB; i++)
        {
            pi_B[i] = new int[n_k]();
            guide_values_B[i] = new int[numofY]();
        };

    // time to fill matrices!
        generate_guides();

    // go!
        int sum = 0;
        int index;
        int *guiderow;

        // evaluating pi_A
        for(int i = 0; i < subsA; i++) // перебор по векторам
        {
            index = 0;
            guiderow = new int[length_n]();

            for(int k = 0; k < length_n; k++)
            {
                if (arrayX[k] == 1)
                {
                    guiderow[k] = guide_values_A[i][index];
                    index += 1;
                };
            };

            for(int j = 0; j < n_k; j++) // перебор по элементам конкретного вектора
            {
                if (arrayJ[j] != 0) // если строка выбрана во множество J
                {
                    sum = 0;

                    for(int l = 0; l < length_n; l++)
                        sum += guiderow[l]*codematrix[j][l];

                    sum = sum % 2;

                    pi_A[i][j] = sum;

                }; //endif

            }; //end for j

            delete guiderow;
        }; //end for i

        // evaluating pi_B
        for(int i = 0; i < subsB; i++) // перебор по векторам
        {
            index = 0;
            guiderow = new int[length_n]();

            for(int k = 0; k < length_n; k++)
            {
                if (arrayY[k] == 1)
                {
                    guiderow[k] = guide_values_B[i][index];
                    index += 1;
                };
            };

            for(int j = 0; j < n_k; j++) // перебор по элементам конкретного вектора
            {
                if (arrayJ[j] != 0) // если строка выбрана во множество J
                {
                    sum = 0;

                    for(int l = 0; l < length_n; l++)
                        sum += guiderow[l]*codematrix[j][l];

                    sum = sum % 2;

                    pi_B[i][j] = sum;

                }; //endif

            }; //end for j

            delete guiderow;
        }; //end for i



     return 0;
};

int stern_attack_view::step5()
{
    num_of_pairs = 0;
    pairs = new int*[subsA*subsB];


    for(int i = 0; i < subsA; i++)
        for(int j = 0; j < subsB; j++)
        {
            if (vectors_are_equal(pi_A[i], pi_B[j], n_k))
            {
                pairs[num_of_pairs] = new int[2];
                pairs[num_of_pairs][0] = i;
                pairs[num_of_pairs][1] = j;
                num_of_pairs += 1;
            };
        };

    //test candidate vectors
    vector = new int[n_k];
    int sum;
    int index;
    int pair_index;
    int *guiderowA;
    int *guiderowB;
    int candidate_weight;

    for(int i = 0; i < num_of_pairs; i++)
    {
        // evaluate guide row A
        index = 0;
        pair_index = pairs[i][0];

        guiderowA = new int[length_n]();

        for(int k = 0; k < length_n; k++)
        {
            if (arrayX[k] == 1)
            {
                guiderowA[k] = guide_values_A[pair_index][index];
                index += 1;
            };
        };

        // evaluate guide row B
        index = 0;
        pair_index = pairs[i][1];

        guiderowB = new int[length_n]();

        for(int k = 0; k < length_n; k++)
        {
            if (arrayY[k] == 1)
            {
                guiderowB[k] = guide_values_B[pair_index][index];
                index += 1;
            };
        };

        // evaluate vector
        for(int k = 0; k < n_k; k++) //цикл по элементам вектора
        {
            sum = 0;

            for(int s = 0; s < length_n; s++) //цикл по суммируемым элементам
            {
                if(guiderowA[s] != 0 or guiderowB[s] != 0)
                    sum += codematrix[k][i];
            };

            vector[k] = sum % 2;
        }; // end for k

        candidate_weight = weight_of_vector(vector, n_k);

        if (candidate_weight == (weight_w - 2*p))
        {
            candidate = true;
            candidate_index = i;
            return 0;
        }

    };

    if(num_of_pairs == 0) return 5;
    return 6;
};

void stern_attack_view::step5_view()
{
    QString str;

    str = "There is " + QString::number(num_of_pairs) + " pairs";
    ui->plainTextEdit_step5->appendPlainText(str);
    ui->plainTextEdit_step5->appendPlainText("List of pairs");
    ui->plainTextEdit_step5->appendPlainText("A B");

    for(int i = 0; i < num_of_pairs; i++)
    {
        str = QString::number(pairs[i][0]) + " " + QString::number(pairs[i][1]);

        ui->plainTextEdit_step5->appendPlainText(str);
    };

    ui->plainTextEdit_step5->appendPlainText("");

    if (candidate)
    {
        str = "Candidate parents are A#" + QString::number(pairs[candidate_index][0]);
        str += " and B#" + QString::number(pairs[candidate_index][1]);
        ui->plainTextEdit_step5->appendPlainText(str);

        ui->plainTextEdit_step5->appendPlainText("");
        ui->plainTextEdit_step5->appendPlainText("Here is a candidate");

        str = "";

        for(int i = 0; i < n_k; i++)
            str += QString::number(vector[i]) + " ";


        ui->plainTextEdit_step5->appendPlainText(str);

    }
    else
        ui->plainTextEdit_step5->appendPlainText("There is no candidate");


};

void stern_attack_view::step4_view()
{
    QString str;

    str = "Subs A: " + QString::number(subsA) + " Subs B: " + QString::number(subsB);
    ui->plainTextEdit_step4->appendPlainText(str);

    ui->plainTextEdit_step4->appendPlainText("guides A");

    for(int i = 0; i < subsA; i++)
    {
        str = "";

        for (int j = 0; j < numofX; j++)
                    str += QString::number(guide_values_A[i][j]);

        str += " | ";

        for (int j = 0; j < n_k; j++)
                    str += QString::number(pi_A[i][j]);

        ui->plainTextEdit_step4->appendPlainText(str);

    }; // it was guides A

    ui->plainTextEdit_step4->appendPlainText("guides B");

    for(int i = 0; i < subsB; i++)
    {
        str = "";

        for (int j = 0; j < numofY; j++)
                    str += QString::number(guide_values_B[i][j]);

        str += " | ";

        for (int j = 0; j < n_k; j++)
                    str += QString::number(pi_B[i][j]);

        ui->plainTextEdit_step4->appendPlainText(str);

    }; // it was guides B


    ui->plainTextEdit_step4->appendPlainText("");
};

int stern_attack_view::c_n_k(int n, int k)
{
    if (k == 0) return 1;
    if (k == 1) return n;
    if (k == n) return 1;

    if (k > n) return -1;
    if (k < 0) return -1;
    if (n < 0) return -1;

    int res1 = 1;
    int res2 = 1;
    int n_k = n-k;

    for(int i = (n_k+1); i <= n; i++)
        res1 *= i;

    for(int i = 2; i <= k; i++)
        res2 *= i;

    int res = res1/res2;

    return res;
};

int stern_attack_view::generate_guides()
{
    int curr = qPow(2, p) - 1;
    int count = 0;
    bool next_is_finded = false;

    for(int i = 0; i < subsA; i++)
    {
        next_is_finded = false;

        for(int j = 0; j < numofX; j++)
            guide_values_A[i][j] = (curr >> j) % 2;

        while(!next_is_finded)
        {
            curr += 1;
            count = 0;

            for(int k = 0; k < numofX; k++)
                count += (curr >> k) % 2;

            if (count == p) next_is_finded = true;
        };
    }; // guide A created

    curr = qPow(2, p) - 1;
    count = 0;
    next_is_finded = false;

    for(int i = 0; i < subsB; i++)
    {
        next_is_finded = false;

        for(int j = 0; j < numofY; j++)
            guide_values_B[i][j] = (curr >> j) % 2;

        while(!next_is_finded)
        {
            curr += 1;
            count = 0;

            for(int k = 0; k < numofY; k++)
                count += (curr >> k) % 2;

            if (count == p) next_is_finded = true;
        };
    }; // guide B created

    return 0;
};


bool stern_attack_view::vectors_are_equal(int *vec1, int* vec2, int len)
{
    for(int i = 0; i < len; i++)
        if (vec1[i] != vec2[i])
        return false;

    return true;
};

int stern_attack_view::weight_of_vector(int *vec, int len)
{
    int s = 0;

    for(int i = 0; i < len; i++)
        if(vec[i] != 0) s += 1;

    return s;
};

int stern_attack_view::step6()
{
    x = new int[length_n]();
    y = new int[length_n]();
    z = new int[length_n]();

    int index;
    int pair_index;
    int *guiderowA;
    int *guiderowB;

//-----------------------------------------------------
// evaluate guide row A
        index = 0;
        pair_index = pairs[candidate_index][0];

        guiderowA = new int[length_n]();

        for(int k = 0; k < length_n; k++)
        {
            if (arrayX[k] == 1)
            {
                guiderowA[k] = guide_values_A[pair_index][index];
                index += 1;
            };
        };

// evaluate guide row B
        index = 0;
        pair_index = pairs[candidate_index][1];

        guiderowB = new int[length_n]();

        for(int k = 0; k < length_n; k++)
        {
            if (arrayY[k] == 1)
            {
                guiderowB[k] = guide_values_B[pair_index][index];
                index += 1;
            };
        };
//-----------------------------------------------------

    for(int i = 0; i < length_n; i++)
    {
        if(guiderowA[i] != 0 or guiderowB != 0)
            y[i] = 1;
    };

    for(int i = 0; i < n_k; i++)
    {
        if (vector[i] != 0)
            for(int j = 0; j < length_n; j++)
            {
                if(chosencolumns[j] == 1 and codematrix[i][j] == 1)
                z[i] = 1;
            };
    };

    for(int i = 0; i < length_n; i++)
    {
        x[i] = (y[i] + z[i]) % 2;
    };

    return 0;
};

void stern_attack_view::step6_view(){
    QString str;
    ui->plainTextEdit_step5->appendPlainText("Vectors x, y, z");

    for(int i = 0; i < length_n; i++)
    {
        str = QString::number(x[i]) + " | ";
        str += QString::number(y[i]) + " | ";
        str += QString::number(z[i]);

        ui->plainTextEdit_step6->appendPlainText(str);
    };


    ui->plainTextEdit_step5->appendPlainText("");
};

//----------------------------------------------------------------
/*       for(int i = 0; i < n_k; i++)
       {
           if (pivots[i] == 0 and codematrix[i][col_num] == 1)
           {
              // elimination(codematrix, n_k, length_n, col_num, i);
               pivots[i] = 1;
               num_columns_processed += 1;
               break;
           }; //endif
       }; //endfor*/
//----------------------------------------------------------------
/*   QString strX, strY, chsn;


   for (int i = 0; i < length_n; i++)
   {
       if (arrayX[i] == 1)
           strX += "+ ";
       else
           strX += "  ";

       if (arrayY[i] == 1)
           strY += "+ ";
       else
           strY += "  ";

       if (chosencolumns[i] == 1)
           chsn += "+ ";
       else
           chsn += "  ";

   ui->plainTextEdit_step2->appendPlainText("X set");
   ui->plainTextEdit_step2->appendPlainText(strX);
   ui->plainTextEdit_step2->appendPlainText("Y set");
   ui->plainTextEdit_step2->appendPlainText(strY);
   ui->plainTextEdit_step2->appendPlainText("Chosen columns");
   ui->plainTextEdit_step2->appendPlainText(chsn);

   };*/
//-----------------------------------------------------
/*      int min_combo = qPow(2, p) - 1;
        int max_combo_A = qPow(2, arrayX[0]) - qPow(2, p);
        int max_combo_B = qPow(2, arrayY[0]) - qPow(2, p);
*/
