#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "field.h"
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    bool timerstart = 0;
private:
    Ui::Widget *ui;
    QTimer *timer1;
protected:
    void resizeEvent(QResizeEvent *event);
signals:
    void Settings(int,int,int); //rows,cols, cellcount
    void CellClicked(int,int); //row,col
    void StepForward();
    void Timer();
private slots:
    void on_pushButton_clicked();
    void DrawField(int**,int,int); //map array, rows, cols
    void on_tableWidget_cellClicked(int row, int column);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // WIDGET_H
