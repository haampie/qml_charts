#include "MajorLinesY.h"

#include <cmath>
#include <QPainter>
#include <QPainterPath>

MajorLinesY::MajorLinesY(QQuickPaintedItem *parent) : QQuickPaintedItem(parent)
{
   // Enable opengl / antialiasing
   setRenderTarget(QQuickPaintedItem::FramebufferObject);
   setAntialiasing(true);
}

void MajorLinesY::paint(QPainter * painter)
{
   auto h = height();
   auto w = width();

   auto from = m_axis == nullptr ? 0.0 : m_axis->getFrom();
   auto to = m_axis == nullptr ? 0.0 : m_axis->getTo();
   auto gety = [from, to, h](qreal y) { return h * (1 - (y - from) / (to - from));};

   // number of steps
   auto steprange = [this](qreal from, qreal to, qreal ticksize) -> std::pair<int64_t, int64_t> {
      auto fst = (from - m_origin) / ticksize;
      auto snd = (to - m_origin) / ticksize;
      auto r = std::minmax(fst, snd);
      return {static_cast<int64_t>(std::floor(r.first)), static_cast<int64_t>(std::ceil(r.second))};
   };

   // Small ticks
   auto smalltickrange = steprange(from, to, m_smallTick);
   {
      painter->setPen(QPen{m_color, m_strokeWidthSmall});
      QPainterPath path;
      for (int64_t idx = smalltickrange.first; idx <= smalltickrange.second; ++idx)
      {
         auto y_pixel = gety(m_origin + m_smallTick * idx);

         path.moveTo(0.0, y_pixel);
         path.lineTo(w * 0.6, y_pixel);
      }

      painter->drawPath(path);
   }

   // large ticks
   auto largetickrange = steprange(from, to, m_largeTick);
   {
      painter->setPen(QPen{m_color, m_strokeWidthLarge});
      QPainterPath path;

      // Ticks around 1 unit
      for (int64_t idx = largetickrange.first; idx <= largetickrange.second; ++idx)
      {
         auto y_pixel = gety(m_origin + m_largeTick * idx);

         path.moveTo(0.0, y_pixel);
         path.lineTo(w, y_pixel);
      }

      painter->drawPath(path);
   }
}

Axis *MajorLinesY::getAxis() const
{
   return m_axis;
}

void MajorLinesY::setAxis(Axis *axis)
{
   // Disconnect signals from previous axis
   if (m_axis != nullptr)
      disconnect(m_axis, &Axis::rangeChanged, this, &MajorLinesY::onAxisChanged);

   m_axis = axis;

   // Listen to new axis changes
   if (m_axis != nullptr)
      connect(m_axis, &Axis::rangeChanged, this, &MajorLinesY::onAxisChanged);

   // Repaint
   update();
}

void MajorLinesY::onAxisChanged() {
   update();
}

qreal MajorLinesY::getOrigin() const
{
   return m_origin;
}

void MajorLinesY::setOrigin(const qreal &origin)
{
   m_origin = origin;
}

qreal MajorLinesY::getStrokeWidthLarge() const
{
   return m_strokeWidthLarge;
}

void MajorLinesY::setStrokeWidthLarge(const qreal &strokeWidthLarge)
{
   m_strokeWidthLarge = strokeWidthLarge;
   update();
}

QColor MajorLinesY::getColor() const
{
   return m_color;
}

void MajorLinesY::setColor(const QColor &color)
{
   m_color = color;
   update();
}

qreal MajorLinesY::getStrokeWidthSmall() const
{
   return m_strokeWidthSmall;
}

void MajorLinesY::setStrokeWidthSmall(const qreal &strokeWidth)
{
   m_strokeWidthSmall = strokeWidth;
   update();
}

qreal MajorLinesY::getSmallTick() const
{
   return m_smallTick;
}

void MajorLinesY::setSmallTick(const qreal &spacing)
{
   m_smallTick = spacing;
   update();
}


qreal MajorLinesY::getLargeTick() const
{
   return m_largeTick;
}

void MajorLinesY::setLargeTick(const qreal &spacing)
{
   m_largeTick = spacing;
   update();
}
