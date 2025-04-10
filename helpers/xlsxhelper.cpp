#include "xlsxhelper.h"

#include <QSqlRecord>
#include <QSqlError>
#include <QDir>
#include <QFile>

bool XlsxHelper::dumpQueryToXlsxNoThrow(const QString &fileName, const QString queryString, bool includeHeaders)
{
    if (QFile::exists(fileName)
            && !QFile::remove(fileName))
        return false;

    QXlsx::Document xlsx;

    QSqlQuery q;
    q.setForwardOnly(true);
    q.prepare(queryString);

    dumpQueryToXlsx(xlsx, q, includeHeaders);
    return xlsx.saveAs(fileName);
}

bool XlsxHelper::dumpQueryToXlsxNoThrow(const QString &fileName, QSqlQuery &query, bool includeHeaders)
{
    if (QFile::exists(fileName)
            && !QFile::remove(fileName))
        return false;

    QXlsx::Document xlsx;

    dumpQueryToXlsx(xlsx, query, includeHeaders);
    return xlsx.saveAs(fileName);
}

void XlsxHelper::dumpQueryToXlsx(const QString &fileName, const QString queryString, bool includeHeaders)
{
    if (QFile::exists(fileName)
            && !QFile::remove(fileName))
        throw QObject::tr("Impossibile eliminare il file ").append(QDir::toNativeSeparators(fileName));

    QXlsx::Document xlsx;

    QSqlQuery q;
    q.setForwardOnly(true);
    q.prepare(queryString);

    dumpQueryToXlsx(xlsx, q, includeHeaders);
    if (!xlsx.saveAs(fileName))
        throw QObject::tr("Errore generico durante il salvataggio del file ").append(QDir::toNativeSeparators(fileName));
}

void XlsxHelper::dumpQueryToXlsx(const QString &fileName, QSqlQuery &query, bool includeHeaders)
{
    if (QFile::exists(fileName)
            && !QFile::remove(fileName))
        throw QObject::tr("Impossibile eliminare il file ").append(QDir::toNativeSeparators(fileName));

    QXlsx::Document xlsx;

    dumpQueryToXlsx(xlsx, query, includeHeaders);
    if (!xlsx.saveAs(fileName))
        throw QObject::tr("Errore generico durante il salvataggio del file ").append(QDir::toNativeSeparators(fileName));
}

void XlsxHelper::dumpQueryToXlsx(QXlsx::Document &xlsx, QSqlQuery &query, bool includeHeaders)
{
    QXlsx::Format defaultFormat;
    QXlsx::Format headerFormat;
    QXlsx::Format dateFormat;
    QXlsx::Format dateTimeFormat;

    defaultFormat.setFontName("Arial");
    defaultFormat.setFontSize(10);

    headerFormat = defaultFormat;
    headerFormat.setFontBold(true);

    dateFormat = defaultFormat;
    dateFormat.setNumberFormat("dd/mm/yyyy");

    dateTimeFormat = defaultFormat;
    dateTimeFormat.setNumberFormat("dd/mm/yyyy hh:MM:ss");

    if (!query.isActive()
            && !query.exec())
        throw QObject::tr("Impossibile eseguire la query:\n").append(query.lastError().text());

    int columns = query.record().count();
    int r = 0;

    if (includeHeaders)
    {
        for (int c = 0; c < columns; ++c)
        {
            xlsx.write(r + 1, c + 1, query.record().fieldName(c), headerFormat);
        }
        ++r;
    }

    QXlsx::Format format;

    while (query.next())
    {
        for (int c = 0; c < columns; ++c)
        {
            QVariant value = query.value(c);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if (value.typeId() == QMetaType::QDate)
                format = dateFormat;
            else if (value.typeId() == QMetaType::QDateTime)
                format = dateTimeFormat;
            else
                format = defaultFormat;
#else
            if (value.type() == QMetaType::QDate)
                format = dateFormat;
            else if (value.type() == QMetaType::QDateTime)
                format = dateTimeFormat;
            else
                format = defaultFormat;
#endif

            xlsx.write(r + 1, c + 1, value, format);
        }

        ++r;
    }
}

bool XlsxHelper::dumpModelToXlsxNoThrow(const QString &fileName, const QAbstractItemModel &model, bool includeHeaders)
{
    if (QFile::exists(fileName)
            && !QFile::remove(fileName))
        return false;

    QXlsx::Document xlsx;

    dumpModelToXlsxWithForegroundRole(xlsx, model, includeHeaders);
    return xlsx.saveAs(fileName);
}

