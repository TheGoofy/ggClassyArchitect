#ifndef GGGRAPHICSMANIPULATORHANDLEITEM_H
#define GGGRAPHICSMANIPULATORHANDLEITEM_H

// 1) include system or QT
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"

// 3) forward declarations

/**
 *
 */
template <class TBaseItem>
class ggGraphicsHandleItemT :
  public TBaseItem
{

public:

  ggGraphicsHandleItemT(QGraphicsItem* aParent = nullptr)
    : TBaseItem(aParent)
  {
    Construct();
  }

  ggGraphicsHandleItemT(float aSize, QGraphicsItem* aParent = nullptr)
    : TBaseItem(-0.5f*aSize, -0.5f*aSize, aSize, aSize, aParent)
  {
    Construct();
  }

  void SetSize(float aSize) {
    TBaseItem::setRect(-aSize/2.0f, -aSize/2.0f, aSize, aSize);
  }

  float GetSize() const {
    return TBaseItem::rect().width();
  }

  const ggSubject* GetSubjectPosition() const {
    return &mSubjectPosition;
  }

  const ggSubject* GetSubjectMousePress() const {
    return &mSubjectMousePress;
  }

  const ggSubject* GetSubjectMouseRelease() const {
    return &mSubjectMouseRelease;
  }

  void LinkX(ggGraphicsHandleItemT* aItem) {
    mLinkedItemsX << aItem;
    aItem->mLinkedItemsX << this;
  }

  void LinkY(ggGraphicsHandleItemT* aItem) {
    mLinkedItemsY << aItem;
    aItem->mLinkedItemsY << this;
  }

protected:

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

  virtual void mousePressEvent(QGraphicsSceneMouseEvent* aEvent) override {
    // handle is clicked (for dragging): if the scene selection is not cleared,
    // all the selected items will be moved. alternatively this HandleItem could have
    // its selectable-flag set, but that looks ugly.
    if (TBaseItem::scene() != nullptr) {
      TBaseItem::scene()->clearSelection();
    }
    // notify press event
    mSubjectMousePress.Notify();
    // do inherited event handling
    TBaseItem::mousePressEvent(aEvent);
  }

  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* aEvent) override {
    // notify release event
    mSubjectMouseRelease.Notify();
    // do inherited event handling
    TBaseItem::mouseReleaseEvent(aEvent);
  }

private:

  void Construct() {
    TBaseItem::setFlag(TBaseItem::ItemIsMovable);
    TBaseItem::setFlag(TBaseItem::ItemSendsGeometryChanges);
    TBaseItem::setFlag(TBaseItem::ItemIgnoresTransformations);
    TBaseItem::setAcceptHoverEvents(true);
    TBaseItem::setCursor(Qt::SizeAllCursor);
    TBaseItem::setToolTip("Click and drag for changing the size.");
    TBaseItem::setZValue(1.0f);
    TBaseItem::setPen(Qt::NoPen);
  }

  ggSubject mSubjectPosition;
  ggSubject mSubjectMousePress;
  ggSubject mSubjectMouseRelease;

  QList<ggGraphicsHandleItemT*> mLinkedItemsX;
  QList<ggGraphicsHandleItemT*> mLinkedItemsY;

};

typedef ggGraphicsHandleItemT<QGraphicsEllipseItem> ggGraphicsHandleItemEllipse;
typedef ggGraphicsHandleItemT<QGraphicsRectItem> ggGraphicsHandleItemRect;

#endif // GGGRAPHICSMANIPULATORHANDLEITEM_H
