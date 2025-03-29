#include "scopedtransaction.h"

#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlDriver>

#include <exception>

ScopedTransaction::ScopedTransaction(ScopedTransaction::Policy policy, QString connectionName) :
    m_policy{policy},
    m_connectionName{std::move(connectionName)},
    m_active{false}
{
    if (!db().driver()->hasFeature(QSqlDriver::Transactions))
        throw std::runtime_error(QCoreApplication::translate("ScopedTransaction", "SQL database driver does not support transactions").toStdString());

    begin();
}

ScopedTransaction::~ScopedTransaction()
{
    if (isActive())
    {
        switch (m_policy)
        {
        case Policy::AutoCommit:
            commit();
            break;
        case Policy::AutoRollback:
            rollback();
            break;
        }
    }
}

bool ScopedTransaction::isActive() const
{
    return m_active;
}

void ScopedTransaction::begin() const
{
    if (isActive())
        return;

    m_active = db().transaction();
}

void ScopedTransaction::commit() const
{
    if (!isActive())
        return;

    db().commit();
    m_active = false;
}

void ScopedTransaction::rollback() const
{
    if (!isActive())
        return;

    db().rollback();
    m_active = false;
}

QSqlDatabase ScopedTransaction::db() const
{
    if (m_connectionName.isEmpty())
        return QSqlDatabase::database();

    return QSqlDatabase::database(m_connectionName);
}
