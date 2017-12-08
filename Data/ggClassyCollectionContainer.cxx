#include "Data/ggClassyCollectionContainer.h"


const QString& ggClassyCollectionContainer::TypeID()
{
  static const QString vTypeID("ggClassyCollectionContainer");
  return vTypeID;
}


const QString& ggClassyCollectionContainer::VTypeID() const
{
  return TypeID();
}
