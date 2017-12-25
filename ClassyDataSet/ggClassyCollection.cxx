// 0) include own header
#include "ggClassyCollection.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "ClassyDataSet/ggClassyDataSet.h"


ggClassyCollection::ggClassyCollection() :
  mName("unknown"),
  mDataSet(nullptr)
{
}


ggClassyCollection::ggClassyCollection(const QString& aName) :
  mName(aName),
  mDataSet(nullptr)
{
}


ggClassyCollection::~ggClassyCollection()
{
  if (mDataSet != nullptr) mDataSet->RemoveCollection(mName);
}


const QString& ggClassyCollection::TypeID()
{
  static const QString vTypeID("ggClassyCollection");
  return vTypeID;
}


const QString& ggClassyCollection::VTypeID() const
{
  return TypeID();
}


void ggClassyCollection::SetDataSet(ggClassyDataSet* aDataSet)
{
  if (aDataSet != mDataSet) {
    mDataSet = aDataSet;
  }
}


ggClassyDataSet* ggClassyCollection::GetDataSet() const
{
  return mDataSet;
}


const QString& ggClassyCollection::GetName() const
{
  return mName;
}


void ggClassyCollection::SetName(const QString& aName)
{
  if (aName != mName) {
    mName = aName;
  }
}
