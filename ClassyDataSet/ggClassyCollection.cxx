// 0) include own header
#include "ggClassyCollection.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)


const QString& ggClassyCollection::TypeID()
{
  static const QString vTypeID("ggClassyCollection");
  return vTypeID;
}


const QString& ggClassyCollection::VTypeID() const
{
  return TypeID();
}
