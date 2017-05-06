#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stern_attack_view.h"

#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    codematrix = nullptr;

    ui->label_path->setText("filepath is not specified yet");
    ui->label_dimension->setText("Dimension k");
    ui->label_length->setText("Code lenght n");
    ui->label_weight->setText("Weight w");

    ui->label_parameterP->setText("Parameter p");
    ui->label_parameterL->setText("Parameter l");

    ui->label_description->setText("Codematrix H have dimensions n-k x n");
    ui->label_loaderror->setText("");

    ui->plainTextEdit_matrix->setReadOnly(true);
    ui->label_answer->setText("Matrix is not loaded yet");


// CONNECT TO BUTTONS
    connect(ui->pushButton_select, SIGNAL(pressed()), this, SLOT (handlebutton_select()));
    connect(ui->pushButton_load, SIGNAL(pressed()), this, SLOT (handlebutton_load()));


// STERN ATTACK

    connect(ui->pushButton_proceed, SIGNAL(pressed()), this, SLOT(handlebutton_proceed()));


};

void MainWindow::handlebutton_select(){
    QString str;
    str=QFileDialog::getOpenFileName(0, "Select File", "..//", "*.txt");

    if (!str.isEmpty())
    {
        filepath=str;
        ui->label_path->setText(filepath);
    };
    //else do nothing//
};

void MainWindow::handlebutton_load(){

    if (!filepath.isEmpty())
    {
        ui->plainTextEdit_matrix->clear();

        matrix_is_loaded = load_matrix();

        ui->label_loaderror->setText("Err: "+QString::number(matrix_is_loaded));

        if (matrix_is_loaded == 0)
        {
            QString str;

            for (int i = 0; i < n_k; i++)
            {
                str = "";

                for (int j = 0; j < length_n; j++)
                    str += QString::number(codematrix[i][j])+" ";

                ui->plainTextEdit_matrix->appendPlainText(str);
            };// end for
        }; // else do nothing //
    };
    // else do nothing //

};

int MainWindow::load_matrix(){

    if (codematrix == nullptr)
    {
        dimension_k = ui->spinBox_dimension->value();
        length_n = ui->spinBox_length->value();
        weight_w = ui->spinBox_weight->value();
        n_k = length_n - dimension_k;

        if (n_k < 1)
        {
            ui->label_answer->setText("Incorrect parameters: n-k < 1");
            return 2;
        };

        codematrix = new int*[n_k];

        for(int i = 0; i < n_k; i++)
            codematrix[i] = new int[length_n];
    }
    else
    {
        for(int i = 0; i < n_k; i++)
          delete [] codematrix[i];
        delete [] codematrix;

        dimension_k = ui->spinBox_dimension->value();
        length_n = ui->spinBox_length->value();
        weight_w = ui->spinBox_weight->value();
        n_k = length_n - dimension_k;

        if (n_k < 1)
        {
            ui->label_answer->setText("Incorrect parameters: n-k < 1");
            ui->plainTextEdit_matrix->clear();
            return 2;
        };

        codematrix = new int*[n_k];

        for(int i = 0; i < n_k; ++i)
            codematrix[i] = new int[length_n];

    }; //endif

    QFile file(filepath);
    file.open(QIODevice::ReadOnly);

    QTextStream fileStream(&file);
    QString str;

    int i = 0;
    int j = 0;

    str = fileStream.readLine();

    while (!str.isNull() and (i < n_k) and (j < length_n)) {

        QTextStream strStream(&str);

        while((!strStream.atEnd()) and (i < n_k) and (j < length_n)){
            int number;
            strStream >> number;
            number = number % 2;

            codematrix[i][j] = number;

            j += 1;

            if (j == length_n)
            {
                i += 1;
                j = 0;

            }; //endif

        }; //endwhile

        str = fileStream.readLine();
    }; //endforwhile


    file.close();

    if (i != n_k)
    {
        ui->label_answer->setText("There isn't enough numbers");
        return 1;
    };

    ui->label_answer->setText("Matrix is loaded");
    return 0;
};

MainWindow::~MainWindow()
{
    if (codematrix != nullptr)
    {
    for(int i=0; i < n_k; ++i)
        delete [] codematrix[i];

        delete [] codematrix;
    };

    delete ui;
};

void MainWindow::handlebutton_proceed()
{
    if (matrix_is_loaded == 0)
    {
        strn_atck  = new stern_attack_view();

        p = ui->spinBox_parameterP->value();
        l = ui->spinBox_parameterL->value();

        strn_atck->set_params(codematrix, length_n, dimension_k, weight_w, p, l);
        strn_atck->show();
    }; //else do nothing

};
