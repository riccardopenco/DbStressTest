#include "mainwindow.h"
#include "dialogs/sqlconnectiondialog.h"

#include "version.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle(QStyleFactory::create("Fusion"));    // Fusion, Windows11, WindowsVista, Windows

    a.addLibraryPath(".");
    a.setWindowIcon(QIcon(":/img/logo.png"));
    a.setApplicationName(VER_FILEDESCRIPTION_STR);
    a.setApplicationVersion(VER_PRODUCTVERSION_STR);
    a.setOrganizationName(VER_COMPANYNAME_STR);
    a.setOrganizationDomain(VER_COMPANYDOMAIN_STR);

    MainWindow w;
    w.show();
    return a.exec();
}
