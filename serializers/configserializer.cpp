#include "configserializer.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include<QDebug>
namespace detail {

st::DB dbFromJson(const QJsonObject &json)
{
    return st::DB
      {
        .name = json["name"].toString(),
        .specs = {
          .type                       = json["type"].toString(),
          .connectOptions             = json["connectOptions"].toString(),
          .hostname                   = json["hostname"].toString(),
          .port                       = json["port"].toInt(),
          .databaseName               = json["databaseName"].toString(),
          .username                   = json["username"].toString(),
          .password                   = json["password"].toString(),
          .numericalPrecisionPolicy   = QSql::LowPrecisionDouble,
        }
      };
}

QJsonObject dbToJson(const st::DB &db)
{
    auto json = QJsonObject();

    json["name"] = db.name;
    json["type"] = db.specs.type;
    json["connectOptions"] = db.specs.connectOptions;
    json["hostname"] = db.specs.hostname;
    json["port"] = db.specs.port;
    json["databaseName"] = db.specs.databaseName;
    json["username"] = db.specs.username;
    json["password"] = db.specs.password;
    // QSql::NumericalPrecisionPolicy numericalPrecisionPolicy = QSql::LowPrecisionDouble;

    return json;
}

st::QueryDef querDefFromJson(const QJsonObject &json)
{
    auto q = st::QueryDef();

    q.setName(json["name"].toString());
    q.setSql(json["sql"].toString());
    q.setType(st::QueryDef::typeFromDescr(json["type"].toString()));

    return q;
}

QJsonObject querDefToJson(const st::QueryDef &qd)
{
    auto json = QJsonObject();

    json["name"] = qd.name();
    json["sql"] = qd.sql();
    json["type"] = qd.typeDescr();

    return json;
}

st::Configuration cfgFromJson(const QJsonObject &json)
{
    auto cfg = st::Configuration();

    cfg.setDb(detail::dbFromJson(json["db"].toObject()));
    const auto queries = json["queries"].toArray();
    for (const auto &q : queries)
        cfg.addQuery(detail::querDefFromJson(q.toObject()));
    return cfg;
}

QJsonObject cfgToJson(const st::Configuration &cfg)
{
    auto json = QJsonObject();

    json["db"] = detail::dbToJson(cfg.db());
    auto ar = QJsonArray();
    for (const auto &q : cfg.queries())
        ar.append(detail::querDefToJson(q));
    json["queries"] = ar;

    return json;
}

} // namespace detail

namespace st
{

Configuration ConfigSerializer::openConfiguration(const QString &filename)
{
    auto loadFile = QFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open save file.");
        return {};
    }

    auto data = loadFile.readAll();
    auto doc = QJsonDocument::fromJson(data);

    auto cfg = detail::cfgFromJson(doc.object());

    return cfg;
}

bool ConfigSerializer::saveConfiguration(const Configuration &cfg, const QString &filename)
{
    auto saveFile = QFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return false;
    }

    auto json = detail::cfgToJson(cfg);
    saveFile.write(QJsonDocument(json).toJson(QJsonDocument::JsonFormat::Indented));

    return true;
}

} // namespace st
