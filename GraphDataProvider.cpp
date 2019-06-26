#include "GraphDataProvider.h"

#include <QRandomGenerator>

GraphDataProvider::GraphDataProvider(QObject * parent)
    : QObject(parent), timer(new QTimer(this)), t(0)
{
    timer->setInterval(16);

    // Number of points
    uint64_t n = 30;

    for (size_t i = 0; i < n; ++i) {
       auto x = i / static_cast<double>(n - 1);
       splineAreaLower.emplace_back(x, QRandomGenerator::global()->bounded(0.3));
       splineAreaUpper.emplace_back(x, 1 - QRandomGenerator::global()->bounded(0.3));
       spline.emplace_back(x, QRandomGenerator::global()->bounded(1.0));
    }

    for (size_t i = 0; i < n; ++i) {
       auto x = 2 * 3.1415 * i / static_cast<double>(n - 1);
       sinewave1.emplace_back(x, f1(x, 0));
       sinewave2.emplace_back(x, f2(x, 0));
       sinewave3.emplace_back(x, f3(x, 0));
    }

    // Update the sines example continuously
    connect(timer, &QTimer::timeout, this, &GraphDataProvider::updateGraph);
    timer->start();
}

double GraphDataProvider::f1(double x, double t) {
    return std::sin(x - t);
}

double GraphDataProvider::f2(double x, double t) {
    return 0.5 * std::sin(2.0 * x - (t - 1.0));
};

double GraphDataProvider::f3(double x, double t) {
    return f1(x, t) + f2(x, t);
};

void GraphDataProvider::updateGraph() {
    ++t;

    for (size_t i = 0; i < sinewave1.size(); ++i) {
        sinewave1[i].setY(f1(sinewave1[i].x(), 0.05 * static_cast<double>(t)));
        sinewave2[i].setY(f2(sinewave2[i].x(), 0.05 * static_cast<double>(t)));
        sinewave3[i].setY(f3(sinewave3[i].x(), 0.05 * static_cast<double>(t)));
    }

    emit sineChanged();
}
