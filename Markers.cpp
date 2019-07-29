#include "Markers.h"

#include <QPainter>

Markers::Markers(QQuickPaintedItem *parent) : AbstractSeries(parent)
{
   setRenderTarget(QQuickPaintedItem::FramebufferObject);
   setAntialiasing(true);
}

void Markers::paint(QPainter *painter) {

   auto w = width();
   auto h = height();
   auto xfrom = m_axisX == nullptr ? 0.0 : m_axisX->getFrom();
   auto yfrom = m_axisY == nullptr ? 0.0 : m_axisY->getFrom();
   auto xto = m_axisX == nullptr ? 1.0 : m_axisX->getTo();
   auto yto = m_axisY == nullptr ? 1.0 : m_axisY->getTo();

   // To chart coords (note: we flip y here)
   auto getx = [xfrom, xto, w](qreal x) { return w * (x - xfrom) / (xto - xfrom);};
   auto gety = [yfrom, yto, h](qreal y) { return h * (1 - (y - yfrom) / (yto - yfrom));};

   painter->setPen({m_strokeColor, m_strokeWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});
   painter->setBrush({m_fillColor});

   for (auto const &coord : m_coords)
      painter->drawEllipse(QPointF{getx(coord.x()), gety(coord.y())}, m_radius, m_radius);
}

qreal Markers::strokeWidth() const
{
   return m_strokeWidth;
}

void Markers::setStrokeWidth(const qreal &strokeWidth)
{
   m_strokeWidth = strokeWidth;
   update();
}

QColor Markers::strokeColor() const
{
   return m_strokeColor;
}

void Markers::setStrokeColor(const QColor &strokeColor)
{
   m_strokeColor = strokeColor;
   update();
}

QColor Markers::fillColor() const
{
   return m_fillColor;
}

void Markers::setFillColor(const QColor &fillColor)
{
   m_fillColor = fillColor;
   update();
}

qreal Markers::radius() const
{
   return m_radius;
}

void Markers::setRadius(const qreal &radius)
{
   m_radius = radius;
   update();
}

void Markers::setData(const std::vector<QPointF> &coords) {
   m_coords = coords;
   update();
}
