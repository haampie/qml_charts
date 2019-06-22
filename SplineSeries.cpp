#include "SplineSeries.h"

#include <QDebug>
#include <QRandomGenerator>
#include <chrono>
#include <iostream>

using namespace std::chrono;

SplineSeries::SplineSeries(QQuickItem *parent) : QQuickPaintedItem(parent) {

   // Enable opengl / antialiasing
   setAntialiasing(true);
   setRenderTarget(QQuickPaintedItem::FramebufferObject);

   // Number of points
   uint64_t n = 1500;
   for (size_t i = 0; i < n; ++i)
      coords.emplace_back(i / static_cast<double>(n), QRandomGenerator::global()->bounded(1.0));
   helper = compute(coords);
}

void SplineSeries::paint(QPainter *painter) {
   if (coords.size() < 3) return;

   auto w = width();
   auto h = height();

   QPen pen(m_color, m_strokeWidth);
   painter->setPen(pen);

   QPainterPath path;
   path.moveTo(coords[0].x() * width(), coords[0].y() * height());
   for (uint64_t i = 0; i < coords.size() - 1; ++i) {
      auto p = helper[i];
      path.cubicTo(p.first.x() * w, p.first.y() * h, p.second.x() * w, p.second.y() * h, coords[i + 1].x() * w, coords[i + 1].y() * h);
   }
   painter->drawPath(path);

   if (!m_drawKnots) return;

   // Finally draw the knots
   painter->setPen({});
   painter->setBrush(QColor{0, 0, 0});

   for (uint64_t i = 0; i < coords.size(); ++i)
      painter->drawEllipse(QPointF{coords[i].x() * w, coords[i].y() * h}, m_knotSize, m_knotSize);
}

void SplineSeries::setData(quint64 i, QPointF p) {
   coords[i] = p;
   helper = compute(coords);
   update();
}

std::vector<std::pair<QPointF,QPointF>> SplineSeries::compute(std::vector<QPointF> const &coords) {
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

QColor SplineSeries::color() const { return m_color; }

void SplineSeries::setColor(const QColor &color) {
   m_color = color;
   update();
}

qreal SplineSeries::strokeWidth() const { return m_strokeWidth; }

void SplineSeries::setStrokeWidth(const qreal &strokeWidth) {
   m_strokeWidth = strokeWidth;
   update();
}

quint64 SplineSeries::getSize() const { return coords.size(); }

qreal SplineSeries::getKnotSize() const
{
    return m_knotSize;
}

void SplineSeries::setKnotSize(const qreal &knotSize)
{
    m_knotSize = knotSize;
}

bool SplineSeries::getDrawKnots() const
{
    return m_drawKnots;
}

void SplineSeries::setDrawKnots(bool drawKnots)
{
    m_drawKnots = drawKnots;
}

