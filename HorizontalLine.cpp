#include "HorizontalLine.h"

using namespace std::chrono;

HorizontalLine::HorizontalLine(QQuickPaintedItem *parent)
   : AbstractSeries(parent)
{
   setRenderTarget(QQuickPaintedItem::FramebufferObject);
   setAntialiasing(true);
}

void HorizontalLine::paint(QPainter *painter)
{
   auto w = width();
   auto h = height();
   auto yfrom = m_axisY == nullptr ? 0.0 : m_axisY->getFrom();
   auto yto = m_axisY == nullptr ? 1.0 : m_axisY->getTo();

   // To chart coords (note: we flip y here)
   auto ychart = h * (m_valueY - yfrom) / (yto - yfrom);

   painter->setPen({m_color, m_strokeWidth});
   painter->drawLine(QPointF{0.0, ychart}, QPointF{w, ychart});
}

QColor HorizontalLine::color() const { return m_color; }

void HorizontalLine::setColor(const QColor &color) {
   m_color = color;
   update();
}

qreal HorizontalLine::strokeWidth() const { return m_strokeWidth; }

qreal HorizontalLine::valueY() const {
   return m_valueY;
}

void HorizontalLine::setStrokeWidth(qreal strokeWidth) {
   m_strokeWidth = strokeWidth;
   update();
}

void HorizontalLine::setValueY(qreal valueY) {
   m_valueY = valueY;
   update();
}
