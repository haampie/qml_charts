#ifndef FANCYPROVIDER_H
#define FANCYPROVIDER_H

#include <QObject>

#include "SplineSeries.h"
#include "AreaSplineSeries.h"
#include "MaybeValue.h"
#include "Histogram.h"
#include "Markers.h"

class FancyProvider : public QObject
{
   Q_OBJECT
   static constexpr float p10 = 0.1f;
   static constexpr float p50 = 0.5f;
   static constexpr float p90 = 0.9f;

public:
   explicit FancyProvider(Histogram histogram, QObject *parent = nullptr);

signals:
   void graphsChanged();

public slots:
   void insert(float x, float y);
   void drawMedianTo(SplineSeries * series);
   void drawBoundaries(AreaSplineSeries * series);
   void drawMarkers(Markers * markers);

private:
   std::vector<QPointF> median();
   std::pair<std::vector<QPointF>, std::vector<QPointF>> boundaries();

   Histogram m_histogram;

   // Cache for the median value per bucket
   std::vector<MaybeValue<float>> m_10_percentile_per_bucket;
   std::vector<MaybeValue<float>> m_median_per_bucket;
   std::vector<MaybeValue<float>> m_90_percentile_per_bucket;
};

#endif // FANCYPROVIDER_H
