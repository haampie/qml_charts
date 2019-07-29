#ifndef SPLINEHELPER_H
#define SPLINEHELPER_H

#include <vector>
#include <QPointF>

struct SplineHelper
{
    static std::vector<std::pair<QPointF,QPointF>> spline1d(std::vector<QPointF> const &coords) {
        // Basically the same cubic spline, but with a fixed interpolation
        // in in x (
        if (coords.size() <= 1)
            return {};

        if (coords.size() == 2)
            return {{coords[0], coords[1]}};

        uint64_t n = coords.size() - 1;

        // Thomas
        std::vector<qreal> as(n);
        std::vector<qreal> bs(n);
        std::vector<qreal> cs(n);
        std::vector<qreal> r(n);
        std::vector<std::pair<QPointF,QPointF>> helper(n);

        auto dx = [&coords](uint64_t i){ return coords[i + 1].x() - coords[i].x();};
        auto x = [&coords](uint64_t i){ return coords[i].x(); };
        auto y = [&coords](uint64_t i){ return coords[i].y(); };

        // first row and right-hand side
        {
            as[0] = 0;
            bs[0] = 2;
            cs[0] = dx(0) / dx(1);
            r[0] = y(0) + (1 + dx(0) / dx(1)) * y(1);
        }

        // interior nodes
        for (uint64_t i = 1; i < n - 1; ++i) {
            auto frac_prev = dx(i) / dx(i - 1);
            auto frac_next = dx(i) / dx(i + 1);
            as[i] = frac_prev * frac_prev;
            bs[i] = 2 + 2 * frac_prev;
            cs[i] = frac_next;
            r[i] = (1 + 2 * frac_prev + frac_prev * frac_prev) * y(i) + (1 + frac_next) * y(i + 1);
        }

        {
            auto frac = dx(n - 2) / dx(n - 3);
            as[n - 1] = 2 * frac * frac;
            bs[n - 1] = 3 + 4 * frac;
            cs[n - 1] = 0;
            r[n - 1] = (2 * frac * frac + 4 * frac + 2) * y(n - 1) + y(n);
        }

        // Forward substitution
        for (uint64_t i = 1; i < n; ++i) {
           auto m = as[i] / bs[i - 1];
           bs[i] = bs[i] - m * cs[i - 1];
           r[i] = r[i] - m * r[i - 1];
        }

        // To interpolate x linearly at t = 1/3 and 2/3
        constexpr double frac13 = 1.0 / 3;
        constexpr double frac23 = 2.0 / 3;

        // Backward substitution
        // immediately set the x coords
        helper[n - 1].first = {frac23 * x(n - 1) + frac13 * x(n), r[n - 1] / bs[n - 1]};

        for (uint64_t i = n - 1; i > 0; --i)
           helper[i - 1].first = {frac23 * x(i - 1) + frac13 * x(i), (r[i - 1] - cs[i - 1] * helper[i].first.y()) / bs[i - 1]};

        // Solve for p2s
        for (uint64_t i = 0; i < n - 1; ++i) {
            auto frac = dx(i) / dx(i + 1);
            helper[i].second = {frac13 * x(i) + frac23 * x(i + 1), (1 + frac) * y(i + 1) - frac * helper[i + 1].first.y()};
        }

        // Boundary
        helper[n - 1].second = {frac13 * x(n - 1) + frac23 * x(n), (y(n) + helper[n - 1].first.y()) / 2};

        return helper;
    }

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
       for (uint64_t i = 0; i < n - 1; ++i)
           helper[i].second = 2 * coords[i + 1] - helper[i + 1].first;

       // Boundary
       helper[n - 1].second = (coords[n] + helper[n - 1].first) / 2;

       return helper;
    }

    static std::vector<QPointF> smoothen(std::vector<QPointF> const &&v)
    {
       // simple [1 1 1] filter
       std::vector<QPointF> smooth = v;

       // Smooth the non-boundary points (avoid overflow .size() - 1, yikes)
       for (uint64_t i = 1; i + 1 < smooth.size(); ++i)
          smooth[i].setY((v[i - 1].y() + 2 * v[i].y() + v[i + 1].y()) / 4);

       return smooth;
    }
};

#endif // SPLINEHELPER_H
