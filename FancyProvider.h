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
   explicit FancyProvider(uint64_t buckets, float from, float to, QObject *parent = nullptr)
      : QObject(parent), m_buckets(buckets), m_from(from), m_to(to), m_values(buckets)
   {
   }

signals:

public slots:
   void insert(float x, float y)
   {
      auto r = (x - m_from) / (m_to - m_from);
      auto idx = static_cast<uint64_t>(std::floor(r * m_buckets));
      if (idx >= m_buckets) return;
      m_values[idx].push_back(y);
   }

   void drawMedianTo(SplineSeries * series) {
      series->setData(median());
   }

   void drawBoundaries(AreaSplineSeries * series) {
      auto bs = boundaries();
      series->setData(std::move(bs.first), std::move(bs.second));
   }

private:
   std::vector<QPointF> median()
   {
      std::vector<QPointF> medianCoords;
      for (uint64_t idx = 0; idx < m_buckets; ++idx)
      {
         auto &v = m_values[idx];
         if (v.size() == 0) continue;

         // Put the median in the middle
         std::nth_element(v.begin(), v.begin() + v.size()/2, v.end());

         float x = (idx + 0.5f) * (m_to - m_from) / m_buckets;
         float y = v[v.size() / 2];
         medianCoords.emplace_back(x, y);
      }

      return medianCoords;
   }

   std::pair<std::vector<QPointF>, std::vector<QPointF>> boundaries()
   {
      std::vector<QPointF> lo, hi;
      for (uint64_t idx = 0; idx < m_buckets; ++idx)
      {
         auto &v = m_values[idx];
         if (v.size() == 0) continue;

         // Put the median in the middle
         float x = (idx + 0.5f) * (m_to - m_from) / m_buckets;
         std::nth_element(v.begin(), v.begin() + v.size()/2, v.end());
         lo.emplace_back(x, v[v.size() / 10]);

         std::nth_element(v.begin(), v.begin() + (9 * v.size()) / 10, v.end());
         hi.emplace_back(x, v[9 * v.size() / 10]);
      }

      return {std::move(lo), std::move(hi)};

   }

   uint64_t m_buckets;
   float m_from;
   float m_to;
   std::vector<std::vector<float>> m_values;
};

#endif // FANCYPROVIDER_H
