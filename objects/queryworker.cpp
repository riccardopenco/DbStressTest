#include "queryworker.h"

#include "connectionparameters.h"

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

void QueryWorker::process(const QString &query)
{
    if (m_query == nullptr)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(ConnectionParameters::driverName(), m_connectionName);

        // if (ConnectionParameters::useConnectionString())
        // {
        //     ConnectionParameters::con
        //     // ui->connectionString->setText(generateConnectionString());
        //     db.setDatabaseName(generateConnectionString());
        // }
        // else
        // {
            if (!ConnectionParameters::databaseName().isEmpty())
                db.setDatabaseName(ConnectionParameters::databaseName());
            if (!ConnectionParameters::userName().isEmpty())
                db.setUserName(ConnectionParameters::userName());
            if (!ConnectionParameters::password().isEmpty())
                db.setPassword(ConnectionParameters::password());
            // if (!ConnectionParameters::hostName().isEmpty())
                db.setHostName(ConnectionParameters::hostName());
        // }

        db.open();
        m_query = new QSqlQuery(db);
        m_query->setForwardOnly(true);
    }
    emit running();
    QueryStats result{query};
    m_query->prepare(query);
    emit execStarted();
    result.setExecStartTime(QDateTime::currentDateTime());
    bool success{m_query->exec()};
    result.setExecEndTime(QDateTime::currentDateTime());
    emit execFinished();
    result.setSuccess(success);
    result.setAffectedRows(m_query->numRowsAffected());
    if (success)
    {
        emit succeeded();
        if (m_query->isSelect())
        {
            int rows{0};
            int singleWeight{0};
            int weight{0};

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
