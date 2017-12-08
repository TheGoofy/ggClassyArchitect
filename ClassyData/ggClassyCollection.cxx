#include "ClassyData/ggClassyCollection.h"


const QString& ggClassyCollection::TypeID()
{
  static const QString vTypeID("ggClassyCollection");
  return vTypeID;
}


const QString& ggClassyCollection::VTypeID() const
{
  return TypeID();
}
