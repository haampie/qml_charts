#ifndef FANCYPROVIDER_H
#define FANCYPROVIDER_H

#include <QObject>
#include <QDebug>
#include <QPointF>
#include <cmath>
#include <algorithm>

#include "SplineSeries.h"
#include "AreaSplineSeries.h"

class FancyProvider : public QObject
{
   Q_OBJECT
public:
   explicit FancyProvider(uint64_t buckets, float from, float to, QObject *parent = nullptr);

public slots:
   void insert(float x, float y);
   void drawMedianTo(SplineSeries * series);
   void drawBoundaries(AreaSplineSeries * series);

private:
   std::vector<QPointF> median();
   std::pair<std::vector<QPointF>, std::vector<QPointF>> boundaries();

   uint64_t m_buckets;
   float m_from;
   float m_to;
   std::vector<std::vector<float>> m_values;
};

#endif // FANCYPROVIDER_H
