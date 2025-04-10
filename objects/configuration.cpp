#include "configuration.h"

namespace st {

Configuration::Configuration()
{
}

const DB &Configuration::db() const
{
    return m_db;
}

void Configuration::setDb(const DB &newDb)
{
    m_db = newDb;
}

const QMap<QString, QueryDef> &Configuration::queries() const
{
    return m_queries;
}

void Configuration::removeQuery(const QString &name)
{
    m_queries.remove(name);
}

void Configuration::addQuery(QueryDef newQuery)
{
    m_queries.insert(newQuery.name(), std::move(newQuery));
}

void Configuration::setQueries(const QMap<QString, QueryDef> &newQueries)
{
    m_queries = newQueries;
}

bool Configuration::isValid() const
{
    return m_db.isValid() && !m_queries.empty();
}

} // namespace st
