#include "numericdelegate.h"

NumericDelegate::NumericDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

NumericDelegate::NumericDelegate(int decimals, QObject *parent) :
    QStyledItemDelegate(parent),
    m_decimals{decimals}
{
}

void NumericDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem myOption{option};
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
    QStyledItemDelegate::paint(painter, myOption, index);
}

QString NumericDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    bool ok;
    double val = value.toDouble(&ok);
    QLocale myLocale{locale};
    myLocale.setNumberOptions(QLocale::DefaultNumberOptions);   // this should enable the use of group separators

    if (ok)
    {
        if (m_decimals == -1)
            return myLocale.toString(val);
        else
            return myLocale.toString(val, 'f', m_decimals);
    }
    else
        return QStringLiteral("");
}
