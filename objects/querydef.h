#ifndef QUERYDEF_H
#define QUERYDEF_H

#include <QString>

namespace st
{

class QueryDef
{
public:
    enum class Type
    {
        Generic,
        Performance,
        Bandwidth
    };

    QueryDef() = default;
    QueryDef(const QueryDef &) = default;
    QueryDef(QueryDef &&) = default;
    explicit QueryDef(const QString &name, const QString &sql, Type type = Type::Generic);
    ~QueryDef() = default;

    QueryDef &operator=(const QueryDef &) = default;
    QueryDef &operator=(QueryDef &&) = default;

    QString name() const;
    void setName(const QString &newName);

    QString sql() const;
    void setSql(const QString &newSql);

    Type type() const;
    QString typeDescr() const;
    void setType(Type newType);

    bool isValid() const;

    static Type typeFromDescr(const QString &typeDescr);
    static QStringList allTypeDescr();

private:
    QString m_name;
    QString m_sql;
    Type    m_type = Type::Generic;
};

} // namespace st

#endif // QUERYDEF_H
