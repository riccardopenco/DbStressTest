#include "connectionparameters.h"

QString ConnectionParameters::m_driverName = QStringLiteral("");
QString ConnectionParameters::m_databaseName = QStringLiteral("");
QString ConnectionParameters::m_userName = QStringLiteral("");
QString ConnectionParameters::m_password = QStringLiteral("");
QString ConnectionParameters::m_hostName = QStringLiteral("");
int ConnectionParameters::m_port = 0;
QString ConnectionParameters::m_nativeClientVersion = QStringLiteral("11.0");
bool ConnectionParameters::m_useConnectionString = false;

QString ConnectionParameters::driverName()
{
    return m_driverName;
}

void ConnectionParameters::setDriverName(const QString &driverName)
{
    m_driverName = driverName;
}

QString ConnectionParameters::databaseName()
{
    return m_databaseName;
}

void ConnectionParameters::setDatabaseName(const QString &databaseName)
{
    m_databaseName = databaseName;
}

QString ConnectionParameters::userName()
{
    return m_userName;
}

void ConnectionParameters::setUserName(const QString &userName)
{
    m_userName = userName;
}

QString ConnectionParameters::password()
{
    return m_password;
}

void ConnectionParameters::setPassword(const QString &password)
{
    m_password = password;
}

QString ConnectionParameters::hostName()
{
    return m_hostName;
}

void ConnectionParameters::setHostName(const QString &hostName)
{
    m_hostName = hostName;
}

int ConnectionParameters::port()
{
    return m_port;
}

void ConnectionParameters::setPort(int port)
{
    m_port = port;
}

QString ConnectionParameters::nativeClientVersion()
{
    return m_nativeClientVersion;
}

void ConnectionParameters::setNativeClientVersion(QString nativeClientVersion)
{
    m_nativeClientVersion = nativeClientVersion;
}

bool ConnectionParameters::useConnectionString()
{
    return m_useConnectionString;
}

void ConnectionParameters::setUseConnectionString(bool useConnectionString)
{
    m_useConnectionString = useConnectionString;
}

QString ConnectionParameters::testConnectionName()
{
    return QStringLiteral("_test_connection_");
}

QString ConnectionParameters::defaultConnectionName()
{
    return QStringLiteral("_dbstresstest_");
}
