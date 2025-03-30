#ifndef SQLCONNECTIONDIALOG_H
#define SQLCONNECTIONDIALOG_H

#include "db/dbcommon.h"

#include <QDialog>

namespace Ui {
class SqlConnectionDialog;
}

class SqlConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SqlConnectionDialog(const st::DB &db = st::DB(), QWidget *parent = nullptr);
    ~SqlConnectionDialog();

    st::DB db() const;

private:
    st::DBConnectionSpecs specs() const;

    void setName(const QString &name) const;
    void setDriverName(const QString &driverName) const;
    void setConnectOptions(const QString &connectionOptions) const;
    void setDatabaseName(const QString &databaseName) const;
    void setUsername(const QString &username) const;
    void setPassword(const QString &password) const;
    void setHostname(const QString &hostname) const;
    void setPort(int port) const;

    QString name() const;
    QString driverName() const;
    QString connectOptions() const;
    QString databaseName() const;
    QString username() const;
    QString password() const;
    QString hostname() const;
    int port() const;

private slots:
    void testConnection();
    void Confirm();

private:
    void setupForm(const st::DB &db);
    void setUiParameters(const st::DB &db);
    st::ConnectionTestResult tryConnection(const QString &connectionName);

private:
    Ui::SqlConnectionDialog *ui;
};

#endif // SQLCONNECTIONDIALOG_H
