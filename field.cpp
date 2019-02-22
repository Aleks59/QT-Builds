#include "field.h"

Field::Field(QObject *parent) : QObject(parent)
{

}

void Field::GetNeighbors(int row, int col)
{
    int Part[8];
    //0 1 2
    //7 X 3
    //6 5 4
    for(int i=0;i<8;i++)
        Part[i] = 0;
    if(row!=0&&col!=0&&Cells[row-1][col-1]==1) Part[0]=1;
    if(row!=0&&col!=(cols-1)&&Cells[row-1][col+1]==1) Part[2]=1;
    if(row!=(rows-1)&&col!=(cols-1)&&Cells[row+1][col+1]==1) Part[4]=1;
    if(row!=(rows-1)&&col!=0&&Cells[row+1][col-1]==1) Part[6]=1;

    if(row!=0&&Cells[row-1][col]==1) Part[1]=1;
    if(col!=(cols-1)&&Cells[row][col+1]==1) Part[3]=1;
    if(row!=(rows-1)&&Cells[row+1][col]==1) Part[5]=1;
    if(col!=0&&Cells[row][col-1]==1) Part[7]=1;

    if(row!=0||col!=0||row!=(rows-1)||cols!=(cols-1))
    {
    Counter[row][col] = 0;
    for (int i=0;i<8;i++)
        Counter[row][col]+=Part[i];
    }
}

bool Field::IsEmpty()
{
    for (int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            if(Cells[i][j]!=0) return false;
    else return true;
}

void Field::Create(int rows, int cols, int cellcount)
{
    this->rows=rows;
    this->cols = cols;
    if (cellcount>rows*cols)
        this->cellcount = rows*cols;
    else this->cellcount = cellcount;
    //allocate memory for dynamics arrays Cells and Counter (live cells and their neighbors)
    this->Cells = new int* [(this->rows)];
    this->Counter = new int*[this->rows];
    for (int i=0;i<(this->rows);i++)
    {
        this->Cells[i] = new int[(this->cols)];
        this->Counter[i] = new int[this->cols];
    }
    //randomise cells
    for(int i=0;i<this->rows;i++)
        for(int j=0;j<this->cols;j++)
            Cells[i][j]=0;
    int k=0;    
    while (k<(this->cellcount))
    {
        if(this->Cells[rand()%(this->rows)][rand()%(this->cols)]!=1)
        {
            this->Cells[rand()%(this->rows)][rand()%(this->cols)] = 1;
            k++;
        }
    }
    //count neighbors
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            GetNeighbors(i,j);
    /*for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
            printf("%d ",Counter[i][j]);
        printf("\n");
    }*/
    emit FieldCreated(this->Cells,this->rows,this->cols);
}

void Field::Step()
{
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
        {
            /*if((Counter[i][j]==2)&&(Cells[i][j]==1))
                Cells[i][j]=1;
            else
            if(Counter[i][j]==3)
                Cells[i][j]=1;
            else Cells[i][j]=0;*/
            if(Counter[i][j]==3||((Counter[i][j]==2)&&(Cells[i][j]==1)))
                Cells[i][j]=1;
            else Cells[i][j]=0;
        }
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            GetNeighbors(i,j);
    emit FieldCreated(this->Cells,this->rows,this->cols);
}

void Field::ChangeCell(int r, int c)
{
   if(this->Cells[r][c]==1)
       this->Cells[r][c] =0;
   else
   if(this->Cells[r][c]==0)
       this->Cells[r][c] = 1;

   /*for(int i=0;i<rows;i++)
       for(int j=0;j<cols;j++)
           GetNeighbors(i,j);*/
   GetNeighbors(r,c);
   if(r>0&&c>0) GetNeighbors(r-1,c-1);
   if(r>0&&c<(cols-1)) GetNeighbors(r-1,c+1);
   if(r<(rows-1)&&c<(cols-1)) GetNeighbors(r+1,c+1);
   if(r<(rows-1)&&c>0) GetNeighbors(r+1,c-1);

   if(r>0)GetNeighbors(r-1,c);
   if(c<(cols-1))GetNeighbors(r,c+1);
   if(r<(rows-1))GetNeighbors(r+1,c);
   if(c>0)GetNeighbors(r,c-1);

   /*GetNeighbors(r-1,c-1);GetNeighbors(r-1,c);GetNeighbors(r-1,c+1);
   GetNeighbors(r,c-1);GetNeighbors(r,c);GetNeighbors(r,c+1);
   GetNeighbors(r+1,c-1);GetNeighbors(r+1,c);GetNeighbors(r+1,c+1);
   */

   emit FieldCreated(this->Cells,this->rows,this->cols);

}

void Field::Timer_on()
{
    Step();
}
