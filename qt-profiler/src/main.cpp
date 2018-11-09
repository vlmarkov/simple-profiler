#include <QQuickStyle>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <include/base-profiler.hpp>


int main(int argc, char** argv)
{
    QGuiApplication::setApplicationName("Basic Profiling Tool");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QQuickStyle::setStyle("Fusion");

    qmlRegisterType<BaseProfiler>("baseProfiler", 0, 1, "BaseProfiler");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
