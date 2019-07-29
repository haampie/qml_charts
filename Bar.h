#ifndef BAR_H
#define BAR_H

#include <QObject>
#include "AbstractSeries.h"

class Bar : public AbstractSeries
{
   Q_OBJECT

   Q_PROPERTY(QColor color READ color WRITE setColor)
   Q_PROPERTY(qreal barWidth READ barWidth WRITE setBarWidth)

public:
   explicit Bar(QQuickPaintedItem *parent = nullptr);

   void paint(QPainter * painter) override;

   QColor color() const;
   void setColor(const QColor &color);
   qreal barWidth() const;
   void setBarWidth(qreal width);

public slots:
   void setData(std::vector<QPointF> const &data);

private:

   std::vector<QPointF> m_data;
   QColor m_color;
   qreal m_barWidth = 1.0;
};

#endif // BAR_H
