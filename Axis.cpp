#include "Axis.h"

Axis::Axis(QObject *parent) : QObject(parent)
{

}

qreal Axis::getFrom() const
{
    return from;
}

void Axis::setFrom(qreal value)
{
    from = value;
    emit rangeChanged(from, to);
}

qreal Axis::getTo() const
{
    return to;
}

void Axis::setTo(qreal value)
{
    to = value;
    emit rangeChanged(from, to);
}
