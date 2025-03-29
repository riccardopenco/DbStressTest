#include "dbconnection.h"

#include <QCoreApplication>

namespace st
{

std::atomic<int> DBConnection::connectionId(0);

DBConnection::DBConnection(DBConnectionSpecs connectionDetails) :
    DBConnection(std::move(connectionDetails), [](const QString &){})
{
}

DBConnection::DBConnection(DBConnectionSpecs connectionDetails, std::function<void (const QString &)> logger)
{
    ++connectionId;
    m_connectionName = QString("db_connection_%1").arg(connectionId);
    m_connectionDetails = std::move(connectionDetails);
    m_logger = std::move(logger);
    open();
}

DBConnection::~DBConnection()
{
    close();
}

QSqlQuery DBConnection::tryGetQuery() const
{
    if (!isOpen()
            && !open())
    {
        throw std::runtime_error(QCoreApplication::translate("Connection",
                                                             "La connessione al database è chiusa e non è stato possibile aprirla").toStdString());
    }

    return QSqlQuery(QSqlDatabase::database(m_connectionName));
}

QSqlQuery DBConnection::getQuery() const
{
    return QSqlQuery(QSqlDatabase::database(m_connectionName));
}

ScopedTransaction DBConnection::transaction(ScopedTransaction::Policy policy) const
{
    return ScopedTransaction(policy, m_connectionName);
}

bool DBConnection::open() const
{
    maybeLog(QCoreApplication::translate("Connection", "Apro la connessione '%1' al database").arg(m_connectionName));

    auto db = QSqlDatabase::addDatabase(m_connectionDetails.type, m_connectionName);

    if (!m_connectionDetails.connectOptions.isEmpty())
        db.setConnectOptions(m_connectionDetails.connectOptions);

    if (!m_connectionDetails.hostname.isEmpty())
        db.setHostName(m_connectionDetails.hostname);

    if (m_connectionDetails.port != -1)
        db.setPort(m_connectionDetails.port);

    if (!m_connectionDetails.databaseName.isEmpty())
        db.setDatabaseName(m_connectionDetails.databaseName);

    if (!m_connectionDetails.username.isEmpty())
        db.setUserName(m_connectionDetails.username);

    if (!m_connectionDetails.password.isEmpty())
        db.setPassword(m_connectionDetails.password);

    db.setNumericalPrecisionPolicy(m_connectionDetails.numericalPrecisionPolicy);

    const auto result = db.open();
    const auto text = result ? QCoreApplication::translate("DBConnection",
                                                           "Connessione aperta correttamente")
                             : QCoreApplication::translate("DBConnection",
                                                           "Errore durante l'apertura della connessione");
    maybeLog(text);
    return result;
}

void DBConnection::close() const
{
    maybeLog(QCoreApplication::translate("DBConnection", "Chiudo la connessione '%1' al database").arg(m_connectionName));
    QSqlDatabase::database(m_connectionName).close();
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool DBConnection::isOpen() const
{
    maybeLog(QCoreApplication::translate("Connection", "Verifico la connessione '%1' al database").arg(m_connectionName));

    const auto result = QSqlDatabase::database(m_connectionName).isOpen();
    const auto text = result ? QCoreApplication::translate("Connection", "aperta") : QCoreApplication::translate("Connection", "chiusa");
    maybeLog(QCoreApplication::translate("Connection", "La connessione è %1").arg(text));
    return result;
}

void DBConnection::maybeLog(const QString &text) const
{
    m_logger(text);
}

} // namespace st
