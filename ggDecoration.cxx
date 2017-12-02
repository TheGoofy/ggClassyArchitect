#include "ggDecoration.h"


ggDecoration::ggDecoration() :
  mType(cType::eLine),
  mLength(0.0f),
  mSolid(true)
{
}


ggDecoration::ggDecoration(cType aType,
                           float aLength,
                           bool aSolid) :
  mType(aType),
  mLength(aLength),
  mSolid(aSolid)
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


void ggDecoration::SetSolid(bool aSolid)
{
  mSolid = aSolid;
}


bool ggDecoration::GetSolid() const
{
  return mSolid;
}


void ggDecoration::Set(cType aType,
                       float aLength,
                       bool aSolid)
{
  mType = aType;
  mLength = aLength;
  mSolid = aSolid;
}
