#include "Point.h"

Point::Point(QQuickPaintedItem *parent) : AbstractSeries(parent)
{

}

qreal Point::valueX() const
{
   return m_valueX;
}

void Point::setValueX(const qreal &valueX)
{
   m_valueX = valueX;
}

qreal Point::valueY() const
{
   return m_valueY;
}

void Point::setValueY(const qreal &valueY)
{
   m_valueY = valueY;
}
