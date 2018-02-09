// 0) include own header
#include "ggClassyGraphicsBoxItem.h"

// 1) include system or QT
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>
#include <QMessageBox>
#include <QDebug>

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"
#include "BaseWidgets/ggUtilityQt.h"
#include "BaseGraphics/ggGraphicsTextItem.h"
#include "BaseGraphics/ggGraphicsCheckBoxItem.h"
#include "BaseGraphics/ggGraphicsShadowRectItem.h"
#include "BaseGraphics/ggGraphicsDecoratedPathItem.h"
#include "ClassyDataSet/ggClassySettings.h"
#include "ClassyDataSet/ggClassyDataSet.h"
#include "ClassyGraphics/ggClassyConnectionManipulatorItem.h"


ggClassyGraphicsBoxItem::ggClassyGraphicsBoxItem(const QRectF& aRect) :
  ggGraphicsManipulatorBarItemT<>(QRect()),
  mClassNameText(nullptr),
  mMembersText(nullptr),
  mDescriptionText(nullptr),
  mMembersCheckBox(nullptr),
  mDescriptionCheckBox(nullptr),
  mClass(nullptr),
  mClassBox(nullptr)
{
  Construct();

  // setup geometry
  SetPosition(aRect.topLeft());
  SetWidth(aRect.width());

  // some default text
  mClassNameText->SetText("TheClassyClass");
  mMembersText->SetText("GetName()\n"
                        "YouWannaDance()\n"
                        "DamnImLookingGood()");
  mDescriptionText->SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
                            "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");

  UpdateLayout();
  UpdateConnectionPoints();
}


ggClassyGraphicsBoxItem::ggClassyGraphicsBoxItem(ggClassyClass* aClass,
                                                 ggClassyClassBox* aClassBox) :
  ggGraphicsManipulatorBarItemT<>(QRect()),
  mClassNameText(nullptr),
  mMembersText(nullptr),
  mDescriptionText(nullptr),
  mMembersCheckBox(nullptr),
  mDescriptionCheckBox(nullptr),
  mClass(nullptr),
  mClassBox(nullptr)
{
  Construct();
  SetClassBox(aClass, aClassBox);
}


void ggClassyGraphicsBoxItem::Construct()
{
  setFlag(ItemIsSelectable);
  setBrush(Qt::NoBrush);
  setPen(QPen(Qt::black, 0.0f, Qt::NoPen));

  mShadow = new ggGraphicsShadowRectItem(this);
  mShadow->setFlag(ItemStacksBehindParent);
  mShadow->setPen(Qt::NoPen);
  mBoxBorder = new QGraphicsRectItem(this);
  mBoxBorder->setFlag(ItemStacksBehindParent);
  mBoxBorder->setBrush(Qt::NoBrush);

  mClassNameText = new ggGraphicsTextItem(this);
  mClassNameText->SetSuppressLineBreaks(true);
  mClassNameText->SetEnterKeyFinishesEdit(true);
  mClassNameText->setDefaultTextColor(Qt::white);
  mClassNameText->SetBrush(QBrush(Qt::black, Qt::SolidPattern));
  mClassNameText->SetPen(Qt::NoPen);

  mMembersText = new ggGraphicsTextItem(this);
  mMembersText->SetSuppressLineBreaks(false);
  mMembersText->SetEnterKeyFinishesEdit(true);
  mMembersText->SetBrush(QBrush(Qt::white, Qt::SolidPattern));
  mMembersText->SetPen(Qt::NoPen);

  mDescriptionText = new ggGraphicsTextItem(this);
  mDescriptionText->SetSuppressLineBreaks(false);
  mDescriptionText->SetEnterKeyFinishesEdit(false);
  mDescriptionText->SetBrush(QBrush(QColor(230, 230, 230, 255), Qt::SolidPattern));
  mDescriptionText->SetPen(Qt::NoPen);

  mMembersCheckBox = new ggGraphicsCheckBoxItem(this);
  mDescriptionCheckBox = new ggGraphicsCheckBoxItem(this);

  mBaseClassConnector = new ggClassyConnectionManipulatorItem(this);

  mClassConnectionTop.SetDirectionUp();
  mClassConnectionBottom.SetDirectionDown();
  mClassConnectionLeft.SetDirectionLeft();
  mClassConnectionRight.SetDirectionRight();

  Attach(ggClassySettings::GetInstance());
  Attach(mClassNameText->GetSubjectText());
  Attach(mClassNameText->GetSubjectEditingFinished());
  Attach(mMembersText->GetSubjectText());
  Attach(mMembersText->GetSubjectEditingFinished());
  Attach(mDescriptionText->GetSubjectText());
  Attach(mDescriptionText->GetSubjectEditingFinished());
  Attach(mMembersCheckBox->GetSubjectChecked());
  Attach(mDescriptionCheckBox->GetSubjectChecked());
  Attach(GetSubjectPosition());
  Attach(GetSubjectWidth());

  UpdateSettings();
}


