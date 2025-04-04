#ifndef QUERYWORKER_H
#define QUERYWORKER_H

#include "querytimings.h"

#include <QObject>

class QSqlQuery;

class QueryWorker : public QObject
{
    Q_OBJECT
public:
    explicit QueryWorker(QObject *parent = 0);
    ~QueryWorker();

    QString connectionName() const;

signals:
    void execStarted();
    void execFinished();
    void fetchStarted();
    void fetchFinished();
    void succeeded();
    void failed();
    void running();
    void stopped();
    void resultReady(const QueryTimings &);

public slots:
    void process(const QString &name, const QString &sql);

private:
    static int m_connectionId;
    QString m_connectionName{};
    QSqlQuery *m_query{nullptr};
};

#endif // QUERYWORKER_H
