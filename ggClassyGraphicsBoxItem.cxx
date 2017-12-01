#include "ggClassyGraphicsBoxItem.h"

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QFontMetrics>
#include <QDebug>

#include "ggClassyApplication.h"
#include "ggGraphicsManipulatorBarItemT.h"
#include "ggGraphicsTextItem.h"
#include "ggGraphicsCheckBoxItem.h"
#include "ggClassyGraphicsBoxItems.h"
#include "ggClassyDataSet.h"


ggClassyGraphicsBoxItem::ggClassyGraphicsBoxItem(const QRectF& aRect) :
  ggGraphicsManipulatorBarItemT<>(QRect()),
  mClassNameText(nullptr),
  mMembersText(nullptr),
  mCommentText(nullptr),
  mMembersCheckBox(nullptr),
  mCommentCheckBox(nullptr),
  mClassBox(nullptr),
  mBoxItems(nullptr)
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
  mCommentText->SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
                        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");

  UpdateLayout();
}


ggClassyGraphicsBoxItem::ggClassyGraphicsBoxItem(ggClassyClassBox* aClassBox) :
  ggGraphicsManipulatorBarItemT<>(QRect()),
  mClassNameText(nullptr),
  mMembersText(nullptr),
  mCommentText(nullptr),
  mMembersCheckBox(nullptr),
  mCommentCheckBox(nullptr),
  mClassBox(nullptr),
  mBoxItems(nullptr)
{
  Construct();
  SetClassBox(aClassBox);
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

  mCommentText = new ggGraphicsTextItem(this);
  mCommentText->SetSuppressLineBreaks(false);
  mCommentText->SetEnterKeyFinishesEdit(false);
  mCommentText->SetBrush(QColor(255, 240, 220, 255));
  mCommentText->SetPen(Qt::NoPen);

  mMembersCheckBox = new ggGraphicsCheckBoxItem(this);
  mCommentCheckBox = new ggGraphicsCheckBoxItem(this);

  Attach(mClassNameText->GetSubjectText());
  Attach(mClassNameText->GetSubjectEditingFinished());
  Attach(mMembersText->GetSubjectText());
  Attach(mMembersText->GetSubjectEditingFinished());
  Attach(mCommentText->GetSubjectText());
  Attach(mCommentText->GetSubjectEditingFinished());
  Attach(mMembersCheckBox->GetSubjectChecked());
  Attach(mCommentCheckBox->GetSubjectChecked());
  Attach(GetSubjectPosition());
  Attach(GetSubjectWidth());
}


void ggClassyGraphicsBoxItem::SetClassBox(ggClassyClassBox* aClassBox)
{
  if (mClassBox != aClassBox) {
    if (mClassBox != nullptr) {
      Detach(mClassBox);
      Detach(mClassBox->mClass);
    }
    mClassBox = aClassBox;
    if (mClassBox != nullptr) {
      Attach(mClassBox);
      Attach(mClassBox->mClass);
    }
    UpdateClassRead();
    UpdateClassBoxRead();
    UpdateLayout();
  }
}


ggClassyClassBox* ggClassyGraphicsBoxItem::GetClassBox() const
{
  return mClassBox;
}


ggClassyClass* ggClassyGraphicsBoxItem::GetClass() const
{
  if (mClassBox == nullptr) return nullptr;
  return mClassBox->mClass;
}


void ggClassyGraphicsBoxItem::SetBoxItems(const ggClassyGraphicsBoxItems* aBoxItems)
{
  Detach(mBoxItems);
  mBoxItems = aBoxItems;
  Attach(mBoxItems);
}


const ggSubject* ggClassyGraphicsBoxItem::GetSubjectHeight() const
{
  return &mSubjectHeight;
}


QPointF ggClassyGraphicsBoxItem::GetPositionTopCenter() const
{
  return GetPosition() + QPointF(rect().width()/2.0f, 0.0f);
}


QPointF ggClassyGraphicsBoxItem::GetPositionBottomCenter() const
{
  return GetPosition() + QPointF(rect().width()/2.0f, rect().height());
}


void ggClassyGraphicsBoxItem::hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent)
{
  mMembersCheckBox->SetHighlightOn();
  mCommentCheckBox->SetHighlightOn();
  ggGraphicsManipulatorBarItemT<>::hoverEnterEvent(aEvent);
}


void ggClassyGraphicsBoxItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent)
{
  mMembersCheckBox->SetHighlightOff();
  mCommentCheckBox->SetHighlightOff();
  ggGraphicsManipulatorBarItemT<>::hoverLeaveEvent(aEvent);
}


