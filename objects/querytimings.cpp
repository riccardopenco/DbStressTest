#include "querytimings.h"

QueryTimings::QueryTimings(const QString &name, const QString &sql)
    : m_queryName{name}
    , m_querySql{sql}
{
}

QueryTimings::~QueryTimings()
{
}

qint64 QueryTimings::totalDurationMs() const
{
    return execDurationMs() + fetchDurationMs();
}

qint64 QueryTimings::execDurationMs() const
{
    return m_execStartTime.msecsTo(m_execEndTime);
}

qint64 QueryTimings::fetchDurationMs() const
{
    return m_fetchStartTime.msecsTo(m_fetchEndTime);
}

QString QueryTimings::queryName() const
{
    return m_queryName;
}

void QueryTimings::setQueryName(const QString &name)
{
    m_queryName = name;
}

QDateTime QueryTimings::execStartTime() const
{
    return m_execStartTime;
}

void QueryTimings::setExecStartTime(const QDateTime &execStartTime)
{
    m_execStartTime = execStartTime;
}

QDateTime QueryTimings::execEndTime() const
{
    return m_execEndTime;
}

void QueryTimings::setExecEndTime(const QDateTime &execEndTime)
{
    m_execEndTime = execEndTime;
}

QDateTime QueryTimings::fetchStartTime() const
{
    return m_fetchStartTime;
}

void QueryTimings::setFetchStartTime(const QDateTime &fetchStartTime)
{
    m_fetchStartTime = fetchStartTime;
}

QDateTime QueryTimings::fetchEndTime() const
{
    return m_fetchEndTime;
}

void QueryTimings::setFetchEndTime(const QDateTime &fetchEndTime)
{
    m_fetchEndTime = fetchEndTime;
}

bool QueryTimings::success() const
{
    return m_success;
}

void QueryTimings::setSuccess(bool success)
{
    m_success = success;
}

int QueryTimings::rowCount() const
{
    return m_rowCount;
}

void QueryTimings::setRowCount(int rowCount)
{
    m_rowCount = rowCount;
}

int QueryTimings::affectedRows() const
{
    return m_affectedRows;
}

void QueryTimings::setAffectedRows(int affectedRows)
{
    m_affectedRows = affectedRows;
}

int QueryTimings::weight() const
{
    return m_weight;
}

void QueryTimings::setWeight(int weight)
{
    m_weight = weight;
}
