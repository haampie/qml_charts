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

   void paint(QPainter * painter) override;

private:
   qreal m_valueX = 0;
   qreal m_valueY = 0;
};

#endif // POINT_H
