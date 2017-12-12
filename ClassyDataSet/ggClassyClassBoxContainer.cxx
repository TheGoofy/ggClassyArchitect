#include "ggClassyClassBoxContainer.h"


const QString& ggClassyClassBoxContainer::TypeID()
{
  static QString vTypeID("ggClassyClassBoxContainer");
  return vTypeID;
}


const QString& ggClassyClassBoxContainer::VTypeID() const
{
  return TypeID();
}
