#include "checkableproxymodel.h"

CheckableProxyModel::CheckableProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void CheckableProxyModel::setSourceModel(QAbstractItemModel *model)
{
    const auto clear = [this](){ m_selectedIndexes.clear(); };

    // connect(model, &QAbstractItemModel::dataChanged, this, clear);
    connect(model, &QAbstractItemModel::rowsInserted, this, clear);
    connect(model, &QAbstractItemModel::columnsInserted, this, clear);
    connect(model, &QAbstractItemModel::rowsRemoved, this, clear);
    connect(model, &QAbstractItemModel::columnsRemoved, this, clear);
    connect(model, &QAbstractItemModel::modelReset, this, clear);

    QSortFilterProxyModel::setSourceModel(model);
}

void CheckableProxyModel::setCheckableColumns(QList<int> columns)
{
    if (m_checkableColumns == columns)
        return;

    m_checkableColumns = columns;
    emit layoutChanged();
}

void CheckableProxyModel::setHideCheckBoxForParentItems(bool value)
{
    if (m_hideCheckBoxForParentItems == value)
        return;

    m_hideCheckBoxForParentItems = value;
    emit layoutChanged();
}

void CheckableProxyModel::setHideCheckBoxForInvalidItems(bool value)
{
    if (m_hideCheckBoxForInvalidItems == value)
        return;

    m_hideCheckBoxForInvalidItems = value;
    emit layoutChanged();
}

Qt::ItemFlags CheckableProxyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QSortFilterProxyModel::flags(index);

    if (m_checkableColumns.contains(index.column()))
        f |= Qt::ItemIsUserCheckable;

    return f;
}

QVariant CheckableProxyModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::CheckStateRole
            && m_checkableColumns.contains(index.column()))
    {
        bool hide{false};

        if (m_hideCheckBoxForParentItems
                && index.model()->rowCount(index) > 0)
            hide = true;
        if (m_hideCheckBoxForInvalidItems
                && !index.data().isValid())
            hide = true;

        if (hide)
            return {};
        else
            return m_selectedIndexes.contains(mapToSource(index)) ? Qt::Checked : Qt::Unchecked;
    }

    return QSortFilterProxyModel::data(index, role);
}

bool CheckableProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole
            && m_checkableColumns.contains(index.column()))
    {
        bool processed{true};
        QModelIndex sourceIndex = mapToSource(index);
        if (value.toInt() == Qt::Checked)
        {
            if (!m_selectedIndexes.contains(sourceIndex))
                m_selectedIndexes.append(sourceIndex);
        }
        else if (value.toInt() == Qt::Unchecked)
            m_selectedIndexes.removeAll(sourceIndex);
        else
            processed = false;

        if (processed)
        {
            emit dataChanged(index, index, QVector<int>{role});
            emit selectedIndexesCountChanged(m_selectedIndexes.count());
        }

        return processed;
    }

    return QSortFilterProxyModel::setData(index, value, role);
}

QList<QModelIndex> CheckableProxyModel::selectedSourceIndexes() const
{
    return m_selectedIndexes;
}

void CheckableProxyModel::selectAllIndexes()
{
    selectAllIndexesInColumns(m_checkableColumns);
}

void CheckableProxyModel::selectAllIndexesInColumns(QList<int> columns)
{
    if (columns.isEmpty())
        columns = m_checkableColumns;
    int missingColumns = std::accumulate(std::begin(columns), std::end(columns), 0, [this](int missingCount, int column){ return missingCount + (m_checkableColumns.contains(column) ? 0 : 1); });
    if (missingColumns > 0)
        return;

    beginResetModel();
    m_selectedIndexes.clear();
    for (int row = 0; row < rowCount(); ++row)
        for (int column : columns)
            m_selectedIndexes.append(mapToSource(index(row, column)));
    endResetModel();
    emit selectedIndexesCountChanged(m_selectedIndexes.count());
}

void CheckableProxyModel::clearSelectedIndexes()
{
    if (!m_selectedIndexes.isEmpty())
    {
        beginResetModel();
        m_selectedIndexes.clear();
        endResetModel();
        emit selectedIndexesCountChanged(0);
    }
}
