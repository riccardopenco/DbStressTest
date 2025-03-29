#include "dbmanager.h"

DBManager &DBManager::instance()
{
    static DBManager manager;

    return manager;
}

void DBManager::setConnectionDetails(QString connectionName, DBConnectionDetails details)
{
    m_connectionsDetails.insert(std::move(connectionName), std::move(details));
}

const QMap<QString, DBConnectionDetails> &DBManager::connectionDetails() const
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
