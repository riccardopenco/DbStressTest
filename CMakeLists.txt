cmake_minimum_required(
    VERSION 3.18.0
    FATAL_ERROR
)
project(DbStressTest
    VERSION 1.0.0
    LANGUAGES CXX
)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

set(SOURCES
    main.cpp
    mainwindow.h mainwindow.cpp
    db/dbcommon.h
    db/dbconnection.h db/dbconnection.cpp
    db/dbmanager.h db/dbmanager.cpp
    db/scopedtransaction.h db/scopedtransaction.cpp
    delegates/numericdelegate.h delegates/numericdelegate.cpp
    dialogs/sqlconnectiondialog.h dialogs/sqlconnectiondialog.cpp
    objects/querystats.h objects/querystats.cpp
    objects/queryworker.h objects/queryworker.cpp
    objects/querycontroller.h objects/querycontroller.cpp
    objects/query.h objects/query.cpp
    models/checkableproxymodel.h models/checkableproxymodel.cpp
    models/querystatsmodel.h models/querystatsmodel.cpp
    models/querymodel.h models/querymodel.cpp
    helpers/xlsxhelper.h helpers/xlsxhelper.cpp
    connectionparameters.h

    version.h
    stable.h
    mainwindow.ui
    dialogs/sqlconnectiondialog.ui
)

set(RESOURCES
resources.qrc
)

include(FetchContent)
FetchContent_Declare(
    QXlsx
    GIT_REPOSITORY https://github.com/QtExcel/QXlsx.git
    GIT_TAG        4f188ed320d44a5ec7e1423959518ff940de48d7
    SOURCE_SUBDIR  QXlsx
)

FetchContent_MakeAvailable(QXlsx)

# DISTFILES += \
#     other/settings.ini \
#     DbStressTest.rc

# QMAKE_TARGET_COMPANY = "Zelando"
# QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2013-2016 Zelando <info@zelando.com>"
# QMAKE_TARGET_DESCRIPTION = "Simple application to stress test a database"

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/DbStressTest.rc)

    add_executable(DbStressTest WIN32
        ${RESOURCES}
        ${SOURCES}
        ${WIN32_RESOURCES}
        objects/configuration.h objects/configuration.cpp
        objects/querydef.h objects/querydef.cpp
        serializers/dbserializer.h serializers/dbserializer.cpp
        serializers/configserializer.h serializers/configserializer.cpp
    )
else()
    add_executable(DbStressTest
        ${RESOURCES}
        ${SOURCES}
    )
endif()

set(DEBUG_OPTIONS "-g -Wall -Wextra")
set(RELEASE_OPTIONS "-O2 -Wall -Wextra")
# set(DEBUG_OPTIONS "-Wall -Wextra")
# set(RELEASE_OPTIONS "-Wall -Wextra")

target_compile_options(DbStressTest PUBLIC "$<$<CONFIG:DEBUG>:SHELL:${DEBUG_OPTIONS}>")
target_compile_options(DbStressTest PUBLIC "$<$<CONFIG:RELEASE>:SHELL:${RELEASE_OPTIONS}>")

target_link_libraries(DbStressTest
    #PUBLIC
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    QXlsx::QXlsx
)
