#ifndef GGGRAPHICSMANIPULATORHANDLEITEM_H
#define GGGRAPHICSMANIPULATORHANDLEITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>

#include "Base/ggSubject.h"

template <class TBaseItem>
class ggGraphicsHandleItemT : public TBaseItem
{

public:

  ggGraphicsHandleItemT(float aSize, QGraphicsItem* aParent = nullptr)
    : TBaseItem(-0.5f*aSize, -0.5f*aSize, aSize, aSize, aParent)
  {
    TBaseItem::setFlag(TBaseItem::ItemIsMovable);
    TBaseItem::setFlag(TBaseItem::ItemSendsGeometryChanges);
    TBaseItem::setFlag(TBaseItem::ItemIgnoresTransformations);
    TBaseItem::setAcceptHoverEvents(true);
    TBaseItem::setCursor(Qt::DragMoveCursor);
    TBaseItem::setToolTip("Click and drag for changing the size.");
    TBaseItem::setZValue(1.0f);
    TBaseItem::setPen(Qt::NoPen);
  }

  QVariant itemChange(typename TBaseItem::GraphicsItemChange aChange,
                      const QVariant& aValue) override {
    // notify position change
    if (aChange == TBaseItem::ItemPositionHasChanged) {
      foreach (QGraphicsItem* vItem, mLinkedItemsX) vItem->setX(aValue.toPointF().x());
      foreach (QGraphicsItem* vItem, mLinkedItemsY) vItem->setY(aValue.toPointF().y());
      mSubjectPosition.Notify();
    }
    // execute base item change
    return TBaseItem::itemChange(aChange, aValue);
  }

  const ggSubject* GetSubjectPosition() const {
    return &mSubjectPosition;
  }

  void LinkX(ggGraphicsHandleItemT* aItem) {
    mLinkedItemsX << aItem;
    aItem->mLinkedItemsX << this;
  }

  void LinkY(ggGraphicsHandleItemT* aItem) {
    mLinkedItemsY << aItem;
    aItem->mLinkedItemsY << this;
  }

private:

  ggSubject mSubjectPosition;

  QList<ggGraphicsHandleItemT*> mLinkedItemsX;
  QList<ggGraphicsHandleItemT*> mLinkedItemsY;

};

typedef ggGraphicsHandleItemT<QGraphicsEllipseItem> ggGraphicsHandleItemEllipse;
typedef ggGraphicsHandleItemT<QGraphicsRectItem> ggGraphicsHandleItemRect;

#endif // GGGRAPHICSMANIPULATORHANDLEITEM_H
