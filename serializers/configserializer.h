#ifndef CONFIGSERIALIZER_H
#define CONFIGSERIALIZER_H

#include "objects/configuration.h"

#include <QJsonObject>

/*
Example JSON:
{
    "db": {
        "name": "test sqlite",
        "type": "QSQLITE",
        "connectOptions": "",
        "hostname": "",
        "port": "",
        "databaseName": ":memory:",
        "username": "",
        "password": ""
    },
    "queries": [
        {
            "name": "Query #1",
            "sql": "select * from ZAC_Agencies;",
            "type": "GENERIC"
        },
        {
            "name": "Query #2",
            "sql": "select * from ZAC_Promoters;",
            "type": "BANDWIDTH"
        }
    ]
}
*/

namespace st
{

class ConfigSerializer
{
public:
    static Configuration openConfiguration(const QString &filename);
};

} // namespace st

#endif // CONFIGSERIALIZER_H
