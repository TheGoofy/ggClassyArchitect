// 0) include own header
#include "ggClassyGraphicsFrameItem.h"

// 1) include system or QT
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QCursor>

// 2) include own project-related (sort by component dependency)
#include "BaseGraphics/ggGraphicsTextItem.h"
#include "BaseGraphics/ggGraphicsHandleItemT.h"
#include "BaseGraphics/ggGraphicsShadowRectItem.h"
#include "ClassyDataSet/ggClassyFrame.h"
#include "ClassyDataSet/ggClassyCollection.h"


ggClassyGraphicsFrameItem::ggClassyGraphicsFrameItem(ggClassyFrame* aFrame,
                                                     QGraphicsItem* aParent) :
  ggGraphicsRoundedRectItem(QRectF(), 8.0f, aParent),
  mShadow(nullptr),
  mText(nullptr),
  mHandleTL(nullptr),
  mHandleTR(nullptr),
  mHandleBL(nullptr),
  mHandleBR(nullptr),
  mFrame(nullptr)
{
  Construct();
  SetFrame(aFrame);
}


ggClassyGraphicsFrameItem::ggClassyGraphicsFrameItem(const QRectF& aRect,
                                                     const qreal aRadius,
                                                     ggClassyFrame* aFrame,
                                                     QGraphicsItem* aParent) :
  ggGraphicsRoundedRectItem(aRect, aRadius, aParent),
  mShadow(nullptr),
  mText(nullptr),
  mHandleTL(nullptr),
  mHandleTR(nullptr),
  mHandleBL(nullptr),
  mHandleBR(nullptr),
  mFrame(nullptr)
{
  Construct();
  SetFrame(aFrame);
}


void ggClassyGraphicsFrameItem::SetFrame(ggClassyFrame* aFrame)
{
  if (aFrame != mFrame) {

    // disconnect (old) subjects
    Detach(mFrame);
    Detach(GetCollection());

    // assign new frame
    mFrame = aFrame;

    // connect new subjects
    Attach(aFrame);
    Attach(GetCollection());

    // initialize the item
    UpdateFrameRead();
    UpdateCollectionRead();
    UpdateLayout();
  }
}


ggClassyFrame* ggClassyGraphicsFrameItem::GetFrame() const
{
  return mFrame;
}


void ggClassyGraphicsFrameItem::SetRadius(qreal aRadius)
{
  ggGraphicsRoundedRectItem::SetRadius(aRadius, aRadius);
  UpdateLayout();
}


void ggClassyGraphicsFrameItem::SetRadius(qreal aRadiusX, qreal aRadiusY)
{
  ggGraphicsRoundedRectItem::SetRadius(aRadiusX, aRadiusY);
  UpdateLayout();
}


QRectF ggClassyGraphicsFrameItem::GetRectAbsolute() const
{
  const QRectF& vRect(rect());
  return QRectF(pos() + vRect.topLeft(), vRect.size());
}


void ggClassyGraphicsFrameItem::SetRectAbsolute(const QRectF& aRect)
{
  setFlag(ItemSendsGeometryChanges, false);
  const QRectF vRect(QPointF(), aRect.size());
  setPos(aRect.topLeft());
  setRect(vRect);
  setFlag(ItemSendsGeometryChanges, true);
}


void ggClassyGraphicsFrameItem::setPen(const QPen& aPen)
{
  ggGraphicsRoundedRectItem::setPen(aPen);
  UpdateLayout();
}


void ggClassyGraphicsFrameItem::Update(const ggSubject* aSubject)
{
  if (aSubject == mText->GetSubjectText()) {
    UpdateLayout();
  }

  else if (aSubject == mText->GetSubjectEditingFinished()) {
    UpdateFrameWrite();
  }

  else if (aSubject == mHandleTL->GetSubjectPosition()) {
    QRectF vRect(mHandleTL->pos(), mHandleBR->pos());
    if (vRect.width() < 2.0f * GetRadiusX()) vRect.setLeft(vRect.right() - 2.0f * GetRadiusX());
    if (vRect.height() < 2.0f * GetRadiusY()) vRect.setTop(vRect.bottom() - 2.0f * GetRadiusY());
    setRect(vRect.normalized());
    UpdateLayout();
    UpdateFrameWrite();
  }

  else if (aSubject == mHandleBR->GetSubjectPosition()) {
    QRectF vRect(mHandleTL->pos(), mHandleBR->pos());
    if (vRect.width() < 2.0f * GetRadiusX()) vRect.setWidth(2.0f * GetRadiusX());
    if (vRect.height() < 2.0f * GetRadiusY()) vRect.setHeight(2.0f * GetRadiusY());
    setRect(vRect.normalized());
    UpdateLayout();
    UpdateFrameWrite();
  }

  else if (aSubject == GetCollection()) {
    UpdateCollectionRead();
    UpdateLayout();
  }

  else if (aSubject == mFrame) {
    UpdateFrameRead();
    UpdateCollectionRead();
    UpdateLayout();
  }
}


