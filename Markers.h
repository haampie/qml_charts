#ifndef MARKERS_H
#define MARKERS_H

#include <QObject>
#include "AbstractSeries.h"

class Markers : public AbstractSeries
{
   Q_OBJECT

   Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth)
   Q_PROPERTY(qreal radius READ radius WRITE setRadius)
   Q_PROPERTY(QColor strokeColor READ strokeColor WRITE setStrokeColor)
   Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor)

public:
   explicit Markers(QQuickPaintedItem *parent = nullptr);
   void paint(QPainter *painter) override;

   qreal strokeWidth() const;
   void setStrokeWidth(const qreal &strokeWidth);

   QColor strokeColor() const;
   void setStrokeColor(const QColor &strokeColor);

   QColor fillColor() const;
   void setFillColor(const QColor &fillColor);

   qreal radius() const;
   void setRadius(const qreal &radius);

public slots:
   void setData(std::vector<QPointF> const &coords);

private:
   std::vector<QPointF> m_coords;

   QColor m_fillColor{0, 0, 0};
   QColor m_strokeColor{0, 0, 0};
   qreal m_strokeWidth{1.0};
   qreal m_radius{1.0};
};

#endif // MARKERS_H
