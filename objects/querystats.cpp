#include "querystats.h"

#include <cmath>

void QueryStats::clear()
{
    m_timings.clear();
}

void QueryStats::appendTiming(QueryTimings timing)
{
    m_timings.append(std::move(timing));
}

qsizetype QueryStats::count() const
{
    return m_timings.size();
}

qint64 QueryStats::totQueryDurationMs() const
{
    return std::accumulate(std::cbegin(m_timings),
                           std::cend(m_timings),
                           static_cast<qint64>(0),
                           [](qint64 sum, const QueryTimings &timing){ return sum + timing.totalDurationMs(); });
}

qint64 QueryStats::totExecDurationMs() const
{
    return std::accumulate(std::cbegin(m_timings),
                           std::cend(m_timings),
                           static_cast<qint64>(0),
                           [](qint64 sum, const QueryTimings &timing){ return sum + timing.execDurationMs(); });
}

qint64 QueryStats::totFetchDurationMs() const
{
    return std::accumulate(std::cbegin(m_timings),
                           std::cend(m_timings),
                           static_cast<qint64>(0),
                           [](qint64 sum, const QueryTimings &timing){ return sum + timing.fetchDurationMs(); });
}

qint64 QueryStats::minQueryDurationMs() const
{
    const auto it = std::min_element(std::cbegin(m_timings),
                                     std::cend(m_timings),
                                     [](const QueryTimings &l, const QueryTimings &r){ return l.totalDurationMs() < r.totalDurationMs(); });
    return it->totalDurationMs();
}

qint64 QueryStats::minExecDurationMs() const
{
    const auto it = std::min_element(std::cbegin(m_timings),
                                     std::cend(m_timings),
                                     [](const QueryTimings &l, const QueryTimings &r){ return l.execDurationMs() < r.execDurationMs(); });
    return it->execDurationMs();
}

qint64 QueryStats::minFetchDurationMs() const
{
    const auto it = std::min_element(std::cbegin(m_timings),
                                     std::cend(m_timings),
                                     [](const QueryTimings &l, const QueryTimings &r){ return l.fetchDurationMs() < r.fetchDurationMs(); });
    return it->fetchDurationMs();
}

qint64 QueryStats::maxQueryDurationMs() const
{
    const auto it = std::max_element(std::cbegin(m_timings),
                                     std::cend(m_timings),
                                     [](const QueryTimings &l, const QueryTimings &r){ return l.totalDurationMs() < r.totalDurationMs(); });
    return it->totalDurationMs();
}

qint64 QueryStats::maxExecDurationMs() const
{
    const auto it = std::max_element(std::cbegin(m_timings),
                                     std::cend(m_timings),
                                     [](const QueryTimings &l, const QueryTimings &r){ return l.execDurationMs() < r.execDurationMs(); });
    return it->execDurationMs();
}

qint64 QueryStats::maxFetchDurationMs() const
{
    const auto it = std::max_element(std::cbegin(m_timings),
                                     std::cend(m_timings),
                                     [](const QueryTimings &l, const QueryTimings &r){ return l.fetchDurationMs() < r.fetchDurationMs(); });
    return it->fetchDurationMs();
}

qint64 QueryStats::avgQueryDurationMs() const
{
    const auto cnt = m_timings.size();
    return cnt == 0 ? 0 : totQueryDurationMs() / cnt;
}

qint64 QueryStats::avgExecDurationMs() const
{
    const auto cnt = m_timings.size();
    return cnt == 0 ? 0 : totExecDurationMs() / cnt;
}

qint64 QueryStats::avgFetchDurationMs() const
{
    const auto cnt = m_timings.size();
    return cnt == 0 ? 0 : totFetchDurationMs() / cnt;
}

double QueryStats::stdErrQueryDurationMs() const
{
    const auto cnt = m_timings.size();
    const auto avg = avgQueryDurationMs();
    const auto sum = std::accumulate(std::cbegin(m_timings),
                                     std::cend(m_timings),
                                     static_cast<qint64>(0),
                                     [&avg](qint64 sum, const QueryTimings &timing){ const auto delta = timing.totalDurationMs() - avg; return sum + delta * delta; });
    return std::sqrt(sum / cnt);
}

qint64 QueryStats::stdErrExecDurationMs() const
{
    const auto cnt = m_timings.size();
    const auto avg = avgExecDurationMs();
    const auto sum = std::accumulate(std::cbegin(m_timings),
                                     std::cend(m_timings),
                                     static_cast<qint64>(0),
                                     [&avg](qint64 sum, const QueryTimings &timing){ const auto delta = timing.execDurationMs() - avg; return sum + delta * delta; });
    return std::sqrt(sum / cnt);
}

qint64 QueryStats::stdErrFetchDurationMs() const
{
    const auto cnt = m_timings.size();
    const auto avg = avgFetchDurationMs();
    const auto sum = std::accumulate(std::cbegin(m_timings),
                                     std::cend(m_timings),
                                     static_cast<qint64>(0),
                                     [&avg](qint64 sum, const QueryTimings &timing){ const auto delta = timing.fetchDurationMs() - avg; return sum + delta * delta; });
    return std::sqrt(sum / cnt);
}