void ggClassyGraphicsBoxItem::SetClassBox(ggClassyClass* aClass,
                                          ggClassyClassBox* aClassBox)
{
  // if class or class-box changed...
  if (mClass != aClass ||
      mClassBox != aClassBox) {
    // detach previously attached subjects
    Detach(mClassBox);
    Detach(mClass);
    // remember the new class and class-box
    mClass = aClass;
    mClassBox = aClassBox;
    // re-attach them
    Attach(mClassBox);
    Attach(mClass);
    // read the class information and update layout
    UpdateClassRead();
    UpdateClassBoxRead();
    UpdateLayout();
    UpdateConnectionPoints();
  }
}


ggClassyClass* ggClassyGraphicsBoxItem::GetClass() const
{
  return mClass;
}


ggClassyClassBox* ggClassyGraphicsBoxItem::GetClassBox() const
{
  return mClassBox;
}


const ggSubject* ggClassyGraphicsBoxItem::GetSubjectSize() const
{
  return &mSubjectSize;
}


QPointF ggClassyGraphicsBoxItem::GetClassPositionTop() const
{
  return GetPosition() + QPointF(rect().width() / 2.0f, -pen().widthF() / 2.0f);
}


QPointF ggClassyGraphicsBoxItem::GetClassPositionBottom() const
{
  return GetPosition() + QPointF(rect().width() / 2.0f, rect().height() + pen().widthF() / 2.0f);
}


QPointF ggClassyGraphicsBoxItem::GetClassPositionLeft() const
{
  // offset from upper left corner
  QPointF vPosition = GetPosition();
  vPosition.rx() -= pen().widthF() / 2.0f;
  vPosition.ry() += mClassNameText->boundingRect().height() / 2.0f;
  return vPosition;
}


QPointF ggClassyGraphicsBoxItem::GetClassPositionRight() const
{
  // same as left, just displaced by the box width
  QPointF vPosition = GetClassPositionLeft();
  vPosition.rx() += GetWidth() + pen().widthF();
  return vPosition;
}


QPointF ggClassyGraphicsBoxItem::GetMemberPositionLeft(ggUSize aMemberIndex) const
{
  // calculate the height of a single member  
  ggUSize vNumberOfMembers = 1;
  if (GetClass() != nullptr) vNumberOfMembers = GetClass()->GetMembers().size();
  if (vNumberOfMembers < 1) vNumberOfMembers = 1;
  float vIndentY = 3.0f;
  float vMemberHeight = (mMembersText->boundingRect().height() - 2.0f* vIndentY) / vNumberOfMembers;

  // offset from upper left corner
  QPointF vPosition = GetPosition();
  vPosition.rx() -= pen().widthF() / 2.0f;
  vPosition.ry() += mClassNameText->boundingRect().height() + vIndentY;
  vPosition.ry() += aMemberIndex * vMemberHeight + vMemberHeight / 2.0f;

  // done...
  return vPosition;
}


QPointF ggClassyGraphicsBoxItem::GetMemberPositionRight(ggUSize aMemberIndex) const
{
  // same as left, just displaced by the box width
  QPointF vPosition = GetMemberPositionLeft(aMemberIndex);
  vPosition.rx() += GetWidth() + pen().widthF();
  return vPosition;
}


const ggSubjectConnectionPoint* ggClassyGraphicsBoxItem::GetClassConnectionTop() const
{
  return &mClassConnectionTop;
}


