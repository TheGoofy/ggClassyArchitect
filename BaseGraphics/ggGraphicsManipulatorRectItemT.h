#ifndef GGGRAPHICSMANIPULATORITEM_H
#define GGGRAPHICSMANIPULATORITEM_H

#include <QGraphicsRectItem>

#include "BaseGraphics/ggGraphicsHandleItemT.h"
#include "Base/ggObserver.h"

template <typename TBaseItem = QGraphicsRectItem,
          typename THandleItem = ggGraphicsHandleItemEllipse>
class ggGraphicsManipulatorRectItemT :
  public TBaseItem,
  public ggObserver
{

public:

  ggGraphicsManipulatorRectItemT(const QRectF& aRect,
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
    mHandleItemBR = new THandleItem(aHandleSize, this);
    mHandleItemBL = new THandleItem(aHandleSize, this);

    mHandleItemTL->setCursor(Qt::SizeFDiagCursor);
    mHandleItemTR->setCursor(Qt::SizeBDiagCursor);
    mHandleItemBR->setCursor(Qt::SizeFDiagCursor);
    mHandleItemBL->setCursor(Qt::SizeBDiagCursor);

    mHandleItemTL->LinkY(mHandleItemTR);
    mHandleItemTR->LinkX(mHandleItemBR);
    mHandleItemBR->LinkY(mHandleItemBL);
    mHandleItemBL->LinkX(mHandleItemTL);

    mHandleItemTL->setPos(TBaseItem::rect().topLeft());
    mHandleItemBR->setPos(TBaseItem::rect().bottomRight());

    Attach(mHandleItemTL->GetSubjectPosition());
    Attach(mHandleItemBR->GetSubjectPosition());

    HideHandles();
  }

  virtual ~ggGraphicsManipulatorRectItemT() {
  }

  const ggSubject* GetSubjectPosition() const {
    return &mSubjectPosition;
  }

  const ggSubject* GetSubjectSize() const {
    return &mSubjectSize;
  }

protected:

  void hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent) override {
    ShowHandles();
    TBaseItem::hoverEnterEvent(aEvent);
  }

  void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent) override {
    HideHandles();
    TBaseItem::hoverLeaveEvent(aEvent);
  }

  QVariant itemChange(typename TBaseItem::GraphicsItemChange aChange,
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
        aSubject == mHandleItemBR->GetSubjectPosition()) {
      UpdateRect();
      mSubjectSize.Notify();
    }
  }

private:

  void ShowHandles() {
    SetHandleBrush(QColor(255, 0, 0, 255));
  }

  void HideHandles() {
    SetHandleBrush(QColor(255, 255, 255, 0));
  }

  void SetHandleBrush(const QBrush& aBrush) {
    mHandleItemTL->setBrush(aBrush);
    mHandleItemTR->setBrush(aBrush);
    mHandleItemBR->setBrush(aBrush);
    mHandleItemBL->setBrush(aBrush);
  }

  void UpdateRect() {
    TBaseItem::setRect(QRectF(mHandleItemTL->pos(), mHandleItemBR->pos()).normalized());
  }

  THandleItem* mHandleItemTL;
  THandleItem* mHandleItemTR;
  THandleItem* mHandleItemBR;
  THandleItem* mHandleItemBL;

  ggSubject mSubjectPosition;
  ggSubject mSubjectSize;

};

#endif // GGGRAPHICSMANIPULATORITEM_H
