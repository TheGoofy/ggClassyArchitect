#ifndef GGGRAPHICSMANIPULATORBART_H
#define GGGRAPHICSMANIPULATORBART_H

#include <QGraphicsRectItem>
#include <QBrush>

#include "ggGraphicsHandleItemT.h"
#include "ggObserver.h"

template <typename TBaseItem = QGraphicsRectItem,
          typename THandleItem = ggGraphicsHandleItemEllipse>
class ggGraphicsManipulatorBarItemT :
  public TBaseItem,
  public ggObserver
{

public:

  ggGraphicsManipulatorBarItemT(const QRectF& aRect,
                                const float& aHandleSize = 9.0f,
                                QGraphicsItem* aParent = nullptr) :
    TBaseItem(aRect, aParent)
  {
    TBaseItem::setFlag(TBaseItem::ItemIsMovable);
    TBaseItem::setCursor(Qt::DragMoveCursor);
    TBaseItem::setAcceptHoverEvents(true);
    TBaseItem::setFlag(TBaseItem::ItemSendsGeometryChanges);

    mHandleItemTL = new THandleItem(aHandleSize, this);
    mHandleItemTR = new THandleItem(aHandleSize, this);

    mHandleItemTL->setCursor(Qt::SizeFDiagCursor);
    mHandleItemTR->setCursor(Qt::SizeBDiagCursor);

    mHandleItemTL->LinkY(mHandleItemTR);

    mHandleItemTL->setPos(TBaseItem::rect().topLeft());
    mHandleItemTR->setPos(TBaseItem::rect().topRight());

    Attach(mHandleItemTL->GetSubjectPosition());
    Attach(mHandleItemTR->GetSubjectPosition());

    HideHandles();
  }

  virtual ~ggGraphicsManipulatorBarItemT() {
  }

  const ggSubject* GetSubjectPosition() const {
    return &mSubjectPosition;
  }

  QPointF GetPosition() const {
    return TBaseItem::pos() + TBaseItem::rect().topLeft();
  }

  void SetPosition(const QPointF& aPosition) {
    ggSubject::ExecutorBlocking vBlocker(&mSubjectPosition);
    TBaseItem::setPos(aPosition);
    QRectF vRect(TBaseItem::rect());
    vRect.moveTo(0.0f, 0.0f);
    TBaseItem::setRect(vRect);
  }

  const ggSubject* GetSubjectWidth() const {
    return &mSubjectWidth;
  }

  void SetWidth(float aWidth) {
    ggSubject::ExecutorBlocking vBlocker(&mSubjectWidth);
    mHandleItemTR->setPos(QPointF(mHandleItemTL->pos()) + QPointF(aWidth, 0.0f));
  }

  float GetWidth() const {
    return (mHandleItemTR->pos().x() - mHandleItemTL->pos().x());
  }

protected:

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent) override {
    ShowHandles();
    TBaseItem::hoverEnterEvent(aEvent);
  }

  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent) override {
    HideHandles();
    TBaseItem::hoverLeaveEvent(aEvent);
  }

  virtual QVariant itemChange(typename TBaseItem::GraphicsItemChange aChange,
                              const QVariant& aValue) override {
    // notify position change
    if (aChange == TBaseItem::ItemPositionHasChanged) {
      mSubjectPosition.Notify();
    }
    // execute base item change
    return TBaseItem::itemChange(aChange, aValue);
  }

  virtual void Update(const ggSubject* aSubject) override {
    if (aSubject == mHandleItemTL->GetSubjectPosition() ||
        aSubject == mHandleItemTR->GetSubjectPosition()) {
      UpdateRect();
      mSubjectWidth.Notify();
    }
  }

private:

  void ShowHandles() {
    SetHandleBrush(QColor(255, 0, 0, 255));
  }

  void HideHandles() {
    SetHandleBrush(Qt::transparent);
  }

  void SetHandleBrush(const QBrush& aBrush) {
    mHandleItemTL->setBrush(aBrush);
    mHandleItemTR->setBrush(aBrush);
  }

  void UpdateRect() {
    QRectF vRect(TBaseItem::rect());
    float vWidth = mHandleItemTR->pos().x() - mHandleItemTL->pos().x();
    float vHeight = vRect.height();
    vRect.setTopLeft(mHandleItemTL->pos());
    vRect.setSize(QSizeF(vWidth, vHeight));
    TBaseItem::setRect(vRect.normalized());
  }

  THandleItem* mHandleItemTL;
  THandleItem* mHandleItemTR;

  ggSubject mSubjectPosition;
  ggSubject mSubjectWidth;

};

#endif // GGGRAPHICSMANIPULATORBART_H
