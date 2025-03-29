#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "dbcommon.h"
#include "dbconnection.h"

#include <QString>
#include <QSql>
#include <QMap>

namespace st
{

class DBManager
{
public:    
private:
    DBManager() = default;
    DBManager(const DBManager &) = delete;
    DBManager &operator=(const DBManager &) = delete;

public:
    static DBManager &instance();
    static void addDb(DB db);
    static QStringList allDbs();
    static DB getDb(const QString &name);
    static DB tryGetDb(const QString &name);

    static ConnectionTestResult testConnection(const QString &connectionName, const DBConnectionSpecs &specs);

private:
    void addDbImpl(DB db);
    QStringList allDbsImpl() const;
    DB getDbImpl(const QString &name) const;
    DB tryGetDbImpl(const QString &name) const;

    ConnectionTestResult testConnectionImpl(const QString &connectionName, const DBConnectionSpecs &specs) const;

    void setConnectionDetails(QString connectionName, DBConnectionSpecs details);
    const QMap<QString, DBConnectionSpecs> &connectionDetails() const;

    DBConnection connection(const QString &connectionName) const;
    DBConnection connection(const QString &connectionName, std::function<void(const QString &)> logger) const;

private:
    QMap<QString, DB> m_dbs;
    QMap<QString, DBConnectionSpecs> m_connectionsDetails;
};

} // namespace st

#endif // DBMANAGER_H
