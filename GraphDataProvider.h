#ifndef GRAPHDATAPROVIDER_H
#define GRAPHDATAPROVIDER_H

#include <QObject>
#include <QPointF>
#include <QTimer>

#include "SplineSeries.h"
#include "AreaSplineSeries.h"

class GraphDataProvider : public QObject
{
    Q_OBJECT

public:
    GraphDataProvider(QObject * parent = nullptr);

    std::vector<QPointF> spline;
    std::vector<QPointF> splineAreaLower;
    std::vector<QPointF> splineAreaUpper;

    std::vector<QPointF> sinewave1;
    std::vector<QPointF> sinewave2;
    std::vector<QPointF> sinewave3;

signals:
    void sineChanged();

public slots:

    void drawSpline(SplineSeries * series) const
    {
       if (series == nullptr) return;
        series->setData(spline);
    }

    void drawAreaSpline(AreaSplineSeries * series) const
    {
       if (series == nullptr) return;
        series->setData(splineAreaLower, splineAreaUpper);
    }

    void drawSines(SplineSeries * series1, SplineSeries * series2, SplineSeries * series3) const
    {
       if (series1 == nullptr || series2 == nullptr || series3 == nullptr) return;
        series1->setData(sinewave1);
        series2->setData(sinewave2);
        series3->setData(sinewave3);
    }

private:
    QTimer * timer;
};

#endif // GRAPHDATAPROVIDER_H