void XlsxHelper::dumpModelToXlsx(const QString &fileName, const QAbstractItemModel &model, bool includeHeaders)
{
    if (QFile::exists(fileName)
            && !QFile::remove(fileName))
        throw QObject::tr("Impossibile eliminare il file ").append(QDir::toNativeSeparators(fileName));

    QXlsx::Document xlsx;

    dumpModelToXlsxWithForegroundRole(xlsx, model, includeHeaders);
    if (!xlsx.saveAs(fileName))
        throw QObject::tr("Errore generico durante il salvataggio del file ").append(QDir::toNativeSeparators(fileName));
}

void XlsxHelper::dumpModelToXlsx(QXlsx::Document &xlsx, const QAbstractItemModel &model, bool includeHeaders)
{
    QXlsx::Format defaultFormat;
    QXlsx::Format headerFormat;
    QXlsx::Format dateFormat;
    QXlsx::Format dateTimeFormat;

    defaultFormat.setFontName("Arial");
    defaultFormat.setFontSize(10);

    headerFormat = defaultFormat;
    headerFormat.setFontBold(true);

    dateFormat = defaultFormat;
    dateFormat.setNumberFormat("dd/mm/yyyy");

    dateTimeFormat = defaultFormat;
    dateTimeFormat.setNumberFormat("dd/mm/yyyy hh:MM:ss");

    int columns = model.columnCount();
    int rows = model.rowCount();
    int rn = 0;

    if (includeHeaders)
    {
        for (int c = 0; c < columns; ++c)
        {
            xlsx.write(rn + 1, c + 1, model.headerData(c, Qt::Horizontal), headerFormat);
        }
        ++rn;
    }

    QXlsx::Format format;

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < columns; ++c)
        {
            QVariant value = model.data(model.index(r, c), Qt::DisplayRole);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if (value.typeId() == QMetaType::QDate)
                format = dateFormat;
            else if (value.typeId() == QMetaType::QDateTime)
                format = dateTimeFormat;
            else
                format = defaultFormat;
#else
            if (value.type() == QMetaType::QDate)
                format = dateFormat;
            else if (value.type() == QMetaType::QDateTime)
                format = dateTimeFormat;
            else
                format = defaultFormat;
#endif
            xlsx.write(rn + 1, c + 1, value, format);
        }

        ++rn;
    }
}

void XlsxHelper::dumpModelToXlsxWithForegroundRole(QXlsx::Document &xlsx, const QAbstractItemModel &model, bool includeHeaders)
{
    QXlsx::Format defaultFormat;
    QXlsx::Format headerFormat;
    QXlsx::Format dateFormat;
    QXlsx::Format dateTimeFormat;

    defaultFormat.setFontName("Arial");
    defaultFormat.setFontSize(10);

    headerFormat = defaultFormat;
    headerFormat.setFontBold(true);

    dateFormat = defaultFormat;
    dateFormat.setNumberFormat("dd/mm/yyyy");

    dateTimeFormat = defaultFormat;
    dateTimeFormat.setNumberFormat("dd/mm/yyyy hh:MM:ss");

    int columns = model.columnCount();
    int rows = model.rowCount();
    int rn = 0;

    if (includeHeaders)
    {
        for (int c = 0; c < columns; ++c)
        {
            xlsx.write(rn + 1, c + 1, model.headerData(c, Qt::Horizontal), headerFormat);
        }
        ++rn;
    }

    QXlsx::Format format;

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < columns; ++c)
        {
            QVariant value = model.data(model.index(r, c), Qt::DisplayRole);
            QVariant foreground = model.data(model.index(r, c), Qt::ForegroundRole);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if (value.typeId() == QMetaType::QDate)
                format = dateFormat;
            else if (value.typeId() == QMetaType::QDateTime)
                format = dateTimeFormat;
            else
                format = defaultFormat;
#else
            if (value.type() == QMetaType::QDate)
                format = dateFormat;
            else if (value.type() == QMetaType::QDateTime)
                format = dateTimeFormat;
            else
                format = defaultFormat;
#endif
            QColor color = foreground.value<QColor>();
            format.setFontColor(color);

            xlsx.write(rn + 1, c + 1, value, format);
        }

        ++rn;
    }
}
