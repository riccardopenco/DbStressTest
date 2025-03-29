#include "query.h"

Query::Query(const QString &query, const QString &name) :
    m_query{query},
    m_name{name}
{
}

Query::Query(Type type, const QString &query, const QString &name) :
    m_type{type},
    m_query{query},
    m_name{name}
{
}

Query::Type Query::type() const
{
    return m_type;
}

QString Query::typeDescription() const
{
    QString resp{};
    switch (m_type)
    {
    case Type::Generic:
        resp = QObject::tr("Generica");
        break;
    case Type::Performance:
        resp = QObject::tr("Prestazioni");
        break;
    case Type::Bandwidth:
        resp = QObject::tr("Banda");
        break;
    }
    return resp;
}

void Query::setType(const Type &type)
{
    m_type = type;
}

QString Query::query() const
{
    return m_query;
}

void Query::setQuery(const QString &query)
{
    m_query = query;
}

QString Query::name() const
{
    return m_name;
}

void Query::setName(const QString &name)
{
    m_name = name;
}

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
    return m_totalExecTimeMs;
}

qint64 Query::totalFetchTimeMs() const
{
    return m_totalFetchTimeMs;
}

qint64 Query::averageExecTimeMs() const
{
    return m_averageExecTimeMs;
}

qint64 Query::averageFetchTimeMs() const
{
    return m_averageFetchTimeMs;
}

int Query::rowCount() const
{
    return m_rowCount;
}

int Query::affectedRowsCount() const
{
    return m_affectedRowsCount;
}

int Query::weight()
{
    return m_weight;
}

void Query::appendResult(const QueryStats &result)
{
    m_stats.append(result);
    if (result.success())
        ++m_successCount;
    else
        ++m_failCount;
    m_totalExecTimeMs += result.execDurationMs();
    m_totalFetchTimeMs += result.fetchDurationMs();
    m_averageExecTimeMs = m_totalExecTimeMs / m_stats.count();
    m_averageFetchTimeMs = m_totalFetchTimeMs / m_stats.count();
    m_rowCount = result.rowCount();
//    if (result.affectedRows() != -1)
        m_affectedRowsCount = result.affectedRows();
        m_weight += result.weight();
}

void Query::clearResults()
{
    m_stats.clear();
    m_successCount = 0;
    m_failCount = 0;
    m_totalExecTimeMs = 0;
    m_totalFetchTimeMs = 0;
    m_averageExecTimeMs = 0;
    m_averageFetchTimeMs = 0;
    m_rowCount = 0;
    m_affectedRowsCount = 0;
    m_weight = 0;
}

//void Query::appendResult(QueryStats &&result)
//{
//    m_stats.append(std::forward(result));
//}
