#ifndef QMTABLEVIEW_H
#define QMTABLEVIEW_H

#include <QTableView>
#include <QKeyEvent>

class QMTableView:public QTableView{

    Q_OBJECT

    QMTableView(QWidget* parent=nullptr):QTableView(parent){}

protected:
       void keyPressEvent(QKeyEvent *e)
       {
          if(e->key()==Qt::Key_Enter)
          {
             emit enterPressed(this->currentRow(),this->currentColumn());
          }
          else { QTableView::keyPressEvent(e); }
       }
private:
    int currentRow();
    int currentColumn();

signals:
       void enterPressed(int r, int c);
};
#endif // QMTABLEVIEW_H

int QMTableView::currentRow()
{
   QModelIndexList indexList =  selectionModel()->selectedIndexes();
   return indexList.at(0).row();

}

int QMTableView::currentColumn()
{
    QModelIndexList indexList =  selectionModel()->selectedIndexes();
    return indexList.at(0).column();
}
