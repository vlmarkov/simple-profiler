#include "view.h"


View::View(Model* model, Controller* controller, Ui::MainWindow* ui) :
    model_(model),
    controller_(controller),
    ui_(ui)
{
    model_->addObserver(this);
    controller_->addObserver(this);
}

View::~View()
{
    ;
}

void View::update(const Event& event)
{
    switch (event)
    {
        case Event::FindFiles:
        {
            this->ui_->textEdit->setStyleSheet("font: bold 11pt \"Courier\";");
            this->ui_->textEdit->setText(this->model_->getMemProfillerResult());
            break;
        }
        default:
        {
            break;
        }
    }
}
