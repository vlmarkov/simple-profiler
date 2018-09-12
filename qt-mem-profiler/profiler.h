#ifndef PROFILER_H
#define PROFILER_H

#include <QMainWindow>

#include "model.h"
#include "view.h"
#include "controller.h"


namespace Ui {
    class MainWindow;
}

class Profiler : public QMainWindow
{
    Q_OBJECT

public:
    explicit Profiler(QWidget *parent = nullptr);
    ~Profiler();

private slots:
    void on_closeButton_clicked();
    void on_runButton_clicked();

private:
    Ui::MainWindow* ui_;
    Model*          model_;
    Controller*     controller_;
    View*           view_;

    void runProfiler_(QString& pathToElf);
};

#endif // PROFILER_H
