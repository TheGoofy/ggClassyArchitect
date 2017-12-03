#include "ggDecoration.h"


ggDecoration::ggDecoration() :
  mType(cType::eLine),
  mLength(0.0f),
  mFill(cFill::eSolid)
{
}


ggDecoration::ggDecoration(cType aType,
                           float aLength,
                           cFill aFill) :
  mType(aType),
  mLength(aLength),
  mFill(aFill)
{
}


void ggDecoration::SetType(cType aType)
{
  mType = aType;
}


ggDecoration::cType ggDecoration::GetType() const
{
  return mType;
}


void ggDecoration::SetLength(float aLength)
{
  mLength = aLength;
}


float ggDecoration::GetLength() const
{
  return mLength;
}


void ggDecoration::SetFill(cFill aFill)
{
  mFill = aFill;
}


ggDecoration::cFill ggDecoration::GetFill() const
{
  return mFill;
}


bool ggDecoration::GetFillEmpty() const
{
  return mFill == cFill::eEmpty;
}


bool ggDecoration::GetFillSolid() const
{
  return mFill == cFill::eSolid;
}


void ggDecoration::Set(cType aType,
                       float aLength,
                       cFill aFill)
{
  mType = aType;
  mLength = aLength;
  mFill = aFill;
}