const ggSubjectConnectionPoint* ggClassyGraphicsBoxItem::GetClassConnectionBottom() const
{
  return &mClassConnectionBottom;
}


const ggSubjectConnectionPoint* ggClassyGraphicsBoxItem::GetClassConnectionLeft() const
{
  return &mClassConnectionLeft;
}


const ggSubjectConnectionPoint* ggClassyGraphicsBoxItem::GetClassConnectionRight() const
{
  return &mClassConnectionRight;
}


const ggSubjectConnectionPoint* ggClassyGraphicsBoxItem::GetMemberConnectionLeft(ggUSize aMemberIndex) const
{
  if (aMemberIndex < mMembersConnectionLeft.size()) {
    return &mMembersConnectionLeft[aMemberIndex];
  }
  return nullptr;
}


const ggSubjectConnectionPoint* ggClassyGraphicsBoxItem::GetMemberConnectionRight(ggUSize aMemberIndex) const
{
  if (aMemberIndex < mMembersConnectionRight.size()) {
    return &mMembersConnectionRight[aMemberIndex];
  }
  return nullptr;
}


void ggClassyGraphicsBoxItem::hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent)
{
  mMembersCheckBox->SetHighlightOn();
  mDescriptionCheckBox->SetHighlightOn();
  mBaseClassConnector->ShowDragger();
  ggGraphicsManipulatorBarItemT<>::hoverEnterEvent(aEvent);
}


void ggClassyGraphicsBoxItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent)
{
  mMembersCheckBox->SetHighlightOff();
  mDescriptionCheckBox->SetHighlightOff();
  mBaseClassConnector->HideDragger();
  ggGraphicsManipulatorBarItemT<>::hoverLeaveEvent(aEvent);
}


void ggClassyGraphicsBoxItem::paint(QPainter* aPainter, const QStyleOptionGraphicsItem* aOption, QWidget* aWidget)
{
  // don't draw the (ugly) default selection box
  QStyle::State vState = aOption->state;
  const_cast<QStyleOptionGraphicsItem*>(aOption)->state = aOption->state & !QStyle::State_Selected;
  ggGraphicsManipulatorBarItemT<>::paint(aPainter, aOption, aWidget);
  const_cast<QStyleOptionGraphicsItem*>(aOption)->state = vState;
}


QVariant ggClassyGraphicsBoxItem::itemChange(GraphicsItemChange aChange, const QVariant& aValue)
{
  if (aChange == ItemSelectedHasChanged) {
    mBoxBorder->setPen(GetBoxBorderPen());
    mShadow->SetShadowColors(GetShadowColor());
    mShadow->setPos(GetShadowOffset());
  }

  return ggGraphicsManipulatorBarItemT<>::itemChange(aChange, aValue);
}


void ggClassyGraphicsBoxItem::Update(const ggSubject* aSubject)
{
  if (aSubject == GetSubjectPosition()) {
    UpdateClassBoxWrite();
    UpdateConnectionPoints();
    NotifyConnectionPoints();
  }

  else if (aSubject == GetSubjectWidth()) {
    UpdateLayout();
    NotifySize();
    UpdateClassBoxWrite();
    UpdateConnectionPoints();
    NotifyConnectionPoints();
  }

  else if (aSubject == mClassNameText->GetSubjectText() ||
           aSubject == mMembersText->GetSubjectText() ||
           aSubject == mDescriptionText->GetSubjectText()) {
    UpdateLayout();
    NotifySize();
    UpdateClassBoxWrite();
    UpdateConnectionPoints();
    NotifyConnectionPoints();
  }

  else if (aSubject == mClassNameText->GetSubjectEditingFinished() ||
           aSubject == mMembersText->GetSubjectEditingFinished() ||
           aSubject == mDescriptionText->GetSubjectEditingFinished()) {
    UpdateClassWrite();
  }

  else if (aSubject == mMembersCheckBox->GetSubjectChecked() ||
           aSubject == mDescriptionCheckBox->GetSubjectChecked()) {
    UpdateLayout();
    NotifySize();
    UpdateClassBoxWrite();
    UpdateConnectionPoints();
    NotifyConnectionPoints();
  }

  else if (aSubject == GetClass()) {
    UpdateClassRead();
    UpdateLayout();
    NotifySize();
    UpdateConnectionPoints();
    NotifyConnectionPoints();
  }

  else if (aSubject == GetClassBox()) {
    UpdateClassBoxRead();
    UpdateLayout();
    NotifySize();
    UpdateConnectionPoints();
    NotifyConnectionPoints();
  }

  else if (aSubject == GetCollection()) {
    UpdateCollectionRead();
  }

  else if (aSubject == ggClassySettings::GetInstance()) {
    UpdateSettings();
  }

  ggGraphicsManipulatorBarItemT<>::Update(aSubject);
}


