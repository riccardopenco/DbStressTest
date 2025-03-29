#include "sqlconnectiondialog.h"
#include "ui_sqlconnectiondialog.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>

SqlConnectionDialog::SqlConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlConnectionDialog)
{
    ui->setupUi(this);

    connect(ui->driver, &QComboBox::currentTextChanged,
            [this](const QString &driver)
            {
                bool enabled{driver.compare(QStringLiteral("QODBC"), Qt::CaseInsensitive) == 0};
                if (!enabled)
                    ui->useConnectionString->setChecked(false);
                ui->useConnectionString->setEnabled(enabled);
                ui->nativeClientVersion->setEnabled(enabled);
            });
    connect(ui->buttonTest, &QPushButton::clicked, this, &SqlConnectionDialog::testConnection);
    connect(ui->buttonConnect, &QPushButton::clicked, this, &SqlConnectionDialog::openConnection);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &SqlConnectionDialog::reject);

    QStringList drivers = QSqlDatabase::drivers();

    // remove compat names
    drivers.removeAll("QMYSQL3");
    drivers.removeAll("QOCI8");
    drivers.removeAll("QODBC3");
    drivers.removeAll("QPSQL7");
    drivers.removeAll("QTDS7");

//    if (!drivers.contains("QSQLITE"))
//        ui.dbCheckBox->setEnabled(false);

    ui->driver->addItems(drivers);

    QTimer::singleShot(0, this, &SqlConnectionDialog::setUiParameters);
//    setUiParameters();
}

SqlConnectionDialog::~SqlConnectionDialog()
{
    delete ui;
}

QString SqlConnectionDialog::driverName() const
{
    return ui->driver->currentText();
}

QString SqlConnectionDialog::databaseName() const
{
    return ui->databaseName->text();
}

QString SqlConnectionDialog::userName() const
{
    return ui->userName->text();
}

QString SqlConnectionDialog::password() const
{
    return ui->password->text();
}

QString SqlConnectionDialog::hostname() const
{
    return ui->hostName->text();
}

int SqlConnectionDialog::port() const
{
    return ui->port->value();
}

void SqlConnectionDialog::testConnection()
{
    QMessageBox msg;
    if (tryConnection(ConnectionParameters::testConnectionName()))
    {
        QSqlDatabase::database(ConnectionParameters::testConnectionName()).close();
        msg.setIcon(QMessageBox::Information);
        msg.setText(tr("Connessione con il database effettuata correttamente"));
        ui->testResult->setText(tr("Test effettuato correttamente"));
    }
    else
    {
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("Errore durante la connessione con il server:\n").append(QSqlDatabase::database(ConnectionParameters::testConnectionName()).lastError().text()));
        ui->testResult->setText(tr("Errore di connessione"));
    }

    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
    QSqlDatabase::removeDatabase(ConnectionParameters::testConnectionName());
}

void SqlConnectionDialog::openConnection()
{
    QMessageBox msg;
    if (ui->driver->currentText().isEmpty())
    {
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle(tr("Driver database"));
        msg.setText(tr("Devi selezionare il driver del database"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ui->driver->setFocus();
    }
    else
    {
        if (QSqlDatabase::connectionNames().contains(ConnectionParameters::defaultConnectionName()))
        {
            QSqlDatabase::database(ConnectionParameters::defaultConnectionName()).close();
            QSqlDatabase::removeDatabase(ConnectionParameters::defaultConnectionName());
        }
        if (tryConnection())
        {
            setConnectionParameters();
            accept();
        }
        else
        {
            msg.setIcon(QMessageBox::Critical);
            msg.setText(tr("Errore durante la connessione con il server:\n").append(QSqlDatabase::database(ConnectionParameters::defaultConnectionName()).lastError().text()));
            ui->testResult->setText(tr("Errore di connessione"));
        }
    }
}

void SqlConnectionDialog::setUiParameters()
{
    if (!ConnectionParameters::driverName().isEmpty())
        ui->driver->setCurrentIndex(ui->driver->findText(ConnectionParameters::driverName()));
    ui->useConnectionString->setChecked(ConnectionParameters::useConnectionString());
    if (!ConnectionParameters::databaseName().isEmpty())
        ui->databaseName->setText(ConnectionParameters::databaseName());
    if (!ConnectionParameters::userName().isEmpty())
        ui->userName->setText(ConnectionParameters::userName());
    if (!ConnectionParameters::password().isEmpty())
        ui->password->setText(ConnectionParameters::password());
    if (!ConnectionParameters::hostName().isEmpty())
        ui->hostName->setText(ConnectionParameters::hostName());
    ui->port->setValue(ConnectionParameters::port());
    QStringList values{ConnectionParameters::nativeClientVersion().split(".")};
    double value = static_cast<double>(values.value(0, QStringLiteral("11")).toInt()) + static_cast<double>(values.value(1, QStringLiteral("0")).toInt()) / 10.0;
    ui->nativeClientVersion->setValue(value);
}

void SqlConnectionDialog::setConnectionParameters()
{
    ConnectionParameters::setDriverName(ui->driver->currentText());
    ConnectionParameters::setUseConnectionString(ui->useConnectionString->isChecked());
    ConnectionParameters::setDatabaseName(ui->databaseName->text());
    ConnectionParameters::setUserName(ui->userName->text());
    ConnectionParameters::setPassword(ui->password->text());
    ConnectionParameters::setHostName(ui->hostName->text());
    ConnectionParameters::setPort(ui->port->value());
    const int major{static_cast<int>(ui->nativeClientVersion->value())};
    const int minor{static_cast<int>((ui->nativeClientVersion->value() - major) * 10)};
    const QString nacli{QString("%1.%2").arg(major).arg(minor)};
    ConnectionParameters::setNativeClientVersion(nacli);
}

QString SqlConnectionDialog::generateConnectionString() const
{
    const int major{static_cast<int>(ui->nativeClientVersion->value())};
    const int minor{static_cast<int>((ui->nativeClientVersion->value() - major) * 10)};
    const QString nacli{QString("%1.%2").arg(major).arg(minor)};
    return QString("Driver={SQL Server Native Client %1};Server=%2;Database=%3;Uid=%4;Pwd=%5;")
            .arg(nacli, ui->hostName->text(), ui->databaseName->text(), ui->userName->text(), ui->password->text());
}

bool SqlConnectionDialog::tryConnection(const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(ui->driver->currentText(), connectionName);

    if (ui->useConnectionString->isChecked())
    {
        ui->connectionString->setText(generateConnectionString());
        db.setDatabaseName(generateConnectionString());
    }
    else
    {
        if (!ui->databaseName->text().isEmpty())
            db.setDatabaseName(ui->databaseName->text());
        if (!ui->userName->text().isEmpty())
            db.setUserName(ui->userName->text());
        if (!ui->password->text().isEmpty())
            db.setPassword(ui->password->text());
        if (!ui->hostName->text().isEmpty())
            db.setHostName(ui->hostName->text());
    }
//    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", connectionName);

//    db.setDatabaseName("Driver={SQL Server Native Client 11.0};Server=192.168.1.189;Database=ZAC;Uid=sa;Pwd=us%1admin;");

    return db.open();
}
