#ifndef CHECKABLEPROXYMODEL_H
#define CHECKABLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QList>

class CheckableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CheckableProxyModel(QObject *parent = 0);

    void setSourceModel(QAbstractItemModel *model) override;
    void setCheckableColumns(QList<int> columns);
    void setHideCheckBoxForParentItems(bool value);
    void setHideCheckBoxForInvalidItems(bool value);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::EditRole) const override;
    bool setData(const QModelIndex &index,const QVariant &value, int role = Qt::EditRole) override;

    QList<QModelIndex> selectedSourceIndexes() const;
    void selectAllIndexes();
    void selectAllIndexesInColumns(QList<int> columns);
    void clearSelectedIndexes();

signals:
    void selectedIndexesCountChanged(int count);

private:
    QList<int> m_checkableColumns{};
    QList<QModelIndex> m_selectedIndexes{};
    bool m_hideCheckBoxForParentItems{false};
    bool m_hideCheckBoxForInvalidItems{true};
};

#endif // CHECKABLEPROXYMODEL_H
