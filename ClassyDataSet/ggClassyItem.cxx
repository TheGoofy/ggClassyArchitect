// 0) include own header
#include "ggClassyItem.h"

// 1) include system or QT
#include <QString>

// 2) include own project-related (sort by component dependency)


const QString& ggClassyItem::TypeID()
{
  static const QString vTypeID("ggClassyItem");
  return vTypeID;
}


const QString& ggClassyItem::VTypeID() const
{
  return TypeID();
}
