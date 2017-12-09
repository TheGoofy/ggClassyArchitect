#include "ggClassyGraphicsBoxItem.h"

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QFontMetrics>
#include <QMessageBox>
#include <QDebug>

#include "Base/ggWalkerT.h"
#include "BaseGraphics/ggGraphicsTextItem.h"
#include "BaseGraphics/ggGraphicsCheckBoxItem.h"
#include "ClassyData/ggClassyDataSet.h"
#include "ClassyMain/ggClassyApplication.h"


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
  // setFlag(ItemIsSelectable); // size-handle don't work when item is selected
  setBrush(QColor(255, 255, 255, 255));
  QPen vPen(QColor(230, 230, 200, 255), 3.0f);
  vPen.setCapStyle(Qt::FlatCap);
  vPen.setJoinStyle(Qt::RoundJoin);
  setPen(vPen);

  mClassNameText = new ggGraphicsTextItem(this);
  mClassNameText->SetSuppressLineBreaks(true);
  mClassNameText->SetEnterKeyFinishesEdit(true);
  mClassNameText->SetBrush(QColor(120, 60, 0, 255));
  mClassNameText->SetPen(Qt::NoPen);
  mClassNameText->setDefaultTextColor(Qt::white);
  QFont vFont(mClassNameText->font());
  vFont.setBold(true);
  mClassNameText->setFont(vFont);

  mMembersText = new ggGraphicsTextItem(this);
  mMembersText->SetSuppressLineBreaks(false);
  mMembersText->SetEnterKeyFinishesEdit(true);
  mMembersText->SetBrush(brush());
  mMembersText->SetPen(Qt::NoPen);

  mDescriptionText = new ggGraphicsTextItem(this);
  mDescriptionText->SetSuppressLineBreaks(false);
  mDescriptionText->SetEnterKeyFinishesEdit(false);
  mDescriptionText->SetBrush(QColor(255, 240, 220, 255));
  mDescriptionText->SetPen(Qt::NoPen);

  mMembersCheckBox = new ggGraphicsCheckBoxItem(this);
  mDescriptionCheckBox = new ggGraphicsCheckBoxItem(this);

  mClassConnectionTop.SetDirectionUp();
  mClassConnectionBottom.SetDirectionDown();
  mClassConnectionLeft.SetDirectionLeft();
  mClassConnectionRight.SetDirectionRight();

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
  return GetPosition() + QPointF(rect().width()/2.0f, 0.0f);
}


QPointF ggClassyGraphicsBoxItem::GetClassPositionBottom() const
{
  return GetPosition() + QPointF(rect().width()/2.0f, rect().height());
}


QPointF ggClassyGraphicsBoxItem::GetClassPositionLeft() const
{
  // offset from upper left corner
  QPointF vPosition = GetPosition();
  vPosition.ry() += mClassNameText->boundingRect().height() / 2.0f;
  return vPosition;
}


QPointF ggClassyGraphicsBoxItem::GetClassPositionRight() const
{
  // same as left, just displaced by the box width
  QPointF vPosition = GetClassPositionLeft();
  vPosition.rx() += GetWidth();
  return vPosition;
}


QPointF ggClassyGraphicsBoxItem::GetMemberPositionLeft(ggUSize aMemberIndex) const
{
  // calculate the height of a single member  
  ggUSize vNumberOfMembers = 1;
  if (GetClass() != nullptr) vNumberOfMembers = GetClass()->mMembers.size();
  if (vNumberOfMembers < 1) vNumberOfMembers = 1;
  float vIndentY = 3.0f;
  float vMemberHeight = (mMembersText->boundingRect().height() - 2.0f* vIndentY) / vNumberOfMembers;

  // offset from upper left corner
  QPointF vPosition = GetPosition();
  vPosition.ry() += mClassNameText->boundingRect().height() + vIndentY;
  vPosition.ry() += aMemberIndex * vMemberHeight + vMemberHeight / 2.0f;

  // done...
  return vPosition;
}


QPointF ggClassyGraphicsBoxItem::GetMemberPositionRight(ggUSize aMemberIndex) const
{
  // same as left, just displaced by the box width
  QPointF vPosition = GetMemberPositionLeft(aMemberIndex);
  vPosition.rx() += GetWidth();
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
  ggGraphicsManipulatorBarItemT<>::hoverEnterEvent(aEvent);
}


void ggClassyGraphicsBoxItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent)
{
  mMembersCheckBox->SetHighlightOff();
  mDescriptionCheckBox->SetHighlightOff();
  ggGraphicsManipulatorBarItemT<>::hoverLeaveEvent(aEvent);
}


void ggClassyGraphicsBoxItem::Update(const ggSubject* aSubject)
{
  if (aSubject == GetSubjectPosition()) {
    UpdateClassBoxWrite();
    NotifyClassBoxChange();
    UpdateConnectionPoints();
    NotifyConnectionPoints();
  }

  else if (aSubject == GetSubjectWidth()) {
    UpdateLayout();
    NotifySize();
    UpdateClassBoxWrite();
    NotifyClassBoxChange();
    UpdateConnectionPoints();
    NotifyConnectionPoints();
  }

  else if (aSubject == mClassNameText->GetSubjectText() ||
           aSubject == mMembersText->GetSubjectText() ||
           aSubject == mDescriptionText->GetSubjectText()) {
    UpdateLayout();
    NotifySize();
    UpdateClassBoxWrite();
    NotifyClassBoxChange();
    UpdateConnectionPoints();
    NotifyConnectionPoints();
  }

  else if (aSubject == mClassNameText->GetSubjectEditingFinished() ||
           aSubject == mMembersText->GetSubjectEditingFinished() ||
           aSubject == mDescriptionText->GetSubjectEditingFinished()) {
    UpdateClassWrite();
    NotifyClassChange();
  }

  else if (aSubject == mMembersCheckBox->GetSubjectChecked() ||
           aSubject == mDescriptionCheckBox->GetSubjectChecked()) {
    UpdateLayout();
    NotifySize();
    UpdateClassBoxWrite();
    NotifyClassBoxChange();
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

  ggGraphicsManipulatorBarItemT<>::Update(aSubject);
}


void ggClassyGraphicsBoxItem::UpdateClassRead()
{
  if (GetClass() != nullptr) {
    mClassNameText->SetText(GetClass()->mName);
    mMembersText->SetText(GetClass()->GetMembersText());
    mDescriptionText->SetText(GetClass()->mDescription);
  }
}




void ggClassyGraphicsBoxItem::UpdateClassWrite()
{
  if (GetClass() != nullptr) {
    cExecutorBlocking vBlock(this, GetClass());
    if (!ggClassyApplication::GetInstance().GetDataSet()->mClasses.RenameClass(GetClass()->mName, mClassNameText->GetText())) {
      QMessageBox::information(nullptr, "Can't change class name",
                               "Another class with the name \"" + mClassNameText->GetText() + "\" already exists. Please choose a different name!",
                               QMessageBox::Ok);
      mClassNameText->SetText(GetClass()->mName);
    }
    GetClass()->SetMembersText(mMembersText->GetText());
    GetClass()->mDescription = mDescriptionText->GetText();
  }
}


void ggClassyGraphicsBoxItem::NotifyClassChange()
{
  if (GetClass() != nullptr) GetClass()->Notify(this);
}


void ggClassyGraphicsBoxItem::UpdateClassBoxRead()
{
  if (GetClassBox() != nullptr) {
    SetPosition(GetClassBox()->mPosition);
    SetWidth(GetClassBox()->mWidth);
    mMembersCheckBox->SetChecked(GetClassBox()->mMembersVisible);
    mDescriptionCheckBox->SetChecked(GetClassBox()->mDescriptionVisible);
  }
}


void ggClassyGraphicsBoxItem::UpdateClassBoxWrite()
{
  if (GetClassBox() != nullptr) {
    GetClassBox()->mPosition = GetPosition();
    GetClassBox()->mWidth = GetWidth();
    GetClassBox()->mMembersVisible = mMembersCheckBox->GetChecked();
    GetClassBox()->mDescriptionVisible = mDescriptionCheckBox->GetChecked();
  }
}


void ggClassyGraphicsBoxItem::NotifyClassBoxChange()
{
  if (GetClassBox() != nullptr) GetClassBox()->Notify(this);
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
    const ggClassyClass::tMembers& vMembers = GetClass()->mMembers;
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
