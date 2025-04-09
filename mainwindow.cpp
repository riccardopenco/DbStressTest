#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serializers/configserializer.h"
#include "db/dbmanager.h"
#include "helpers/xlsxhelper.h"

#include "dialogs/sqlconnectiondialog.h"
#include "dialogs/queryeditdialog.h"
#include "objects/querytimings.h"
#include "delegates/numericdelegate.h"
#include "xlsxdocument.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QSysInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer::singleShot(0, this, &MainWindow::setupForm);
}

MainWindow::~MainWindow()
{
    // SettingsHelper::saveSettings(m_queryModel.queryList());
    delete ui;
    qDeleteAll(m_workers);
}

void MainWindow::setupForm()
{
    m_workerCount = QThread::idealThreadCount();
    ui->workerCount->setValue(m_workerCount);
    clearStatistics();

    qRegisterMetaType<QueryTimings>("QueryStats");

    m_queryProxyModel.setSourceModel(&m_queryModel);
    m_queryProxyModel.setCheckableColumns({checkableColumn});
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    const QList<int> numericColumns{
      {
        static_cast<int>(QueryStatsModel::ModelColumn::TotalExecTimeMs),
        static_cast<int>(QueryStatsModel::ModelColumn::TotalFetchTimeMs),
        static_cast<int>(QueryStatsModel::ModelColumn::AverageExecTimeMs),
        static_cast<int>(QueryStatsModel::ModelColumn::AverageFetchTimeMs),
        static_cast<int>(QueryStatsModel::ModelColumn::RowCount),
        static_cast<int>(QueryStatsModel::ModelColumn::AffectedRows),
        static_cast<int>(QueryStatsModel::ModelColumn::Weight),
      }
    };
    for (auto column : numericColumns)
        ui->tableView->setItemDelegateForColumn(column, new NumericDelegate());
    ui->tableView->setModel(&m_queryProxyModel);

    ui->buttonPause->hide();

    m_disableControls = { ui->buttonRun, /*ui->buttonPause, ui->buttonStop, */ui->buttonCheckAll, ui->buttonClearStats, ui->buttonStatsToExcel, ui->workerCount, ui->repetitionCount };

    prepareQueries();

    connect(ui->actionCfgOpen, &QAction::triggered, this, &MainWindow::loadConfiguration);
    connect(ui->actionCfgSave, &QAction::triggered, this, &MainWindow::saveConfiguration);
    connect(ui->actionChangeConnection, &QAction::triggered, this, &MainWindow::changeConnection);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->buttonRun, &QPushButton::clicked, this, &MainWindow::start);
    connect(ui->buttonStop, &QPushButton::clicked, this, &MainWindow::stop);
    connect(ui->buttonClearStats, &QPushButton::clicked, &m_queryModel, &QueryStatsModel::clearStats);
    connect(ui->buttonStatsToExcel, &QPushButton::clicked, this, &MainWindow::extractStatsToXlsx);
    connect(ui->buttonCheckAll, &QPushButton::toggled, this, &MainWindow::checkUncheckAllQueries);
    connect(ui->buttonAddQuery, &QPushButton::clicked, this, &MainWindow::addQuery);
    connect(ui->buttonEditQuery, &QPushButton::clicked, this, &MainWindow::editQuery);
    connect(ui->buttonRemoveQuery, &QPushButton::clicked, this, &MainWindow::removeQuery);

    connect(&m_timer, &QTimer::timeout, this, [this]() { m_endTime = QDateTime::currentDateTime(); updateDuration(); });

    ui->buttonCheckAll->setChecked(false);
}

void MainWindow::loadConfiguration()
{
    const auto fileName = QFileDialog::getOpenFileName(this,
                                                       tr("Open File"),
                                                       QString(),
                                                       tr("DbStressTest configuration (*.json *.cfg)"));
    if (fileName.isEmpty())
        return;
    const auto cfg = st::ConfigSerializer::openConfiguration(fileName);
    if (!cfg.isValid())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("La configurazione letta non è valida."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }
    setConfiguration(cfg);
}

