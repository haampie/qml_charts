#include "Histogram.h"
#include <cmath>
#include <iostream>
#include <QDebug>

Histogram::Histogram(uint64_t buckets, std::pair<float, float> const &range) :
   m_range(range), m_buckets(buckets), m_histogram(buckets)
{

}

Histogram::InsertionResult Histogram::insert(float x, float y) {
   auto idx = index(x);

   // could not insert
   if (!inbounds(idx))
      return InsertionResult{false, 0};

   auto uidx = static_cast<uint64_t>(idx);

   m_histogram[uidx].push_back(y);

   return {true, uidx};
}

MaybeValue<float> Histogram::percentile(uint64_t bucket, float p) {
   if (!inbounds(bucket) || m_histogram[bucket].empty())
      return {};

   auto &v = m_histogram[bucket];
   auto pidx = static_cast<uint64_t>(std::floor(p * v.size()));

   if (pidx >= v.size())
      return {};

   std::nth_element(v.begin(), v.begin() + static_cast<int64_t>(pidx), v.end());

   return MaybeValue<float>(v[pidx]);
}

float Histogram::from() const {
   return m_range.first;
}

float Histogram::to() const {
   return m_range.second;
}

uint64_t Histogram::buckets() const {
   return m_buckets;
}

std::pair<float, float> Histogram::bucket_range(uint64_t bucket) const
{
   if (bucket >= m_buckets)
      return {m_range.second, std::numeric_limits<float>::infinity()};

   auto lo = bucket * (m_range.second - m_range.first) / m_buckets + m_range.first;
   auto hi = (bucket + 1) * (m_range.second - m_range.first) / m_buckets + m_range.first;

   return {lo, hi};
}

uint64_t Histogram::bucket_size(uint64_t idx) const {
   return m_histogram[idx].size();
}

void Histogram::clear()
{
   // Clear all buckets.
   for (auto &v : m_histogram)
      v.clear();
}

uint64_t Histogram::index(float x) {
   auto r = (x - m_range.first) / (m_range.second - m_range.first);
   return static_cast<uint64_t>(std::floor(r * m_buckets));
}

bool Histogram::inbounds(uint64_t idx) {
   return idx < m_buckets;
}
