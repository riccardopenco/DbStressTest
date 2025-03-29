#include "querystats.h"

//QueryStats::QueryStats(QObject *parent) :
//    QObject(parent)
//{
//}

QueryStats::QueryStats(const QString &query/*, QObject *parent*/) :
//    QObject(parent),
    m_query{query}
{
}

QueryStats::~QueryStats()
{
}

qint64 QueryStats::totalDurationMs() const
{
    return execDurationMs() + fetchDurationMs();
}

qint64 QueryStats::execDurationMs() const
{
    return m_execStartTime.msecsTo(m_execEndTime);
}

qint64 QueryStats::fetchDurationMs() const
{
    return m_fetchStartTime.msecsTo(m_fetchEndTime);
}

QString QueryStats::query() const
{
    return m_query;
}

void QueryStats::setQuery(const QString &query)
{
    m_query = query;
}

QDateTime QueryStats::execStartTime() const
{
    return m_execStartTime;
}

void QueryStats::setExecStartTime(const QDateTime &execStartTime)
{
    m_execStartTime = execStartTime;
}

QDateTime QueryStats::execEndTime() const
{
    return m_execEndTime;
}

void QueryStats::setExecEndTime(const QDateTime &execEndTime)
{
    m_execEndTime = execEndTime;
}

QDateTime QueryStats::fetchStartTime() const
{
    return m_fetchStartTime;
}

void QueryStats::setFetchStartTime(const QDateTime &fetchStartTime)
{
    m_fetchStartTime = fetchStartTime;
}

QDateTime QueryStats::fetchEndTime() const
{
    return m_fetchEndTime;
}

void QueryStats::setFetchEndTime(const QDateTime &fetchEndTime)
{
    m_fetchEndTime = fetchEndTime;
}

bool QueryStats::success() const
{
    return m_success;
}

void QueryStats::setSuccess(bool success)
{
    m_success = success;
}

int QueryStats::rowCount() const
{
    return m_rowCount;
}

void QueryStats::setRowCount(int rowCount)
{
    m_rowCount = rowCount;
}

int QueryStats::affectedRows() const
{
    return m_affectedRows;
}

void QueryStats::setAffectedRows(int affectedRows)
{
    m_affectedRows = affectedRows;
}

int QueryStats::weight() const
{
    return m_weight;
}

void QueryStats::setWeight(int weight)
{
    m_weight = weight;
}
