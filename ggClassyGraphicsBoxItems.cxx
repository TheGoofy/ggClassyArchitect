#include "ggClassyGraphicsBoxItems.h"

#include "ggWalkerT.h"


ggClassyGraphicsBoxItems::ggClassyGraphicsBoxItems()
{
}


void ggClassyGraphicsBoxItems::AddItem(ggClassyGraphicsBoxItem* aBoxItem)
{
  if (aBoxItem != nullptr) {
    mBoxItems.insert(aBoxItem);
    if (aBoxItem->GetClass() != nullptr) {
      mBoxItemsMap[aBoxItem->GetClass()->mClassName].push_back(aBoxItem);
    }
    Notify();
  }
}


const ggClassyGraphicsBoxItems::tBoxItemsSet& ggClassyGraphicsBoxItems::GetBoxItems() const
{
  return mBoxItems;
}


const ggClassyGraphicsBoxItems::tBoxItemsVec& ggClassyGraphicsBoxItems::GetBoxItems(const QString& aClassName) const
{
  static tBoxItemsVec vNoBoxItems;
  tBoxItemsMap::const_iterator vBoxItemsIterator = mBoxItemsMap.find(aClassName);
  if (vBoxItemsIterator != mBoxItemsMap.end()) return vBoxItemsIterator->second;
  else return vNoBoxItems;
}


ggClassyGraphicsBoxItems::tPointsSet ggClassyGraphicsBoxItems::GetClassPointsTop(const QString& aClassName) const
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


ggClassyGraphicsBoxItems::tPointsSet ggClassyGraphicsBoxItems::GetClassPointsBottom(const QString& aClassName) const
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


ggClassyGraphicsBoxItems::tPointsSet ggClassyGraphicsBoxItems::GetClassPointsLeftRight(const QString& aClassName) const
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
