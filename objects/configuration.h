#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "db/dbcommon.h"
#include "querydef.h"

#include <QMap>

namespace st {

class Configuration
{
public:
    Configuration();

    const DB &db() const;
    void setDb(const DB &newDb);

    const QMap<QString, QueryDef> &queries() const;
    void removeQuery(const QString &name);
    void addQuery(QueryDef newQuery);
    void setQueries(const QMap<QString, QueryDef> &newQueries);

    bool isValid() const;

private:
    DB m_db;
    QMap<QString, QueryDef> m_queries;
};

} // namespace st

#endif // CONFIGURATION_H
