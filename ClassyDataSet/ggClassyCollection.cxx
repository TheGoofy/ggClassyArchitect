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


bool ggClassyCollection::operator() (const ggClassyCollection* aCollectionA,
                                     const ggClassyCollection* aCollectionB) const
{
  if (aCollectionA == nullptr || aCollectionB == nullptr) {
    return aCollectionA < aCollectionB;
  }
  else {
    return aCollectionA->mName < aCollectionB->mName;
  }
}


void ggClassyCollection::SetDataSet(ggClassyDataSet* aDataSet)
{
  if (aDataSet != mDataSet) {
    mDataSet = aDataSet;
    Notify();
  }
}


ggClassyDataSet* ggClassyCollection::GetDataSet() const
{
  return mDataSet;
}