void MainWindow::saveConfiguration()
{
    if (!m_cfg.isValid())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("L'attuale configurazione non è valida."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Open File"),
                                                 QString(),
                                                 tr("DbStressTest configuration (*.json *.cfg)"));
    if (fileName.isEmpty())
        return;
    if (!fileName.endsWith(".json", Qt::CaseInsensitive)
        && !fileName.endsWith(".cfg", Qt::CaseInsensitive))
    {
        fileName = fileName.append(".json");
    }
    if (st::ConfigSerializer::saveConfiguration(m_cfg, fileName))
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setText(tr("Configurazione salvata correttamente."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("Non è stato possibile salvare la configurazione."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }
}

void MainWindow::setConfiguration(st::Configuration cfg)
{
    qDeleteAll(m_workers);
    m_workers.clear();
    m_cfg = cfg;
    st::DBManager::setDb(m_cfg.db());
    ui->dbName->setText(m_cfg.db().name);
    ui->hostname->setText(m_cfg.db().specs.hostname);
    ui->databaseName->setText(m_cfg.db().specs.databaseName);
    prepareQueries();
}

void MainWindow::changeConnection()
{
    if (m_running)
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setText(tr("Impossibile cambiare la connessione al database mentre il test è in corso.\n"
                       "Attendi il completamento del test prima di modificare la connessione."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }
    qDeleteAll(m_workers);
    m_workers.clear();

    auto d = SqlConnectionDialog(st::DBManager::getDb());
    if (d.exec() == QDialog::Rejected)
        return;
    auto db = d.db();
    m_cfg.setDb(db);
    st::DBManager::setDb(std::move(db));
    ui->dbName->setText(m_cfg.db().name);
    ui->hostname->setText(m_cfg.db().specs.hostname);
    ui->databaseName->setText(m_cfg.db().specs.databaseName);
}

void MainWindow::start()
{
    m_queriesToRun.clear();

    for (int n = 0; n < ui->repetitionCount->value(); ++n)
    {
        for (const auto &index : m_queryProxyModel.selectedSourceIndexes())
            m_queriesToRun.append(m_queryModel.index(index.row(), 0).data().toInt());
        m_queriesToRunCount = m_queriesToRun.count();
    }
    if (m_queriesToRunCount == 0)
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setText(tr("Selezionare le query da eseguire"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
        return;
    }

    disableControls();
    m_startTime = m_endTime = QDateTime::currentDateTime();
    updateDuration();
    m_timer.start(125);

    m_workerCount = ui->workerCount->value();
    prepareWorkers();

    m_running = true;
    m_stopped = false;
    for (int i = 0; i < m_workerCount; ++i)
    {
        const auto q = nextQuery();
        m_workers.at(i)->process(q.name(), q.sql());
    }
}

void MainWindow::pause()
{
    m_stopped = true;
}

void MainWindow::stop()
{
    m_stopped = true;
}

void MainWindow::addQuery()
{
    auto d = QueryEditDialog();
    if (d.exec() == QDialog::Rejected)
        return;
    auto q = d.query();
    if (!q.isValid())
        return;

    m_queryModel.appendQuery(Query(q));
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::editQuery()
{
    const auto idx = ui->tableView->currentIndex();
    if (!idx.isValid())
        return;

    const auto query = m_queryModel.at(idx.row()).queryDef();
    auto d = QueryEditDialog(query);
    if (d.exec() == QDialog::Rejected)
        return;
    auto q = d.query();
    if (!q.isValid())
        return;

    m_queryModel.updateQuery(Query(q));
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::removeQuery()
{
    const auto idx = ui->tableView->currentIndex();
    if (!idx.isValid())
        return;

    m_queryModel.removeQuery(idx.row());
}

void MainWindow::workedStarted()
{
    ui->workersRunning->setText(QString::number(++m_workersRunning));
}

void MainWindow::workerFinished()
{
    ui->execCount->setText(QString::number(--m_queriesToRunCount));
    ui->workersRunning->setText(QString::number(--m_workersRunning));
}

void MainWindow::execStarted()
{
    ui->execRunning->setText(QString::number(++m_execRunning));
}

void MainWindow::execFinished()
{
    ui->execRunning->setText(QString::number(--m_execRunning));
}

void MainWindow::fetchStarted()
{
    ui->fetchRunning->setText(QString::number(++m_fetchRunning));
}

void MainWindow::fetchFinished()
{
    ui->fetchRunning->setText(QString::number(--m_fetchRunning));
}

void MainWindow::querySucceeded()
{
    ui->succeededCount->setText(QString::number(++m_succeeded));
}

void MainWindow::queryFailed()
{
    ui->failedCount->setText(QString::number(++m_failed));
}

void MainWindow::handleResult(QueryTimings result)
{
    m_queryModel.addResult(std::move(result));

    const auto query = nextQuery();
    const auto sql = query.sql();
    if (sql.isEmpty())
    {
        if (m_workersRunning == 0)
        {
            m_running = false;
            m_timer.stop();
            m_endTime = QDateTime::currentDateTime();
            updateDuration();
            enableControls();
        }
    }
    else
    {
        QueryController *worker = qobject_cast<QueryController *>(sender());
        worker->process(query.name(), sql);
    }
}

void MainWindow::updateDuration()
{
    QTime time{QTime(0, 0).addMSecs(m_startTime.msecsTo(m_endTime))};
    ui->duration->setText(time.toString("hh:mm:ss.zzz"));
}

void MainWindow::setControlsEnabled(bool enabled)
{
    for (auto control : m_disableControls)
        control->setEnabled(enabled);
}

void MainWindow::disableControls()
{
    setControlsEnabled(false);
}

void MainWindow::enableControls()
{
    setControlsEnabled(true);
}

void MainWindow::extractStatsToXlsx()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Seleziona il nome del file"),
                                                    QString(),
                                                    tr("File Excel (*.xlsx)"));

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".xlsx", Qt::CaseInsensitive))
        fileName.append(".xlsx");

    if (QFile::exists(fileName))
    {
        QMessageBox msg;

        msg.setIcon(QMessageBox::Question);
        msg.setText(tr("Attenzione, il file %1 esiste già. "
                       "Lo sovrascrivo?")
                    .arg(QDir::toNativeSeparators(fileName)));
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);

        int ret = msg.exec();

        if (ret == QMessageBox::No)
            return;

        if (!QFile::remove(fileName))
        {
            msg.setIcon(QMessageBox::Critical);
            msg.setText(tr("Impossibile sovrascrivere il file %1.")
                        .arg(QDir::toNativeSeparators(fileName)));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);

            msg.exec();
            return;
        }
    }

    using namespace QXlsx;
    Document xlsx;
    // XlsxHelper::dumpModelToXlsxWithForegroundRole(xlsx, m_queryModel);
    Format defaultFormat;
    Format headerFormat;

    defaultFormat.setFontName("Arial");
    defaultFormat.setFontSize(10);

    headerFormat = defaultFormat;
    headerFormat.setFontBold(true);

    auto row = 1;
    auto col = 1;
    xlsx.write(row, 5, tr("Tempo totale"), headerFormat);
    xlsx.write(row, 6, tr("Tempo totale"), headerFormat);
    xlsx.write(row, 7, tr("Tempo totale"), headerFormat);
    xlsx.mergeCells(CellRange(row, 5, row, 7));
    xlsx.write(row, 8, tr("Tempo minimo"), headerFormat);
    xlsx.write(row, 9, tr("Tempo minimo"), headerFormat);
    xlsx.write(row, 10, tr("Tempo minimo"), headerFormat);
    xlsx.mergeCells(CellRange(row, 8, row, 10));
    xlsx.write(row, 11, tr("Tempo massimo"), headerFormat);
    xlsx.write(row, 12, tr("Tempo massimo"), headerFormat);
    xlsx.write(row, 13, tr("Tempo massimo"), headerFormat);
    xlsx.mergeCells(CellRange(row, 11, row, 13));
    xlsx.write(row, 14, tr("Tempo medio"), headerFormat);
    xlsx.write(row, 15, tr("Tempo medio"), headerFormat);
    xlsx.write(row, 16, tr("Tempo medio"), headerFormat);
    xlsx.mergeCells(CellRange(row, 14, row, 16));
    xlsx.write(row, 17, tr("Errore"), headerFormat);
    xlsx.write(row, 18, tr("Errore"), headerFormat);
    xlsx.write(row, 19, tr("Errore"), headerFormat);
    xlsx.mergeCells(CellRange(row, 17, row, 19));

    xlsx.write(++row, col, tr("Nome"), headerFormat);
    xlsx.write(row, ++col, tr("Tipo"), headerFormat);
    xlsx.write(row, ++col, tr("N.ro OK"), headerFormat);
    xlsx.write(row, ++col, tr("N.ro KO"), headerFormat);
    xlsx.write(row, ++col, tr("somma"), headerFormat);
    xlsx.write(row, ++col, tr("esecuzione"), headerFormat);
    xlsx.write(row, ++col, tr("ricezione"), headerFormat);
    xlsx.write(row, ++col, tr("somma"), headerFormat);
    xlsx.write(row, ++col, tr("esecuzione"), headerFormat);
    xlsx.write(row, ++col, tr("ricezione"), headerFormat);
    xlsx.write(row, ++col, tr("somma"), headerFormat);
    xlsx.write(row, ++col, tr("esecuzione"), headerFormat);
    xlsx.write(row, ++col, tr("ricezione"), headerFormat);
    xlsx.write(row, ++col, tr("somma"), headerFormat);
    xlsx.write(row, ++col, tr("esecuzione"), headerFormat);
    xlsx.write(row, ++col, tr("ricezione"), headerFormat);
    xlsx.write(row, ++col, tr("somma"), headerFormat);
    xlsx.write(row, ++col, tr("esecuzione"), headerFormat);
    xlsx.write(row, ++col, tr("ricezione"), headerFormat);
    xlsx.write(row, ++col, tr("N.ro righe"), headerFormat);
    xlsx.write(row, ++col, tr("Righe coinvolte"), headerFormat);
    xlsx.write(row, ++col, tr("Stima bytes"), headerFormat);
    xlsx.write(row, ++col, tr("SQL"), headerFormat);

    for (const auto &query : m_queryModel.queryList())
    {
        col = 1;
        xlsx.write(++row, col, query.name(), defaultFormat);
        xlsx.write(row, ++col, query.typeDescription(), defaultFormat);
        xlsx.write(row, ++col, query.successCount(), defaultFormat);
        xlsx.write(row, ++col, query.failCount(), defaultFormat);
        xlsx.write(row, ++col, query.stats().totQueryDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().totExecDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().totFetchDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().minQueryDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().minExecDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().minFetchDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().maxQueryDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().maxExecDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().maxFetchDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().avgQueryDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().avgExecDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().avgFetchDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().stdErrQueryDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().stdErrExecDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.stats().stdErrFetchDurationMs(), defaultFormat);
        xlsx.write(row, ++col, query.rowsCount(), defaultFormat);
        xlsx.write(row, ++col, query.affectedRowsCount(), defaultFormat);
        xlsx.write(row, ++col, query.weight(), defaultFormat);
        xlsx.write(row, ++col, query.sql(), defaultFormat);
    }

    // auto row = m_queryModel.rowCount() + 2;
    ++row;
    xlsx.write(++row, 1, tr("Configurazione: ").append(m_cfg.db().name));
    xlsx.write(++row, 1, tr("Server: ").append(m_cfg.db().specs.hostname));
    xlsx.write(++row, 1, tr("Database: ").append(m_cfg.db().specs.databaseName));
    xlsx.write(++row, 1, tr("Numero worker: %1").arg(m_workerCount));
    xlsx.write(++row, 1, tr("Numero query eseguite con successo: %1").arg(m_succeeded));
    xlsx.write(++row, 1, tr("Numero query eseguite con errore: %1").arg(m_failed));
    QTime time{QTime(0, 0).addMSecs(m_startTime.msecsTo(m_endTime))};
    xlsx.write(++row, 1, tr("Tempo totale di esecuzione: ").append(time.toString("hh:mm:ss.zzz")));

    QMessageBox msg;
    if (xlsx.saveAs(fileName))
    {
        msg.setIcon(QMessageBox::Information);
        msg.setText(tr("Il file %1 è stato salvato correttamente").arg(QDir::toNativeSeparators(fileName)));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
    }
    else
    {
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("Errore durante il salvataggio del file %1").arg(QDir::toNativeSeparators(fileName)));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
    }
    msg.exec();
}

