#ifndef QUERY_H
#define QUERY_H

#include "querydef.h"
#include "querystats.h"

#include <QString>
#include <QList>

class Query
{
public:
    enum class Type
    {
        Generic,
        Performance,
        Bandwidth
    };

    Query() = default;
    Query(const Query &) = default;
    Query(Query &&) = default;
    explicit Query(st::QueryDef q);
    // explicit Query(const QString &query = {}, const QString &name = {});
    // Query(Type type, const QString &query = {}, const QString &name = {});
    ~Query() = default;

    Query &operator=(const Query &) = default;
    Query &operator=(Query &&) = default;

    st::QueryDef::Type type() const;
    QString typeDescription() const;
    // void setType(const Type &type);

    QString query() const;
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
    int rowCount() const;
    int affectedRowsCount() const;
    int weight();

    void appendResult(const QueryStats &result);
    void clearResults();
//    void appendResult(QueryStats &&result);

private:
    st::QueryDef m_query;
    // Type m_type{Type::Generic};
    // QString m_query{};
    // QString m_name{};
    QList<QueryStats> m_stats{};

    int m_successCount{0};
    int m_failCount{0};
    qint64 m_totalExecTimeMs{0};
    qint64 m_totalFetchTimeMs{0};
    qint64 m_averageExecTimeMs{0};
    qint64 m_averageFetchTimeMs{0};
    int m_rowCount{0};
    int m_affectedRowsCount{0};
    int m_weight{0};
};

#endif // QUERY_H
