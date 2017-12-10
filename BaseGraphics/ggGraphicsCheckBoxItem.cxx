// 0) include own header
#include "ggGraphicsCheckBoxItem.h"

// 1) include system or QT
#include <QCursor>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

// 2) include own project-related (sort by component dependency)


ggGraphicsCheckBoxItem::ggGraphicsCheckBoxItem(QGraphicsItem* aParent) :
  QGraphicsRectItem(aParent),
  mChecked(true, false),
  mColor(Qt::red)
{
  setAcceptHoverEvents(true);
  setCursor(Qt::PointingHandCursor);
  setToolTip("Click to show or hide.");
  setZValue(1.0f);
  setPen(Qt::NoPen);

  // create the lines
  mCrossLineA = new QGraphicsLineItem(this);
  mCrossLineB = new QGraphicsLineItem(this);
  SetCrossLineWidth(1.5f);
  SetHighlightOff();
  UpdateCross();
  UpdateColor();
}


void ggGraphicsCheckBoxItem::SetChecked(bool aChecked)
{
  mChecked = aChecked;
  UpdateColor();
}


bool ggGraphicsCheckBoxItem::GetChecked() const
{
  return *mChecked;
}


const ggSubjectBool* ggGraphicsCheckBoxItem::GetSubjectChecked() const
{
  return &mChecked;
}


void ggGraphicsCheckBoxItem::SetSize(float aSize)
{
  setRect(QRectF(0.0f, 0.0f, aSize, aSize));
  UpdateCross();
}


void ggGraphicsCheckBoxItem::SetCrossLineWidth(float aWidth)
{
  QPen vPen(mCrossLineA->pen());
  vPen.setWidthF(aWidth);
  vPen.setCapStyle(Qt::RoundCap);
  mCrossLineA->setPen(vPen);
  mCrossLineB->setPen(vPen);
}


void ggGraphicsCheckBoxItem::SetColor(const QColor& aColor)
{
  mColor = aColor;
  UpdateColor();
}


void ggGraphicsCheckBoxItem::SetHighlightOn()
{
  setVisible(true);
}


void ggGraphicsCheckBoxItem::SetHighlightOff()
{
  setVisible(false);
}


void ggGraphicsCheckBoxItem::mousePressEvent(QGraphicsSceneMouseEvent* aEvent)
{
  // toggle checked
  SetChecked(!GetChecked());
  mChecked.Notify();
  aEvent->accept();
}


void ggGraphicsCheckBoxItem::UpdateCross()
{
  const QRectF& vRect = rect();
  mCrossLineA->setLine(QLineF(vRect.topLeft(), vRect.bottomRight()));
  mCrossLineB->setLine(QLineF(vRect.topRight(), vRect.bottomLeft()));
}


void ggGraphicsCheckBoxItem::UpdateColor()
{
  setBrush(GetChecked() ? Qt::transparent : mColor);
  QPen vPen(mCrossLineA->pen());
  vPen.setColor(GetChecked() ? mColor : Qt::transparent);
  mCrossLineA->setPen(vPen);
  mCrossLineB->setPen(vPen);
}
