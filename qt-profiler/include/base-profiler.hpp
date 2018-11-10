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

        Q_INVOKABLE void runMemoryCheck(const QString& pathToFile);
        Q_INVOKABLE void runPerfomanceCheck(const QString& pathToFile);

        QString getResult() noexcept;
        void setResult(const QString& rhs) noexcept;

    signals:
        void resultChanged();
};
