#include "widget.h"
#include "ui_widget.h"
#include "field.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer1 = new QTimer();
    connect(timer1,SIGNAL(timeout()),this,SLOT(on_pushButton_2_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int f_width = ui->tableWidget->width()-ui->tableWidget->verticalHeader()->width();
    int f_heigth = ui->tableWidget->height()-ui->tableWidget->horizontalHeader()->height();
    int rows = ui->tableWidget->rowCount();
    int cols = ui->tableWidget->columnCount();

    for(int i=0;i<cols;i++)
        ui->tableWidget->setColumnWidth(i,(f_width/cols));
    for (int i=0;i<rows;i++)
        ui->tableWidget->setRowHeight(i,(f_heigth/rows));
}

void Widget::DrawField(int** map,int rows,int cols)
{
    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(cols);
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
        {
            QTableWidgetItem* newItem = new QTableWidgetItem(tr(""));
            if(map[i][j] == 1 )
                newItem->setBackgroundColor(QColor(10,10,10));
            if(map[i][j] == 0)
                newItem->setBackgroundColor(QColor(255,255,255));
            ui->tableWidget->setItem(i,j,newItem);
        }
}

void Widget::on_pushButton_clicked()
{
    emit Settings(ui->spinBox->value(),ui->spinBox_2->value(),ui->spinBox_3->value());

    int f_width = ui->tableWidget->width()-ui->tableWidget->verticalHeader()->width();
    int f_heigth = ui->tableWidget->height()-ui->tableWidget->horizontalHeader()->height();
    int rows = ui->tableWidget->rowCount();
    int cols = ui->tableWidget->columnCount();

    for(int i=0;i<cols;i++)
        ui->tableWidget->setColumnWidth(i,(f_width/cols));
    for (int i=0;i<rows;i++)
        ui->tableWidget->setRowHeight(i,(f_heigth/rows));
}

void Widget::on_tableWidget_cellClicked(int row, int column)
{
    emit CellClicked(row,column);
}

void Widget::on_pushButton_2_clicked()
{
    emit StepForward();
}

void Widget::on_pushButton_3_clicked()
{
    int timestep = ui->spinBox_4->value();
    if(timerstart == 0)
    {
        emit Timer();
        ui->pushButton_3->setText("Stop");
        timerstart=1;
        timer1->start(timestep);
    }
    else
    {
        ui->pushButton_3->setText("Start");
        timerstart=0;
        timer1->stop();
    }
}
