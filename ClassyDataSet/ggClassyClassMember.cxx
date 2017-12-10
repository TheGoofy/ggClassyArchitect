// 0) include own header
#include "ggClassyClassMember.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)


ggClassyClassMember::ggClassyClassMember(const QString& aName,
                                         const QString& aClassName) :
  mName(aName),
  mClassName(aClassName)
{
}


const QString& ggClassyClassMember::TypeID()
{
  static const QString vTypeID("ggClassyClassMember");
  return vTypeID;
}


const QString& ggClassyClassMember::VTypeID() const
{
  return TypeID();
}


void ggClassyClassMember::SetName(const QString& aName)
{
  mName = aName;
}


const QString& ggClassyClassMember::GetName() const
{
  return mName;
}


void ggClassyClassMember::SetClassName(const QString& aClassName)
{
  mClassName = aClassName;
}


const QString& ggClassyClassMember::GetClassName() const
{
  return mClassName;
}


bool ggClassyClassMember::operator < (const ggClassyClassMember& aOther) const
{
  if (mName != aOther.mName)
    return (mName < aOther.mName);
  else
    return (mClassName < aOther.mClassName);
}


QDomElement ggClassyClassMember::CreateDomElement(QDomDocument& aDocument) const
{
  QDomElement vElement = aDocument.createElement(TypeID());
  vElement.setAttribute("mClassName", mClassName);
  QDomText vNameText = aDocument.createTextNode(mName);
  vElement.appendChild(vNameText);
  return vElement;
}
