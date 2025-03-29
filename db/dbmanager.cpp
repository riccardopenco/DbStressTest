#include "dbmanager.h"

#include <QSqlError>

namespace st
{

DBManager &DBManager::instance()
{
    static DBManager manager;

    return manager;
}

void DBManager::clear()
{
    DBManager::instance().clearImpl();
}

void DBManager::setDb(DB db)
{
    DBManager::instance().clearImpl();
    DBManager::instance().addDbImpl(std::move(db));
}

void DBManager::addDb(DB db)
{
    DBManager::instance().addDbImpl(std::move(db));
}

QStringList DBManager::allDbs()
{
    return DBManager::instance().allDbsImpl();
}

DB DBManager::getDb(const QString &name)
{
    return DBManager::instance().getDbImpl(name);
}

DB DBManager::tryGetDb(const QString &name)
{
    return DBManager::instance().tryGetDbImpl(name);
}

ConnectionTestResult DBManager::testConnection(const QString &connectionName, const DBConnectionSpecs &specs)
{
    return DBManager::instance().testConnectionImpl(connectionName, specs);
}

void DBManager::clearImpl()
{
    m_dbs.clear();
}

void DBManager::addDbImpl(DB db)
{
    m_dbs.insert(db.name, std::move(db));
}

QStringList DBManager::allDbsImpl() const
{
    return m_dbs.keys();
}

DB DBManager::getDbImpl(const QString &name) const
{
    return m_dbs.value(name);
}

DB DBManager::tryGetDbImpl(const QString &name) const
{
    const auto it = m_dbs.find(name);
    if (it == m_dbs.end())
        throw std::runtime_error("database not configured");

    return *it;
}

ConnectionTestResult DBManager::testConnectionImpl(const QString &connectionName, const DBConnectionSpecs &specs) const
{
    if (!specs.areValid())
        return
          {
            .isOk   = false,
            .errMsg = QStringLiteral("Specifiche non corrette"),
          };

    auto res = ConnectionTestResult
      {
        .isOk   = false,
        .errMsg = {},
      };

    // scoped to avoid warning message about still used connection on db removal
    {
        auto db = QSqlDatabase::addDatabase(specs.type, connectionName);
        db.setDatabaseName(specs.databaseName);
        db.setNumericalPrecisionPolicy(specs.numericalPrecisionPolicy);

        if (!specs.connectOptions.isEmpty())
            db.setConnectOptions(specs.connectOptions);
        if (!specs.hostname.isEmpty())
            db.setHostName(specs.hostname);
        if (specs.port != -1)
            db.setPort(specs.port);
        if (!specs.username.isEmpty())
            db.setUserName(specs.username);
        if (!specs.password.isEmpty())
            db.setPassword(specs.password);

        res.isOk = db.open();
        if (!res.isOk)
            res.errMsg = QSqlDatabase::database(connectionName).lastError().text();
    }
    QSqlDatabase::removeDatabase(connectionName);
    return res;
}


void DBManager::setConnectionDetails(QString connectionName, DBConnectionSpecs details)
{
    m_connectionsDetails.insert(std::move(connectionName), std::move(details));
}

const QMap<QString, DBConnectionSpecs> &DBManager::connectionDetails() const
{
    return m_connectionsDetails;
}

DBConnection DBManager::connection(const QString &connectionName) const
{
    return DBConnection(m_connectionsDetails.value(connectionName));
}

DBConnection DBManager::connection(const QString &connectionName, std::function<void (const QString &)> logger) const
{
    return DBConnection(m_connectionsDetails.value(connectionName), std::move(logger));
}

} // namespace st
