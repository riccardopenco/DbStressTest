#ifndef XLSXHELPER_H
#define XLSXHELPER_H

#include "xlsxdocument.h"

#include <QSqlQuery>
#include <QAbstractItemModel>

class XlsxHelper
{
public:
    static bool dumpQueryToXlsxNoThrow(const QString &fileName, const QString queryString, bool includeHeaders = true);
    static bool dumpQueryToXlsxNoThrow(const QString &fileName, QSqlQuery &query, bool includeHeaders = true);
    static void dumpQueryToXlsx(const QString &fileName, const QString queryString, bool includeHeaders = true);
    static void dumpQueryToXlsx(const QString &fileName, QSqlQuery &query, bool includeHeaders = true);
    static void dumpQueryToXlsx(QXlsx::Document &xlsx, QSqlQuery &query, bool includeHeaders = true);

    static bool dumpModelToXlsxNoThrow(const QString &fileName, const QAbstractItemModel &model, bool includeHeaders = true);
    static void dumpModelToXlsx(const QString &fileName, const QAbstractItemModel &model, bool includeHeaders = true);
    static void dumpModelToXlsx(QXlsx::Document &xlsx, const QAbstractItemModel &model, bool includeHeaders = true);
    static void dumpModelToXlsxWithForegroundRole(QXlsx::Document &xlsx, const QAbstractItemModel &model, bool includeHeaders = true);
};

#endif // XLSXHELPER_H