QVariant ggClassyGraphicsFrameItem::itemChange(GraphicsItemChange aChange, const QVariant& aValue)
{
  if (aChange == ItemPositionHasChanged) {
    UpdateFrameWrite();
  }

  if (aChange == ItemSelectedHasChanged) {
    mShadow->SetShadowColors(aValue.toBool() ? QColor(200, 255, 0, 255) : QColor(0, 0, 0, 40));
    mShadow->setPos(aValue.toBool() ? QPointF(0.0f, 0.0f) : QPointF(3.0f, 2.0f));
  }

  return ggGraphicsRoundedRectItem::itemChange(aChange, aValue);
}


void ggClassyGraphicsFrameItem::hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent)
{
  ShowHandles();
  ggGraphicsRoundedRectItem::hoverEnterEvent(aEvent);
}


void ggClassyGraphicsFrameItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent)
{
  HideHandles();
  ggGraphicsRoundedRectItem::hoverLeaveEvent(aEvent);
}


void ggClassyGraphicsFrameItem::mousePressEvent(QGraphicsSceneMouseEvent* aEvent)
{
  // make it editable, if user clicks twice to the same position
  QPointF vPos = aEvent->pos();
  if (vPos == mLastMousePressPos) mText->SetEditable(true);
  mLastMousePressPos = vPos;

  // do inherited event handling
  ggGraphicsRoundedRectItem::mousePressEvent(aEvent);
}


void ggClassyGraphicsFrameItem::Construct()
{
  // own properties
  setFlag(ItemIsSelectable);
  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  setAcceptHoverEvents(true);
  setCursor(Qt::SizeAllCursor);
  setZValue(-2.0f);
  setBrush(Qt::white);

  // shadow
  mShadow = new ggGraphicsShadowRectItem(this);
  mShadow->setFlag(ItemStacksBehindParent);
  mShadow->setPen(Qt::NoPen);
  mShadow->SetShadowColors(QColor(0, 0, 0, 40));
  mShadow->setPos(3.0f, 2.0f);

  // text
  mText = new ggGraphicsTextItem(this);
  mText->SetPen(Qt::NoPen);
  mText->SetText("Goofy was here!");
  mText->SetAlignment(Qt::AlignCenter | Qt::AlignVCenter);

  // size handles
  mHandleTL = new ggGraphicsHandleItemEllipse(9.0f, this);
  mHandleTR = new ggGraphicsHandleItemEllipse(9.0f, this);
  mHandleBL = new ggGraphicsHandleItemEllipse(9.0f, this);
  mHandleBR = new ggGraphicsHandleItemEllipse(9.0f, this);
  mHandleTL->LinkX(mHandleBL);
  mHandleTL->LinkY(mHandleTR);
  mHandleBR->LinkX(mHandleTR);
  mHandleBR->LinkY(mHandleBL);
  mHandleTL->setCursor(Qt::SizeFDiagCursor);
  mHandleTR->setCursor(Qt::SizeBDiagCursor);
  mHandleBL->setCursor(Qt::SizeBDiagCursor);
  mHandleBR->setCursor(Qt::SizeFDiagCursor);

  // attach subjects
  Attach(mText->GetSubjectText());
  Attach(mText->GetSubjectEditingFinished());
  Attach(mHandleTL->GetSubjectPosition());
  Attach(mHandleBR->GetSubjectPosition());

  // arrange layout
  UpdateCollectionRead();
  UpdateLayout();
}


