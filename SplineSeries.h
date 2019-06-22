#ifndef GRAPHSTUFF_H
#define GRAPHSTUFF_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QObject>
#include <QString>
#include <QPen>
#include <QPainter>
#include <QPointF>

class SplineSeries : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth)
    Q_PROPERTY(quint64 size READ getSize CONSTANT)

public:
    explicit SplineSeries(QQuickItem *parent = nullptr);
    QPointF interpolate(uint64_t bucket, double t);

    QColor color() const;
    void setColor(const QColor &color);
    void paint(QPainter *painter) override;
    qreal strokeWidth() const;
    void setStrokeWidth(const qreal &strokeWidth);
    quint64 getSize() const;

public slots:
    void setData(quint64 i, QPointF p) {
        coords[i] = p;
        compute();
        update();
    }

private:
    void compute();

    std::vector<QPointF> p1s;
    std::vector<QPointF> p2s;
    std::vector<QPointF> coords;

    QColor m_color;
    qreal m_strokeWidth = 1;
};

#endif // GRAPHSTUFF_H
