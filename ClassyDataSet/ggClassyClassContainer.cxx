// 0) include own header
#include "ggClassyClassContainer.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)


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
