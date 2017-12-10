// 0) include own header
#include "ggClassyDescription.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)


const QString& ggClassyDescription::TypeID()
{
  static const QString vTypeID("ggClassyDescription");
  return vTypeID;
}


const QString& ggClassyDescription::VTypeID() const
{
  return TypeID();
}


QDomElement ggClassyDescription::CreateDomElement(QDomDocument& aDocument) const
{
  QDomElement vElement = aDocument.createElement(TypeID());
  QDomText vDescriptionText = aDocument.createTextNode(*this);
  vElement.appendChild(vDescriptionText);
  return vElement;
}
