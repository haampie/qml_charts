#include "AreaSplineSeries.h"

#include <QDebug>
#include <QRandomGenerator>
#include <chrono>
#include <iostream>
#include <QRectF>
#include "SplineHelper.h"
#include <tuple>

using namespace std::chrono;

AreaSplineSeries::AreaSplineSeries(QQuickPaintedItem *parent)
   : AbstractSeries(parent)
{
   // Enable opengl / antialiasing
   setAntialiasing(true);
   setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

void AreaSplineSeries::paint(QPainter *painter) {
   if (lower.size() == 0 || upper.size() == 0)
       return;

   auto n = upper.size();
   auto w = width();
   auto h = height();
   auto xfrom = m_axisX == nullptr ? 0.0 : m_axisX->getFrom();
   auto yfrom = m_axisY == nullptr ? 0.0 : m_axisY->getFrom();
   auto xto = m_axisX == nullptr ? 1.0 : m_axisX->getTo();
   auto yto = m_axisY == nullptr ? 1.0 : m_axisY->getTo();

   // To chart coords (note: we flip y here)
   auto getx = [xfrom, xto, w](qreal x) { return w * (x - xfrom) / (xto - xfrom);};
   auto gety = [yfrom, yto, h](qreal y) { return h * (1 - (y - yfrom) / (yto - yfrom));};

   QPen pen(m_color);
   painter->setPen(pen);

   // Bottom from left to right
   QPainterPath path;
   path.moveTo(getx(lower[0].x()), gety(lower[0].y()));
   for (uint64_t i = 0; i < lower.size() - 1; ++i) {
      auto p = lower_helper[i];
      path.cubicTo(
         getx(p.first.x()),
         gety(p.first.y()),
         getx(p.second.x()),
         gety(p.second.y()),
         getx(lower[i + 1].x()),
         gety(lower[i + 1].y())
      );
   }

   // Connect lower and upper right
   path.lineTo(getx(upper[n - 1].x()), gety(upper[n - 1].y()));

   // Top right to left
   for (uint64_t i = n - 1; i > 0; --i) {
      auto p = upper_helper[i - 1];
      path.cubicTo(
         getx(p.second.x()),
         gety(p.second.y()),
         getx(p.first.x()),
         gety(p.first.y()),
         getx(upper[i - 1].x()),
         gety(upper[i - 1].y())
      );
   }

   // Connect upper and lower left
   path.lineTo(getx(lower[0].x()), gety(lower[0].y()));

   painter->drawPath(path);
   painter->fillPath(path, m_color);

   if (!m_drawKnots) return;

   // Finally draw the knots
   painter->setPen({});
   painter->setBrush(QColor{0, 0, 0, 100});

   for (uint64_t i = 0; i < lower.size(); ++i)
      painter->drawRect(QRectF{getx(lower[i].x()) - m_knotSize/2, gety(lower[i].y()) - m_knotSize/2, m_knotSize, m_knotSize});

   for (uint64_t i = 0; i < upper.size(); ++i)
      painter->drawRect(QRectF{getx(upper[i].x()) - m_knotSize/2, gety(upper[i].y()) - m_knotSize/2, m_knotSize, m_knotSize});
}

void AreaSplineSeries::setData(std::vector<QPointF> const &l, std::vector<QPointF> const &u) {
   lower = l;
   upper = u;
   upper_helper = SplineHelper::compute(upper);
   lower_helper = SplineHelper::compute(lower);
   update();
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
