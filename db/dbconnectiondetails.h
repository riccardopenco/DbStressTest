#ifndef DBCONNECTIONDETAILS_H
#define DBCONNECTIONDETAILS_H

#include <QString>
#include <QSql>

struct DBConnectionDetails
{
    QString type;
    QString connectOptions;
    QString hostName;
    int port = -1;
    QString databaseName;
    QString userName;
    QString password;
    QSql::NumericalPrecisionPolicy numericalPrecisionPolicy = QSql::LowPrecisionDouble;
};

#endif // DBCONNECTIONDETAILS_H
