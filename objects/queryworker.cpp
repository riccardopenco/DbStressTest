#include "queryworker.h"

#include "db/dbmanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDateTime>

int QueryWorker::m_connectionId = 0;

QueryWorker::QueryWorker(QObject *parent) :
    QObject(parent),
    m_connectionName{QString("qwc_%1").arg(++m_connectionId)}
{
}

QueryWorker::~QueryWorker()
{
    if (m_query != nullptr)
    {
        QSqlDatabase::database(m_connectionName).close();
        delete m_query;
        QSqlDatabase::removeDatabase(m_connectionName);
    }
}

QString QueryWorker::connectionName() const
{
    return m_connectionName;
}

void QueryWorker::process(const QString &name, const QString &sql)
{
    if (m_query == nullptr)
    {
        auto specs = st::DBManager::getDb().specs;
        QSqlDatabase db = QSqlDatabase::addDatabase(specs.type, m_connectionName);

        if (!specs.hostname.isEmpty())
            db.setHostName(specs.hostname);
        if (!specs.connectOptions.isEmpty())
            db.setConnectOptions(specs.connectOptions);
        if (!specs.databaseName.isEmpty())
            db.setDatabaseName(specs.databaseName);
        if (!specs.username.isEmpty())
            db.setUserName(specs.username);
        if (!specs.password.isEmpty())
            db.setPassword(specs.password);

        auto res = db.open();
        if (!res)
            return;
        m_query = new QSqlQuery(db);
        m_query->setForwardOnly(true);
    }
    emit running();
    auto result = QueryTimings(name, sql);
    m_query->prepare(sql);
    emit execStarted();
    result.setExecStartTime(QDateTime::currentDateTime());
    auto success = m_query->exec();
    result.setExecEndTime(QDateTime::currentDateTime());
    emit execFinished();
    result.setSuccess(success);
    result.setAffectedRows(m_query->numRowsAffected());
    if (success)
    {
        emit succeeded();
        if (m_query->isSelect())
        {
            auto rows = 0;
            auto singleWeight = 0;
            auto weight = 0;

            result.setFetchStartTime(QDateTime::currentDateTime());
            emit fetchStarted();

            do
            {
                singleWeight = 0;
                if (m_query->next())
                {
                    ++rows;
                    for (int f = 0; f < m_query->record().count(); ++f)
                        singleWeight += m_query->value(f).toString().size();
                    weight += singleWeight;
                }
                while (m_query->next())
                {
                    ++rows;
                    weight += singleWeight;
                }
            } while (m_query->nextResult());

            result.setFetchEndTime(QDateTime::currentDateTime());
            result.setRowCount(rows);
            result.setWeight(weight);
            emit fetchFinished();
        }
    }
    else
        emit failed();
    emit stopped();
    emit resultReady(result);
}
