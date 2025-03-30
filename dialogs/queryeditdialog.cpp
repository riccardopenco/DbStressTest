#include "queryeditdialog.h"
#include "ui_queryeditdialog.h"

#include <QTimer>
#include <QMessageBox>

QueryEditDialog::QueryEditDialog(QWidget *parent) : QDialog(parent), ui(new Ui::QueryEditDialog)
{
    ui->setupUi(this);
    QTimer::singleShot(0, this, [this](){ setupForm({}); });
}

QueryEditDialog::QueryEditDialog(st::QueryDef query, QWidget *parent) : QDialog(parent), ui(new Ui::QueryEditDialog)
{
    ui->setupUi(this);
    QTimer::singleShot(0, this, [this, &query](){ setupForm(query); });
}

QueryEditDialog::~QueryEditDialog()
{
    delete ui;
}

st::QueryDef QueryEditDialog::query() const
{
    return st::QueryDef(ui->name->text(), ui->sql->toPlainText(), st::QueryDef::typeFromDescr(ui->type->currentText()));
}

void QueryEditDialog::setupForm(const st::QueryDef &query)
{
    connect(ui->buttonConfirm, &QPushButton::clicked, this, &QueryEditDialog::Confirm);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &QueryEditDialog::reject);

    ui->type->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
    ui->type->addItems(st::QueryDef::allTypeDescr());

    if (query.isValid())
    {
        ui->type->setCurrentIndex(ui->type->findText(query.typeDescr()));
        ui->name->setText(query.name());
        ui->name->setEnabled(false);
        ui->sql->setPlainText(query.sql());
    }
    else
    {
        ui->type->setCurrentIndex(0);
        ui->name->setEnabled(true);
    }
}

void QueryEditDialog::Confirm()
{
    auto type = st::QueryDef::typeFromDescr(ui->type->currentText());
    auto q = st::QueryDef(ui->name->text(), ui->sql->toPlainText(), type);
    if (!q.isValid())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle(tr("Errore"));
        msg.setText(tr("Devono essere definiti sia l'identificativo che il codice"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }
    QMessageBox msg;
    msg.setIcon(QMessageBox::Warning);
    msg.setWindowTitle(tr("Conferma"));
    msg.setText(tr("Attenzione, il codice della query non viene verificato e potrebbe causare perdita di dati se non è corretto.\n"
                   "Salvo comunque la query?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    auto res = msg.exec();
    if (res == QMessageBox::No)
        return;
    accept();
}
