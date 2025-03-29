#ifndef CONNECTIONPARAMETERS_H
#define CONNECTIONPARAMETERS_H

#include <QString>

class ConnectionParameters
{
public:
    static QString driverName();
    static void setDriverName(const QString &driverName);

    static QString databaseName();
    static void setDatabaseName(const QString &databaseName);

    static QString userName();
    static void setUserName(const QString &userName);

    static QString password();
    static void setPassword(const QString &password);

    static QString hostName();
    static void setHostName(const QString &hostName);

    static int port();
    static void setPort(int port);

    static QString nativeClientVersion();
    static void setNativeClientVersion(QString nativeClientVersion);

    static bool useConnectionString();
    static void setUseConnectionString(bool useConnectionString);

    static QString testConnectionName();
    static QString defaultConnectionName();

private:
    static QString m_driverName;
    static QString m_databaseName;
    static QString m_userName;
    static QString m_password;
    static QString m_hostName;
    static int m_port;
    static QString m_nativeClientVersion;
    static bool m_useConnectionString;
};

#endif // CONNECTIONPARAMETERS_H
