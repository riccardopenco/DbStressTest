#include "querydef.h"

#include <QStringList>

namespace st
{

QueryDef::QueryDef(const QString &name, const QString &sql, Type type)
    : m_name(name)
    , m_sql(sql)
    , m_type(type)
{
}

QString QueryDef::name() const
{
    return m_name;
}

void QueryDef::setName(const QString &newName)
{
    m_name = newName;
}

QString QueryDef::sql() const
{
    return m_sql;
}

void QueryDef::setSql(const QString &newSql)
{
    m_sql = newSql;
}

QString QueryDef::typeDescr() const
{
    switch (m_type) {
    case Type::Generic      : return QStringLiteral("Generic");
    case Type::Performance  : return QStringLiteral("Performance");
    case Type::Bandwidth    : return QStringLiteral("Bandwidth");
    }
    return {};
}

QueryDef::Type QueryDef::type() const
{
    return m_type;
}

void QueryDef::setType(Type newType)
{
    m_type = newType;
}

bool QueryDef::isValid() const
{
    return !m_name.isEmpty() && !m_sql.isEmpty();
}

QStringList QueryDef::allTypeDescr()
{
    return
      {
        QStringLiteral("Generic"),
        QStringLiteral("Performance"),
        QStringLiteral("Bandwidth"),
      };
}

QueryDef::Type QueryDef::typeFromDescr(const QString &typeDescr)
{
    if (typeDescr.compare(QStringLiteral("Generic"), Qt::CaseInsensitive) == 0)
        return Type::Generic;
    if (typeDescr.compare(QStringLiteral("Performance"), Qt::CaseInsensitive) == 0)
        return Type::Performance;
    if (typeDescr.compare(QStringLiteral("Bandwidth"), Qt::CaseInsensitive) == 0)
        return Type::Bandwidth;
    return Type::Generic;
}

} // namespace st
