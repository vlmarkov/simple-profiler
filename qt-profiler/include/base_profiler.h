#pragma once

#include <QObject>
#include <QString>


// Base API
class BaseProfiler : public QObject
{
    Q_OBJECT

    public:
        BaseProfiler() = default;
        ~BaseProfiler();

        Q_INVOKABLE void memoryCheck(const QString& str);
        Q_INVOKABLE void perfomanceCheck(const QString& str);

        QString getResult() noexcept;
        void setResult(const QString str) noexcept;

    signals:
        void resultChanged();

    private:
        Q_PROPERTY(QString getResult READ getResult NOTIFY resultChanged)
        QString result_;
};
