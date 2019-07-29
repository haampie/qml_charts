#include "Bar.h"

#include <QPainterPath>
#include <QPainter>
#include <cmath>

Bar::Bar(QQuickPaintedItem *parent) : AbstractSeries(parent)
{
   // Enable opengl / antialiasing
   setAntialiasing(true);
   setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

void Bar::paint(QPainter * painter)
{
   auto w = width();
   auto h = height();
   auto xfrom = m_axisX == nullptr ? 0.0 : m_axisX->getFrom();
   auto yfrom = m_axisY == nullptr ? 0.0 : m_axisY->getFrom();
   auto xto = m_axisX == nullptr ? 1.0 : m_axisX->getTo();
   auto yto = m_axisY == nullptr ? 1.0 : m_axisY->getTo();

   // To chart coords (note: we flip y here)
   auto getx = [xfrom, xto, w](qreal x) { return w * (x - xfrom) / (xto - xfrom);};
   auto gety = [yfrom, yto, h](qreal y) { return h * (1 - (y - yfrom) / (yto - yfrom));};

   // Calculate the width in pixels
   auto barwidthpx = getx(m_barWidth) - getx(0.0);

   // To compute the height in pixels
   auto yorigin = gety(0.0);

   painter->setPen(QColor{100, 100, 100, 100});

   for (size_t i = 0; i < m_data.size(); ++i)
   {
      // todo: negative values for y...
      auto x = getx(m_data[i].x());
      auto y = gety(m_data[i].y());
      auto h = std::abs(y - yorigin);
      painter->fillRect(QRectF{x, y, barwidthpx, h}, {m_color});
   }
}

QColor Bar::color() const
{
   return m_color;
}

void Bar::setColor(const QColor &color)
{
   if (color == m_color) return;
   m_color = color;
   update();
}

qreal Bar::barWidth() const
{
   return m_barWidth;
}

void Bar::setBarWidth(qreal width)
{
   m_barWidth = width;
   update();
}

void Bar::setData(const std::vector<QPointF> &data)
{
   m_data = data;
   update();
}
