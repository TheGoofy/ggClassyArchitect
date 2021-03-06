#ifndef GGCLASSYGRAPHICSFRAMEITEM_H
#define GGCLASSYGRAPHICSFRAMEITEM_H

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggObserver.h"
#include "BaseGraphics/ggGraphicsRoundedRectItem.h"

// 3) forward declarations
class ggGraphicsShadowRectItem;
class ggGraphicsTextItem;
class QGraphicsEllipseItem;
template <class T> class ggGraphicsHandleItemT;
class ggClassyFrame;
class ggClassyCollection;

/**
 * @brief The ggClassyGraphicsFrameItem class
 */
class ggClassyGraphicsFrameItem :
  public ggGraphicsRoundedRectItem,
  public ggObserver
{
public:

  ggClassyGraphicsFrameItem(ggClassyFrame* aFrame = nullptr,
                            QGraphicsItem* aParent = nullptr);

  ggClassyGraphicsFrameItem(const QRectF& aRect,
                            const qreal aRadius = 8.0f,
                            ggClassyFrame* aFrame = nullptr,
                            QGraphicsItem* aParent = nullptr);

  void SetFrame(ggClassyFrame* aFrame);
  ggClassyFrame* GetFrame() const;

  void SetRadius(qreal aRadius);
  void SetRadius(qreal aRadiusX, qreal aRadiusY);
  void setPen(const QPen& aPen);

protected:

  virtual void Update(const ggSubject* aSubject) override;

  virtual QVariant itemChange(GraphicsItemChange aChange, const QVariant& aValue) override;
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent) override;
  virtual void mousePressEvent(QGraphicsSceneMouseEvent* aEvent) override;

private:

  void Construct();

  void UpdateFrameRead();
  void UpdateFrameWrite();
  void UpdateCollectionRead();
  void UpdateLayout();
  void UpdateSettings();

  const ggClassyCollection* GetCollection() const;
  Qt::Alignment GetVerticalTextAlignment() const;
  void ShowHandles();
  void HideHandles();
  void SetHandleBrush(const QBrush& aBrush);
  QRectF GetRectAbsolute() const;
  void SetRectAbsolute(const QRectF& aRect);
  QPen GetBoxBorderPen() const;
  const QColor& GetShadowColor() const;
  const QPointF& GetShadowOffset() const;
  void UpdateShadow(qreal aShadowWidth);

  ggGraphicsShadowRectItem* mShadow;
  ggGraphicsTextItem* mText;

  ggGraphicsHandleItemT<QGraphicsEllipseItem>* mHandleTL;
  ggGraphicsHandleItemT<QGraphicsEllipseItem>* mHandleTR;
  ggGraphicsHandleItemT<QGraphicsEllipseItem>* mHandleBL;
  ggGraphicsHandleItemT<QGraphicsEllipseItem>* mHandleBR;

  QPointF mLastMousePressPos;
  ggClassyFrame* mFrame;

};

#endif // GGCLASSYGRAPHICSFRAMEITEM_H
