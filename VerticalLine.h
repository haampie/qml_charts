#ifndef VERTICALLINE_H
#define VERTICALLINE_H

#include <QObject>
#include <QPainter>
#include "AbstractSeries.h"

class VerticalLine : public AbstractSeries
{
   Q_OBJECT

   Q_PROPERTY(QColor color READ color WRITE setColor)
   Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth)
   Q_PROPERTY(qreal valueX READ valueX WRITE setValueX)

public:
   explicit VerticalLine(QQuickPaintedItem * parent = nullptr);

   QColor color() const;
   void setColor(const QColor &color);
   qreal strokeWidth() const;
   void setStrokeWidth(qreal strokeWidth);
   qreal valueX() const;
   void setValueX(qreal valueX);

   void paint(QPainter *painter) override;

private:
   qreal m_valueX = 0.0;
   QColor m_color{0, 0, 0};
   qreal m_strokeWidth{1.0};
};

#endif // VERTICALLINE_H
