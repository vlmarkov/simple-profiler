#ifndef BASE_PROFILER_H
#define BASE_PROFILER_H

#include <QObject>
#include <QString>


// Base API
class BaseProfiler : public QObject
{
    Q_OBJECT

    public:
        BaseProfiler();
        ~BaseProfiler();

        Q_INVOKABLE void analyze_memory(const QString& str);
        Q_INVOKABLE void analyze_perfomance(const QString& str);

        QString getResult();
        void setResult(const QString str);

    signals:
        void resultChanged();

    private:
        Q_PROPERTY(QString getResult READ getResult NOTIFY resultChanged)
        QString result_;
};

#endif // BASE_PROFILER_H
