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
    return m_rowCount;
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

    int row{index.row()};
    Query q{m_queries.value(row)};
    ModelColumn col = static_cast<ModelColumn>(index.column());

//    const auto display = [](auto value){ QLocale locale; locale.setNumberOptions(0); return locale.toString(value); };
    const auto display = [](auto value){ return value; };

    QVariant result{};

    if (role == Qt::DisplayRole)
    {
        auto count = static_cast<int>(m_runningQueries.count(row));
        switch (col)
        {
        case ModelColumn::QueryId:
            result = index.row();
            break;
        case ModelColumn::Type:
            result = q.typeDescription();
            break;
        case ModelColumn::Name:
            result = q.name();
            break;
        case ModelColumn::Status:
            result = count > 0 ? tr("In esecuzione (%1)").arg(count) : QString();
            break;
        case ModelColumn::RunCount:
            result = q.runCount();
            break;
        case ModelColumn::SuccessCount:
            result = q.successCount();
            break;
        case ModelColumn::FailCount:
            result = q.failCount();
            break;
        case ModelColumn::TotalExecTimeMs:
            result = display(q.totalExecTimeMs());
            break;
        case ModelColumn::TotalFetchTimeMs:
            result = display(q.totalFetchTimeMs());
            break;
        case ModelColumn::AverageExecTimeMs:
            result = display(q.averageExecTimeMs());
            break;
        case ModelColumn::AverageFetchTimeMs:
            result = display(q.averageFetchTimeMs());
            break;
        case ModelColumn::RowCount:
            result = display(q.rowCount());
            break;
        case ModelColumn::AffectedRows:
            result = display(q.affectedRowsCount());
            break;
        case ModelColumn::Weight:
            result = display(q.weight());
            break;
        case ModelColumn::Query:
            result = q.query();
            break;
        case ModelColumn::LastColumn:
            result = QString();
            break;
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        switch (col)
        {
        case ModelColumn::QueryId:
            result = Qt::AlignCenter;
            break;
        case ModelColumn::Type:
            result = QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
            break;
        case ModelColumn::Name:
            result = QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
            break;
        case ModelColumn::Query:
            result = QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
            break;
        case ModelColumn::Status:
            result = QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
            break;
        case ModelColumn::RunCount:
            result = Qt::AlignCenter;
            break;
        case ModelColumn::SuccessCount:
            result = Qt::AlignCenter;
            break;
        case ModelColumn::FailCount:
            result = Qt::AlignCenter;
            break;
        case ModelColumn::TotalExecTimeMs:
            result = QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
            break;
        case ModelColumn::TotalFetchTimeMs:
            result = QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
            break;
        case ModelColumn::AverageExecTimeMs:
            result = QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
            break;
        case ModelColumn::AverageFetchTimeMs:
            result = QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
            break;
        case ModelColumn::RowCount:
            result = QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
            break;
        case ModelColumn::AffectedRows:
            result = QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
            break;
        case ModelColumn::Weight:
            result = QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
            break;
        case ModelColumn::LastColumn:
            result = QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
            break;
        }
    }
    else if (role == Qt::SizeHintRole)
    {
        QFontMetrics fm(qApp->font());
        switch (col)
        {
        case ModelColumn::QueryId:
            result = QVariant();
            break;
        case ModelColumn::Type:
            result = QVariant();
            break;
        case ModelColumn::Name:
            result = QVariant();
            break;
        case ModelColumn::Status:
            result = QSize(fm.horizontalAdvance("WIn esecuzione (99)W"), fm.height());
            break;
        case ModelColumn::RunCount:
            result = QVariant();
            break;
        case ModelColumn::SuccessCount:
            result = QVariant();
            break;
        case ModelColumn::FailCount:
            result = QVariant();
            break;
        case ModelColumn::TotalExecTimeMs:
            result = QVariant();
            break;
        case ModelColumn::TotalFetchTimeMs:
            result = QVariant();
            break;
        case ModelColumn::AverageExecTimeMs:
            result = QVariant();
            break;
        case ModelColumn::AverageFetchTimeMs:
            result = QVariant();
            break;
        case ModelColumn::RowCount:
            result = QVariant();
            break;
        case ModelColumn::AffectedRows:
            result = QVariant();
            break;
        case ModelColumn::Weight:
            result = QSize(fm.horizontalAdvance("WWW.WWW.WWW"), fm.height());
            break;
        case ModelColumn::Query:
            result = QSize(fm.horizontalAdvance("SELECT * FROM TABLE ORDER BY FIELD"), fm.height());
            break;
        case ModelColumn::LastColumn:
            result = QVariant();
            break;
        }
    }
    else if (role == Qt::ForegroundRole)
    {
        if (q.failCount() > 0)
            result = QBrush(Qt::red);
    }
    return result;
}

QVariant QueryStatsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal
            || role != Qt::DisplayRole)
        return QAbstractTableModel::headerData(section, orientation, role);

    ModelColumn col = static_cast<ModelColumn>(section);

    switch (col)
    {
    case ModelColumn::QueryId:
        return tr("ID");
    case ModelColumn::Type:
        return tr("Tipologia");
    case ModelColumn::Name:
        return tr("Nome");
    case ModelColumn::Query:
        return tr("Query");
    case ModelColumn::Status:
        return tr("Stato");
    case ModelColumn::RunCount:
        return tr("Numero\r\nesecuzioni");
    case ModelColumn::SuccessCount:
        return tr("Numero\r\nsuccessi");
    case ModelColumn::FailCount:
        return tr("Numero\r\nErrori");
    case ModelColumn::TotalExecTimeMs:
        return tr("Tempo totale\r\nesecuzione (ms)");
    case ModelColumn::TotalFetchTimeMs:
        return tr("Tempo totale\r\ntrasferimento (ms)");
    case ModelColumn::AverageExecTimeMs:
        return tr("Tempo medio\r\nesecuzione (ms)");
    case ModelColumn::AverageFetchTimeMs:
        return tr("Tempo medio\r\ntrasferimento (ms)");
    case ModelColumn::RowCount:
        return tr("Numero righe\r\n(ultimo run)");
    case ModelColumn::AffectedRows:
        return tr("Righe coinvolte\r\n(ultimo run)");
    case ModelColumn::Weight:
        return tr("Peso in bytes\r\n(stimato)");
    case ModelColumn::LastColumn:
        return QString();
    }
    return QString();
}

void QueryStatsModel::clear()
{
    beginResetModel();
    m_queries.clear();
    m_indexes.clear();
    m_rowCount = 0;
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
    if (m_indexes.contains(query.query()))
        return false;
    beginInsertRows(QModelIndex(), m_rowCount, m_rowCount);
    m_queries.insert(m_rowCount, query);
    m_indexes.insert(query.query(), m_rowCount);
    ++m_rowCount;
    endInsertRows();
    return true;
}

bool QueryStatsModel::addResult(const QueryStats &result)
{
    int row = rowFor(result.query());
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

int QueryStatsModel::rowFor(const QString &query) const
{
    return m_indexes.value(query, -1);
}

QList<Query> QueryStatsModel::queryList() const
{
    return m_queries.values();
}
