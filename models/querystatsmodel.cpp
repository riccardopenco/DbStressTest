#include "querystatsmodel.h"

#include <QApplication>
#include <QLocale>
#include <QFontMetrics>
#include <QBrush>

QueryStatsModel::QueryStatsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QueryStatsModel::~QueryStatsModel()
{
}

int QueryStatsModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_indexes.size();
}

int QueryStatsModel::columnCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(ModelColumn::LastColumn);
}

QVariant QueryStatsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole
            && role != Qt::TextAlignmentRole
            && role != Qt::SizeHintRole
            && role != Qt::ForegroundRole)
        return {};

    auto row = index.row();
    auto q = m_queries.value(row);
    ModelColumn col = static_cast<ModelColumn>(index.column());

//    const auto display = [](auto value){ QLocale locale; locale.setNumberOptions(0); return locale.toString(value); };
    const auto display = [](auto value){ return value; };

    if (role == Qt::DisplayRole)
    {
        auto count = static_cast<int>(m_runningQueries.count(row));
        switch (col)
        {
        case ModelColumn::QueryId: return index.row();
        case ModelColumn::Type: return q.typeDescription();
        case ModelColumn::Name: return q.name();
        case ModelColumn::Status: return count > 0 ? tr("In esecuzione (%1)").arg(count) : QString();
        case ModelColumn::RunCount: return q.runCount();
        case ModelColumn::SuccessCount: return q.successCount();
        case ModelColumn::FailCount: return q.failCount();
        case ModelColumn::TotalExecTimeMs: return display(q.totalExecTimeMs());
        case ModelColumn::TotalFetchTimeMs: return display(q.totalFetchTimeMs());
        case ModelColumn::AverageExecTimeMs: return display(q.averageExecTimeMs());
        case ModelColumn::AverageFetchTimeMs: return display(q.averageFetchTimeMs());
        case ModelColumn::RowCount: return display(q.rowCount());
        case ModelColumn::AffectedRows: return display(q.affectedRowsCount());
        case ModelColumn::Weight: return display(q.weight());
        case ModelColumn::Query: return q.sql();
        case ModelColumn::LastColumn: return QString();
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        switch (col)
        {
        case ModelColumn::QueryId: return Qt::AlignCenter;
        case ModelColumn::Type: return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
        case ModelColumn::Name: return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
        case ModelColumn::Query: return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
        case ModelColumn::Status: return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
        case ModelColumn::RunCount: return Qt::AlignCenter;
        case ModelColumn::SuccessCount: return Qt::AlignCenter;
        case ModelColumn::FailCount: return Qt::AlignCenter;
        case ModelColumn::TotalExecTimeMs: return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        case ModelColumn::TotalFetchTimeMs: return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        case ModelColumn::AverageExecTimeMs: return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        case ModelColumn::AverageFetchTimeMs: return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        case ModelColumn::RowCount: return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        case ModelColumn::AffectedRows: return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        case ModelColumn::Weight: return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        case ModelColumn::LastColumn: return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    else if (role == Qt::SizeHintRole)
    {
        QFontMetrics fm(qApp->font());
        switch (col)
        {
        case ModelColumn::QueryId: return {};
        case ModelColumn::Type: return {};
        case ModelColumn::Name: return {};
        case ModelColumn::Status: return QSize(fm.horizontalAdvance("WIn esecuzione (99)W"), fm.height());
        case ModelColumn::RunCount: return {};
        case ModelColumn::SuccessCount: return {};
        case ModelColumn::FailCount: return {};
        case ModelColumn::TotalExecTimeMs: return {};
        case ModelColumn::TotalFetchTimeMs: return {};
        case ModelColumn::AverageExecTimeMs: return {};
        case ModelColumn::AverageFetchTimeMs: return {};
        case ModelColumn::RowCount: return {};
        case ModelColumn::AffectedRows: return {};
        case ModelColumn::Weight: return QSize(fm.horizontalAdvance("WWW.WWW.WWW"), fm.height());
        case ModelColumn::Query: return QSize(fm.horizontalAdvance("SELECT * FROM TABLE_WITH_QUITE_LONG_NAME ORDER BY SOME_FIELD, OTHER_FIELD;"), fm.height());
        case ModelColumn::LastColumn: return {};
        }
    }
    else if (role == Qt::ForegroundRole)
    {
        if (q.failCount() > 0)
            return QBrush(Qt::red);
    }
    return {};
}

QVariant QueryStatsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal
            || role != Qt::DisplayRole)
        return QAbstractTableModel::headerData(section, orientation, role);

    ModelColumn col = static_cast<ModelColumn>(section);

    switch (col)
    {
    case ModelColumn::QueryId: return tr("ID");
    case ModelColumn::Type: return tr("Tipologia");
    case ModelColumn::Name: return tr("Nome");
    case ModelColumn::Query: return tr("Query");
    case ModelColumn::Status: return tr("Stato");
    case ModelColumn::RunCount: return tr("Numero\nesecuzioni");
    case ModelColumn::SuccessCount: return tr("Numero\nsuccessi");
    case ModelColumn::FailCount: return tr("Numero\nErrori");
    case ModelColumn::TotalExecTimeMs: return tr("Tempo totale\nesecuzione (ms)");
    case ModelColumn::TotalFetchTimeMs: return tr("Tempo totale\ntrasferimento (ms)");
    case ModelColumn::AverageExecTimeMs: return tr("Tempo medio\nesecuzione (ms)");
    case ModelColumn::AverageFetchTimeMs: return tr("Tempo medio\ntrasferimento (ms)");
    case ModelColumn::RowCount: return tr("Numero righe\n(ultimo run)");
    case ModelColumn::AffectedRows: return tr("Righe coinvolte\n(ultimo run)");
    case ModelColumn::Weight: return tr("Peso in bytes\n(stimato)");
    case ModelColumn::LastColumn: return {};
    }
    return {};
}

