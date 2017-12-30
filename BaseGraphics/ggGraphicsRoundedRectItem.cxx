// 0) include own header
#include "ggGraphicsRoundedRectItem.h"

// 1) include system or QT
#include <QPainter>

// 2) include own project-related (sort by component dependency)


ggGraphicsRoundedRectItem::ggGraphicsRoundedRectItem(QGraphicsItem* aParent) :
  QGraphicsRectItem(aParent),
  mRadiusX(3.0f),
  mRadiusY(3.0f)
{
}


ggGraphicsRoundedRectItem::ggGraphicsRoundedRectItem(const QRectF& aRect,
                                                     const qreal aRadius,
                                                     QGraphicsItem* aParent) :
  QGraphicsRectItem(aRect, aParent),
  mRadiusX(aRadius),
  mRadiusY(aRadius)
{
}


qreal ggGraphicsRoundedRectItem::GetRadiusX() const
{
  return mRadiusX;
}


qreal ggGraphicsRoundedRectItem::GetRadiusY() const
{
  return mRadiusY;
}


void ggGraphicsRoundedRectItem::SetRadius(qreal aRadius)
{
  SetRadius(aRadius, aRadius);
}


void ggGraphicsRoundedRectItem::SetRadius(qreal aRadiusX, qreal aRadiusY)
{
  if (mRadiusX != aRadiusX ||
      mRadiusY != aRadiusY) {
    mRadiusX = aRadiusX;
    mRadiusY = aRadiusY;
    update();
  }
}


QRectF ggGraphicsRoundedRectItem::GetInnerRect() const
{
  return rect() - QMarginsF(mRadiusX, mRadiusY, mRadiusX, mRadiusY);
}


void ggGraphicsRoundedRectItem::SetInnerRect(const QRectF& aRect)
{
  setRect(aRect + QMarginsF(mRadiusX, mRadiusY, mRadiusX, mRadiusY));
}


void ggGraphicsRoundedRectItem::paint(QPainter* aPainter,
                                      const QStyleOptionGraphicsItem*,
                                      QWidget*)
{
  aPainter->setPen(pen());
  aPainter->setBrush(brush());
  aPainter->drawRoundedRect(rect(), mRadiusX, mRadiusY);
}


QPainterPath ggGraphicsRoundedRectItem::shape() const
{
  if (brush().style() != Qt::NoBrush) {
    QPainterPath vPath;
    qreal vPenWidth2 = (pen().style() != Qt::NoPen) ? pen().widthF()/2.0f : 0.0f;
    QRectF vRect = rect() + QMarginsF(vPenWidth2, vPenWidth2, vPenWidth2, vPenWidth2);
    vPath.addRoundedRect(vRect, mRadiusX + vPenWidth2, mRadiusY + vPenWidth2);
    return vPath;
  }
  else {
    QPainterPath vPath;
    vPath.addRoundedRect(rect(), mRadiusX, mRadiusY);
    QPainterPathStroker vPathStroker(pen());
    return vPathStroker.createStroke(vPath).simplified();
  }
}
