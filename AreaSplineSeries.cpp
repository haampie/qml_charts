#include "AreaSplineSeries.h"
#include "SplineHelper.h"

#include <QRectF>

AreaSplineSeries::AreaSplineSeries(QQuickPaintedItem *parent)
   : AbstractSeries(parent)
{
   // Enable opengl / antialiasing
   setAntialiasing(true);
   setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

void AreaSplineSeries::paint(QPainter *painter) {
   if (lower.size() == 0 || upper.size() == 0)
       return;

   // This prevents stroke opacity + fill opacity multiplying troubles
   painter->setCompositionMode(QPainter::CompositionMode_Source);

   auto w = width();
   auto h = height();
   auto xfrom = m_axisX == nullptr ? 0.0 : m_axisX->getFrom();
   auto yfrom = m_axisY == nullptr ? 0.0 : m_axisY->getFrom();
   auto xto = m_axisX == nullptr ? 1.0 : m_axisX->getTo();
   auto yto = m_axisY == nullptr ? 1.0 : m_axisY->getTo();

   // To chart coords (note: we flip y here)
   auto getx = [xfrom, xto, w](qreal x) { return w * (x - xfrom) / (xto - xfrom);};
   auto gety = [yfrom, yto, h](qreal y) { return h * (1 - (y - yfrom) / (yto - yfrom));};

   painter->setPen(QPen{m_color, m_strokeWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});

   // Bottom from left to right
   QPainterPath path;
   path.moveTo(getx(lower[0].x()), gety(lower[0].y()));
   for (uint64_t i = 0; i < lower.size() - 1; ++i) {
      auto p = lower_helper[i];
      path.cubicTo(
         getx(p.first.x()),
         gety(p.first.y()),
         getx(p.second.x()),
         gety(p.second.y()),
         getx(lower[i + 1].x()),
         gety(lower[i + 1].y())
      );
   }

   // Connect lower and upper right
   path.lineTo(getx(upper.back().x()), gety(upper.back().y()));

   // Top right to left
   for (uint64_t i = upper.size() - 1; i > 0; --i) {
      auto p = upper_helper[i - 1];
      path.cubicTo(
         getx(p.second.x()),
         gety(p.second.y()),
         getx(p.first.x()),
         gety(p.first.y()),
         getx(upper[i - 1].x()),
         gety(upper[i - 1].y())
      );
   }

   // Connect upper and lower left
   path.lineTo(getx(lower[0].x()), gety(lower[0].y()));

   painter->fillPath(path, m_color);
   painter->drawPath(path);
}

void AreaSplineSeries::setData(std::vector<QPointF> const &l, std::vector<QPointF> const &u) {
   lower = std::move(l);
   upper = std::move(u);
   upper_helper = m_splineType == 0 ? SplineHelper::compute(upper) : SplineHelper::spline1d(upper);
   lower_helper = m_splineType == 0 ? SplineHelper::compute(lower) : SplineHelper::spline1d(lower);
   update();
}

int AreaSplineSeries::splineType() const
{
   return m_splineType;
}

void AreaSplineSeries::setSplineType(int value)
{
   m_splineType = value;
   upper_helper = m_splineType == 0 ? SplineHelper::compute(upper) : SplineHelper::spline1d(upper);
   lower_helper = m_splineType == 0 ? SplineHelper::compute(lower) : SplineHelper::spline1d(lower);
   update();
}

QColor AreaSplineSeries::color() const { return m_color; }

void AreaSplineSeries::setColor(const QColor &color) {
   m_color = color;
   update();
}

qreal AreaSplineSeries::strokeWidth() const { return m_strokeWidth; }

void AreaSplineSeries::setStrokeWidth(const qreal &strokeWidth) {
   m_strokeWidth = strokeWidth;
   update();
}

quint64 AreaSplineSeries::getSize() const { return upper.size(); }