void ggClassyGraphicsBoxItem::UpdateClassRead()
{
  if (GetClass() != nullptr) {
    Attach(GetCollection()); // what about detaching from the "old" collection? we'll still be
    UpdateCollectionRead();
    mClassNameText->SetText(GetClass()->GetName());
    mMembersText->SetText(GetClass()->GetMembersText());
    mDescriptionText->SetText(GetClass()->GetDescription());    
  }
}


void ggClassyGraphicsBoxItem::UpdateClassWrite()
{
  if (GetClass() != nullptr) {
    // no need to send notifications to myself
    cExecutorBlocking vBlock(this, GetClass());
    // collect notifications and only send one to al other observers
    cExecutorLazy vLazyClass(GetClass());
    cExecutorLazy vLazyConnection(GetClass()->GetDataSet()->GetSubjectConnections());
    // renaming fails, if there is another class with the same name
    if (!GetClass()->SetName(mClassNameText->GetText())) {
      QMessageBox::information(nullptr, "Can't change class name",
                               "Another class with the name \"" + mClassNameText->GetText() +
                               "\" already exists. Please choose a different name!",
                               QMessageBox::Ok);
      mClassNameText->SetText(GetClass()->GetName());
    }
    GetClass()->SetMembersText(mMembersText->GetText());
    GetClass()->SetDescription(mDescriptionText->GetText());
    UpdateLayout();
    UpdateConnectionPoints();
  }
}


void ggClassyGraphicsBoxItem::UpdateClassBoxRead()
{
  if (GetClassBox() != nullptr) {
    SetPosition(GetClassBox()->GetPosition());
    SetWidth(GetClassBox()->GetWidth());
    mMembersCheckBox->SetChecked(GetClassBox()->GetMembersVisible());
    mDescriptionCheckBox->SetChecked(GetClassBox()->GetDescriptionVisible());
  }
}


void ggClassyGraphicsBoxItem::UpdateClassBoxWrite()
{
  if (GetClassBox() != nullptr) {
    ggObserver::cExecutorBlocking vBlock(this, GetClassBox());
    ggSubject::cExecutorLazy vLazy(GetClassBox());
    GetClassBox()->SetPosition(GetPosition());
    GetClassBox()->SetWidth(GetWidth());
    GetClassBox()->SetMembersVisible(mMembersCheckBox->GetChecked());
    GetClassBox()->SetDescriptionVisible(mDescriptionCheckBox->GetChecked());
  }
}


void ggClassyGraphicsBoxItem::UpdateShadowLayout()
{
  float vBorderWidth2 = mBoxBorder->pen().widthF() / 2.0f;
  QRectF vBorderRect = ggUtilityQt::GetRectInflated(rect(), vBorderWidth2);
  mShadow->SetInnerRect(vBorderRect);
}


