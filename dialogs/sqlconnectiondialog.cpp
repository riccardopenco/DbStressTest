#include "sqlconnectiondialog.h"
#include "ui_sqlconnectiondialog.h"

#include "db/dbmanager.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>

SqlConnectionDialog::SqlConnectionDialog(const st::DB &db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlConnectionDialog)
{
    ui->setupUi(this);
    QTimer::singleShot(0, this, [this, &db](){ setupForm(db); });
}

SqlConnectionDialog::~SqlConnectionDialog()
{
    delete ui;
}

st::DB SqlConnectionDialog::db() const
{
    return st::DB
      {
        .name   = name(),
        .specs  = specs(),
      };
}

st::DBConnectionSpecs SqlConnectionDialog::specs() const
{
    return
      {
        .type                       = driverName(),
        .connectOptions             = connectionOptions(),
        .hostname                   = hostname(),
        .port                       = port(),
        .databaseName               = databaseName(),
        .username                   = username(),
        .password                   = password(),
        .numericalPrecisionPolicy   = QSql::LowPrecisionDouble,
      };
}

void SqlConnectionDialog::setName(const QString &name) const
{
    ui->name->setText(name);
}

void SqlConnectionDialog::setDriverName(const QString &driverName) const
{
    if (driverName.isEmpty())
        ui->driver->setCurrentIndex(-1);
    else
        ui->driver->setCurrentIndex(ui->driver->findText(driverName));
}

void SqlConnectionDialog::setConnectionOptions(const QString &connectionOptions) const
{
    ui->connectionOptions->setText(connectionOptions);
}

void SqlConnectionDialog::setDatabaseName(const QString &databaseName) const
{
    ui->databaseName->setText(databaseName);
}

void SqlConnectionDialog::setUsername(const QString &username) const
{
    ui->username->setText(username);
}

void SqlConnectionDialog::setPassword(const QString &password) const
{
    ui->password->setText(password);
}

void SqlConnectionDialog::setHostname(const QString &hostname) const
{
    ui->hostname->setText(hostname);
}

void SqlConnectionDialog::setPort(int port) const
{
    ui->port->setValue(port);
}

QString SqlConnectionDialog::name() const
{
    return ui->name->text();
}

QString SqlConnectionDialog::driverName() const
{
    return ui->driver->currentText();
}

QString SqlConnectionDialog::connectionOptions() const
{
    return ui->connectionOptions->text();
}

QString SqlConnectionDialog::databaseName() const
{
    return ui->databaseName->text();
}

QString SqlConnectionDialog::username() const
{
    return ui->username->text();
}

QString SqlConnectionDialog::password() const
{
    return ui->password->text();
}

QString SqlConnectionDialog::hostname() const
{
    return ui->hostname->text();
}

int SqlConnectionDialog::port() const
{
    return ui->port->value();
}

void SqlConnectionDialog::testConnection()
{
    auto res = tryConnection(QStringLiteral("__test_connection__"));
    QMessageBox msg;
    if (res.isOk)
    {
        msg.setIcon(QMessageBox::Information);
        msg.setText(tr("Connessione con il database effettuata correttamente"));
        ui->testResult->setText(tr("Test effettuato correttamente"));
    }
    else
    {
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("Errore durante la connessione con il server:\n").append(res.errMsg));
        ui->testResult->setText(tr("Errore di connessione"));
    }

    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void SqlConnectionDialog::Confirm()
{
    if (name().isEmpty())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle(tr("Errore"));
        msg.setText(tr("L'identificativo del database deve essere necessariamente specificato"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ui->name->setFocus();
        return;
    }
    if (!specs().areValid())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle(tr("Errore"));
        msg.setText(tr("La configurazione del database non è corretta"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }
    accept();
}

void SqlConnectionDialog::setupForm(const st::DB &db)
{
    connect(ui->buttonTest, &QPushButton::clicked, this, &SqlConnectionDialog::testConnection);
    connect(ui->buttonConfirm, &QPushButton::clicked, this, &SqlConnectionDialog::Confirm);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &SqlConnectionDialog::reject);

    QStringList drivers = QSqlDatabase::drivers();

    // remove compat names
    drivers.removeAll("QMYSQL3");
    drivers.removeAll("QOCI8");
    drivers.removeAll("QODBC3");
    drivers.removeAll("QPSQL7");
    drivers.removeAll("QTDS7");

    ui->driver->addItems(drivers);

    setUiParameters(db);
}

void SqlConnectionDialog::setUiParameters(const st::DB &db)
{
    ui->name->setText(db.name);

    setDriverName(db.specs.type);
    setConnectionOptions(db.specs.connectOptions);
    setHostname(db.specs.hostname);
    setPort(db.specs.port);
    setDatabaseName(db.specs.databaseName);
    setUsername(db.specs.username);
    setPassword(db.specs.password);
}

st::ConnectionTestResult SqlConnectionDialog::tryConnection(const QString &connectionName)
{
    return st::DBManager::testConnection(connectionName, specs());
}
