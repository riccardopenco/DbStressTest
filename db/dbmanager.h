#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "dbconnectiondetails.h"
#include "dbconnection.h"

#include <QString>
#include <QSql>
#include <QMap>

class DBManager
{
public:
private:
    DBManager() = default;
    DBManager(const DBManager &) = delete;
    DBManager &operator=(const DBManager &) = delete;

public:
    static DBManager &instance();

    void setConnectionDetails(QString connectionName, DBConnectionDetails details);
    const QMap<QString, DBConnectionDetails> &connectionDetails() const;

    DBConnection connection(const QString &connectionName) const;
    DBConnection connection(const QString &connectionName, std::function<void(const QString &)> logger) const;

private:
    QMap<QString, DBConnectionDetails> m_connectionsDetails;
};

#endif // DBMANAGER_H
