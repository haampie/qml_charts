#ifndef SPLINEHELPER_H
#define SPLINEHELPER_H

#include <vector>
#include <QPointF>

struct SplineHelper
{
    static std::vector<std::pair<QPointF,QPointF>> compute(std::vector<QPointF> const &coords)
    {
       if (coords.size() <= 1)
           return {};

       if (coords.size() == 2)
           return {{coords.front(), coords.back()}};

       // n pieces
       uint64_t n = coords.size() - 1;

       // Thomas algorithm
       std::vector<qreal> as(n);
       std::vector<qreal> bs(n);
       std::vector<qreal> cs(n);
       std::vector<QPointF> r(n);
       std::vector<std::pair<QPointF,QPointF>> helper(n);

       // Boundary conditions
       as[0] = 0;
       bs[0] = 2;
       cs[0] = 1;
       r[0] = coords[0] + 2 * coords[1];

       // Set up matrix and right-hand side (i.e. solve for p1s)
       for (uint64_t i = 1; i < n - 1; ++i) {
          as[i] = 1;
          bs[i] = 4;
          cs[i] = 1;
          r[i] = 4 * coords[i] + 2 * coords[i + 1];
       }

       as[n - 1] = 2;
       bs[n - 1] = 7;
       cs[n - 1] = 0;
       r[n - 1] = 8 * coords[n - 1] + coords[n];

       // Forward substitution
       for (uint64_t i = 1; i < n; ++i) {
          auto m = as[i] / bs[i - 1];
          bs[i] = bs[i] - m * cs[i - 1];
          r[i] = r[i] - m * r[i - 1];
       }

       // Backward substitution
       helper[n - 1].first = r[n - 1] / bs[n - 1];

       for (uint64_t i = n - 1; i > 0; --i)
          helper[i - 1].first = (r[i - 1] - cs[i - 1] * helper[i].first) / bs[i - 1];

       // Solve for p2s
       for (uint64_t i = 0; i < n - 1; ++i) helper[i].second = 2 * coords[i + 1] - helper[i + 1].first;

       // Boundary
       helper[n - 1].second = (coords[n] + helper[n - 1].first) / 2;

       return helper;
    }
};

#endif // SPLINEHELPER_H
