#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include "dbcommon.h"
#include "scopedtransaction.h"

#include <QString>
#include <QSqlQuery>

#include <atomic>
#include <functional>

namespace st
{

class DBConnection
{
    friend class DBManager;

    DBConnection(DBConnectionSpecs connectionDetails);
    DBConnection(DBConnectionSpecs connectionDetails, std::function<void(const QString &)> logger);
public:
    DBConnection(const DBConnection &) = delete;
    DBConnection(DBConnection &&) = delete;
    ~DBConnection();

    DBConnection &operator=(const DBConnection &) = delete;
    DBConnection &operator=(DBConnection &&) = delete;

public:
    QSqlQuery tryGetQuery() const;
    QSqlQuery getQuery() const;

    ScopedTransaction transaction(ScopedTransaction::Policy policy = ScopedTransaction::Policy::AutoRollback) const;

private:
    bool open() const;
    void close() const;
    bool isOpen() const;

    void maybeLog(const QString &text) const;

private:
    static std::atomic<int> connectionId;
    QString m_connectionName;
    DBConnectionSpecs m_connectionDetails;
//    std::function<void(const QString &)> m_logger = [](const QString &){};
    std::function<void(const QString &)> m_logger;
};

} // namespace st

#endif // DBCONNECTION_H
