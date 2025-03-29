#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T18:22:53
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DbStressTest
TEMPLATE = app

include(3rdparty/QtXlsxWriter/src/xlsx/qtxlsx.pri)
include(3rdparty/ZelandoQtLib/zelandoqtlib.pri)

CONFIG += c++14

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = stable.h
CONFIG += precompile_header

SOURCES += main.cpp\
        mainwindow.cpp \
    dialogs/sqlconnectiondialog.cpp \
    objects/querystats.cpp \
    objects/queryworker.cpp \
    objects/querycontroller.cpp \
    objects/query.cpp \
    models/querystatsmodel.cpp \
    models/querymodel.cpp \
    helpers/xlsxhelper.cpp \
    connectionparameters.cpp \
    helpers/settingshelper.cpp

HEADERS  += mainwindow.h \
    dialogs/sqlconnectiondialog.h \
    objects/querystats.h \
    objects/queryworker.h \
    objects/querycontroller.h \
    objects/query.h \
    models/querystatsmodel.h \
    models/querymodel.h \
    helpers/xlsxhelper.h \
    connectionparameters.h \
    helpers/settingshelper.h \
    version.h \
    stable.h

FORMS    += mainwindow.ui \
    dialogs/sqlconnectiondialog.ui

DISTFILES += \
    other/settings.ini \
    DbStressTest.rc

RC_FILE = DbStressTest.rc

QMAKE_TARGET_COMPANY = "Zelando"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2013-2016 Zelando <info@zelando.com>"
QMAKE_TARGET_DESCRIPTION = "Simple application to stress test a database"

RESOURCES += \
    resources.qrc
