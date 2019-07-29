#ifndef HORIZONTALLINE_H
#define HORIZONTALLINE_H

#include <QObject>
#include <QPainter>
#include "AbstractSeries.h"

class HorizontalLine : public AbstractSeries
{
   Q_OBJECT

   Q_PROPERTY(QColor color READ color WRITE setColor)
   Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth)
   Q_PROPERTY(qreal valueY READ valueY WRITE setValueY)

public:
   explicit HorizontalLine(QQuickPaintedItem * parent = nullptr);

   QColor color() const;
   void setColor(const QColor &color);
   qreal strokeWidth() const;
   void setStrokeWidth(qreal strokeWidth);
   qreal valueY() const;
   void setValueY(qreal valueX);

   void paint(QPainter *painter) override;

private:
   qreal m_valueY = 0.0;
   QColor m_color{0, 0, 0};
   qreal m_strokeWidth{1.0};
};

#endif // HORIZONTALLINE_H
