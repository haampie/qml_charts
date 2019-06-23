#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>

#include "SplineSeries.h"
#include "AreaSplineSeries.h"
#include "Axis.h"
#include "GraphDataProvider.h"
#include "VerticalLine.h"
#include "Point.h"

int main(int argc, char *argv[])
{
    // Necessary for antialising of graphs
    QSurfaceFormat fmt;
    fmt.setSamples(2);
    QSurfaceFormat::setDefaultFormat(fmt);

    // Not really necessary?
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // Register the QML graph types
    qmlRegisterType<SplineSeries>("Harmen", 1, 0, "SplineSeries");
    qmlRegisterType<AreaSplineSeries>("Harmen", 1, 0, "AreaSplineSeries");
    qmlRegisterType<Axis>("Harmen", 1, 0, "Axis");
    qmlRegisterType<VerticalLine>("Harmen", 1, 0, "VerticalLine");
    qmlRegisterType<Point>("Harmen", 1, 0, "Point");

    // Create a data provider for the graphs
    GraphDataProvider data;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    // QML is responsible for updating the graphs, so we share a data provider
    engine.rootContext()->setContextProperty("GraphData", &data);

    engine.load(url);

    return app.exec();
}
