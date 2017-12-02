#include "ggSubjectConnectionPoint.h"


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
