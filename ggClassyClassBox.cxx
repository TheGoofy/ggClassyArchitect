#include "ggClassyClassBox.h"


ggClassyClassBox::ggClassyClassBox(const QString& aClassName) :
  mClassName(aClassName),
  mPosition(0.0f, 0.0f),
  mWidth(150.0f),
  mMembersVisible(true),
  mDescriptionVisible(true)
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
  return vElement;
}