Qt::Alignment ggClassyGraphicsFrameItem::GetVerticalTextAlignment() const
{
  if (mText->GetAlignment() & Qt::AlignTop) return Qt::AlignTop;
  if (mText->GetAlignment() & Qt::AlignVCenter) return Qt::AlignVCenter;
  if (mText->GetAlignment() & Qt::AlignBottom) return Qt::AlignBottom;
  return Qt::AlignTop;
}


void ggClassyGraphicsFrameItem::ShowHandles()
{
  SetHandleBrush(QColor(255, 0, 0, 255));
}


void ggClassyGraphicsFrameItem::HideHandles()
{
  SetHandleBrush(Qt::transparent);
}


void ggClassyGraphicsFrameItem::SetHandleBrush(const QBrush& aBrush)
{
  mHandleTL->setBrush(aBrush);
  mHandleTR->setBrush(aBrush);
  mHandleBL->setBrush(aBrush);
  mHandleBR->setBrush(aBrush);
}


void ggClassyGraphicsFrameItem::UpdateFrameRead()
{
  if (GetFrame() != nullptr) {
    SetRectAbsolute(GetFrame()->GetRect());
    mText->SetText(GetFrame()->GetDescription());
    mText->SetAlignment(GetFrame()->GetAlignment());
  }
}


void ggClassyGraphicsFrameItem::UpdateFrameWrite()
{
  if (GetFrame() != nullptr) {
    ggObserver::cExecutorBlocking vBlock(this, GetFrame());
    ggSubject::cExecutorLazy vLazy(GetFrame());
    GetFrame()->SetRect(GetRectAbsolute());
    GetFrame()->SetDescription(mText->GetText());
    GetFrame()->SetAlignment(mText->GetAlignment());
  }
}


void ggClassyGraphicsFrameItem::UpdateLayout()
{
  // adjust the shadow
  qreal vShadowWidth = 7.0f;
  qreal vFrameBorder2 = pen().widthF() / 2.0f;
  qreal vShadowMargin = vFrameBorder2 + vShadowWidth;
  const QRectF& vRect(rect());
  mShadow->SetShadowWidth(vShadowWidth);
  mShadow->SetRadius(GetRadiusX() + vFrameBorder2 + vShadowWidth);
  mShadow->setRect(vRect + QMarginsF(vShadowMargin, vShadowMargin, vShadowMargin, vShadowMargin));

  // adjust the text
  qreal vTextMargin = mText->document()->documentMargin();
  qreal vTextBorder = (GetRadiusX() > vTextMargin + 3.0f) ? GetRadiusX() - vTextMargin - 3.0f: 0.0f;
  qreal vTextWidth = vRect.width() - 2.0f * vTextBorder;
  mText->setTextWidth(vTextWidth);
  qreal vTextHeight = mText->boundingRect().height();
  mText->setX(vRect.left() + vTextBorder);
  switch (GetVerticalTextAlignment()) {
    case Qt::AlignTop: mText->setY(vRect.top() + vTextBorder); break;
    case Qt::AlignVCenter: mText->setY(vRect.top() + (vRect.height() - vTextHeight) / 2.0f); break;
    case Qt::AlignBottom: mText->setY(vRect.bottom() - vTextHeight - vTextBorder); break;
    default: mText->setY(vRect.top() + vTextBorder); break;
  }

  // adjust the handles position
  ggObserver::cExecutorBlocking vBlockTL(this, mHandleTL->GetSubjectPosition());
  ggObserver::cExecutorBlocking vBlockBR(this, mHandleBR->GetSubjectPosition());
  mHandleTL->setPos(vRect.topLeft());
  mHandleBR->setPos(vRect.bottomRight());
}


const ggClassyCollection* ggClassyGraphicsFrameItem::GetCollection() const
{
  const ggClassyCollection* vCollection = nullptr;
  if (mFrame != nullptr) vCollection = mFrame->GetCollection();
  if (vCollection == nullptr) vCollection = ggClassyCollection::GetDefaultCollection();
  return vCollection;
}


void ggClassyGraphicsFrameItem::UpdateCollectionRead()
{
  if (GetCollection() != nullptr) {
    // box border
    setPen(GetCollection()->mBoxBorder);
    // description
    mText->setFont(GetCollection()->mDescriptionFont);
    mText->setDefaultTextColor(GetCollection()->mDescriptionColor);
    setBrush(GetCollection()->mDescriptionBackground);
  }
}
