// 0) include own header
#include "ggClassyCollectionContainer.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)


const QString& ggClassyCollectionContainer::TypeID()
{
  static const QString vTypeID("ggClassyCollectionContainer");
  return vTypeID;
}


const QString& ggClassyCollectionContainer::VTypeID() const
{
  return TypeID();
}
