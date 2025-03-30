#ifndef QUERYEDITDIALOG_H
#define QUERYEDITDIALOG_H

#include "objects/querydef.h"

#include <QDialog>

namespace Ui
{
class QueryEditDialog;
}

class QueryEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryEditDialog(QWidget *parent = nullptr);
    explicit QueryEditDialog(st::QueryDef query, QWidget *parent = nullptr);
    ~QueryEditDialog();

    st::QueryDef query() const;

private slots:
    void setupForm(const st::QueryDef &query);
    void Confirm();

private:
    Ui::QueryEditDialog *ui;
};

#endif // QUERYEDITDIALOG_H
