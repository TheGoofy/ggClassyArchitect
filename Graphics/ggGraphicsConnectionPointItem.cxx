#include "Graphics/ggGraphicsConnectionPointItem.h"

#include <QCursor>
#include <QPen>
#include <QGraphicsSceneWheelEvent>


ggGraphicsConnectionPointItem::ggGraphicsConnectionPointItem(float aSize,
                                                             QGraphicsItem* aParent) :
  QGraphicsEllipseItem(QRect(-aSize/2.0f, -aSize/2.0f, aSize, aSize), aParent),
  mDirectionLine(nullptr)
{
  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  setFlag(ItemIgnoresTransformations);
  setAcceptHoverEvents(true);
  setCursor(Qt::DragMoveCursor);
  setToolTip("Click and drag.");
  setZValue(1.0f);
  setPen(Qt::NoPen);
  setBrush(Qt::red);

  QPen vPen(Qt::DotLine);
  vPen.setWidthF(0.8f);
  vPen.setCapStyle(Qt::RoundCap);
  mDirectionLine = new QGraphicsLineItem(this);
  mDirectionLine->setPen(vPen);

  Attach(&mConnectionPoint);

  UpdateConnectionPoint();
}


void ggGraphicsConnectionPointItem::SetSize(float aSize)
{
  setRect(-aSize/2.0f, -aSize/2.0f, aSize, aSize);
}


float ggGraphicsConnectionPointItem::GetSize() const
{
  return rect().width();
}


void ggGraphicsConnectionPointItem::SetPointPosition(const QPointF& aPosition)
{
  mConnectionPoint.SetPosition(aPosition);
  mConnectionPoint.Notify();
}


void ggGraphicsConnectionPointItem::SetPointDirection(float aAngle)
{
  mConnectionPoint.SetDirectionAngle(aAngle);
  mConnectionPoint.Notify();
}


void ggGraphicsConnectionPointItem::SetPointDirection(const QVector2D& aDirection)
{
  mConnectionPoint.SetDirection(aDirection);
  mConnectionPoint.Notify();
}


void ggGraphicsConnectionPointItem::SetPointDirectionRight()
{
  mConnectionPoint.SetDirectionRight();
  mConnectionPoint.Notify();
}


void ggGraphicsConnectionPointItem::SetPointDirectionLeft()
{
  mConnectionPoint.SetDirectionLeft();
  mConnectionPoint.Notify();
}


void ggGraphicsConnectionPointItem::SetPointDirectionUp()
{
  mConnectionPoint.SetDirectionUp();
  mConnectionPoint.Notify();
}


void ggGraphicsConnectionPointItem::SetPointDirectionDown()
{
  mConnectionPoint.SetDirectionDown();
  mConnectionPoint.Notify();
}


const ggSubjectConnectionPoint* ggGraphicsConnectionPointItem::GetSubjectConnectionPoint() const
{
  return &mConnectionPoint;
}


void ggGraphicsConnectionPointItem::Update(const ggSubject* aSubject)
{
  if (aSubject == &mConnectionPoint) {
    UpdateConnectionPoint();
  }
}


QVariant ggGraphicsConnectionPointItem::itemChange(GraphicsItemChange aChange,
                                                   const QVariant& aValue)
{
  // notify position change
  if (aChange == ItemPositionHasChanged) {
    mConnectionPoint.SetPosition(aValue.toPointF());
    mConnectionPoint.Notify(this);
  }
  // execute base item change
  return QGraphicsEllipseItem::itemChange(aChange, aValue);
}


void ggGraphicsConnectionPointItem::wheelEvent(QGraphicsSceneWheelEvent* aEvent)
{
  float vAngle = mConnectionPoint.GetDirectionAngle();
  vAngle -= 0.001f * aEvent->delta();
  mConnectionPoint.SetDirectionAngle(vAngle);
  mConnectionPoint.Notify();
}


void ggGraphicsConnectionPointItem::UpdateConnectionPoint()
{
  // adjust position
  setPos(mConnectionPoint.GetPosition());

  // adjust direction indicator line
  QPointF vPointA(0.0f, 0.0f);
  QPointF vPointB = (5.0f * GetSize() * mConnectionPoint.GetDirection()).toPointF();
  mDirectionLine->setLine(QLineF(vPointA, vPointB));
}
