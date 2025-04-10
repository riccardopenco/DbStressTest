#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "objects/configuration.h"
#include "objects/querycontroller.h"
#include "objects/querytimings.h"
#include "objects/query.h"

#include "models/querystatsmodel.h"
#include "models/checkableproxymodel.h"

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QDateTime>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setupForm();
    void loadConfiguration();
    void saveConfiguration();
    void setConfiguration(st::Configuration cfg);
    void changeConnection();
    void start();
    void pause();
    void stop();
    void addQuery();
    void editQuery();
    void removeQuery();

    void workedStarted();
    void workerFinished();
    void execStarted();
    void execFinished();
    void fetchStarted();
    void fetchFinished();
    void querySucceeded();
    void queryFailed();
    void handleResult(QueryTimings result);
    void updateDuration();
    void setControlsEnabled(bool enabled);
    void disableControls();
    void enableControls();
    void extractStatsToXlsx();
    void checkUncheckAllQueries(bool checked);

private:
    void clearStatistics();
    void prepareWorkers();
    void addWorker();
    void prepareQueries();
    
    Query nextQuery();

    void about();

private:
    Ui::MainWindow *ui;
    st::Configuration m_cfg;

    int m_workerCount{4};
    QList<int> m_queriesToRun{};
    int m_queriesToRunCount{0};
    QList<QueryController *> m_workers;
    int m_workersRunning{0};
    int m_execRunning{0};
    int m_fetchRunning{0};
    int m_succeeded{0};
    int m_failed{0};
    QDateTime m_startTime{};
    QDateTime m_endTime{};
    QTimer m_timer{};
    QList<QWidget *> m_disableControls{};
    QueryStatsModel m_queryModel{};
    CheckableProxyModel m_queryProxyModel{};
    bool m_running{false};
    bool m_stopped{true};
    const int checkableColumn{2};
};

#endif // MAINWINDOW_H
