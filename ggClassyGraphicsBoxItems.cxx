#include "ggClassyGraphicsBoxItems.h"


ggClassyGraphicsBoxItems::ggClassyGraphicsBoxItems()
{
}


void ggClassyGraphicsBoxItems::AddItem(ggClassyGraphicsBoxItem* aBoxItem)
{
  if (aBoxItem->GetClass() != nullptr) {
    mBoxItemsMap[aBoxItem->GetClass()->mName].push_back(aBoxItem);
    aBoxItem->SetBoxItems(this);
    Notify();
  }
}


const ggClassyGraphicsBoxItems::tBoxItemsVec& ggClassyGraphicsBoxItems::GetBoxItems(const QString& aClassName) const
{
  static tBoxItemsVec vNoBoxItems;
  tBoxItemsMap::const_iterator vBoxItemsIterator = mBoxItemsMap.find(aClassName);
  if (vBoxItemsIterator != mBoxItemsMap.end()) return vBoxItemsIterator->second;
  else return vNoBoxItems;
}
