#ifndef QUERYCONTROLLER_H
#define QUERYCONTROLLER_H

#include "querytimings.h"

#include <QObject>
#include <QThread>

class QueryController : public QObject
{
    Q_OBJECT
public:
    explicit QueryController(QObject *parent = 0);
    ~QueryController();

    void process(const QString &name, const QString &sql);
    bool isRunning() const;

signals:
    void execStarted();
    void execFinished();
    void fetchStarted();
    void fetchFinished();
    void succeeded();
    void failed();
    void running();
    void stopped();
    void exec(const QString &, const QString &);
    void resultReady(const QueryTimings &);

public slots:
    void workerStarted();
    void workerStopped();
    void handleResult(const QueryTimings &result);

private:
    QThread m_workerThread;
    bool m_workerRunning{false};
};

#endif // QUERYCONTROLLER_H
