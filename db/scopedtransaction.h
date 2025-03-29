#ifndef SCOPEDTRANSACTION_H
#define SCOPEDTRANSACTION_H

#include <QSqlDatabase>
#include <QString>

class ScopedTransaction
{
public:
    enum class Policy
    {
        AutoCommit,
        AutoRollback,
    };
    explicit ScopedTransaction(Policy policy, QString connectionName = QString());
    ScopedTransaction(const ScopedTransaction &) = delete;
//    ScopedTransaction(ScopedTransaction &&) = delete;
//    ScopedTransaction(ScopedTransaction &&) = default;  // NOTE: gcc 5.3 does not have GUARANTEED COPY ELISION of c++17, so this ctor is required
                                                        //       in order to let Connection return a ScopedTransaction
    ~ScopedTransaction();

    ScopedTransaction &operator=(const ScopedTransaction &) = delete;
    ScopedTransaction &operator=(ScopedTransaction &&) = delete;

    bool isActive() const;
    void begin() const;
    void commit() const;
    void rollback() const;

private:
    QSqlDatabase db() const;

private:
    const Policy m_policy;
    const QString m_connectionName;
    mutable bool m_active{false};
};

#endif // SCOPEDTRANSACTION_H
