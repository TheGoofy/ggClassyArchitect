// 0) include own header
#include "ggGraphicsShadowRectItem.h"

// 1) include system or QT
#include <QPainter>

// 2) include own project-related (sort by component dependency)
#include "Base/ggUtility.h"


ggGraphicsShadowRectItem::ggGraphicsShadowRectItem(QGraphicsItem* aParent) :
  QGraphicsRectItem(aParent),
  mRadius(1.0f),
  mWidth(1.0f),
  mOuterColor(Qt::black)
{
}


ggGraphicsShadowRectItem::ggGraphicsShadowRectItem(const QRectF& aInnerRect, qreal aRadius,
                                                   QGraphicsItem* aParent) :
  QGraphicsRectItem(aInnerRect + QMarginsF(aRadius, aRadius, aRadius, aRadius), aParent),
  mRadius(aRadius),
  mWidth(aRadius),
  mOuterColor(Qt::black)
{
}


float ggGraphicsShadowRectItem::GetRadius() const
{
  return mRadius;
}


void ggGraphicsShadowRectItem::SetRadius(float aRadius)
{
  if (aRadius != mRadius) {
    mRadius = aRadius;
    update();
  }
}


float ggGraphicsShadowRectItem::GetShadowWidth() const
{
  return mWidth;
}


void ggGraphicsShadowRectItem::SetShadowWidth(float aWidth)
{
  if (aWidth != mWidth) {
    mWidth = aWidth;
    update();
  }
}


QRectF ggGraphicsShadowRectItem::GetInnerRect() const
{
  return rect() - QMarginsF(mRadius, mRadius, mRadius, mRadius);
}


void ggGraphicsShadowRectItem::SetInnerRect(const QRectF& aRect)
{
  setRect(aRect + QMarginsF(mRadius, mRadius, mRadius, mRadius));
}


const QColor& ggGraphicsShadowRectItem::GetOuterColor() const
{
  return mOuterColor;
}


void ggGraphicsShadowRectItem::SetOuterColor(const QColor& aColor)
{
  if (aColor != mOuterColor) {
    mOuterColor = aColor;
    update();
  }
}


void ggGraphicsShadowRectItem::SetShadowColors(const QColor& aColor)
{
  mOuterColor = QColor(aColor.red(), aColor.green(), aColor.blue(), 0);
  setBrush(aColor);
  update();
}


void ggGraphicsShadowRectItem::paint(QPainter* aPainter,
                                     const QStyleOptionGraphicsItem* aOption,
                                     QWidget* aWidget)
{
  GG_UTILITY_UNUSED(aOption);
  GG_UTILITY_UNUSED(aWidget);

  // adjust radius, if rect too small
  qreal vRadius = mRadius;
  if (vRadius > 2.0f * rect().width()) vRadius = rect().width() / 2.0f;
  if (vRadius > 2.0f * rect().height()) vRadius = rect().height() / 2.0f;
  qreal vDiameter = 2.0f * vRadius;

  // inner and rectangle dimensions
  const QRectF& vRect(rect());
  const QRectF& vRectR(vRect - QMarginsF(vRadius, vRadius, vRadius, vRadius));
  const QRectF& vRectD(vRect - QMarginsF(vDiameter, vDiameter, vDiameter, vDiameter));

  // no pen for inner parts
  aPainter->setPen(Qt::NoPen);

  // draw inner rect
  aPainter->setBrush(brush());
  aPainter->drawRect(vRectR);

  // draw outer bars
  aPainter->setBrush(GetLinearBrush(vRectR.left(), vRectR.top(), vRectR.left(), vRect.top()));
  aPainter->drawRect(QRectF(vRectR.left(), vRect.top(), vRectR.width(), vRadius));
  aPainter->setBrush(GetLinearBrush(vRectR.left(), vRectR.bottom(), vRectR.left(), vRect.bottom()));
  aPainter->drawRect(QRectF(vRectR.left(), vRectR.bottom(), vRectR.width(), vRadius));
  aPainter->setBrush(GetLinearBrush(vRectR.left(), vRectR.top(), vRect.left(), vRectR.top()));
  aPainter->drawRect(QRectF(vRect.left(), vRectR.top(), vRadius, vRectR.height()));
  aPainter->setBrush(GetLinearBrush(vRectR.right(), vRectR.top(), vRect.right(), vRectR.top()));
  aPainter->drawRect(QRectF(vRectR.right(), vRectR.top(), vRadius, vRectR.height()));

  // draw rounded corners
  aPainter->setBrush(GetRadialBrush(vRectR.topRight(), vRadius));
  aPainter->drawPie(QRectF(vRectD.right(), vRect.top(), vDiameter, vDiameter), 0, 16*90);
  aPainter->setBrush(GetRadialBrush(vRectR.topLeft(), vRadius));
  aPainter->drawPie(QRectF(vRect.left(), vRect.top(), vDiameter, vDiameter), 16*90, 16*90);
  aPainter->setBrush(GetRadialBrush(vRectR.bottomLeft(), vRadius));
  aPainter->drawPie(QRectF(vRect.left(), vRectD.bottom(), vDiameter, vDiameter), 16*180, 16*90);
  aPainter->setBrush(GetRadialBrush(vRectR.bottomRight(), vRadius));
  aPainter->drawPie(QRectF(vRectD.right(), vRectD.bottom(), vDiameter, vDiameter), 16*270, 16*90);

  // draw outline
  if (pen() != Qt::NoPen) {
    aPainter->setPen(pen());
    aPainter->setBrush(Qt::NoBrush);
    aPainter->drawRoundedRect(vRect, mRadius, mRadius);
  }
}


qreal ggGraphicsShadowRectItem::GetGradientMiddle() const
{
  if ((mRadius > 0.0f) && (mWidth > 0.0f) && (mRadius >= mWidth)) {
    return (mRadius - mWidth) / mRadius;
  }
  return 0.999f;
}


QBrush ggGraphicsShadowRectItem::GetLinearBrush(qreal aX1, qreal aY1, qreal aX2, qreal aY2) const
{
  QLinearGradient vGradient(aX1, aY1, aX2, aY2);
  vGradient.setColorAt(0.0f, brush().color());
  vGradient.setColorAt(GetGradientMiddle(), brush().color());
  vGradient.setColorAt(1.0f, mOuterColor);
  return vGradient;
}


QBrush ggGraphicsShadowRectItem::GetRadialBrush(const QPointF& aCenter, qreal aRadius) const
{
  QRadialGradient vGradient(aCenter, aRadius);
  vGradient.setColorAt(0.0f, brush().color());
  vGradient.setColorAt(GetGradientMiddle(), brush().color());
  vGradient.setColorAt(1.0f, mOuterColor);
  return vGradient;
}
