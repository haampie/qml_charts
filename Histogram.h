#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <tuple>
#include "MaybeValue.h"
#include <vector>

class Histogram
{
public:

   Histogram(uint64_t buckets, std::pair<float,float> const &range);

   struct InsertionResult {
      bool inserted;
      uint64_t idx;
   };

   // Basic API
   InsertionResult insert(float x, float y);
   void clear();

   // p should be in the range [0, 1)
   MaybeValue<float> percentile(uint64_t bucket, float p);

   float from() const;
   float to() const;
   uint64_t buckets() const;
   std::pair<float,float> bucket_range(uint64_t bucket) const;
   uint64_t bucket_size(uint64_t idx) const;

private:
   inline uint64_t index(float x);
   inline bool inbounds(uint64_t idx);

   std::pair<float,float> m_range;
   uint64_t m_buckets;
   std::vector<std::vector<float>> m_histogram;
};

#endif // HISTOGRAM_H
