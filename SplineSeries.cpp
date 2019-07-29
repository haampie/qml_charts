#include "SplineSeries.h"
#include "SplineHelper.h"

SplineSeries::SplineSeries(QQuickPaintedItem *parent)
   : AbstractSeries(parent)
{
   // Enable opengl / antialiasing
   setRenderTarget(QQuickPaintedItem::FramebufferObject);
   setAntialiasing(true);
}

void SplineSeries::paint(QPainter *painter) {
   if (coords.size() < 3) return;

   auto w = width();
   auto h = height();
   auto xfrom = m_axisX == nullptr ? 0.0 : m_axisX->getFrom();
   auto yfrom = m_axisY == nullptr ? 0.0 : m_axisY->getFrom();
   auto xto = m_axisX == nullptr ? 1.0 : m_axisX->getTo();
   auto yto = m_axisY == nullptr ? 1.0 : m_axisY->getTo();

   // To chart coords (note: we flip y here)
   auto getx = [xfrom, xto, w](qreal x) { return w * (x - xfrom) / (xto - xfrom);};
   auto gety = [yfrom, yto, h](qreal y) { return h * (1 - (y - yfrom) / (yto - yfrom));};

   painter->setPen({m_color, m_strokeWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});

   QPainterPath path;
   path.moveTo(getx(coords[0].x()), gety(coords[0].y()));
   for (uint64_t i = 0; i < coords.size() - 1; ++i) {
      auto p = helper[i];
      path.cubicTo(
         getx(p.first.x()),
         gety(p.first.y()),
         getx(p.second.x()),
         gety(p.second.y()),
         getx(coords[i + 1].x()),
         gety(coords[i + 1].y())
      );
   }
   painter->drawPath(path);
}

void SplineSeries::setData(std::vector<QPointF> const &cs) {
   coords = std::move(cs);
   helper = m_splineType == 0 ? SplineHelper::compute(coords) : SplineHelper::spline1d(coords);
   update();
}

int SplineSeries::splineType() const
{
   return m_splineType;
}

void SplineSeries::setSplineType(int splineType)
{
   m_splineType = splineType;
   helper = m_splineType == 0 ? SplineHelper::compute(coords) : SplineHelper::spline1d(coords);
   update();
}

QColor SplineSeries::color() const { return m_color; }

void SplineSeries::setColor(const QColor &color) {
   m_color = color;
   update();
}

qreal SplineSeries::strokeWidth() const { return m_strokeWidth; }

void SplineSeries::setStrokeWidth(const qreal &strokeWidth) {
   m_strokeWidth = strokeWidth;
   update();
}

quint64 SplineSeries::getSize() const { return coords.size(); }

