#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include "widget.h"

class Field : public QObject
{
    Q_OBJECT
public:
    explicit Field(QObject *parent = 0);
    int rows, cols, cellcount;
    int **Cells;
    int **Counter;
    void GetNeighbors(int row, int col);
    bool IsEmpty();
signals:
    void FieldCreated(int**,int,int); //map, rows, cols

public slots:
   void Create(int,int,int); //rows, cols, cellcount
   void Step();
   void ChangeCell(int,int);//row,col
   void Timer_on();

};

#endif // FIELD_H
