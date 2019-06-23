#ifndef GRAPHSTUFF_H
#define GRAPHSTUFF_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QObject>
#include <QString>
#include <QPen>
#include <QPainter>
#include <QPointF>

#include "AbstractSeries.h"

class SplineSeries : public AbstractSeries
{
   Q_OBJECT

   Q_PROPERTY(QColor color READ color WRITE setColor)
   Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth)
   Q_PROPERTY(qreal knotSize READ getKnotSize WRITE setKnotSize)
   Q_PROPERTY(bool drawKnots READ getDrawKnots WRITE setDrawKnots)

   Q_PROPERTY(quint64 size READ getSize CONSTANT)

public:
   explicit SplineSeries(QQuickPaintedItem *parent = nullptr);
   void paint(QPainter *painter) override;

   QColor color() const;
   void setColor(const QColor &color);
   qreal strokeWidth() const;
   void setStrokeWidth(const qreal &strokeWidth);
   bool getDrawKnots() const;
   void setDrawKnots(bool drawKnots);
   qreal getKnotSize() const;
   void setKnotSize(const qreal &knotSize);
   quint64 getSize() const;

public slots:
    void setData(std::vector<QPointF> const &cs);

private:
    std::vector<QPointF> coords;
    std::vector<std::pair<QPointF,QPointF>> helper;

    bool m_drawKnots{true};
    QColor m_color{0, 0, 0};
    qreal m_strokeWidth{1.0};
    qreal m_knotSize{4.0};
};

#endif // GRAPHSTUFF_H
