#ifndef QUERYFORM_H
#define QUERYFORM_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class QueryForm;
}

class QueryForm : public QWidget
{
    Q_OBJECT

public:
    explicit QueryForm(QWidget *parent = nullptr);
    ~QueryForm();

private:
    Ui::QueryForm *ui;
    QSqlQueryModel *model;
};

#endif // QUERYFORM_H
