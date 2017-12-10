// 0) include own header
#include "ggSubjectConnectionPoint.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)


ggSubjectConnectionPoint::ggSubjectConnectionPoint() :
  mVisible(true)
{
}


void ggSubjectConnectionPoint::SetVisible(bool aVisible)
{
  mVisible = aVisible;
}


bool ggSubjectConnectionPoint::GetVisible() const
{
  return mVisible;
}
