#ifndef DBCOMMON_H
#define DBCOMMON_H

#include <QString>
#include <QSql>

namespace st
{

struct DBConnectionSpecs
{
    QString type;
    QString connectOptions;
    QString hostname;
    int port = -1;
    QString databaseName;
    QString username;
    QString password;
    QSql::NumericalPrecisionPolicy numericalPrecisionPolicy = QSql::LowPrecisionDouble;

    bool areValid() const
    {
        return !type.isEmpty() && !databaseName.isEmpty();  // the simplest connection is QSQLITE with database name :memory:
    }
};

struct DB
{
    QString name;
    DBConnectionSpecs specs;

    bool isValid() const
    {
        return !name.isEmpty() && specs.areValid();
    }
};

struct ConnectionTestResult
{
    bool    isOk = false;
    QString errMsg;
};

} // namespace st

#endif // DBCOMMON_H
