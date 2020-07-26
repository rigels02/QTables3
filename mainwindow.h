#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QFrame>
#include "queryform.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showError(const QSqlError &err);

private slots:
    //void on_tvTables_doubleClicked(const QModelIndex &index);


    void on_tvTables_activated(const QModelIndex &index);

    void on_btnShowTabs_clicked();

    void on_btnSubmit_clicked();

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_btnAdd_t_clicked();

    void on_btnDelete_t_clicked();

    void on_btnSubmit_t_clicked();

    void on_btnSelect_t_clicked();

    void on_actionQueries_triggered();


private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *m_table;
    QSqlRelationalTableModel *m_tableData;
    int tableIdx;
    int selectedTableIdx;
    QueryForm *qryFrm = nullptr;

};
#endif // MAINWINDOW_H
