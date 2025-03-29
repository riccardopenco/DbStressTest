#ifndef NUMERICDELEGATE_H
#define NUMERICDELEGATE_H

#include <QStyledItemDelegate>
#include <QLocale>

class NumericDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NumericDelegate(QObject *parent = nullptr);
    explicit NumericDelegate(int decimals, QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QString displayText(const QVariant &value, const QLocale &locale) const override;

private:
    int m_decimals{-1};
};

#endif // NUMERICDELEGATE_H
