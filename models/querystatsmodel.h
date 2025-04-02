#ifndef QUERYSTATSMODEL_H
#define QUERYSTATSMODEL_H

#include "objects/query.h"

#include <QAbstractTableModel>
#include <QMap>
#include <QString>

class QueryStatsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum class ModelColumn
    {
        QueryId,
        Type,
        Name,
        Status,
        RunCount,
        SuccessCount,
        FailCount,
        TotalExecTimeMs,
        TotalFetchTimeMs,
        AverageExecTimeMs,
        AverageFetchTimeMs,
        RowCount,
        AffectedRows,
        Weight,
        Query,
        LastColumn
    };

    explicit QueryStatsModel(QObject *parent = 0);
    ~QueryStatsModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void clear();
    void clearStats();
    bool appendQuery(const Query &query);
    bool updateQuery(const Query &query);
    bool removeQuery(int row);
    bool addResult(QueryTimings result);

    Query getToRunAt(int row);
    Query at(int row) const;
    //Query at(const QString &name) const;
    int rowFor(const QString &name) const;

    QList<Query> queryList() const;

private:
    QMap<int, Query> m_queries;
    QMap<QString, int> m_indexes;
    QList<int> m_runningQueries;
};

#endif // QUERYSTATSMODEL_H
