#include "query.h"

Query::Query(st::QueryDef q)
    : m_query(std::move(q))
{
}

st::QueryDef Query::queryDef() const
{
    return m_query;
}

st::QueryDef::Type Query::type() const
{
    return m_query.type();
}

QString Query::typeDescription() const
{
    return m_query.typeDescr();
}

// void Query::setType(const Type &type)
// {
//     m_type = type;
// }

QString Query::sql() const
{
    return m_query.sql();
}

// void Query::setQuery(const QString &query)
// {
//     m_query = query;
// }

QString Query::name() const
{
    return m_query.name();
}

// void Query::setName(const QString &name)
// {
//     m_name = name;
// }

int Query::runCount() const
{
    return m_stats.count();
}

int Query::successCount() const
{
    return m_successCount;
}

int Query::failCount() const
{
    return m_failCount;
}

qint64 Query::totalExecTimeMs() const
{
    return m_stats.totExecDurationMs();
}

qint64 Query::totalFetchTimeMs() const
{
    return m_stats.totFetchDurationMs();
}

qint64 Query::averageExecTimeMs() const
{
    return m_stats.avgExecDurationMs();
}

qint64 Query::averageFetchTimeMs() const
{
    return m_stats.avgFetchDurationMs();
}

int Query::rowsCount() const
{
    return m_rowsCount;
}

int Query::affectedRowsCount() const
{
    return m_affectedRowsCount;
}

int Query::weight() const
{
    return m_weight;
}

void Query::appendResult(QueryTimings result)
{
    if (result.success())
        ++m_successCount;
    else
        ++m_failCount;

    m_rowsCount = result.rowCount();
    m_affectedRowsCount = result.affectedRows();
    m_weight += result.weight();

    m_stats.appendTiming(std::move(result));
}

void Query::clearResults()
{
    m_stats.clear();
    m_successCount = 0;
    m_failCount = 0;
    m_rowsCount = 0;
    m_affectedRowsCount = 0;
    m_weight = 0;
}

const QueryStats &Query::stats() const
{
    return m_stats;
}

//void Query::appendResult(QueryStats &&result)
//{
//    m_stats.append(std::forward(result));
//}
