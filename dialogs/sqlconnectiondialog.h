#ifndef SQLCONNECTIONDIALOG_H
#define SQLCONNECTIONDIALOG_H

#include "connectionparameters.h"

#include <QDialog>

namespace Ui {
class SqlConnectionDialog;
}

class SqlConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SqlConnectionDialog(QWidget *parent = 0);
    ~SqlConnectionDialog();

    QString driverName() const;
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostname() const;
    int port() const;

private slots:
    void testConnection();
    void openConnection();

private:
    void setUiParameters();
    void setConnectionParameters();
    QString generateConnectionString() const;
    bool tryConnection(const QString &connectionName = ConnectionParameters::defaultConnectionName());

private:
    Ui::SqlConnectionDialog *ui;
};

#endif // SQLCONNECTIONDIALOG_H
