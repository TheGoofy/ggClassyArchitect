#include "ggClassyGraphicsBoxItem.h"

#include <QGraphicsLineItem>
#include <QDebug>

#include "ggClassyApplication.h"
#include "ggGraphicsManipulatorBarItemT.h"
#include "ggGraphicsTextItem.h"
#include "ggClassyGraphicsBoxItems.h"
#include "ggClassyDataSet.h"


ggClassyGraphicsBoxItem::ggClassyGraphicsBoxItem(const QRectF& aRect) :
  ggGraphicsManipulatorBarItemT<>(aRect),
  mNameText(nullptr),
  mMembersText(nullptr),
  mCommentText(nullptr),
  mClassBox(nullptr),
  mBoxItems(nullptr)
{
  Construct();

  mNameText->SetText("TheClassyClass");
  mMembersText->SetText("GetName()\n"
                        "YouWannaDance()\n"
                        "DamnImLookingGood()");
  mCommentText->SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
                        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");

  UpdateLayout();
}


ggClassyGraphicsBoxItem::ggClassyGraphicsBoxItem(ggClassyClassBox* aClassBox) :
  ggGraphicsManipulatorBarItemT<>(QRect()),
  mNameText(nullptr),
  mMembersText(nullptr),
  mCommentText(nullptr),
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

  mNameText = new ggGraphicsTextItem(this);
  mNameText->SetSuppressLineBreaks(true);
  mNameText->SetEnterKeyFinishesEdit(true);
  mNameText->SetBrush(QColor(120, 60, 0, 255));
  mNameText->SetPen(Qt::NoPen);
  mNameText->setDefaultTextColor(Qt::white);

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

  Attach(mNameText->GetSubjectText());
  Attach(mMembersText->GetSubjectText());
  Attach(mCommentText->GetSubjectText());
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

  else if (aSubject == mNameText->GetSubjectText() ||
           aSubject == mMembersText->GetSubjectText() ||
           aSubject == mCommentText->GetSubjectText()) {
    UpdateLayout();
    NotifyHeight();
    UpdateClassWrite();
    NotifyClassChange();
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
    mNameText->SetText(GetClass()->mName);
    mMembersText->SetText(GetClass()->GetMembersText());
    mCommentText->SetText(GetClass()->mComment);
  }
}


void ggClassyGraphicsBoxItem::UpdateClassWrite()
{
  if (GetClass() != nullptr) {
    GetClass()->mName = mNameText->GetText();
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
  }
}


void ggClassyGraphicsBoxItem::UpdateClassBoxWrite()
{
  if (GetClassBox() != nullptr) {
    GetClassBox()->mPosition = GetPosition();
    GetClassBox()->mWidth = GetWidth();
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
  mNameText->setTextWidth(vWidth);
  mMembersText->setTextWidth(vWidth);
  mCommentText->setTextWidth(vWidth);

  // get the resulting heights of the text items
  float vNameTextHeight = mNameText->boundingRect().height();
  float vMembersTextHeight = mMembersText->boundingRect().height();
  float vCommentTextHeight = mCommentText->boundingRect().height();

  // position and height...
  QPointF vPos(rect().topLeft());
  float vTotalHeight = 0.0f;

  // name is always visible
  mNameText->setPos(vPos);
  vPos.ry() += vNameTextHeight;
  vTotalHeight += vNameTextHeight;

  // members area
  if ((mClassBox == nullptr) || mClassBox->mMembersVisible) {
    mMembersText->setVisible(true);
    mMembersText->setPos(vPos);
    vPos.ry() += vMembersTextHeight;
    vTotalHeight += vMembersTextHeight;
  }
  else {
    mMembersText->setVisible(false);
  }

  // comment area
  if ((mClassBox == nullptr) || mClassBox->mCommentVisible) {
    mCommentText->setVisible(true);
    mCommentText->setPos(vPos);
    vPos.ry() += vCommentTextHeight;
    vTotalHeight += vCommentTextHeight;
  }
  else {
    mCommentText->setVisible(false);
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


