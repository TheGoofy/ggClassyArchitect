#ifndef GGGRAPHICSRECTSHADOWITEM_H
#define GGGRAPHICSRECTSHADOWITEM_H

// 1) include system or QT
#include <QGraphicsRectItem>

// 2) include own project-related (sort by component dependency)

// 3) forward declarations

class ggGraphicsShadowRectItem :
  public QGraphicsRectItem
{
public:

  ggGraphicsShadowRectItem(QGraphicsItem* aParent = nullptr);

  ggGraphicsShadowRectItem(const QRectF& aInnerRect, qreal aRadius,
                           QGraphicsItem* aParent = nullptr);

  float GetRadius() const;
  void SetRadius(float aRadius);
  float GetShadowWidth() const;
  void SetShadowWidth(float aWidth);
  QRectF GetInnerRect() const;
  void SetInnerRect(const QRectF& aRect);
  const QColor& GetOuterColor() const;
  void SetOuterColor(const QColor& aColor);
  void SetShadowColors(const QColor& aColor);

protected:

  virtual void paint(QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget) override;

private:

  qreal GetGradientMiddle() const;
  QBrush GetLinearBrush(qreal aX1, qreal aY1, qreal aX2, qreal aY2) const;
  QBrush GetRadialBrush(const QPointF& aCenter, qreal aRadius) const;

  qreal mRadius;
  qreal mWidth;
  QColor mOuterColor;

};

#endif // GGGRAPHICSRECTSHADOWITEM_H
