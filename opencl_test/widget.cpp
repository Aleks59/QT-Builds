#include "widget.h"
#include "ui_widget.h"
#include <QTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(mykernel.ROW_SIZE+1);
    ui->tableWidget->setColumnCount(mykernel.COL_SIZE);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QTime calc_time = QTime::currentTime();
    int **Ar1,**Ar2;

    Ar1 = new int*[mykernel.ROW_SIZE];
    Ar2 = new int*[mykernel.ROW_SIZE];

    for(int i=0;i<mykernel.ROW_SIZE;i++)
    {        
        Ar1[i] = new int[mykernel.COL_SIZE];
        Ar2[i] = new int[mykernel.COL_SIZE];

        for(int j = 0;j<mykernel.COL_SIZE;j++)
        {
            Ar1[i][j] = i;
            Ar2[i][j] = 2*i;
        }
    }
    mykernel.GetParam(Ar1,Ar2);

    bool r;
    r=mykernel.Initialize("vector.cl","vector_add");
    if(r==1)
    {
        mykernel.Start();

        for(int i =0;i<mykernel.ROW_SIZE;i++)
        {
            for(int j=0;j<mykernel.COL_SIZE;j++)
            {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(mykernel.C[i][j]));
            ui->tableWidget->setItem(i,j,item);
            }
        }

        mykernel.EndWork();
    }
    delete Ar1;
    delete Ar2;
    ui->label->setText(QString::number(calc_time.elapsed()));
}
