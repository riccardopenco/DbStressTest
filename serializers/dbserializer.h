#ifndef DBSERIALIZER_H
#define DBSERIALIZER_H

#include "db/dbcommon.h"

#include<QJsonObject>

namespace st
{

class DBSerializer
{
public:
    static DB fromJson(const QJsonObject &json);
    static QJsonObject toJson(const DB &db);
};

} // namespace st

#endif // DBSERIALIZER_H
