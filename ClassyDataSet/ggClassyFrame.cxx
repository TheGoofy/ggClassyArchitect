// 0) include own header
#include "ggClassyFrame.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)


const QString& ggClassyFrame::TypeID()
{
  static const QString vTypeID("ggClassyFrame");
  return vTypeID;
}


const QString& ggClassyFrame::VTypeID() const
{
  return TypeID();
}

