#include "ClassyDataSet/ggClassyFrame.h"


const QString& ggClassyFrame::TypeID()
{
  static const QString vTypeID("ggClassyFrame");
  return vTypeID;
}


const QString& ggClassyFrame::VTypeID() const
{
  return TypeID();
}

