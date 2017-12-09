#include "ggClassyClassContainer.h"

#include <QDebug>

#include "Base/ggWalkerT.h"


const QString& ggClassyClassContainer::TypeID()
{
  static const QString vTypeID("ggClassyClassContainer");
  return vTypeID;
}


const QString& ggClassyClassContainer::VTypeID() const
{
  return TypeID();
}


ggClassyClass* ggClassyClassContainer::FindClass(const QString& aClassName) const
{
  ggClassyClass vClass(aClassName);
  const_iterator vClassesIterator = find(&vClass);
  if (vClassesIterator != end()) return *vClassesIterator;
  else return nullptr;
}
