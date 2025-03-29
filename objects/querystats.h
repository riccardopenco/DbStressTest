#ifndef QUERYSTATS_H
#define QUERYSTATS_H

#include <QObject>
#include <QString>
#include <QDateTime>

class QueryStats// : public QObject
{
    //Q_OBJECT
public:
    //explicit QueryStats(QObject *parent = 0);
    explicit QueryStats(const QString &query = ""/*, QObject *parent = 0*/);
    ~QueryStats();

    QueryStats (const QueryStats &) = default;
    QueryStats (QueryStats &&) = default;

    QueryStats &operator =(const QueryStats &) = default;
    QueryStats &operator =(QueryStats &&) = default;

    qint64 totalDurationMs() const;
    qint64 execDurationMs() const;
    qint64 fetchDurationMs() const;

    QString query() const;
    void setQuery(const QString &query);

    QDateTime execStartTime() const;
    void setExecStartTime(const QDateTime &execStartTime);

    QDateTime execEndTime() const;
    void setExecEndTime(const QDateTime &execEndTime);

    QDateTime fetchStartTime() const;
    void setFetchStartTime(const QDateTime &fetchStartTime);

    QDateTime fetchEndTime() const;
    void setFetchEndTime(const QDateTime &fetchEndTime);

    bool success() const;
    void setSuccess(bool success);

    int rowCount() const;
    void setRowCount(int rowCount);

    int affectedRows() const;
    void setAffectedRows(int affectedRows);

    int weight() const;
    void setWeight(int weight);

private:
    QString m_query{};
    QDateTime m_execStartTime{};
    QDateTime m_execEndTime{};
    QDateTime m_fetchStartTime{};
    QDateTime m_fetchEndTime{};
    bool m_success{false};
    int m_rowCount{0};
    int m_affectedRows{0};
    int m_weight{0};    // weight is an estimate of the amount of data transferred from the database to the application
                        // it is calculated transforming all the fields of all the rows to string and summing the length
                        // of the strings
};

//Q_DECLARE_METATYPE(QueryStats)

#endif // QUERYSTATS_H
