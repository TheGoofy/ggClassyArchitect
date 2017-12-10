// 0) include own header
#include "ggClassyClassBoxPoints.h"

// 1) include system or QT

// 2) include own project-related (sort by component dependency)
#include "Base/ggWalkerT.h"


ggClassyClassBoxPoints::ggClassyClassBoxPoints()
{
}


void ggClassyClassBoxPoints::AddBoxItem(const ggClassyGraphicsBoxItem* aBoxItem)
{
  if (aBoxItem != nullptr) {
    mBoxItems.insert(aBoxItem);
    if (aBoxItem->GetClass() != nullptr) {
      mBoxItemsMap[aBoxItem->GetClass()->GetName()].push_back(aBoxItem);
    }
  }
}


const ggClassyClassBoxPoints::tBoxItemsSet& ggClassyClassBoxPoints::GetBoxItems() const
{
  return mBoxItems;
}


const ggClassyClassBoxPoints::tBoxItemsVec& ggClassyClassBoxPoints::GetBoxItems(const QString& aClassName) const
{
  static tBoxItemsVec vNoBoxItems;
  tBoxItemsMap::const_iterator vBoxItemsIterator = mBoxItemsMap.find(aClassName);
  if (vBoxItemsIterator != mBoxItemsMap.end()) return vBoxItemsIterator->second;
  else return vNoBoxItems;
}


ggClassyClassBoxPoints::tPointsSet ggClassyClassBoxPoints::GetClassPointsTop(const QString& aClassName) const
{
  tPointsSet vPoints;
  const tBoxItemsVec& vBoxItems(GetBoxItems(aClassName));
  ggWalkerT<tBoxItemsVec::const_iterator> vBoxItemsWalker(vBoxItems);
  while (vBoxItemsWalker) {
    const ggClassyGraphicsBoxItem* vBoxItem = *vBoxItemsWalker;
    vPoints.insert(vBoxItem->GetClassConnectionTop());
  }
  return vPoints;
}


ggClassyClassBoxPoints::tPointsSet ggClassyClassBoxPoints::GetClassPointsBottom(const QString& aClassName) const
{
  tPointsSet vPoints;
  const tBoxItemsVec& vBoxItems(GetBoxItems(aClassName));
  ggWalkerT<tBoxItemsVec::const_iterator> vBoxItemsWalker(vBoxItems);
  while (vBoxItemsWalker) {
    const ggClassyGraphicsBoxItem* vBoxItem = *vBoxItemsWalker;
    vPoints.insert(vBoxItem->GetClassConnectionBottom());
  }
  return vPoints;
}


ggClassyClassBoxPoints::tPointsSet ggClassyClassBoxPoints::GetClassPointsLeftRight(const QString& aClassName) const
{
  tPointsSet vPoints;
  const tBoxItemsVec& vBoxItems(GetBoxItems(aClassName));
  ggWalkerT<tBoxItemsVec::const_iterator> vBoxItemsWalker(vBoxItems);
  while (vBoxItemsWalker) {
    const ggClassyGraphicsBoxItem* vBoxItem = *vBoxItemsWalker;
    vPoints.insert(vBoxItem->GetClassConnectionLeft());
    vPoints.insert(vBoxItem->GetClassConnectionRight());
  }
  return vPoints;
}


void ggClassyClassBoxPoints::Clear()
{
  mBoxItems.clear();
  mBoxItemsMap.clear();
}


void ggClassyClassBoxPoints::Refresh()
{
  // copy the registered box-items
  tBoxItemsSet vBoxItems(mBoxItems);

  // clear all
  mBoxItems.clear();
  mBoxItemsMap.clear();

  // re-add box-items
  ggWalkerT<tBoxItemsSet::iterator> vBoxItemsWalker(vBoxItems);
  while (vBoxItemsWalker) {
    AddBoxItem(*vBoxItemsWalker);
  }
}
