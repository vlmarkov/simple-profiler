#pragma once

#include <QObject>
#include <QString>


// Base API
class BaseProfiler : public QObject
{
    Q_OBJECT

    private:
        Q_PROPERTY(QString getResult READ getResult NOTIFY resultChanged)
        QString result_;

    public:
        BaseProfiler() = default;
        ~BaseProfiler() = default;

        Q_INVOKABLE void memoryCheck(const QString& str);
        Q_INVOKABLE void perfomanceCheck(const QString& str);

        QString getResult() noexcept;
        void setResult(const QString& rhs) noexcept;

    signals:
        void resultChanged();
};
