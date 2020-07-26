#include "queryform.h"
#include "ui_queryform.h"

QueryForm::QueryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryForm)
{
    ui->setupUi(this);
    model = new QSqlQueryModel;
    model->setQuery(R"(
                    SELECT m.name,strftime('%m',d.cdate) mm,d.category,sum(d.amount) FROM mtable m
                    join tabledata d on d.tableid=m.id
                    GROUP by m.name,strftime('%m',d.cdate),d.category,d.amount
                    )");
    ui->tvQuery->setModel(model);
}

QueryForm::~QueryForm()
{
    delete ui;
}
