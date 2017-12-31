// 0) include own header
#include "ggClassyFrame.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyDataSet.h"


const QString& ggClassyFrame::TypeID()
{
  static const QString vTypeID("ggClassyFrame");
  return vTypeID;
}


const QString& ggClassyFrame::VTypeID() const
{
  return TypeID();
}


QDomElement ggClassyFrame::CreateDomElement(QDomDocument& aDocument) const
{
  QDomElement vElement = aDocument.createElement(TypeID());

  vElement.setAttribute("mTextAlignment", mAlignment);
  vElement.setAttribute("mCollectionName", mCollectionName);
  vElement.setAttribute("mRect.x", mRect.x());
  vElement.setAttribute("mRect.y", mRect.y());
  vElement.setAttribute("mRect.width", mRect.width());
  vElement.setAttribute("mRect.height", mRect.height());
  vElement.setAttribute("mIndexZ", mIndexZ);
  vElement.appendChild(mDescription.CreateDomElement(aDocument));

  return vElement;
}


ggClassyFrame* ggClassyFrame::Create(const QDomElement& aElement)
{
  ggClassyFrame* vFrame = nullptr;
  if (aElement.tagName() == TypeID()) {

    // frame properties
    vFrame = new ggClassyFrame();
    vFrame->mAlignment = static_cast<Qt::Alignment>(aElement.attribute("mTextAlignment").toInt());
    vFrame->mCollectionName = aElement.attribute("mCollectionName");
    vFrame->mRect.setX(aElement.attribute("mRect.x").toDouble());
    vFrame->mRect.setY(aElement.attribute("mRect.y").toDouble());
    vFrame->mRect.setWidth(aElement.attribute("mRect.width").toDouble());
    vFrame->mRect.setHeight(aElement.attribute("mRect.height").toDouble());
    vFrame->mIndexZ = aElement.attribute("mIndexZ").toInt();

    // parse children (find the description)
    QDomElement vChildElement = aElement.firstChildElement();
    while (!vChildElement.isNull()) {
      // description
      ggClassyDescription vDescription = ggClassyDescription::Create(vChildElement);
      if (vDescription != "") vFrame->mDescription = vDescription;
      // next child
      vChildElement = vChildElement.nextSiblingElement();
    }
  }
  return vFrame;
}


const QString& ggClassyFrame::GetDescription() const
{
  return mDescription;
}


void ggClassyFrame::SetDescription(const QString& aDescription)
{
  if (aDescription != mDescription) {
    mDescription = aDescription;
    Notify();
  }
}


const QString& ggClassyFrame::GetCollectionName() const
{
  return mCollectionName;
}


void ggClassyFrame::SetCollectionName(const QString& aName)
{
  if (aName != mCollectionName) {
    mCollectionName = aName;
    Notify();
  }
}


ggClassyDataSet* ggClassyFrame::GetDataSet()
{
  return mDataSet;
}


const ggClassyDataSet* ggClassyFrame::GetDataSet() const
{
  return mDataSet;
}


void ggClassyFrame::SetDataSet(ggClassyDataSet* aDataSet)
{
  if (aDataSet != mDataSet) {
    mDataSet = aDataSet;
    Notify();
  }
}


const ggClassyCollection* ggClassyFrame::GetCollection() const
{
  if (mDataSet != nullptr) {
    return mDataSet->GetCollections().FindCollection(mCollectionName);
  }
  return nullptr;
}


int ggClassyFrame::GetIndexZ() const
{
  return mIndexZ;
}


void ggClassyFrame::SetIndexZ(int aIndexZ)
{
  mIndexZ = aIndexZ;
}


const QRectF& ggClassyFrame::GetRect() const
{
  return mRect;
}


void ggClassyFrame::SetRect(const QRectF& aRect)
{
  if (aRect != mRect) {
    mRect = aRect;
    Notify();
  }
}


Qt::Alignment ggClassyFrame::GetAlignment() const
{
  return mAlignment;
}


void ggClassyFrame::SetAlignment(Qt::Alignment aAlignment)
{
  if (mAlignment != aAlignment) {
    mAlignment = aAlignment;
    Notify();
  }
}
