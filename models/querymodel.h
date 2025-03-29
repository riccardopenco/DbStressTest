#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include "objects/query.h"

#include <QAbstractTableModel>

class QueryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum class ModelColumn
    {
        QueryId,
        Type,
        Query,
        LastColumn
    };
    explicit QueryModel(QObject *parent = 0);
    ~QueryModel();

//    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
//    QVariant data(const QModelIndex &index, int role) const override;
//    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
//    Qt::ItemFlags flags(const QModelIndex &index) const override;
//    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
};

#endif // QUERYMODEL_H
