#ifndef POINT_H
#define POINT_H

#include <QObject>
#include "AbstractSeries.h"
#include <QPainter>

class Point : public AbstractSeries
{
   Q_OBJECT

   Q_PROPERTY(qreal valueY READ valueY WRITE setValueY)
   Q_PROPERTY(qreal valueX READ valueX WRITE setValueX)

public:
   explicit Point(QQuickPaintedItem *parent = nullptr);

   qreal valueX() const;
   void setValueX(const qreal &valueX);

   qreal valueY() const;
   void setValueY(const qreal &valueY);

   void paint(QPainter * painter) override {
      auto w = width();
      auto h = height();
      auto xfrom = m_axisX == nullptr ? 0.0 : m_axisX->getFrom();
      auto yfrom = m_axisY == nullptr ? 0.0 : m_axisY->getFrom();
      auto xto = m_axisX == nullptr ? 1.0 : m_axisX->getTo();
      auto yto = m_axisY == nullptr ? 1.0 : m_axisY->getTo();

      // To chart coords (note: we flip y here)
      auto getx = [xfrom, xto, w](qreal x) { return w * (x - xfrom) / (xto - xfrom);};
      auto gety = [yfrom, yto, h](qreal y) { return h * (1 - (y - yfrom) / (yto - yfrom));};

      painter->drawRect(QRectF{getx(m_valueX) - 10., gety(m_valueY) - 10., 20., 20.});
   }

private:
   qreal m_valueX = 0;
   qreal m_valueY = 0;
};

#endif // POINT_H
