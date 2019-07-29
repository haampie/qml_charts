#ifndef AREAGRAPHSTUFF_H
#define AREAGRAPHSTUFF_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QObject>
#include <QString>
#include <QPen>
#include <QPainter>
#include "Axis.h"
#include "AbstractSeries.h"

#include <vector>
#include <tuple>
#include <QPointF>

class AreaSplineSeries : public AbstractSeries
{
   Q_OBJECT

   Q_PROPERTY(QColor color READ color WRITE setColor)
   Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth)
   Q_PROPERTY(int splineType READ splineType WRITE setSplineType)

   Q_PROPERTY(quint64 size READ getSize CONSTANT)

public:
   explicit AreaSplineSeries(QQuickPaintedItem *parent = nullptr);

   void paint(QPainter *painter) override;

   QColor color() const;
   void setColor(const QColor &color);
   qreal strokeWidth() const;
   void setStrokeWidth(const qreal &strokeWidth);
   quint64 getSize() const;
   int splineType() const;
   void setSplineType(int value);

public slots:
   void setData(std::vector<QPointF> const &lower, std::vector<QPointF> const &upper);

private:
   std::vector<std::pair<QPointF,QPointF>> upper_helper;
   std::vector<std::pair<QPointF,QPointF>> lower_helper;
   std::vector<QPointF> lower;
   std::vector<QPointF> upper;

   QColor m_color{0, 0, 0};
   qreal m_strokeWidth{1.0};
   int m_splineType{0};
};

#endif // AREAGRAPHSTUFF_H