void QueryStatsModel::clear()
{
    beginResetModel();
    m_queries.clear();
    m_indexes.clear();
    endResetModel();
}

void QueryStatsModel::clearStats()
{
    beginResetModel();
    for (auto &query : m_queries)
        query.clearResults();
    endResetModel();
}

bool QueryStatsModel::appendQuery(const Query &query)
{
    if (m_indexes.contains(query.name()))
        return false;
    const auto cnt = rowCount();
    beginInsertRows(QModelIndex(), cnt, cnt);
    m_queries.insert(cnt, query);
    m_indexes.insert(query.name(), cnt);
    endInsertRows();
    return true;
}

bool QueryStatsModel::updateQuery(const Query &query)
{
    const auto row = rowFor(query.name());
    beginResetModel();
    m_queries.insert(row, query);
    endResetModel();
    return true;
}

bool QueryStatsModel::removeQuery(int row)
{
    beginRemoveRows({}, row, row);
    // I have to rebuild the indexes structure
    // IT'S UGLY! Yes, I know that...
    const auto name = m_queries.value(row).name();  // get the name of the query
    const auto cnt = rowCount();
    for (auto id = row + 1; id < cnt; ++id)
    {
        auto q = m_queries.value(id);
        m_queries.insert(id - 1, q); // move the queries 1 row up
        m_indexes.insert(q.name(), id -1); // and update the corresponding index
    }
    m_indexes.remove(name); // remove the index for the removed query
    m_queries.remove(cnt -1); // finally remove the last query (copy of the one moved at position cnt - 2

    endRemoveRows();
    return true;
}

bool QueryStatsModel::addResult(const QueryTimings &result)
{
    int row = rowFor(result.queryName());
    if (row == -1)
        return false;

    m_queries[row].appendResult(result);
    m_runningQueries.removeOne(row);
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
    return true;
}

Query QueryStatsModel::getToRunAt(int row)
{
    m_runningQueries.append(row);
    emit dataChanged(index(row, static_cast<int>(ModelColumn::Status)), index(row, static_cast<int>(ModelColumn::Status)));
    return m_queries.value(row);
}

Query QueryStatsModel::at(int row) const
{
    return m_queries.value(row);
}

int QueryStatsModel::rowFor(const QString &name) const
{
    return m_indexes.value(name, -1);
}

QList<Query> QueryStatsModel::queryList() const
{
    return m_queries.values();
}
