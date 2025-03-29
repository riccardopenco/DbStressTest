#include "settingshelper.h"

#include "connectionparameters.h"

#include <QApplication>
#include <QSettings>

QString SettingsHelper::fileName()
{
    return QApplication::applicationDirPath().append("/settings.ini");
}

void SettingsHelper::readConnectionParameters()
{
    QSettings settings(SettingsHelper::fileName(), QSettings::IniFormat);

    ConnectionParameters::setDriverName(settings.value("Connection/Driver", ConnectionParameters::driverName()).toString());
    ConnectionParameters::setUseConnectionString(settings.value("Connection/UseConnectionString", ConnectionParameters::useConnectionString()).toBool());
    ConnectionParameters::setDatabaseName(settings.value("Connection/DatabaseName", ConnectionParameters::databaseName()).toString());
    ConnectionParameters::setUserName(settings.value("Connection/UserName", ConnectionParameters::userName()).toString());
    ConnectionParameters::setHostName(settings.value("Connection/HostName", ConnectionParameters::hostName()).toString());
    ConnectionParameters::setPort(settings.value("Connection/Port", ConnectionParameters::port()).toInt());
    ConnectionParameters::setNativeClientVersion(settings.value("Connection/NativeClientVersion", ConnectionParameters::nativeClientVersion()).toString());
}

QList<Query> SettingsHelper::readQueries()
{
    QList<Query> list;

    QSettings settings(SettingsHelper::fileName(), QSettings::IniFormat);

    int size = settings.beginReadArray("Queries");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        const auto id = settings.value("TypeID");
        const auto sql = settings.value("Sql");
        const auto name = settings.value("Name");
        list.append(Query(static_cast<Query::Type>(id.toInt()),
                          sql.toString(),
                          name.toString()));
    }
    settings.endArray();
    return list;
}

void SettingsHelper::saveSettings(const QList<Query> &queries)
{
    QSettings settings(SettingsHelper::fileName(), QSettings::IniFormat);

    settings.beginGroup("Connection");
    settings.setValue("Driver", ConnectionParameters::driverName());
    settings.setValue("UseConnectionString", ConnectionParameters::useConnectionString());
    settings.setValue("DatabaseName", ConnectionParameters::databaseName());
    settings.setValue("UserName", ConnectionParameters::userName());
    settings.setValue("HostName", ConnectionParameters::hostName());
    settings.setValue("Port", ConnectionParameters::port());
    settings.setValue("NativeClientVersion", ConnectionParameters::nativeClientVersion());
    settings.endGroup();
    settings.beginWriteArray("Queries", queries.count());
    for (int i = 0; i < queries.count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("TypeID", static_cast<int>(queries.at(i).type()));
        settings.setValue("Name", queries.at(i).name());
        settings.setValue("Sql", queries.at(i).query());
    }
    settings.endArray();
}
