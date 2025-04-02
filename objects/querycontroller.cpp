#include "querycontroller.h"

#include "queryworker.h"

QueryController::QueryController(QObject *parent) : QObject(parent)
{
    QueryWorker *worker = new QueryWorker;
    worker->moveToThread(&m_workerThread);

    connect(&m_workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &QueryController::exec, worker, &QueryWorker::process);

    connect(worker, &QueryWorker::execStarted, this, &QueryController::execStarted);
    connect(worker, &QueryWorker::execFinished, this, &QueryController::execFinished);
    connect(worker, &QueryWorker::fetchStarted, this, &QueryController::fetchStarted);
    connect(worker, &QueryWorker::fetchFinished, this, &QueryController::fetchFinished);
    connect(worker, &QueryWorker::succeeded, this, &QueryController::succeeded);
    connect(worker, &QueryWorker::failed, this, &QueryController::failed);
    connect(worker, &QueryWorker::running, this, &QueryController::workerStarted);
    connect(worker, &QueryWorker::stopped, this, &QueryController::workerStopped);

    connect(worker, &QueryWorker::resultReady, this, &QueryController::handleResult);

    m_workerThread.start();
}

QueryController::~QueryController()
{
    m_workerThread.quit();
    m_workerThread.wait(10000);
    m_workerThread.deleteLater();
}

void QueryController::process(const QString &name, const QString &sql)
{
    if (sql.isEmpty())
        return;
    emit exec(name, sql);
}

bool QueryController::isRunning() const
{
    return m_workerRunning;
}

void QueryController::workerStarted()
{
    m_workerRunning = true;
    emit running();
}

void QueryController::workerStopped()
{
    m_workerRunning = false;
    emit stopped();
}

void QueryController::handleResult(const QueryTimings &result)
{
    emit resultReady(result);
}