void ggClassyGraphicsBoxItem::Update(const ggSubject* aSubject)
{
  if (aSubject == GetSubjectPosition()) {
    UpdateClassBoxWrite();
    NotifyClassBoxChange();
  }

  else if (aSubject == GetSubjectWidth()) {
    UpdateLayout();
    NotifyHeight();
    UpdateClassBoxWrite();
    NotifyClassBoxChange();
  }

  else if (aSubject == mClassNameText->GetSubjectText() ||
           aSubject == mMembersText->GetSubjectText() ||
           aSubject == mCommentText->GetSubjectText()) {
    UpdateLayout();
    NotifyHeight();
    UpdateClassBoxWrite();
    NotifyClassBoxChange();
  }

  else if (aSubject == mClassNameText->GetSubjectEditingFinished() ||
           aSubject == mMembersText->GetSubjectEditingFinished() ||
           aSubject == mCommentText->GetSubjectEditingFinished()) {
    UpdateClassWrite();
    NotifyClassChange();
  }

  else if (aSubject == mMembersCheckBox->GetSubjectChecked() ||
           aSubject == mCommentCheckBox->GetSubjectChecked()) {
    UpdateLayout();
    NotifyHeight();
    UpdateClassBoxWrite();
    NotifyClassBoxChange();
  }

  else if (aSubject == GetClass()) {
    UpdateClassRead();
    UpdateLayout();
    NotifyHeight();
  }

  else if (aSubject == GetClassBox()) {
    UpdateClassBoxRead();
    UpdateLayout();
    NotifyHeight();
  }

  ggGraphicsManipulatorBarItemT<>::Update(aSubject);
}


void ggClassyGraphicsBoxItem::UpdateClassRead()
{
  if (GetClass() != nullptr) {
    mClassNameText->SetText(GetClass()->mName);
    mMembersText->SetText(GetClass()->GetMembersText());
    mCommentText->SetText(GetClass()->mComment);
  }
}


void ggClassyGraphicsBoxItem::UpdateClassWrite()
{
  if (GetClass() != nullptr) {
    GetClass()->mName = mClassNameText->GetText();
    GetClass()->SetMembersText(mMembersText->GetText());
    GetClass()->mComment = mCommentText->GetText();
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
    mCommentCheckBox->SetChecked(GetClassBox()->mCommentVisible);
  }
}


void ggClassyGraphicsBoxItem::UpdateClassBoxWrite()
{
  if (GetClassBox() != nullptr) {
    GetClassBox()->mPosition = GetPosition();
    GetClassBox()->mWidth = GetWidth();
    GetClassBox()->mMembersVisible = mMembersCheckBox->GetChecked();
    GetClassBox()->mCommentVisible = mCommentCheckBox->GetChecked();
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
  mCommentText->setTextWidth(vWidth);

  // get the resulting heights of the text items
  float vClassNameTextHeight = mClassNameText->boundingRect().height();
  float vMembersTextHeight = mMembersText->boundingRect().height();
  float vCommentTextHeight = mCommentText->boundingRect().height();

  // adjust height of check boxes depending on font height
  float vClassNameFontHeight = QFontMetrics(mClassNameText->font()).height();
  float vCheckBoxSize = vClassNameFontHeight / 2.0f;
  mMembersCheckBox->SetSize(vCheckBoxSize);
  mCommentCheckBox->SetSize(vCheckBoxSize);

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

  // adjust comment area
  mCommentText->setVisible(mCommentCheckBox->GetChecked());
  if (mCommentText->isVisible()) {
    mCommentText->setPos(vPos);
    mCommentCheckBox->setPos(vPosCheckBox);
    vPos.ry() += vCommentTextHeight;
    vPosCheckBox.ry() += vCommentTextHeight;
    vTotalHeight += vCommentTextHeight;
  }

  // place checkboxes of invisible text areas into class name area
  vPosCheckBox = rect().topRight();
  vPosCheckBox.rx() -= vCheckBoxSize + vCheckBoxIndent;
  vPosCheckBox.ry() += vClassNameTextHeight - vCheckBoxSize - vCheckBoxIndent;
  if (!mMembersText->isVisible()) {
    mMembersCheckBox->setPos(vPosCheckBox);
    vPosCheckBox.rx() -= vCheckBoxSize + vCheckBoxIndent;
  }
  if (!mCommentText->isVisible()) {
    mCommentCheckBox->setPos(vPosCheckBox);
  }

  // adjust own overall height
  QRectF vRect(rect());
  if (vRect.height() != vTotalHeight) {
    vRect.setHeight(vTotalHeight);
    setRect(vRect);
  }
}


void ggClassyGraphicsBoxItem::NotifyHeight()
{
  mSubjectHeight.Notify();
}


