// 0) include own header
#include "ggClassyClassBox.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)


ggClassyClassBox::ggClassyClassBox(const QString& aClassName) :
  mClassName(aClassName),
  mPosition(0.0f, 0.0f),
  mWidth(150.0f),
  mMembersVisible(true),
  mDescriptionVisible(true),
  mIndexZ(0)
{
}


const QString& ggClassyClassBox::TypeID()
{
  static const QString vTypeID("ggClassyClassBox");
  return vTypeID;
}


const QString& ggClassyClassBox::VTypeID() const
{
  return TypeID();
}


QDomElement ggClassyClassBox::CreateDomElement(QDomDocument& aDocument) const
{
  QDomElement vElement = aDocument.createElement(TypeID());
  vElement.setAttribute("mClassName", mClassName);
  vElement.setAttribute("mPosition.x", mPosition.x());
  vElement.setAttribute("mPosition.y", mPosition.y());
  vElement.setAttribute("mWidth", mWidth);
  vElement.setAttribute("mMembersVisible", mMembersVisible);
  vElement.setAttribute("mDescriptionVisible", mDescriptionVisible);
  vElement.setAttribute("mIndexZ", mIndexZ);
  return vElement;
}


ggClassyClassBox* ggClassyClassBox::Create(const QDomElement& aElement)
{
  ggClassyClassBox* vClassBox = nullptr;
  if (aElement.tagName() == TypeID()) {
    vClassBox = new ggClassyClassBox(aElement.attribute("mClassName"));
    vClassBox->mPosition.setX(aElement.attribute("mPosition.x").toFloat());
    vClassBox->mPosition.setY(aElement.attribute("mPosition.y").toFloat());
    vClassBox->mWidth = aElement.attribute("mWidth").toFloat();
    vClassBox->mMembersVisible = aElement.attribute("mMembersVisible").toInt();
    vClassBox->mDescriptionVisible = aElement.attribute("mDescriptionVisible").toInt();
    vClassBox->mIndexZ = aElement.attribute("mIndexZ").toInt();
  }
  return vClassBox;
}


const QString& ggClassyClassBox::GetClassName() const
{
  return mClassName;
}


void ggClassyClassBox::SetClassName(const QString& aClassName)
{
  if (aClassName != mClassName) {
    mClassName = aClassName;
    Notify();
  }
}


const QPointF& ggClassyClassBox::GetPosition() const
{
  return mPosition;
}


void ggClassyClassBox::SetPosition(const QPointF& aPosition)
{
  if (aPosition != mPosition) {
    mPosition = aPosition;
    Notify();
  }
}


float ggClassyClassBox::GetWidth() const
{
  return mWidth;
}


void ggClassyClassBox::SetWidth(float aWidth)
{
  if (aWidth != mWidth) {
    mWidth = aWidth;
    Notify();
  }
}


bool ggClassyClassBox::GetMembersVisible() const
{
  return mMembersVisible;
}


void ggClassyClassBox::SetMembersVisible(bool aVisible)
{
  if (aVisible != mMembersVisible) {
    mMembersVisible = aVisible;
    Notify();
  }
}


bool ggClassyClassBox::GetDescriptionVisible() const
{
  return mDescriptionVisible;
}


void ggClassyClassBox::SetDescriptionVisible(bool aVisible)
{
  if (aVisible != mDescriptionVisible) {
    mDescriptionVisible = aVisible;
    Notify();
  }
}


int ggClassyClassBox::GetIndexZ() const
{
  return mIndexZ;
}


void ggClassyClassBox::SetIndexZ(int aIndexZ)
{
  mIndexZ = aIndexZ;
}
