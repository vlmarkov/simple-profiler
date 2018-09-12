#include <QFileDialog>

#include "profiler.h"


Profiler::Profiler(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    model_      = new Model();
    controller_ = new Controller(model_);
    view_       = new View(model_, controller_, ui_);
}

Profiler::~Profiler()
{
    delete ui_;
    delete model_;
    delete controller_;
    delete view_;
}

void Profiler::on_closeButton_clicked()
{
    QApplication::quit();
}

void Profiler::on_runButton_clicked()
{
    // Open a dialog that allows you to select a file to open
    auto path = QFileDialog::getOpenFileName(this, "Open the file");

    // Run memory profiler
    this->controller_->processRequest(path);
}
