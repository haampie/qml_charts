#include "AreaSplineSeries.h"

#include <QDebug>
#include <QRandomGenerator>
#include <chrono>
#include <iostream>

using namespace std::chrono;

AreaSplineSeries::AreaSplineSeries(QQuickItem *parent) : QQuickPaintedItem(parent) {

   // Enable opengl / antialiasing
   setAntialiasing(true);
   setRenderTarget(QQuickPaintedItem::FramebufferObject);

   // Number of points
   uint64_t n = 20;

   for (size_t i = 0; i < n; ++i) {
      auto x = i / static_cast<double>(n);
      auto l = QRandomGenerator::global()->bounded(0.3);
      auto h = 1 - QRandomGenerator::global()->bounded(0.3);
      lower.emplace_back(x, l);
      upper.emplace_back(x, h);
   }

   upper_helper = compute(upper);
   lower_helper = compute(lower);
}

void AreaSplineSeries::paint(QPainter *painter) {
   if (lower.size() < 3) return;

   auto n = upper.size();
   auto w = width();
   auto h = height();

   QPen pen(m_color, m_strokeWidth);
   painter->setPen(pen);

   // Bottom from left to right
   QPainterPath path;
   path.moveTo(lower[0].x() * w, lower[0].y() * h);
   for (uint64_t i = 0; i < lower.size() - 1; ++i) {
      auto p = lower_helper[i];
      path.cubicTo(p.first.x() * w, p.first.y() * h, p.second.x() * w, p.second.y() * h, lower[i + 1].x() * w, lower[i + 1].y() * h);
   }

   // Connect lower and upper right
   path.lineTo(upper[n - 1].x() * w, upper[n - 1].y() * h);

   // Top right to left
   for (uint64_t i = n - 1; i > 0; --i) {
      auto p = upper_helper[i - 1];
      path.cubicTo(p.second.x() * w, p.second.y() * h, p.first.x() * w, p.first.y() * h, upper[i - 1].x() * w, upper[i - 1].y() * h);
   }

   // Connect upper and lower left
   path.lineTo(lower[0].x() * w, lower[0].y() * h);

   painter->drawPath(path);
   painter->fillPath(path, m_color);

   if (!m_drawKnots) return;

   // Finally draw the knots
   painter->setPen({});
   painter->setBrush(QColor{0, 0, 0});

   for (uint64_t i = 0; i < lower.size(); ++i)
      painter->drawEllipse(QPointF{lower[i].x() * width(), lower[i].y() * height()}, m_knotSize, m_knotSize);

   for (uint64_t i = 0; i < upper.size(); ++i)
      painter->drawEllipse(QPointF{upper[i].x() * width(), upper[i].y() * height()}, m_knotSize, m_knotSize);
}

void AreaSplineSeries::setDataLower(quint64 i, const QPointF &p) {
   lower[i] = p;
   upper_helper = compute(upper);
   lower_helper = compute(lower);
   update();
}

void AreaSplineSeries::setDataUpper(quint64 i, const QPointF &p) {
   upper[i] = p;
   upper_helper = compute(upper);
   lower_helper = compute(lower);
   update();
}

std::vector<std::pair<QPointF,QPointF>> AreaSplineSeries::compute(std::vector<QPointF> const &coords) {
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

QColor AreaSplineSeries::color() const { return m_color; }

void AreaSplineSeries::setColor(const QColor &color) {
   m_color = color;
   update();
}

qreal AreaSplineSeries::strokeWidth() const { return m_strokeWidth; }

void AreaSplineSeries::setStrokeWidth(const qreal &strokeWidth) {
   m_strokeWidth = strokeWidth;
   update();
}

quint64 AreaSplineSeries::getSize() const { return upper.size(); }

qreal AreaSplineSeries::getKnotSize() const
{
    return m_knotSize;
}

void AreaSplineSeries::setKnotSize(const qreal &knotSize)
{
    m_knotSize = knotSize;
}

bool AreaSplineSeries::getDrawKnots() const
{
    return m_drawKnots;
}

void AreaSplineSeries::setDrawKnots(bool drawKnots)
{
    m_drawKnots = drawKnots;
}
