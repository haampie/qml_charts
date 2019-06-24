#include "Point.h"

Point::Point(QQuickPaintedItem *parent) : AbstractSeries(parent)
{
   setRenderTarget(QQuickPaintedItem::FramebufferObject);
   setAntialiasing(true);
}

qreal Point::valueX() const
{
   return m_valueX;
}

void Point::setValueX(const qreal &valueX)
{
   m_valueX = valueX;
   update();
}

qreal Point::valueY() const
{
   return m_valueY;
}

void Point::setValueY(const qreal &valueY)
{
   m_valueY = valueY;
   update();
}

void Point::paint(QPainter *painter) {
   auto w = width();
   auto h = height();
   auto xfrom = m_axisX == nullptr ? 0.0 : m_axisX->getFrom();
   auto yfrom = m_axisY == nullptr ? 0.0 : m_axisY->getFrom();
   auto xto = m_axisX == nullptr ? 1.0 : m_axisX->getTo();
   auto yto = m_axisY == nullptr ? 1.0 : m_axisY->getTo();

   // To chart coords (note: we flip y here)
   auto getx = [xfrom, xto, w](qreal x) { return w * (x - xfrom) / (xto - xfrom);};
   auto gety = [yfrom, yto, h](qreal y) { return h * (1 - (y - yfrom) / (yto - yfrom));};

   painter->setPen({});
   painter->setBrush(QColor{"white"});
   painter->drawEllipse(QPointF{getx(m_valueX), gety(m_valueY)}, 10.0, 10.0);
   painter->setBrush(QColor{"black"});
   painter->setBrush({});
   painter->setPen(QPen{QColor{"black"}, 2});
   painter->drawEllipse(QPointF{getx(m_valueX), gety(m_valueY)}, 10.0, 10.0);
}
