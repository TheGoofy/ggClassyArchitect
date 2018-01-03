// 0) include own header
#include "ggClassyConnectionManipulatorItem.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "BaseGraphics/ggGraphicsDecoratedPathItem.h"
#include "ClassyDataSet/ggClassyClass.h"
#include "ClassyGraphics/ggClassyGraphicsBoxItem.h"


ggClassyConnectionManipulatorItem::ggClassyConnectionManipulatorItem(float aSize,
                                                                     ggClassyGraphicsBoxItem* aParent) :
  QGraphicsItem(aParent)
{
  mDraggerItem = new ggGraphicsHandleItemEllipse(aSize, this);
  mDraggerItem->setCursor(Qt::DragMoveCursor);
  mDraggerItem->setToolTip("Click and drag for connecting a base-class");
  HideDragger();

  mPathItem = new ggGraphicsDecoratedPathItem(this);
  mPathItem->setPen(QPen(Qt::red, 1.5f));
  mPathItem->SetDecorationSrc(ggDecoration::cType::eLine, 2.0f);
  mPathItem->SetDecorationDst(ggDecoration::cType::eTriangle, 13.0f);
  UpdatePath();

  Attach(mDraggerItem->GetSubjectPosition());
  Attach(mDraggerItem->GetSubjectMouseRelease());
}


void ggClassyConnectionManipulatorItem::SetSize(float aSize)
{
  mDraggerItem->SetSize(aSize);
}


float ggClassyConnectionManipulatorItem::GetSize() const
{
  return mDraggerItem->GetSize();
}


void ggClassyConnectionManipulatorItem::SetPositionSrc(const QPointF& aPosition)
{
  mPositionSrc = aPosition;
  mDraggerItem->setPos(aPosition);
  UpdatePath();
}


void ggClassyConnectionManipulatorItem::ShowDragger()
{
  mDraggerItem->setBrush(QColor(255, 0, 0, 255));
}


void ggClassyConnectionManipulatorItem::HideDragger()
{
  mDraggerItem->setBrush(Qt::transparent);
}


void ggClassyConnectionManipulatorItem::Update(const ggSubject* aSubject)
{
  if (aSubject == mDraggerItem->GetSubjectPosition()) {
    UpdatePath();
  }
  else if (aSubject == mDraggerItem->GetSubjectMouseRelease()) {
    // connect base class
    ggClassyGraphicsBoxItem* vDstBoxItem = GetDstBoxItem();
    if (vDstBoxItem != nullptr) {
      GetSrcBoxItem()->GetClass()->AddBaseClassName(vDstBoxItem->GetClass()->GetName());
    }
    // reset manipulator
    mDraggerItem->setPos(mPositionSrc);
    UpdatePath();
  }
}


QRectF ggClassyConnectionManipulatorItem::boundingRect() const
{
  return mPathItem->boundingRect();
}


void ggClassyConnectionManipulatorItem::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
{
}


ggClassyGraphicsBoxItem* ggClassyConnectionManipulatorItem::GetSrcBoxItem()
{
  return dynamic_cast<ggClassyGraphicsBoxItem*>(parentItem());
}


ggClassyGraphicsBoxItem* ggClassyConnectionManipulatorItem::GetDstBoxItem()
{
  ggClassyGraphicsBoxItem* vDstBoxItem = nullptr;
  ggClassyGraphicsBoxItem* vSrcBoxItem = GetSrcBoxItem();
  if (scene() != nullptr) {
    QList<QGraphicsItem*> vItems = scene()->collidingItems(mDraggerItem);
    auto vItemsIterator = vItems.begin();
    while (vItemsIterator != vItems.end()) {
      ggClassyGraphicsBoxItem* vBoxItem = dynamic_cast<ggClassyGraphicsBoxItem*>(*vItemsIterator);
      if (vBoxItem != nullptr &&
          vBoxItem->GetClass()->GetName() != vSrcBoxItem->GetClass()->GetName()) {
        vDstBoxItem = vBoxItem;
        break;
      }
      vItemsIterator++;
    }
  }
  return vDstBoxItem;
}


ggConnectionPoint ggClassyConnectionManipulatorItem::GetConnectionSrc()
{
  return *GetSrcBoxItem()->GetClassConnectionTop();
}


ggConnectionPoint ggClassyConnectionManipulatorItem::GetConnectionDst()
{
  ggClassyGraphicsBoxItem* vDstBoxItem = GetDstBoxItem();
  if (vDstBoxItem != nullptr) {
    return *vDstBoxItem->GetClassConnectionBottom();
  }
  else {
    ggConnectionPoint vPointDst;
    vPointDst.SetPosition(mapToScene(mDraggerItem->pos()));
    vPointDst.SetDirection(QVector2D(mPositionSrc - mDraggerItem->pos()));
    return vPointDst;
  }
}


void ggClassyConnectionManipulatorItem::UpdatePath()
{
  if (mDraggerItem->pos() != mPositionSrc) {
    ggConnectionPoint vSrc = GetConnectionSrc();
    ggConnectionPoint vDst = GetConnectionDst();
    vSrc.SetPosition(mapFromScene(vSrc.GetPosition()));
    vDst.SetPosition(mapFromScene(vDst.GetPosition()));
    mPathItem->SetConnection(vSrc, vDst);
    mPathItem->show();
  }
  else {
    mPathItem->ClearConnection();
    mPathItem->hide();
  }
}
