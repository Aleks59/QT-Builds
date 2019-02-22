#include "widget.h"
#include <QApplication>
#include "field.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    Field Field1;
    w.show();
    QObject::connect(&w,SIGNAL(Settings(int,int,int)),&Field1,SLOT(Create(int,int,int)));
    QObject::connect(&Field1,SIGNAL(FieldCreated(int**,int,int)),&w,SLOT(DrawField(int**,int,int)));
    QObject::connect(&w,SIGNAL(CellClicked(int,int)),&Field1,SLOT(ChangeCell(int,int)));
    QObject::connect(&w,SIGNAL(Timer()),&Field1,SLOT(Timer_on()));
    QObject::connect(&w,SIGNAL(StepForward()),&Field1,SLOT(Step()));
    return a.exec();
}