void ggClassyGraphicsBoxItem::UpdateLayout()
{
  // apply the same width on all text items
  float vWidth = rect().width();
  mClassNameText->setTextWidth(vWidth);
  mMembersText->setTextWidth(vWidth);
  mDescriptionText->setTextWidth(vWidth);

  // get the resulting heights of the text items
  float vClassNameTextHeight = mClassNameText->boundingRect().height();
  float vMembersTextHeight = mMembersText->boundingRect().height();
  float vDescriptionTextHeight = mDescriptionText->boundingRect().height();

  // adjust height of check boxes depending on font height
  float vClassNameFontHeight = QFontMetrics(mClassNameText->font()).height();
  float vCheckBoxSize = vClassNameFontHeight / 2.0f;
  mMembersCheckBox->SetSize(vCheckBoxSize);
  mDescriptionCheckBox->SetSize(vCheckBoxSize);

  // positions and heights...
  float vCheckBoxIndent = 3.0f;
  QPointF vPos(rect().topLeft());
  QPointF vPosCheckBox(rect().topRight());
  vPosCheckBox.rx() -= vCheckBoxSize + vCheckBoxIndent;
  vPosCheckBox.ry() += vCheckBoxIndent;
  float vTotalHeight = 0.0f;

  // class name is always visible
  mClassNameText->setPos(vPos);
  vPos.ry() += vClassNameTextHeight;
  vPosCheckBox.ry() += vClassNameTextHeight;
  vTotalHeight += vClassNameTextHeight;

  // adjust members area
  mMembersText->setVisible(mMembersCheckBox->GetChecked());
  if (mMembersText->isVisible()) {
    mMembersText->setPos(vPos);
    mMembersCheckBox->setPos(vPosCheckBox);
    vPos.ry() += vMembersTextHeight;
    vPosCheckBox.ry() += vMembersTextHeight;
    vTotalHeight += vMembersTextHeight;
  }

  // adjust description area
  mDescriptionText->setVisible(mDescriptionCheckBox->GetChecked());
  if (mDescriptionText->isVisible()) {
    mDescriptionText->setPos(vPos);
    mDescriptionCheckBox->setPos(vPosCheckBox);
    vPos.ry() += vDescriptionTextHeight;
    vPosCheckBox.ry() += vDescriptionTextHeight;
    vTotalHeight += vDescriptionTextHeight;
  }

  // place checkboxes of invisible text areas into class name area
  vPosCheckBox = rect().topRight();
  vPosCheckBox.rx() -= vCheckBoxSize + vCheckBoxIndent;
  vPosCheckBox.ry() += vClassNameTextHeight - vCheckBoxSize - vCheckBoxIndent;
  if (!mMembersText->isVisible()) {
    mMembersCheckBox->setPos(vPosCheckBox);
    vPosCheckBox.rx() -= vCheckBoxSize + vCheckBoxIndent;
  }
  if (!mDescriptionText->isVisible()) {
    mDescriptionCheckBox->setPos(vPosCheckBox);
  }

  // adjust own overall height
  QRectF vRect(rect());
  if (vRect.height() != vTotalHeight) {
    vRect.setHeight(vTotalHeight);
    setRect(vRect);
  }

  // border
  float vBorderWidth2 = mBoxBorder->pen().widthF() / 2.0f;
  QRectF vBorderRect = ggUtilityQt::GetRectInflated(rect(), vBorderWidth2);
  mBoxBorder->setRect(vBorderRect);

  // shadow
  UpdateShadowLayout();

  // connection manipulator
  mBaseClassConnector->SetPositionSrc(QPointF(rect().center().x(), rect().top()));
}


void ggClassyGraphicsBoxItem::NotifySize()
{
  mSubjectSize.Notify();
}


void ggClassyGraphicsBoxItem::UpdateConnectionPoints()
{
  mClassConnectionTop.SetPosition(GetClassPositionTop());
  mClassConnectionBottom.SetPosition(GetClassPositionBottom());
  mClassConnectionLeft.SetPosition(GetClassPositionLeft());
  mClassConnectionRight.SetPosition(GetClassPositionRight());
  if (GetClass() != nullptr) {
    const ggClassyClass::tMembers& vMembers = GetClass()->GetMembers();
    mMembersConnectionLeft.resize(vMembers.size());
    mMembersConnectionRight.resize(vMembers.size());
    for (ggUSize vIndex = 0; vIndex < vMembers.size(); vIndex++) {
      mMembersConnectionLeft[vIndex].SetPosition(GetMemberPositionLeft(vIndex));
      mMembersConnectionLeft[vIndex].SetDirectionLeft();
      mMembersConnectionLeft[vIndex].SetVisible(mMembersText->isVisible());
      mMembersConnectionRight[vIndex].SetPosition(GetMemberPositionRight(vIndex));
      mMembersConnectionRight[vIndex].SetDirectionRight();
      mMembersConnectionRight[vIndex].SetVisible(mMembersText->isVisible());
    }
  }
  else {
    mMembersConnectionLeft.clear();
    mMembersConnectionRight.clear();
  }
}