void MainWindow::checkUncheckAllQueries(bool checked)
{
    for (int i = 0; i < m_queryProxyModel.rowCount(); ++i)
    {
        QModelIndex sourceIndex{m_queryModel.index(i, checkableColumn)};
        QModelIndex index{m_queryProxyModel.mapFromSource(sourceIndex)};
        m_queryProxyModel.setData(index, checked ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
    }
    ui->buttonCheckAll->setText(checked ? tr("Deseleziona tutte") : tr("Seleziona tutte"));
}

void MainWindow::clearStatistics()
{
    ui->hostname->setText(m_cfg.db().specs.hostname);
    ui->databaseName->setText(m_cfg.db().specs.databaseName);
    ui->duration->setText(QTime(0, 0).toString("hh:mm:ss.zzz"));
    ui->workersRunning->setText(QStringLiteral("0"));
    ui->execRunning->setText(QStringLiteral("0"));
    ui->fetchRunning->setText(QStringLiteral("0"));
    ui->execCount->setText(QStringLiteral("0"));
    ui->succeededCount->setText(QStringLiteral("0"));
    ui->failedCount->setText(QStringLiteral("0"));
}

void MainWindow::prepareWorkers()
{
    while (m_workers.count() < m_workerCount)
        addWorker();
}

void MainWindow::addWorker()
{
    QueryController *worker = new QueryController();
    connect(worker, &QueryController::running, this, &MainWindow::workedStarted);
    connect(worker, &QueryController::stopped, this, &MainWindow::workerFinished);
    connect(worker, &QueryController::execStarted, this, &MainWindow::execStarted);
    connect(worker, &QueryController::execFinished, this, &MainWindow::execFinished);
    connect(worker, &QueryController::fetchStarted, this, &MainWindow::fetchStarted);
    connect(worker, &QueryController::fetchFinished, this, &MainWindow::fetchFinished);
    connect(worker, &QueryController::succeeded, this, &MainWindow::querySucceeded);
    connect(worker, &QueryController::failed, this, &MainWindow::queryFailed);
    connect(worker, &QueryController::resultReady, this, &MainWindow::handleResult);
    m_workers.append(worker);
}

void MainWindow::prepareQueries()
{
    m_queryModel.clear();
    for (const auto &query : m_cfg.queries())
        m_queryModel.appendQuery(Query(query));

    ui->tableView->resizeColumnsToContents();
    ui->buttonCheckAll->setChecked(false);
}

Query MainWindow::nextQuery()
{
    if (m_stopped
            || m_queriesToRun.isEmpty())
        return {};

    return m_queryModel.getToRunAt(m_queriesToRun.takeFirst());
}

void MainWindow::about()
{
    QString text;

    text = QString("<b>").append(QApplication::applicationName()).append("</b>")
            .append(tr("<p>Semplice programma per effettuare test di performance di database.</p>"))
            .append(tr("<p>versione: ")).append(qApp->applicationVersion()).append("<br/>")
            .append(tr("versione librerie Qt: %1 (%2)<br/>")).arg(qVersion(), QSysInfo::buildCpuArchitecture())
            .append(tr("Configurazione: ")).append(m_cfg.db().name).append("<br/>")
            .append(tr("Indirizzo server: ")).append(m_cfg.db().specs.hostname).append("<br/>")
            .append(tr("Nome database: ")).append(m_cfg.db().specs.databaseName).append("<br/>")
            .append(tr("Sistema operativo: %1 (%2)<br/>")).arg(QSysInfo::prettyProductName(), QSysInfo::currentCpuArchitecture())
            .append(tr("Postazione: ")).append(QSysInfo::machineHostName()).append("</p>")
            .append("<a href='%1'>%2</a>").arg(qApp->organizationDomain()).arg(qApp->organizationName())
            .append(tr("<p><small>sviluppato da <a href='http://www.zelando.com'>Zelando</a></small></p>"));

    QMessageBox::about(this, QApplication::applicationName(), text);
}
