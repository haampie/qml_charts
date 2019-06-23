#ifndef ABSTRACTSERIES_H
#define ABSTRACTSERIES_H

#include <QObject>
#include "Axis.h"
#include <QQuickPaintedItem>

class AbstractSeries : public QQuickPaintedItem
{
   Q_OBJECT

   Q_PROPERTY(Axis * axisX READ getAxisX WRITE setAxisX)
   Q_PROPERTY(Axis * axisY READ getAxisY WRITE setAxisY)

public:
   explicit AbstractSeries(QQuickPaintedItem *parent = nullptr);

   Axis *getAxisX() const;
   void setAxisX(Axis *axisX);
   Axis *getAxisY() const;
   void setAxisY(Axis *axisY);

private slots:
   void onAxisChanged();

protected:
   Axis * m_axisX = nullptr;
   Axis * m_axisY = nullptr;
};

#endif // ABSTRACTSERIES_H