void ggClassyGraphicsBoxItem::NotifyConnectionPoints()
{
  mClassConnectionTop.Notify();
  mClassConnectionBottom.Notify();
  mClassConnectionLeft.Notify();
  mClassConnectionRight.Notify();
  ggWalkerT<tSubjectConnectionPoints::iterator> vConnectionsWalkerLeft(mMembersConnectionLeft);
  while (vConnectionsWalkerLeft) (*vConnectionsWalkerLeft).Notify();
  ggWalkerT<tSubjectConnectionPoints::iterator> vConnectionsWalkerRight(mMembersConnectionRight);
  while (vConnectionsWalkerRight) (*vConnectionsWalkerRight).Notify();
}


const ggClassyCollection* ggClassyGraphicsBoxItem::GetCollection() const
{
  const ggClassyCollection* vCollection = nullptr;
  if (GetClass() != nullptr) vCollection = GetClass()->GetCollection();
  if (vCollection == nullptr) vCollection = ggClassyCollection::GetDefaultCollection();
  return vCollection;
}


QPen ggClassyGraphicsBoxItem::GetBoxBorderPen() const
{
  QPen vPen = mBoxBorder->pen();
  const ggClassyCollection* vCollection = GetCollection();
  if (vCollection != nullptr) vPen = vCollection->mBoxBorder;
  if (isSelected()) {
    vPen.setColor(ggClassySettings::GetInstance()->GetSelectionColor());
    if (vPen.widthF() < 1.0f) vPen.setWidth(1.0f);
  }
  return vPen;
}


const QColor& ggClassyGraphicsBoxItem::GetShadowColor() const
{
  return isSelected() ? ggClassySettings::GetInstance()->GetSelectionColor() : ggClassySettings::GetInstance()->GetShadowColor();
}


const QPointF& ggClassyGraphicsBoxItem::GetShadowOffset() const
{
  static const QPointF vOffsetSelected(0.0f, 0.0f);
  return isSelected() ? vOffsetSelected : ggClassySettings::GetInstance()->GetShadowOffset();
}


void ggClassyGraphicsBoxItem::UpdateCollectionRead()
{
  const ggClassyCollection* vCollection = GetCollection();
  if (vCollection != nullptr) {
    // box border
    mBoxBorder->setPen(GetBoxBorderPen());
    // class name
    mClassNameText->setFont(vCollection->mNameFont);
    mClassNameText->setDefaultTextColor(vCollection->mNameColor);
    mClassNameText->SetBrush(vCollection->mNameBackground);
    // member functions
    mMembersText->setFont(vCollection->mMembersFont);
    mMembersText->setDefaultTextColor(vCollection->mMembersColor);
    mMembersText->SetBrush(vCollection->mMembersBackground);
    // description
    mDescriptionText->setFont(vCollection->mDescriptionFont);
    mDescriptionText->setDefaultTextColor(vCollection->mDescriptionColor);
    mDescriptionText->SetBrush(vCollection->mDescriptionBackground);
  }
}


void ggClassyGraphicsBoxItem::UpdateSettings()
{
  SetHandleSize(ggClassySettings::GetInstance()->GetHandleSize());
  SetHandleColor(ggClassySettings::GetInstance()->GetHighlightColor());
  mBaseClassConnector->SetSize(ggClassySettings::GetInstance()->GetHandleSize());
  mBaseClassConnector->SetColor(ggClassySettings::GetInstance()->GetHighlightColor());
  mShadow->SetRadius(ggClassySettings::GetInstance()->GetShadowWidth());
  mShadow->SetShadowWidth(ggClassySettings::GetInstance()->GetShadowWidth());
  mShadow->SetShadowColors(GetShadowColor());
  mShadow->setPos(GetShadowOffset());
  mMembersCheckBox->SetColor(ggClassySettings::GetInstance()->GetHighlightColor());
  mDescriptionCheckBox->SetColor(ggClassySettings::GetInstance()->GetHighlightColor());
  UpdateShadowLayout();
  mBoxBorder->setPen(GetBoxBorderPen());
}
