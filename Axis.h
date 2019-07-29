#ifndef AXIS_H
#define AXIS_H

#include <QObject>

class Axis : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal from READ getFrom WRITE setFrom NOTIFY rangeChanged)
    Q_PROPERTY(qreal to READ getTo WRITE setTo NOTIFY rangeChanged)

public:
    explicit Axis(QObject *parent = nullptr);

    qreal getFrom() const;
    void setFrom(qreal value);
    qreal getTo() const;
    void setTo(qreal value);

signals:
    void rangeChanged(qreal from, qreal to);

private:
    qreal from = 0;
    qreal to = 1;
};

#endif // AXIS_H
