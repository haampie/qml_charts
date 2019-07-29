#ifndef MAJORLINESY_H
#define MAJORLINESY_H

#include <QObject>
#include <QQuickPaintedItem>

#include "Axis.h"

class MajorLinesY : public QQuickPaintedItem
{
   Q_OBJECT

   Q_PROPERTY(Axis * axis READ getAxis WRITE setAxis)
   Q_PROPERTY(QColor color READ getColor WRITE setColor)
   Q_PROPERTY(qreal strokeWidthSmall READ getStrokeWidthSmall WRITE setStrokeWidthSmall)
   Q_PROPERTY(qreal strokeWidthLarge READ getStrokeWidthLarge WRITE setStrokeWidthLarge)
   Q_PROPERTY(qreal smallTick READ getSmallTick WRITE setSmallTick)
   Q_PROPERTY(qreal largeTick READ getLargeTick WRITE setLargeTick)
   Q_PROPERTY(qreal origin READ getOrigin WRITE setOrigin)

public:
   explicit MajorLinesY(QQuickPaintedItem *parent = nullptr);
   void paint(QPainter * painter) override;

   Axis *getAxis() const;
   void setAxis(Axis *value);

   QColor getColor() const;
   void setColor(const QColor &color);

   qreal getStrokeWidthSmall() const;
   void setStrokeWidthSmall(const qreal &strokeWidth);

   qreal getSmallTick() const;
   void setSmallTick(const qreal &spacing);

   qreal getLargeTick() const;
   void setLargeTick(const qreal &spacing);

   qreal getStrokeWidthLarge() const;
   void setStrokeWidthLarge(const qreal &strokeWidthLarge);

   qreal getOrigin() const;
   void setOrigin(const qreal &origin);

   void createTickPath();
   
private slots:
   void onAxisChanged();

private:
   Axis * m_axis = nullptr;
   QColor m_color{};
   qreal m_strokeWidthSmall = 1.0;
   qreal m_strokeWidthLarge = 2.0;
   qreal m_smallTick = 1.0;
   qreal m_largeTick = 5.0;
   qreal m_origin = 0.0;
};

#endif // MAJORLINESY_H
