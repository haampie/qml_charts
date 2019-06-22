#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "SplineSeries.h"
#include "AreaSplineSeries.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<SplineSeries>("Harmen", 1, 0, "SplineSeries");
    qmlRegisterType<AreaSplineSeries>("Harmen", 1, 0, "AreaSplineSeries");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
