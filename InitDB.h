#ifndef INITDB_H
#define INITDB_H

#include <QtSql>

const auto TABLES_SQL=R"(
            create table mtable (id integer primary key,modtime date,
                      name varchar)
            )";

const auto INSERT_TABLES_SQL=R"(
                             insert into mtable(modtime,name) values(?,?)
                             )";

const auto TABLESDATA_SQL=R"(
            create table tabledata (id integer primary key,cdate date,
                      category varchar,amount double,note varchar,tableid integer)
            )";
const auto INSERT_TABLEDATA_SQL=R"(
           insert into tabledata (cdate,category,amount,note,tableid)
            values(?,?,?,?,?)
            )";

const auto SELECT_TABLEDATA_SQL=R"(
           select * from tabledata)";
const auto SELECT_TABLES_SQL=R"(
           select * from tables)";

QVariant addTable(QSqlQuery &q,const QString name,const QDate date){
    q.addBindValue(date);
    q.addBindValue(name);

    q.exec();
    return q.lastInsertId();
}

QVariant addTableData(QSqlQuery &q,
                      const QDate cdate,
                      const QString category,
                      const double amount,
                      const QString note,
                      const QVariant tableId){
    q.addBindValue(cdate);
    q.addBindValue(category);
    q.addBindValue(amount);
    q.addBindValue(note);
    q.addBindValue(tableId);
    q.exec();
    return q.lastInsertId();
}


QSqlError InitDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mtables.db"); //

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    QSqlQuery q;

    if (!tables.contains("mtable", Qt::CaseInsensitive)){
        if (!q.exec(TABLES_SQL))
            return q.lastError();
    }
    if( !tables.contains("tabledata", Qt::CaseInsensitive)){
        if (!q.exec(TABLESDATA_SQL))
            return q.lastError();
    }

    return QSqlError();
}



#endif // INITDB_H
