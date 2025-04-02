#ifndef QUERYSTATS_H
#define QUERYSTATS_H

#include "querytimings.h"

#include <QVector>

class QueryStats
{
public:
    void clear();
    void appendTiming(QueryTimings timing);

    qsizetype count() const;

    qint64 totQueryDurationMs() const;
    qint64 totExecDurationMs() const;
    qint64 totFetchDurationMs() const;

    qint64 minQueryDurationMs() const;
    qint64 minExecDurationMs() const;
    qint64 minFetchDurationMs() const;

    qint64 maxQueryDurationMs() const;
    qint64 maxExecDurationMs() const;
    qint64 maxFetchDurationMs() const;

    qint64 avgQueryDurationMs() const;
    qint64 avgExecDurationMs() const;
    qint64 avgFetchDurationMs() const;

    double stdErrQueryDurationMs() const;
    qint64 stdErrExecDurationMs() const;
    qint64 stdErrFetchDurationMs() const;

private:


private:
    QVector<QueryTimings> m_timings;

};

#endif // QUERYSTATS_H
