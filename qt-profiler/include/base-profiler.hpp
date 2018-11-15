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

        Q_INVOKABLE void runMemoryCheck(const QString& pathTo);
        Q_INVOKABLE void runPerfomanceSampleCheck(const QString& pathTo);
        Q_INVOKABLE void runPerfomanceEventCheck(const QString& pathTo);

        QString getResult() noexcept;
        void setResult(const QString& string) noexcept;

    signals:
        void resultChanged();
};
