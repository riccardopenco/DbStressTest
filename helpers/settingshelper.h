#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include "objects/query.h"

#include <QString>
#include <QList>

class SettingsHelper
{
public:
    static QString fileName();
    static void readConnectionParameters();
    static QList<Query> readQueries();
    static void saveSettings(const QList<Query> &queries);
};

#endif // SETTINGSHELPER_H
