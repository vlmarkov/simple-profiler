#include "profiler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Create the application object
    QApplication app(argc, argv);

    // Create the main application object and display it on screen
    Profiler profiler;
    profiler.show();

    // Put the application into a loop in which events are handled
    return app.exec();
}
