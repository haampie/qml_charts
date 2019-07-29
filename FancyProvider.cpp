#include "FancyProvider.h"

#include <chrono>
#include "SplineHelper.h"

using namespace std::chrono;

FancyProvider::FancyProvider(Histogram histogram, QObject *parent)
   : QObject(parent), m_histogram(std::move(histogram)),
     m_10_percentile_per_bucket(m_histogram.buckets()),
     m_median_per_bucket(m_histogram.buckets()),
     m_90_percentile_per_bucket(m_histogram.buckets())
{
}

void FancyProvider::insert(float x, float y)
{
   // Maybe insert
   auto result = m_histogram.insert(x, y);

   // Only compute the median when we are in bounds and have enough data
   if (!result.inserted)
      return;

   // Compute the median
   auto maybelo = m_histogram.percentile(result.idx, p10);
   auto maybemid = m_histogram.percentile(result.idx, p50);
   auto maybehi = m_histogram.percentile(result.idx, p90);

   // Check if things have changed
   if (maybelo == m_10_percentile_per_bucket[result.idx] && maybemid == m_median_per_bucket[result.idx] && maybehi == m_90_percentile_per_bucket[result.idx])
      return;

   m_10_percentile_per_bucket[result.idx] = maybelo;
   m_median_per_bucket[result.idx] = maybemid;
   m_90_percentile_per_bucket[result.idx] = maybehi;

   emit graphsChanged();
}

void FancyProvider::drawMedianTo(SplineSeries *series) {
   series->setData(SplineHelper::smoothen(median()));
}

void FancyProvider::drawBoundaries(AreaSplineSeries *series) {
   auto bs = boundaries();
   series->setData(SplineHelper::smoothen(std::move(bs.first)), SplineHelper::smoothen(std::move(bs.second)));
}

void FancyProvider::drawMarkers(Markers *markers) {
   markers->setData(SplineHelper::smoothen(median()));
}

std::vector<QPointF> FancyProvider::median()
{
   std::vector<QPointF> m;
   for (uint64_t idx = 0; idx < m_histogram.buckets(); ++idx)
   {
      // Skip missing values
      if (!m_median_per_bucket[idx].hasValue) continue;

      // Interpolate x between the bucket boundaries
      auto r = m_histogram.bucket_range(idx);
      auto x = (r.second + r.first) / 2;

      // Store the (x, y) coord.
      m.emplace_back(x, m_median_per_bucket[idx].value);
   }

   return m;
}

std::pair<std::vector<QPointF>, std::vector<QPointF>> FancyProvider::boundaries()
{
   std::vector<QPointF> lo, hi;
   for (uint64_t idx = 0; idx < m_histogram.buckets(); ++idx)
   {
      // Skip missing values
      if (!m_10_percentile_per_bucket[idx].hasValue || !m_90_percentile_per_bucket[idx].hasValue)
         continue;

      auto r = m_histogram.bucket_range(idx);
      auto x = (r.second + r.first) / 2;

      lo.emplace_back(x, m_10_percentile_per_bucket[idx].value);
      hi.emplace_back(x, m_90_percentile_per_bucket[idx].value);
   }

   return {std::move(lo), std::move(hi)};
}
