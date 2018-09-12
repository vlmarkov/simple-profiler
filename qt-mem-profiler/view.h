#ifndef VIEW_H
#define VIEW_H

#include "ui_mainwindow.h"

#include "event.h"
#include "observer.h"
#include "model.h"
#include "controller.h"


class View : public Observer
{
    public:
        View(Model* model, Controller* controller, Ui::MainWindow* ui);
        ~View();

        void update(const Event& event);

    private:
        Model*          model_;
        Controller*     controller_;
        Ui::MainWindow* ui_;
};

#endif // VIEW_H
