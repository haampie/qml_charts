#include "SplineSeries.h"

#include <iostream>
#include <chrono>
#include <QDebug>

using namespace std::chrono;

SplineSeries::SplineSeries(QQuickItem *parent)
    : QQuickPaintedItem(parent), coords{
        {0.0, 0.0},
        {0.1, 0.4},
        {0.2, 0.9},
        {0.3, 0.7},
        {0.4, 0.9},
        {0.5, 0.7},
        {0.6, 0.9},
        {0.7, 0.7},
        {0.8, 0.9},
        {0.9, 0.7}
    }
{
    compute();
}

QPointF SplineSeries::interpolate(uint64_t bucket, double t) {
    if (bucket > coords.size() - 1)
        return {0, 0};

    return (1 - t) * (1 - t) * (1 - t) * coords[bucket] +
            3 * (1 - t) * (1 - t) * t * p1s[bucket] +
            3 * (1 - t) * t * t * p2s[bucket] +
            t * t * t * coords[bucket + 1];
}

QColor SplineSeries::color() const
{
    return m_color;
}

void SplineSeries::setColor(const QColor &color)
{
    m_color = color;
    update();
}

void SplineSeries::paint(QPainter *painter)
{
    if (coords.size() < 3) return;

    QPen pen(m_color, m_strokeWidth);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);

    QPainterPath path;
    path.moveTo(coords[0].x() * width(), coords[0].y() * height());
    for (uint64_t i = 0; i < coords.size() - 1; ++i)
        path.cubicTo(p1s[i].x() * width(), p1s[i].y() * height(), p2s[i].x() * width(), p2s[i].y() * height(), coords[i + 1].x() * width(), coords[i + 1].y() * height());
    painter->drawPath(path);

    pen.setColor(QColor{0, 0, 0});
    painter->setPen(pen);
    for (uint64_t i = 0; i < coords.size(); ++i)
        painter->drawEllipse(QPointF{coords[i].x() * width(), coords[i].y() * height()}, 10, 10);
}

qreal SplineSeries::strokeWidth() const
{
    return m_strokeWidth;
}

void SplineSeries::setStrokeWidth(const qreal &strokeWidth)
{
    m_strokeWidth = strokeWidth;
    update();
}

quint64 SplineSeries::getSize() const {
    return coords.size();
}

void SplineSeries::compute()
{
    // n pieces
    uint64_t n = coords.size() - 1;

    // Thomas algorithm
    std::vector<qreal> as(n);
    std::vector<qreal> bs(n);
    std::vector<qreal> cs(n);
    std::vector<QPointF> r(n);
    p1s = std::vector<QPointF>(n);
    p2s = std::vector<QPointF>(n);

    // Boundary conditions
    as[0] = 0;
    bs[0] = 2;
    cs[0] = 1;
    r[0] = coords[0] + 2 * coords[1];

    // Set up matrix and right-hand side (i.e. solve for p1s)
    r[0] = coords[0] + 2 * coords[1];
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
        r[i] = r[i] - m * r[i-1];
    }

    // Backward substitution
    p1s[n - 1] = r[n - 1] / bs[n - 1];

    for (uint64_t i = n - 1; i > 0; --i)
        p1s[i - 1] = (r[i - 1] - cs[i - 1] * p1s[i]) / bs[i - 1];

    // Solve for p2s
    for (uint64_t i = 0; i < n - 1; ++i)
        p2s[i] = 2 * coords[i + 1] - p1s[i + 1];

    // Boundary
    p2s[n - 1] = (coords[n] + p1s[n - 1]) / 2;
}
