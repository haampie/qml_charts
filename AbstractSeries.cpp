#include "AbstractSeries.h"

AbstractSeries::AbstractSeries(QQuickPaintedItem *parent)
   : QQuickPaintedItem(parent)
{}

Axis *AbstractSeries::getAxisX() const
{
   return m_axisX;
}

void AbstractSeries::setAxisX(Axis *axisX)
{
   // Disconnect signals from previous axis
   if (m_axisX != nullptr)
      disconnect(m_axisX, &Axis::rangeChanged, this, &AbstractSeries::onAxisChanged);

   m_axisX = axisX;

   // Listen to new axis changes
   if (m_axisX != nullptr)
      connect(m_axisX, &Axis::rangeChanged, this, &AbstractSeries::onAxisChanged);

   // Repaint
   update();
}

Axis *AbstractSeries::getAxisY() const
{
   return m_axisY;
}

void AbstractSeries::setAxisY(Axis *axisY)
{
   // Disconnect signals from previous axis
   if (m_axisY != nullptr)
      disconnect(m_axisY, &Axis::rangeChanged, this, &AbstractSeries::onAxisChanged);

   m_axisY = axisY;

   // Listen to new axis changes
   if (m_axisY != nullptr)
      connect(m_axisY, &Axis::rangeChanged, this, &AbstractSeries::onAxisChanged);

   // And repaint
   update();
}

void AbstractSeries::onAxisChanged()
{
   update();
}
