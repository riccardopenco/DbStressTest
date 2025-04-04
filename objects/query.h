#ifndef QUERY_H
#define QUERY_H

#include "querydef.h"
#include "querytimings.h"
#include "querystats.h"

#include <QString>
#include <QList>

class Query
{
public:
    Query() = default;
    Query(const Query &) = default;
    Query(Query &&) = default;
    explicit Query(st::QueryDef q);
    ~Query() = default;

    Query &operator=(const Query &) = default;
    Query &operator=(Query &&) = default;

    st::QueryDef queryDef() const;

    st::QueryDef::Type type() const;
    QString typeDescription() const;
    // void setType(const Type &type);

    QString sql() const;
    // void setQuery(const QString &query);

    QString name() const;
    // void setName(const QString &name);

    int runCount() const;
    int successCount() const;
    int failCount() const;
    qint64 totalExecTimeMs() const;
    qint64 totalFetchTimeMs() const;
    qint64 averageExecTimeMs() const;
    qint64 averageFetchTimeMs() const;
    int rowsCount() const;
    int affectedRowsCount() const;
    int weight() const;

    void appendResult(QueryTimings result);
    void clearResults();
    const QueryStats &stats() const;

private:
    st::QueryDef m_query;
    QueryStats m_stats{};

    int m_successCount{0};
    int m_failCount{0};
    int m_rowsCount{0};
    int m_affectedRowsCount{0};
    int m_weight{0};
};

#endif // QUERY_H
