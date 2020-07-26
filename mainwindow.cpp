#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include "NotEditableDelegate.h"
#include "InitDB.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(
                    this,
                    "Unable to load database",
                    "This demo needs the SQLITE driver"
                    );

    // Initialize the database:
    QSqlError err = InitDB();
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }
    QSqlQuery q;

    if(!q.exec("select * from mtable")){
        showError(q.lastError());
        return;
    }
    /*
    while (q.next()) {
     qDebug()<<q.value(0).toString()<<q.value(1).toString()<<q.value(3).toString();
    }
    */
    //Set autoresizable columns
    ui->tvTables->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents /*|QHeaderView::Stretch*/ );
    ui->tvTableData->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents/* |QHeaderView::Stretch*/ );

    // Create the data model:
    m_table = new QSqlRelationalTableModel(ui->tvTables);
    m_table->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_table->setTable("mtable");


    m_tableData = new QSqlRelationalTableModel(ui->tvTableData);
    m_tableData->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_tableData->setTable("tabledata");

    // Remember the indexes of the columns:
    tableIdx = m_tableData->fieldIndex("tableid");
    selectedTableIdx= 1;
   //filter
    m_tableData->setFilter(QString("tableid=%0").arg(selectedTableIdx));
    // Set the relations to the other database tables:
    //m_tableData->setRelation(tableIdx, QSqlRelation("mtable", "id", "name"));
    //m_table->setHeaderData(authorIdx, Qt::Horizontal, tr("Author Name"));
    // Populate the model:
    if (!m_table->select()) {
        showError(m_table->lastError());
        return;
    }
    if (!m_tableData->select()) {
        showError(m_tableData->lastError());
        return;
    }
    // Set the model and hide the ID column:
    ui->tvTables->setModel(m_table);
    //ui.tvTables->setItemDelegate(new BookDelegate(ui.tvTables));
    //ui->tvTables->setColumnHidden(m_table->fieldIndex("id"), true);
    ui->tvTables->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tvTableData->setModel(m_tableData->relationModel(tableIdx));
    ui->tvTableData->setModel(m_tableData);
    ui->tvTableData->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tvTables->setItemDelegateForColumn(0,new NotEditableDelegate(ui->tvTables));
    ui->tvTableData->setItemDelegateForColumn(0,new NotEditableDelegate(ui->tvTableData));
    ui->tvTableData->setItemDelegateForColumn(tableIdx,new NotEditableDelegate(ui->tvTableData));

}

void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}
MainWindow::~MainWindow()
{

    delete ui;
}


//void MainWindow::on_tvTables_doubleClicked(const QModelIndex &index)
//{
//    qDebug()<<index.row()<<":"<<index.column();
//    qDebug()<<index.data();
//    qDebug()<< index.model()->data(index);
//    qDebug()<< m_table->index(index.row(),0);
//    qDebug()<< m_table->index(index.row(),0).data();
//    QVariant id = m_table->index(index.row(),0).data();
//    m_tableData->setFilter(QString("tableid=%1").arg(id.toLongLong()));
//}



void MainWindow::on_tvTables_activated(const QModelIndex &index)
{
    QVariant id = m_table->index(index.row(),0).data();
    m_tableData->setFilter(QString("tableid=%1").arg(id.toLongLong()));
    qDebug()<<"Activated called....";
}

void MainWindow::on_btnShowTabs_clicked()
{
    qDebug()<<"Button pressed.... Sql request about data from tables...";
    QSqlQuery q;
    qDebug()<<"=======mtable===========";
    if(!q.exec("select * from mtable")){
        showError(q.lastError());
        return;
    }

    while (q.next()) {
     qDebug()<<q.value(0).toString()<<q.value(1).toString()<<q.value(2).toString();
    }

    qDebug()<<"=======tabledata===========";
    if(!q.exec("select * from tabledata")){
        showError(q.lastError());
        return;
    }

    while (q.next()) {
     qDebug()<<q.value(0).toString()<<q.value(1).toString()
            <<q.value(2).toString()<<q.value(3).toString()
            <<q.value(4).toString()<<q.value(5).toString();
    }
}

void MainWindow::on_btnSubmit_clicked()
{
    m_table->submitAll();
    m_tableData->submitAll();
}

void MainWindow::on_btnAdd_clicked()
{
 int rows = ui->tvTableData->model()->rowCount();
 int tableid = m_tableData->fieldIndex("tableid");
 int dateId = m_tableData->fieldIndex("cdate");
 m_tableData->insertRow(m_tableData->rowCount());
 //QVariant tableId = m_tableData->data(
   //          m_tableData->index(rows-1,tableid));


 m_tableData->setData(m_tableData->index(rows,tableid),selectedTableIdx);

 m_tableData->setData(m_tableData->index(rows,dateId),QDate().currentDate().toString("yyyy-MM-dd"));
}

void MainWindow::on_btnDelete_clicked()
{
 QModelIndexList sidx = ui->tvTableData->selectionModel()->selectedIndexes();
 for(QModelIndex id : sidx){
     m_tableData->removeRow(id.row());

 }
}

void MainWindow::on_btnAdd_t_clicked()
{
    int rows = ui->tvTables->model()->rowCount();

    int dateId = m_table->fieldIndex("modtime");
    m_table->insertRow(rows);
    m_table->setData(m_table->index(rows,dateId),QDate().currentDate().toString("yyyy-MM-dd"));

}

void MainWindow::on_btnDelete_t_clicked()
{
    QModelIndexList sidx = ui->tvTables->selectionModel()->selectedIndexes();
    for(QModelIndex id : sidx){
        m_table->removeRow(id.row());

    }
}

void MainWindow::on_btnSubmit_t_clicked()
{
    m_table->submitAll();
}

void MainWindow::on_btnSelect_t_clicked()
{
    //QVariant id = m_table->index(index.row(),0).data();
   QModelIndexList sidx = ui->tvTables->selectionModel()->selectedIndexes();
   QVariant id = sidx.first().data();
   if(!id.isNull()){
        m_tableData->setFilter(QString("tableid=%1").arg(id.toLongLong()));
    selectedTableIdx = id.toInt();
   }
    qDebug()<<"btnSelect_t_clicked() called....";
}

void MainWindow::on_actionQueries_triggered()
{
    qryFrm = new QueryForm();
    qryFrm->setAttribute(Qt::WA_DeleteOnClose);
    qryFrm->show();
}


