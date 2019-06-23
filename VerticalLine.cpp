#include "VerticalLine.h"

VerticalLine::VerticalLine(QQuickPaintedItem *parent) : AbstractSeries(parent) {

   // Enable opengl / antialiasing
   setRenderTarget(QQuickPaintedItem::FramebufferObject);
   setAntialiasing(true);
}

void VerticalLine::paint(QPainter *painter)
{
   auto w = width();
   auto h = height();
   auto xfrom = m_axisX == nullptr ? 0.0 : m_axisX->getFrom();
   auto xto = m_axisX == nullptr ? 1.0 : m_axisX->getTo();

   // To chart coords (note: we flip y here)
   auto xchart = w * (m_valueX - xfrom) / (xto - xfrom);

   painter->setPen({m_color, m_strokeWidth});
   painter->drawLine(QPointF{xchart, 0.0}, QPointF{xchart, h});
}

QColor VerticalLine::color() const { return m_color; }

void VerticalLine::setColor(const QColor &color) {
   m_color = color;
   update();
}

qreal VerticalLine::strokeWidth() const { return m_strokeWidth; }

qreal VerticalLine::valueX() const {
   return m_valueX;
}

void VerticalLine::setStrokeWidth(qreal strokeWidth) {
   m_strokeWidth = strokeWidth;
   update();
}

void VerticalLine::setValueX(qreal valueX) {
   m_valueX = valueX;
   update();
}
